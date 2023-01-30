import java.util
import scala.collection.mutable

object Regex {
  /*
    This function should:
    -> Classify input as either character(or string) or operator
    -> Convert special inputs like [0-9] to their correct form
    -> Convert escaped characters
  */

  // function that gets a the initial regex and determines if the next characters from step i are anything special
  def getNextRegex(s:List[Char], i: Int): (List[Either[Char, Char]], Int) = {

    // drop the characters that dont interest us
    var currentString: List[Char] = s.drop(i)

    //init output
    var outRegex: List[Either[Char, Char]] = List()
    var outI = i;

    // For every special set of characters in the regex we check if they're present at this step
    // and if they are we compute what should be added to the preprocessed regex and the new step in the initial regex we're at

    // eps
    var found = currentString.length >= 3 && currentString.apply(0) == 'e' && currentString.apply(1) == 'p' && currentString.apply(2) == 's'

    if (found && outRegex.equals(List())) {
      outRegex = List(Left('e'), Left('p'), Left('s'))
      outI = i + 3
    }

    // [0-9]
    found = currentString.startsWith("[0-9]")

    if (found && outRegex.equals(List())) {
      outRegex = outRegex ++ List(Left('('))
      for(k <- Range(0,9)) {
        outRegex = outRegex ++ List(Right((k.toString).apply(0)), Left('|'))
      }
      outRegex = outRegex ++ List(Right('9'), Left(')'))
      outI = i + 5
    }

    // [a-z]
    found = currentString.startsWith("[a-z]")

    if (found && outRegex.equals(List())) {
      outRegex = outRegex ++ List(Left('('))
      for(k <- Range('a','z')) {
        outRegex = outRegex ++ List(Right(k.toChar), Left('|'))
      }
      outRegex = outRegex ++ List(Right('z'), Left(')'))
      outI = i + 5
    }

    // [A-Z]
    found = currentString.startsWith("[A-Z]")

    if (found && outRegex.equals(List())) {
      outRegex = outRegex ++ List(Left('('))
      for (k <- Range('A', 'Z')) {
        outRegex = outRegex ++ List(Right(k.toChar), Left('|'))
      }
      outRegex = outRegex ++ List(Right('Z'), Left(')'))
      outI = i + 5
    }

    // 'c'
    found = currentString.length >= 3 && currentString.apply(0) == '\'' && currentString.apply(2) == '\''

    if (found && outRegex.equals(List())) {
      outRegex = outRegex ++ List(Right(currentString.apply(1)))
      outI = i + 3
    }

    // c
    found = currentString.length >= 1 && currentString.apply(0) != '+' && currentString.apply(0) != '*' && currentString.apply(0) != '?' && currentString.apply(0) != '|' &&
            currentString.apply(0) != '(' && currentString.apply(0) != ')'

    if(found && outRegex.equals(List())) {
      outRegex = outRegex ++ List(Right(currentString.apply(0))):List[Either[Char, Char]]
      outI = i + 1
    }

    // Checking for + or ?
    currentString = currentString.drop(outI - i)

    found = currentString.startsWith("+") && outRegex.nonEmpty

    if(found) {
      outRegex = List(Left('(')) ++ List(Left('(')) ++ outRegex ++ List(Left(')')) ++ List(Left('(')) ++ outRegex ++ List(Left(')')) ++ List(Left('*')) ++ List(Left(')'))
      outI = outI + 1
    }

    found = currentString.startsWith("?")

    if(found) {
      outRegex = List(Left('(')) ++ outRegex ++ List(Left('|')) ++ List(Left('e')) ++ List(Left('p')) ++ List(Left('s')) ++ List(Left(')'))
      outI = outI + 1
    }

    (outRegex, outI)
  }

