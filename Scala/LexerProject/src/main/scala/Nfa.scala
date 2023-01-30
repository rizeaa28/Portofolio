class Nfa[A](states: Set[A], transitions: Map[A, Map[String, List[A]]], startState: A, endState: Set[A]) {

  def map[B](f: A => B) : Nfa[B] = {

    // applies f to a transition
    def op(elem: (A, Map[String, List[A]])): (B, Map[String, List[B]]) = {
      val newMap = elem._2 transform ((_, list) => list.map(f))

      (f(elem._1), newMap)
    }

    val statesB = this.states.map(f)
    val transitionsB = this.transitions.map(op)
    val startStateB = f(this.startState)
    val endStateB = this.endState.map(f)

    new Nfa[B](statesB, transitionsB, startStateB, endStateB)
  }

  def next(state:A, c: Char): Set[A] = {
    val statesTransitions = this.transitions.getOrElse(state, Map())

    // if the states has no out-transitions
    if(statesTransitions.isEmpty)
      Set()
    else { //gets the transitions if there are any
      val list = statesTransitions.getOrElse(c.toString, List())
      list.toSet
    }
  }

  // gets the eps-transitions of a state (only one step)
  def getEpsTransition(state: A): Set[A] = {
    this.transitions.getOrElse(state, Map()).getOrElse("eps", List()).toSet
  }

  def accepts(str: String): Boolean = {

    // builds a set of tuples if type (state, current string) like in the lecture
    def buildTuple(states: Set[A], string: String): Set[(A, String)] = {
      states.foldRight(Set[(A, String)]())((elem, acc) => acc ++ Set((elem, string)))
    }

    // gets the next "step" of a tuple
    def op(conf: (A, String)): Set[(A, String)] = {

      // if the string is finished (is eps) then there is no next step
      if(conf._2.equals("eps"))
        Set[(A, String)]()


      if (conf._2.length == 1) //if the string has just one character left
        buildTuple(this.next(conf._1, conf._2.apply(0)), "eps") ++ // builds the tuples where the characters used
          buildTuple(this.getEpsTransition(conf._1), conf._2) // builds the tuples where we transitions through eps
      else
        buildTuple(this.next(conf._1, conf._2.apply(0)), conf._2.drop(1)) ++
          buildTuple(this.getEpsTransition(conf._1), conf._2)
    }

    // gets all configurations (tuples) and calculates the next ones for all of them
    def aux(conf: Set[(A, String)]): Set[(A, String)] = {
        val newConf = conf.foldRight(conf)((elem, acc) => acc ++ op(elem))
        newConf
      }

    // builds the start
    var startSet = Set[(A, String)]()
    if(str.equals("") == true) {
      startSet = Set((this.startState, "eps"))
    } else {
      startSet = Set((this.startState, str))
    }
    var isAccepted = false

    // verifies of the start aleady has "reached an end"
    for(i <- endState) {
      if(startSet.contains((i, "eps")))
        isAccepted = true
    }


    // add to the configurations untill there is no modification or it reached an end
    while(!aux(startSet).equals(startSet) && !isAccepted) {
      startSet = aux(startSet)

      for(i <- endState) {
        if(startSet.contains((i, "eps")))
          isAccepted = true
      }

    }

    isAccepted
  }

  def getStates : Set[A] = states

  def isFinal(state: A): Boolean = {
    endState.contains(state)
  }

  def getStartState: A = this.startState
  def getEndState: Set[A] = this.endState
  def getTransitions: Map[A, Map[String, List[A]]] = this.transitions

  // updates a transitions in a map (practically it has to build a new nfa because of how maps work in scala)
  // here I regretted I chose maps :c
  def updateTransition(start: A, end: A, str: String): Nfa[A] = {
    val startTransitions = this.transitions.getOrElse(start, Map())

    if(startTransitions.isEmpty) {
      val transition = this.transitions + (start -> Map(str -> List(end)))
      new Nfa[A](this.states, transition, this.startState, this.endState)
    } else {
      val list = startTransitions.getOrElse(str, List[A]()) ++ List(end)
      val transition = this.transitions + (start -> Map(str -> list))
      new Nfa[A](this.states, transition, this.startState, this.endState)
    }
  }

}

