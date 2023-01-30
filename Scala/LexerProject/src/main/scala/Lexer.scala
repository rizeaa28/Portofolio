case class Lexer (spec: String) {

  def buildNFAforLexer(nfaList: List[Nfa[Int]]): (Nfa[Int], List[List[Int]]) = {

    var endStatesList: List[List[Int]] = List()

    // starts the big nfa with the first nfa from the list, adding the new start state
    val startState = 0
    val endStates = nfaList.apply(0).map(x => x + 1).getEndState
    val transitions = nfaList.apply(0).map(x => x + 1).getTransitions +
                      ((startState, Map("eps" -> List(nfaList.apply(0).map(x => x + 1).getStartState))))
    val states = nfaList.apply(0).map(x => x + 1).getStates ++ Set(startState)

    var finalNfa = new Nfa[Int](states, transitions, startState, endStates)
    endStatesList = endStatesList ++ List(finalNfa.getEndState.toList)

    // add the other nfas
    for(i <- Range(1, nfaList.length)) {
      val nfaToAdd = nfaList.apply(i).map(x => x + finalNfa.getStates.size)
      val newStates = finalNfa.getStates ++ nfaToAdd.getStates
      val newTransitions = finalNfa.getTransitions ++ nfaToAdd.getTransitions
      val newEndStates = finalNfa.getEndState ++ nfaToAdd.getEndState

      endStatesList = endStatesList ++ List(nfaToAdd.getEndState.toList)
      finalNfa = new Nfa(newStates, newTransitions, startState, newEndStates)
      finalNfa = finalNfa.updateTransition(startState, nfaToAdd.getStartState, "eps")
    }

    (finalNfa, endStatesList)
  }

  def buildNFAfromSpec(): (Nfa[Int], List[(String, List[Int])]) = {
    var listTokens: Array[String] = spec.split(";")   // divides the spec by lines
    var nfaList: List[Nfa[Int]] = List()                     // the list with nfas built from every regex
    var tokenList: List[String] = List()                     // list of tokens


    // builds the nfa for every token's regex
    for(i <- Range(0, listTokens.length - 1)) {
      val token = listTokens.apply(i).split(":").apply(0).trim()
      var regex = listTokens.apply(i).split(":").apply(1).drop(1)

      if(token.equals("NEWLINE"))
        regex = "newline"

      val nfa = Nfa.fromPrenex(Regex.toPrenex(regex))

      nfaList = nfaList ++ List(nfa)
      tokenList = tokenList ++ List(token)
    }


    // builds the "big" nfa like in the homework description and also a list with the endstates list for every nfa
    var builtNfa = buildNFAforLexer(nfaList)

    // list that contains the associations between tokens end the end states in the big nfa
    var tokensAndEndStatesList: List[(String, List[Int])] = List()

    for(i <- Range(0, tokenList.length)) {
      tokensAndEndStatesList = tokensAndEndStatesList ++ List((tokenList.apply(i), builtNfa._2.apply(i)))
    }

    (builtNfa._1, tokensAndEndStatesList)
  }

  // checks if there are any final states which indicates token matches included in this dfa state
  def checkIfTokenState(tokenStateAssociation: List[(String, List[Int])], currentStates: Set[Int]): List[String] = {

    var currentToken = ""
    var currentTokenStates: List[Int] = List()
    var output: List[String] = List()
    var currentStateSet = currentStates

    for(i <- tokenStateAssociation) {
      currentToken = i._1
      currentTokenStates = i._2

      for(j <- currentTokenStates) {
        if(currentStateSet.contains(j)) {
          output = output ++ List(currentToken)
        }
      }
    }
    output
  }

  // finds the next pairs of (lexem, token) for the given part of the word
  def findNextLexem(word: String, lexerDfa: Dfa[Long], tokenStateAssociation: List[(String, List[Int])], nfa: Nfa[Int]): List[(String, String)] = {

    var stop = false                                // stops when found a sinkState
    var currentStates = Set(0)                      // currentState of dfa
    var traversableWord = word                      // for traversing the word
    var prefix = ""                                 // current prefix (current lexem)
    var matches: List[(String, String)] = List()    // the found pairs

    currentStates = nfa.getEpsTransition(0) ++ currentStates
    while(!stop && traversableWord.nonEmpty) {

      if(currentStates.nonEmpty) {
        for(i <- currentStates)
          currentStates = currentStates ++ nfa.getEpsTransition(i)
      }

      currentStates = Dfa.getNextGroup(nfa, currentStates, traversableWord.head.toString)
      prefix = prefix + traversableWord.head
      traversableWord = traversableWord.drop(1)

      if(currentStates.contains(lexerDfa.getSinkState.toInt)) {
        stop = true
        matches = matches ++ List((prefix, "eroare"))
      }

      if(!stop) {
        var tokenMatchedList = checkIfTokenState(tokenStateAssociation, currentStates)

        for(i <- tokenMatchedList) {
          matches = matches ++ List((prefix, i))
        }
      }

      if(currentStates.nonEmpty && traversableWord.isEmpty && matches.isEmpty) {
        stop = true
        matches = matches ++ List((prefix ++ "a", "eroare"))
      }

      if((currentStates.isEmpty || traversableWord.isEmpty) && !stop && matches.isEmpty) {
        stop = true
        matches = matches ++ List((prefix, "eroare"))
      }
    }

    if(matches.length == 1 && matches.head._2.equals("eroare"))
      matches
    else
      matches.filterNot(elem => elem._2.equals("eroare"))
  }

  def getTokenPriority(token: String, tokenOrder: List[String]): Int = {
    tokenOrder.takeWhile(x => !x.equals(token)).length
  }

  // selects the best token from the matches
  def findBestToken(matches: List[(String, String)], tokenOrder: List[String]): (String, String) = {

    var bestMatch = matches.apply(0)

    for(i <- matches.drop(1)) {

      // the one with the longest prefix
      if(i._1.length > bestMatch._1.length)
        bestMatch = i

      // and the best token priority
      if(i._1.length > bestMatch._1.length)
        if(getTokenPriority(i._2, tokenOrder) < getTokenPriority(bestMatch._2, tokenOrder))
          bestMatch = i
    }

    bestMatch
  }

  def escapeCharacters(str: String): String = {
    var output = str.replaceAll("\\n", "newline")
    output
  }
  /*
    This is the main function of the lexer, it splits the given word into a list of lexems
    in the format (LEXEM, TOKEN)
  */
  def lex(word: String): Either[String,List[(String,String)]] = {
    val builtNfa = buildNFAfromSpec()

    val lexerDfa = Dfa.nfaToDfa(builtNfa._1)
    val tokenStateAssociation = builtNfa._2

    var stop = false
    var found: List[(String, String)] = List()
    var traversableWord = escapeCharacters(word)
    val tokenOrder = tokenStateAssociation.foldRight(List[String]())((token, acc) => acc ++ List(token._1)).reverse
    var errorMessage = ""

    while(traversableWord.nonEmpty && !stop) {
      val nextMatches = findNextLexem(traversableWord, lexerDfa, tokenStateAssociation, builtNfa._1)

      if(nextMatches.length == 1 && nextMatches.head._2.equals("eroare")) {
        // tratare erori

        var charCount = found.foldRight(0)((elem, acc) => acc + elem._1.length) + nextMatches.head._1.length - 1
        var newLineCount = found.count(elem => elem._2.equals("NEWLINE"))
        errorMessage = "No viable alternative at character "

        if(traversableWord.drop(nextMatches.head._1.length - 1).isEmpty)
          errorMessage = errorMessage + "EOF"
        else
          errorMessage = errorMessage + charCount

        errorMessage = errorMessage + ", line " + newLineCount

        stop = true
      } else {
        val bestToken = findBestToken(nextMatches, tokenOrder)
        if(bestToken._1.equals("newline"))
          found = found ++ List(("\n", bestToken._2))
        else
          found = found ++ List(bestToken)
        traversableWord = traversableWord.drop(bestToken._1.length)
      }
    }

    var output: Either[String,List[(String,String)]] = Left("eroare")
    if(!stop) {
      output = Right(found)
    } else {
      output = Left(errorMessage)
    }

    output
  }
}