  def preprocess(s:List[Char]): List[Either[Char,Char]] = {

    // init output
    var output: List[Either[Char, Char]] = List()

    // list of outputs that holds count of braces
    var acc: List[List[Either[Char, Char]]] = List()

    // number of braces active and if active
    var accCounter = -1
    var accActive = false

    // traverse the initial regex and compute the output
    var i = 0
    while(i < s.length) {

      // if we see a brace start, we start to add everything we compute to the
      // corresponding list in acc
      // We need this for cases like (regex)+ where we need to know exactly which regex we apply + on to duplicate it
      // to form regexregex*

      if(s.apply(i) == '(') {
        i = i + 1
        accCounter = accCounter + 1
        acc = acc ++ List(List():List[Either[Char, Char]])
        accActive = true
      }

      // we close a brace
      if(s.apply(i) == ')') {
        i = i + 1
        accActive = false
        var done = false

        // We checked if its the case for '?' or '+'
        if(s.length > i) {

          // If '+'
          if(s.apply(i) == '+') {

            // We either add to the output (there is only one brace active) or the braces that included this braces
            if(accCounter > 0) {
              var cur = acc.apply(accCounter - 1) // get what we've build up between the last braces and add it
              cur = cur ++ List(Left('(')) ++ List(Left('(')) ++ acc.apply(accCounter) ++ List(Left(')')) ++ List(Left('(')) ++ acc.apply(accCounter) ++ List(Left(')')) ++ List(Left('*')) ++ List(Left(')'))
              acc = acc.dropRight(2) // drop what we've accumulated
              acc = acc ++ List(cur) // put right back into the acc
            } else {
              output = output ++ List(Left('(')) ++ List(Left('(')) ++ acc.apply(accCounter) ++ List(Left(')')) ++ List(Left('(')) ++ acc.apply(accCounter) ++ List(Left(')')) ++ List(Left('*')) ++ List(Left(')'))
              acc = acc.dropRight(1)
            }
            i = i + 1
            done = true

          } else {

            // Same thing for '?'
            if(s.apply(i) == '?') {

              if(accCounter > 0) {
                var cur = acc.apply(accCounter - 1)
                cur = cur ++ List(Left('(')) ++ acc.apply(accCounter) ++ List(Left('|')) ++ List(Left('e')) ++ List(Left('p')) ++ List(Left('s')) ++ List(Left(')'))
                acc = acc.dropRight(2)
                acc = acc ++ List(cur)
              } else {
                output = output ++ List(Left('(')) ++ acc.apply(accCounter) ++ List(Left('|')) ++ List(Left('e')) ++ List(Left('p')) ++ List(Left('s')) ++ List(Left(')'))
                acc = acc.dropRight(1)
              }
              i = i + 1
              done = true
            }
          }
        }

        // If we didnt encounter '?' or '+' we add it simply
        if(!done) {
          if(accCounter > 0) {
            var cur = acc.apply(accCounter - 1)
            cur = cur ++ List(Left('(')) ++ acc.apply(accCounter) ++ List(Left(')'))
            acc = acc.dropRight(2)
            acc = acc ++ List(cur)
          } else {
            output = output ++ List(Left('(')) ++ acc.apply(accCounter) ++ List(Left(')'))
            acc = acc.dropRight(1)
          }
        }
        accCounter = accCounter - 1
        done = true
      }

      // compute next regex character (next step)
      var out = getNextRegex(s, i)

      // if we found a regex step to add
      if(out._2 != i) {
        i = out._2

        if(accCounter >= 0) {

          // if we have a brace we need to add to we add it there
          var cur = acc.apply(accCounter)
          cur = cur ++ out._1
          acc = acc.dropRight(1)
          acc = acc ++ List(cur)
        } else {

          // else we ad directly to output
          output = output ++ out._1
        }
      } else {

        // If we didnt find anything to preproccess (character like '*' that can just be copied)
        if(s.length > i && !s.apply(i).equals(')') && !s.apply(i).equals('(')) {
          if(accCounter >= 0) {
            var cur = acc.apply(accCounter)
            cur = cur ++ List(Left(s.apply(i)))
            acc = acc.dropRight(1)
            acc = acc ++ List(cur)

          } else {
            output = output ++ List(Left(s.apply(i)))
          }
          i = i + 1
        }
      }
    }


    output
  }

  // proccess the elements from the stack (but only one step at the time)
  def processStack(value: mutable.Stack[Regex.ASTPrenex]): mutable.Stack[Regex.ASTPrenex] = {
    var newStack: mutable.Stack[Regex.ASTPrenex] = mutable.Stack()
    var stack = value
    var processed = false // this is to make sure we do only one step at a time


    // this function transforms things like "UNION x ?" in "UNION x y" if theres an y after union
    while(stack.nonEmpty) {

      if(processed) {
        newStack.pushAll(stack)
        stack.popAll()
      } else {
        val curr = stack.pop()

        curr match {
          case ASTRegex(str) => {
            if(stack.nonEmpty) {
              val newCurr = stack.pop()

              newCurr match {
                case ASTUncompletedRegex2(operation, op1) => newStack.push(ASTRegex(operation + " " + op1 + " " + str))
                                                            processed = true
                case ASTUncompletedRegex3(operation) => newStack.push(ASTUncompletedRegex2(operation, curr.toString))
                                                          processed = true
                case x => newStack.push(curr).push(x)
              }
            } else {
              newStack.push(curr)
            }
          }
          case ASTUncompletedRegex3(operation) => {

            if(newStack.nonEmpty) {
              val newCurr = newStack.pop()

              newCurr match {
                case ASTRegex(strNew) => newStack.push(ASTUncompletedRegex2(operation, strNew))
                  processed = true
              }
            } else {
              newStack.push(curr)
            }
          }
          case ASTUncompletedRegex2(operation, op1) => {
            val newCurr = stack.pop()

            newCurr match {
              case ASTRegex(newStr) => newStack.push(ASTRegex(operation + " " + op1 + " " + newStr))
                processed = true
            }
          }
          case ASTUncompletedRegex1(operation) => {
            val newCurr = stack.pop()

            newCurr match {
              case ASTRegex(newStr) => newStack.push(ASTRegex(operation + " " + newStr))
                processed = true
            }
          }
        }
      }
    }
    newStack
  }