// This is a companion object to the Nfa class. This allows us to call the method fromPrenex without instantiating the Nfa class beforehand.
// You can think of the methods of this object like static methods of the Nfa class
object Nfa {
  def fromPrenex(str: String): Nfa[Int] = {

    // splits the prenex
    var string = mySplit(str)

    // builds the AST recurively
    def buildASTree(): ASTree = {
      if (string.length > 0) {
        string.apply(0) match {
          case "void" => ASTEmpty
          case "STAR" => {
            string = string.drop(1)
            ASTNode("STAR", buildASTree(), ASTEmpty)
          }
          case "PLUS" => {
            string = string.drop(1)
            ASTNode("PLUS", buildASTree(), ASTEmpty)
          }
          case "MAYBE" => {
            string = string.drop(1)
            ASTNode("MAYBE", buildASTree(), ASTEmpty)
          }
          case "UNION" => {
            string = string.drop(1)
            ASTNode("UNION", buildASTree(), buildASTree())
          }
          case "CONCAT" => {
            string = string.drop(1)
            ASTNode("CONCAT", buildASTree(), buildASTree())
          }
          case str => {
            string = string.drop(1)
            ASTNode(str, ASTEmpty, ASTEmpty)
          }
        }
      } else
      ASTEmpty
    }

    // uses the function to build the tree with our prenex
    val tree = buildASTree()

    // builds the nfa with out ast tree
    buildNFA(tree)
  } // TODO implement Prenex -> Nfa transformation.

  // ASTree trait and constructors
  trait ASTree {}
  case object ASTEmpty extends ASTree
  case class ASTNode(key: String, left: ASTree, right: ASTree) extends ASTree

  // personalized split function
  def mySplit(str: String): List[String] = {

    // splits clasically
    var temp = str.split(" ")
    var output = List[String]()

    // then checks for the ' ' character
    // yes there are 20 lines of code to check for the ' ' and to transform 'c' to c
    while(temp.length > 0) {
      if(temp.length > 1)
        if(temp.apply(0).equals("'") && temp.apply(1).equals("'")) {
          output = output ++ List(" ")
          temp = temp.drop(2)
        } else {
          var transitionString = temp.apply(0)
          if(temp.apply(0).length == 3)
            if(temp.apply(0).apply(0).equals('\''))
              transitionString = temp.apply(0).apply(1).toString

          output = output ++ List(transitionString)
          temp = temp.drop(1)
        }
      else {
        var transitionString = temp.apply(0)
        if(temp.apply(0).length == 3)
          if(temp.apply(0).apply(0).equals('\''))
            transitionString = temp.apply(0).apply(1).toString

        output = output ++ List(transitionString)
        temp = temp.drop(1)
      }
    }
    output
  }

  // builds the nfs recursively from ast tree
  def buildNFA(tree: ASTree): Nfa[Int] = {
    tree match {
      case ASTEmpty => buildAcceptVoid()
      case ASTNode(string, left, right) => {
        string match {
          case "STAR" => buildAcceptStar(buildNFA(left))
          case "PLUS" => buildAcceptPlus(buildNFA(left))
          case "MAYBE" => buildAcceptMaybe(buildNFA(left))
          case "UNION" => buildAcceptUNION(buildNFA(left), buildNFA(right))
          case "CONCAT" => buildAcceptCONCAT(buildNFA(left), buildNFA(right))
          case "eps" => buildAcceptEpsilon()
          case "'" => buildAcceptAtom('\'') //special case for ''' character
          case str => buildAcceptAtom(str.split("'").apply(0).charAt(0))
        }
      }
    }
  }

  // ==== Here start the functions which build nodes for every single regex type like in the lecture ====

  def buildAcceptVoid(): Nfa[Int] = {
    val startState = 0
    val endState = 1
    val transitions = Map[Int, Map[String, List[Int]]]()
    val states = Set(0,1)

    new Nfa[Int](states, transitions, startState, Set(endState))
  }

