import scala.Console.in

class LexerTests extends munit.FunSuite{

  test("Test lexer: simple concat (2p)"){
    val spec =
      """A: a;
        #BC: bc;
        #DEF: def;
        #""".stripMargin('#')

    assert(Lexer(spec).lex("a") == Right(List(("a", "A"))))
    assert(Lexer(spec).lex("aa") == Right(List(("a", "A"), ("a", "A"))))
    assert(Lexer(spec).lex("abca") == Right(List(("a", "A"), ("bc", "BC"), ("a", "A"))))
    assert(Lexer(spec).lex("abcdefdefbca") == Right(List(("a", "A"), ("bc", "BC"), ("def", "DEF"), ("def", "DEF"), ("bc", "BC"), ("a", "A"))))
  }

  test("Test lexer: simple union (2p)"){
    val spec =
      """AorB: a|b;
        #DorE: d|e;
        #""".stripMargin('#')

    assert(Lexer(spec).lex("a") == Right(List(("a", "AorB"))))
    assert(Lexer(spec).lex("abba") == Right(List(("a", "AorB"), ("b", "AorB"), ("b", "AorB"), ("a", "AorB"))))
    assert(Lexer(spec).lex("abde") == Right(List(("a", "AorB"), ("b", "AorB"), ("d", "DorE"), ("e", "DorE"))))
    assert(Lexer(spec).lex("adbeb") == Right(List(("a", "AorB"), ("d", "DorE"), ("b", "AorB"), ("e", "DorE"), ("b", "AorB"))))
  }

  test("Test lexer: space and zeros char (5p)"){
    val spec =
      """smallA: a;
        #bigA: aaaa;
        #""".stripMargin('#')


    assert(Lexer(spec).lex("a") == Right(List(("a", "smallA"))))
    assert(Lexer(spec).lex("aa") == Right(List(("a", "smallA"), ("a", "smallA"))))
    assert(Lexer(spec).lex("aaaa") == Right(List(("aaaa", "bigA"))))
    assert(Lexer(spec).lex("aaaaaa") == Right(List(("aaaa", "bigA"), ("a", "smallA"), ("a", "smallA"))))
    assert(Lexer(spec).lex("aaaaaaaa") == Right(List(("aaaa", "bigA"), ("aaaa", "bigA"))))
    assert(Lexer(spec).lex("aaaaaaaaaaaa") == Right(List(("aaaa", "bigA"), ("aaaa", "bigA"), ("aaaa", "bigA"))))
  }

  test("Test lexer: space and zeros char (5p)"){
    val spec =
      """SPACE: ' ';
        #ZEROS: 0+;
        #""".stripMargin('#')

    assert(Lexer(spec).lex("0000 0") == Right(List(("0000", "ZEROS"), (" ", "SPACE"), ("0", "ZEROS"))))
    assert(Lexer(spec).lex(" 0000") == Right(List((" ", "SPACE"), ("0000", "ZEROS"))))
    assert(Lexer(spec).lex("00000000000000000000000000000000000000") == Right(List(("00000000000000000000000000000000000000", "ZEROS"))))
    assert(Lexer(spec).lex("0 00 000 0000 000 000 00 0 ") == Right(List(("0", "ZEROS"), (" ", "SPACE"), ("00", "ZEROS"), (" ", "SPACE"), ("000", "ZEROS"), (" ", "SPACE"), ("0000", "ZEROS"), (" ", "SPACE"), ("000", "ZEROS"), (" ", "SPACE"), ("000", "ZEROS"), (" ", "SPACE"), ("00", "ZEROS"), (" ", "SPACE"), ("0", "ZEROS"), (" ", "SPACE"))))
  }

  test("Test lexer: ones and twos char (8p)"){
    val spec =
      """TWO: 2;
        #PATTERN: 11*(00)*101(0|1)(0|1)*;
        #""".stripMargin('#')

    assert(Lexer(spec).lex("1001010") == Right(List(("1001010", "PATTERN"))))
    assert(Lexer(spec).lex("1101010101") == Right(List(("1101010101", "PATTERN"))))
    assert(Lexer(spec).lex("2110000101112") == Right(List(("2", "TWO"), ("11000010111", "PATTERN"), ("2", "TWO"))))
    assert(Lexer(spec).lex("111100001010211011") == Right(List(("111100001010", "PATTERN"), ("2", "TWO"), ("11011", "PATTERN"))))
    assert(Lexer(spec).lex("2211100000010111011000110110010022") == Right(List(("2", "TWO"), ("2", "TWO"), ("111000000101110110001101100100", "PATTERN"), ("2", "TWO"), ("2", "TWO"))))
    assert(Lexer(spec).lex("2100101121101112110101012100001011211011110111101") == Right(List(("2", "TWO"), ("1001011", "PATTERN"), ("2", "TWO"), ("110111", "PATTERN"), ("2", "TWO"), ("11010101", "PATTERN"), ("2", "TWO"), ("100001011", "PATTERN"), ("2", "TWO"), ("11011110111101", "PATTERN"))))
  }