  // Same thing as the classic processStack but only for CONCAT
  def processConcat(value: mutable.Stack[Regex.ASTPrenex]): mutable.Stack[Regex.ASTPrenex] = {
    var newStack: mutable.Stack[Regex.ASTPrenex] = mutable.Stack()
    var stack = value
    var processed = false

    while(stack.nonEmpty) {

      if(processed) {
        newStack.pushAll(stack)
        stack.popAll()
      } else {
        val curr = stack.pop()

        curr match {
          case ASTRegex(str) => {

            if(stack.nonEmpty) {
              val newCurr = stack.pop()

              newCurr match {
                case ASTRegex(strNew) => newStack.push(ASTRegex("CONCAT " + ASTRegex(strNew).toString + " " + ASTRegex(str).toString))
                  processed = true
                case x => newStack.push(curr).push(x)
              }
            } else {
              newStack.push(curr)
            }
          }
          case x => newStack.push(x)
        }
      }
    }
    newStack.reverse
  }

  // Same thing as the classic processStack but only for STAR
  def processStar(value: mutable.Stack[Regex.ASTPrenex]): mutable.Stack[Regex.ASTPrenex] = {
    var newStack: mutable.Stack[Regex.ASTPrenex] = mutable.Stack()
    var stack = value

    while(stack.nonEmpty) {
      val curr = stack.pop()

      curr match {
        case ASTUncompletedRegex1(operation) => {
          val newCurr = stack.pop()

          newCurr match {
            case ASTRegex(newStr) => newStack.push(ASTRegex(operation + " " + newStr))
          }
        }
        case x => newStack.push(x)
      }
    }

    newStack.reverse
  }


  // This function should construct a prenex expression out of a normal one.
  def toPrenex(str: String): String = {
    var processedRegex = preprocess(str.toList)
    var output = ""
    var stack: mutable.Stack[ASTPrenex] = mutable.Stack()
    var rep = 0 // max number of iterations (generally around max 10, it is so testing wont take long if there is a loop)

    // add everything from the preprocessed regex pe stiva
    while (processedRegex.length > 0) {
      processedRegex.apply(0) match {
        case Right(value) => stack.push(ASTRegex(value.toString))
        case Left(value) => {
          value match {
            case '|' => stack.push(ASTUncompletedRegex3("UNION"))
            case '*' => stack.push(ASTUncompletedRegex1("STAR"))
            case 'e' => stack.push(ASTRegex("eps"))
                        processedRegex = processedRegex.drop(2)
            case '(' => {
              // In case of braces, just compute everything in the braces separately and add it back into the stack
              var miniRegex: List[Either[Char, Char]] = List()
              processedRegex = processedRegex.drop(1)
              while(processedRegex.contains(Left(')')) && !numberOfEqualBraces(processedRegex)) {
                miniRegex = miniRegex ++ List(processedRegex.apply(0))
                processedRegex = processedRegex.drop(1)
              }
              processedRegex = List(Right(')')) ++ processedRegex
              stack.push(ASTRegex(toPrenex((miniRegex.foldRight("")((x, acc) => x match {
                                                                                case Right(y) => y + acc
                                                                                case Left(y) => y + acc
              })).dropRight(1))))
            }
          }
        }
      }
      processedRegex = processedRegex.drop(1)
    }

    // STAR must pe proccesed before CONCAT and everything else, and CONCAT after
    // kinda like we do multiplication before adding

    //println(stack)

    var processedStack = processStar(stack.clone())

    //println(processedStack)
    processedStack = processConcat(processedStack.clone())

    //println(processedStack)

    // process the stack until there is no modification done at a step
    // this has to be done at least once (rep)
    while ((!processedStack.equals(stack) && rep < 50) || rep == 0) {

      stack = processedStack.clone()
      processedStack = processConcat(stack.clone())

      while(!processedStack.equals(stack) && rep < 50) {
        stack = processedStack.clone()
        processedStack = processConcat(stack.clone())
        rep = rep + 1
      }

      stack = processedStack.clone()
      processedStack = processStack(stack.clone())
      rep = rep + 1
    }

    // get the output from the stack
    output = processedStack.pop().toString
    output
  }

  // my type of syntax tree
  trait ASTPrenex {}

  // Completed regex, so just a string (ex: CONCAT a b)
  case class ASTRegex(string: String) extends ASTPrenex {
    override def toString: String = {
      if(string.equals(" "))
        "' '"
      else string
    }
  }

  // Type for things like "STAR ?"
  case class ASTUncompletedRegex1(operation: String) extends ASTPrenex {
    override def toString: String = operation
  }

  // Type for things like "UNION a ?"
  case class ASTUncompletedRegex2(operation: String, op1: String) extends ASTPrenex {
    override def toString: String = {
      if(op1.equals(" "))
        operation + " " + "' '"
      else operation + " " + op1
    }
  }

  // Type for things like "UNION ? ?"
  case class ASTUncompletedRegex3(operation: String) extends ASTPrenex {
    override def toString: String = operation
  }


  // checks if the number of '(' is equal to ')'
  def numberOfEqualBraces(list: List[Either[Char, Char]]): Boolean = {
    val startingBraces = list.count(x => x match {
      case Left('(') => true
      case _ => false
    })

    val endingBraces = list.count(x => x match {
      case Left(')') => true
      case _ => false
    })

    startingBraces == endingBraces
  }
}