  def buildAcceptAtom(str: Char): Nfa[Int] = {
    val startState = 0
    val endStates = 1
    val states = Set(startState) ++ Set(endStates)
    var transitions = Map[Int, Map[String, List[Int]]]()
    transitions = transitions + (0 -> Map(str.toString -> List(1)))

    new Nfa(states, transitions, startState, Set(endStates))
  }

  def buildAcceptEpsilon(): Nfa[Int] = {
    val startState = 0
    val endStates = 0
    val states = Set(startState) ++ Set(endStates)
    val transitions = Map[Int, Map[String, List[Int]]]()

    new Nfa(states, transitions, startState, Set(endStates))
  }

  def buildAcceptStar(e: Nfa[Int]): Nfa[Int] = {
    val startState = e.getStates.max + 1
    val endState = startState + 1
    val states = Set(startState, endState) ++ e.getStates
    var transitions = e.getTransitions + ((startState, Map("eps" -> List(e.getStartState, endState))),
                    (e.getEndState.head, Map("eps" -> List(e.getStartState, endState))))

    new Nfa(states, transitions, startState, Set(endState))
  }

  def buildAcceptPlus(e: Nfa[Int]): Nfa[Int] = {
    var atomNFA = e
    var epsilonNFA = e.map(x => x + e.getStates.max + 1)

    var states = atomNFA.getStates ++ epsilonNFA.getStates
    states = states + (states.max + 1)
    val endState = states.max
    val startState = atomNFA.getStartState
    var transitions = atomNFA.getTransitions ++ epsilonNFA.getTransitions

    var plusNFA = new Nfa[Int](states, transitions, startState, Set(endState))
    plusNFA = plusNFA.updateTransition(atomNFA.getEndState.head, epsilonNFA.getStartState, "eps").
                      updateTransition(atomNFA.getEndState.head, endState, "eps").
                      updateTransition(epsilonNFA.getEndState.head, epsilonNFA.getStartState, "eps").
                      updateTransition(epsilonNFA.getEndState.head, endState, "eps")
    plusNFA
  }

  def buildAcceptMaybe(e: Nfa[Int]): Nfa[Int] = {
    val startState = (e.getStates.max + 1)
    val endState = startState + 1
    var states = e.getStates + startState + endState

    var maybeNFA = new Nfa[Int](states, e.getTransitions, startState, Set(endState))
    maybeNFA = maybeNFA.updateTransition(startState, e.getStartState, "eps").
                        updateTransition(startState, endState, "eps").
                        updateTransition(e.getEndState.head, endState, "eps")

    maybeNFA
  }

  def buildAcceptUNION(e1: Nfa[Int], e2: Nfa[Int]): Nfa[Int] = {
    var e1NFA = e1
    var e2NFA = e2.map(x => x + e1.getStates.max + 1)

    val startState = e2NFA.getStates.max + 1
    val endState = startState + 1
    val transitions = e1NFA.getTransitions ++ e2NFA.getTransitions
    val states = e1NFA.getStates ++ e2NFA.getStates + startState + endState

    var unionNFA = new Nfa[Int](states, transitions, startState, Set(endState))
    unionNFA = unionNFA.updateTransition(startState, e1NFA.getStartState, "eps")
                        .updateTransition(startState, e2NFA.getStartState, "eps")
                        .updateTransition(e1NFA.getEndState.head, endState, "eps")
                        .updateTransition(e2NFA.getEndState.head, endState, "eps")
    unionNFA
  }

  def buildAcceptCONCAT(e1: Nfa[Int], e2: Nfa[Int]): Nfa[Int] = {
    var e1NFA = e1
    var e2NFA = e2.map(x => x + e1.getStates.max + 1)

    val startState = e1NFA.getStartState
    val endState = e2NFA.getEndState
    val transitions = e1NFA.getTransitions ++ e2NFA.getTransitions
    val states = e1NFA.getStates ++ e2NFA.getStates

    var concatNFA = new Nfa[Int](states, transitions, startState, endState)
    concatNFA = concatNFA.updateTransition(e1NFA.getEndState.head, e2NFA.getStartState, "eps")

    concatNFA
  }

}