  test("Test lexer: plus and star char (8p)"){
    val spec =
      """C: c;
        #ABS: (ab)+;
        #BS: b+;
        #""".stripMargin('#')

    assert(Lexer(spec).lex("ab") == Right(List(("ab", "ABS"))))
    assert(Lexer(spec).lex("bbbbb") == Right(List(("bbbbb", "BS"))))
    assert(Lexer(spec).lex("abababcb") == Right(List(("ababab", "ABS"), ("c", "C"), ("b", "BS"))))
    assert(Lexer(spec).lex("bbab") == Right(List(("bb", "BS"), ("ab", "ABS"))))
    assert(Lexer(spec).lex("bbbcbbabbc") == Right(List(("bbb", "BS"), ("c", "C"), ("bb", "BS"), ("ab", "ABS"), ("b", "BS"), ("c", "C"))))
    assert(Lexer(spec).lex("cbbbbcbbabcabbbabb") == Right(List(("c", "C"), ("bbbb", "BS"), ("c", "C"), ("bb", "BS"), ("ab", "ABS"), ("c", "C"), ("ab", "ABS"), ("bb", "BS"), ("ab", "ABS"), ("b", "BS"))))
    assert(Lexer(spec).lex("ababbbbabcabbababcb") == Right(List(("abab", "ABS"), ("bbb", "BS"), ("ab", "ABS"), ("c", "C"), ("ab", "ABS"), ("b", "BS"), ("abab", "ABS"), ("c", "C"), ("b", "BS"))))
    assert(Lexer(spec).lex("cbbbabcabbabcbbcababab") == Right(List(("c", "C"), ("bbb", "BS"), ("ab", "ABS"), ("c", "C"), ("ab", "ABS"), ("b", "BS"), ("ab", "ABS"), ("c", "C"), ("bb", "BS"), ("c", "C"), ("ababab", "ABS"))))
  }

  test("Test lexer: whitespaces char (10p)"){
    val spec =
      """SPACE: ' ';
        #NEWLINE: '\n';
        #PATTERN1: 1' '0;
        #PATTERN2: (10)+;
        #PATTERN3: ' '001' ';
        #PATTERN4: (101' ')+;
        #PATTERN5: 1*01;
        #""".stripMargin('#')

    assert(Lexer(spec).lex("1 0") == Right(List(("1 0", "PATTERN1"))))
    assert(Lexer(spec).lex("101010") == Right(List(("101010", "PATTERN2"))))
    assert(Lexer(spec).lex("101010 1 0 1 0") == Right(List(("101010", "PATTERN2"), (" ", "SPACE"), ("1 0", "PATTERN1"), (" ", "SPACE"), ("1 0", "PATTERN1"))))
    assert(Lexer(spec).lex("1 0 001 1 010 ") == Right(List(("1 0", "PATTERN1"), (" 001 ", "PATTERN3"), ("1 0", "PATTERN1"), ("10", "PATTERN2"), (" ", "SPACE"))))
    assert(Lexer(spec).lex("1 0 \n  001 1 0") == Right(List(("1 0", "PATTERN1"), (" ", "SPACE"), ("\n", "NEWLINE"), (" ", "SPACE"), (" 001 ", "PATTERN3"), ("1 0", "PATTERN1"))))
    assert(Lexer(spec).lex("101 101 1 01010  ") == Right(List(("101 101 ", "PATTERN4"), ("1 0", "PATTERN1"), ("1010", "PATTERN2"), (" ", "SPACE"), (" ", "SPACE"))))
    assert(Lexer(spec).lex("101 1010\n  001   001  101010 ") == Right(List(("101 ", "PATTERN4"), ("1010", "PATTERN2"), ("\n", "NEWLINE"), (" ", "SPACE"), (" 001 ", "PATTERN3"), (" ", "SPACE"), (" 001 ", "PATTERN3"), (" ", "SPACE"), ("101010", "PATTERN2"), (" ", "SPACE"))))
    assert(Lexer(spec).lex("11101\n1 0  001 101 ") == Right(List(("11101", "PATTERN5"), ("\n", "NEWLINE"), ("1 0", "PATTERN1"), (" ", "SPACE"), (" 001 ", "PATTERN3"), ("101 ", "PATTERN4"))))
    assert(Lexer(spec).lex("1010\n1 01111101\n 1010 101 101    001 ") == Right(List(("1010", "PATTERN2"), ("\n", "NEWLINE"), ("1 0", "PATTERN1"), ("1111101", "PATTERN5"), ("\n", "NEWLINE"), (" ", "SPACE"), ("1010", "PATTERN2"), (" ", "SPACE"), ("101 101 ", "PATTERN4"), (" ", "SPACE"), (" ", "SPACE"), (" 001 ", "PATTERN3"))))
  }

  test("Test lexer: abcd diverse char (10p)"){
    val spec =
      """SPACE: ' ';
        #DS: d+;
        #ABS: (ab)*;
        #ABCORC: (abc)|c;
        #APLUSCD: (a+)cd;
        #ABD: abd;
        #""".stripMargin('#')

    assert(Lexer(spec).lex(" acdaacdabd") == Right(List((" ", "SPACE"), ("acd", "APLUSCD"), ("aacd", "APLUSCD"), ("abd", "ABD"))))
    assert(Lexer(spec).lex("abdabc abd ababab ") == Right(List(("abd", "ABD"), ("abc", "ABCORC"), (" ", "SPACE"), ("abd", "ABD"), (" ", "SPACE"), ("ababab", "ABS"), (" ", "SPACE"))))
    assert(Lexer(spec).lex("abababababab ababab c aaacd abd ") == Right(List(("abababababab", "ABS"), (" ", "SPACE"), ("ababab", "ABS"), (" ", "SPACE"), ("c", "ABCORC"), (" ", "SPACE"), ("aaacd", "APLUSCD"), (" ", "SPACE"), ("abd", "ABD"), (" ", "SPACE"))))
    assert(Lexer(spec).lex("abd c abababab") == Right(List(("abd", "ABD"), (" ", "SPACE"), ("c", "ABCORC"), (" ", "SPACE"), ("abababab", "ABS"))))
    assert(Lexer(spec).lex("abababcababdd") == Right(List(("ababab", "ABS"), ("c", "ABCORC"), ("abab", "ABS"), ("dd", "DS"))))
    assert(Lexer(spec).lex("ddddd acd abccdddddd ") == Right(List(("ddddd", "DS"), (" ", "SPACE"), ("acd", "APLUSCD"), (" ", "SPACE"), ("abc", "ABCORC"), ("c", "ABCORC"), ("dddddd", "DS"), (" ", "SPACE"))))
    assert(Lexer(spec).lex(" d abab ddabcabcc") == Right(List((" ", "SPACE"), ("d", "DS"), (" ", "SPACE"), ("abab", "ABS"), (" ", "SPACE"), ("dd", "DS"), ("abc", "ABCORC"), ("abc", "ABCORC"), ("c", "ABCORC"))))
    assert(Lexer(spec).lex("acdabd aacdc dddd abababc") == Right(List(("acd", "APLUSCD"), ("abd", "ABD"), (" ", "SPACE"), ("aacd", "APLUSCD"), ("c", "ABCORC"), (" ", "SPACE"), ("dddd", "DS"), (" ", "SPACE"), ("ababab", "ABS"), ("c", "ABCORC"))))
    assert(Lexer(spec).lex("caaacdabcaacdcddababd ab abd") == Right(List(("c", "ABCORC"), ("aaacd", "APLUSCD"), ("abc", "ABCORC"), ("aacd", "APLUSCD"), ("c", "ABCORC"), ("dd", "DS"), ("abab", "ABS"), ("d", "DS"), (" ", "SPACE"), ("ab", "ABS"), (" ", "SPACE"), ("abd", "ABD"))))
    assert(Lexer(spec).lex("aacd aacd c abcacddddaacd abccab c") == Right(List(("aacd", "APLUSCD"), (" ", "SPACE"), ("aacd", "APLUSCD"), (" ", "SPACE"), ("c", "ABCORC"), (" ", "SPACE"), ("abc", "ABCORC"), ("acd", "APLUSCD"), ("ddd", "DS"), ("aacd", "APLUSCD"), (" ", "SPACE"), ("abc", "ABCORC"), ("c", "ABCORC"), ("ab", "ABS"), (" ", "SPACE"), ("c", "ABCORC"))))
  }

  test("Test lexer: simple error parsing char (5p)"){
    val spec =
      """NEWLINE: '\n';
        #ABC: a(b+)c;
        #""".stripMargin('#')

    assert(Lexer(spec).lex("a zzzz") == Left("No viable alternative at character 1, line 0"))
    assert(Lexer(spec).lex("z zzzz") == Left("No viable alternative at character 0, line 0"))
    assert(Lexer(spec).lex("ab zzzz") == Left("No viable alternative at character 2, line 0"))
    assert(Lexer(spec).lex("abb zzzz") == Left("No viable alternative at character 3, line 0"))
    assert(Lexer(spec).lex("abbb zzzz") == Left("No viable alternative at character 4, line 0"))
    assert(Lexer(spec).lex("a") == Left("No viable alternative at character EOF, line 0"))
    assert(Lexer(spec).lex("ab") == Left("No viable alternative at character EOF, line 0"))
    assert(Lexer(spec).lex("abb") == Left("No viable alternative at character EOF, line 0"))
    assert(Lexer(spec).lex("abbb") == Left("No viable alternative at character EOF, line 0"))
    assert(Lexer(spec).lex("abbbc\nabc\n\n\nabbbbbc\nabbbbb") == Left("No viable alternative at character EOF, line 5"))
  }

}
