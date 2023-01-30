class Dfa[A] (states: Set[A], transitions: List[(A, (String, A))], startState: A, endStates: Set[A], sinkState: A, nfa: Nfa[A]){

  def map[B](f: A => B) : Dfa[B] = {
    def op(curr: (A, (String, A))): (B, (String, B)) = {
      val state = f(curr._1)
      val transition = (curr._2._1, f(curr._2._2))

      (state, transition)
    }

    val statesB = this.states.map(f)
    val startStateB = f(this.startState)
    val endStatesB = this.endStates.map(f)
    val sinkStateB = f(this.sinkState)
    val nfaB = nfa.map(f)

    val transitionsB = this.transitions.map(op)

    new Dfa[B](statesB, transitionsB, startStateB, endStatesB, sinkStateB, nfaB)
  }

  def next(state:A, c: Char): A = {

    // possible next state
    val possibleNext = this.transitions.filter(elem => elem._1.equals(state)).filter(elem => elem._2._1.equals(c.toString))

    if(possibleNext.length > 0)
      possibleNext.apply(0)._2._2
    else
      this.sinkState
  }

  def accepts(str: String): Boolean = {
    var string = str
    var currState = this.startState

    while(string.length > 0 && !currState.equals(this.sinkState)) {
      currState = next(currState, string.charAt(0))
      string = string.drop(1)
    }

    isFinal(currState)
  }

  def getStates : Set[A] = states

  def isFinal(state: A): Boolean = endStates.contains(state)

  def getTransitions: List[(A, (String, A))] = this.transitions
  def getEndStates: Set[A] = this.endStates
  def getStartState: A = this.startState
  def getNfa: Nfa[A] = this.nfa
  def getSinkState: A = this.sinkState
}

// This is a companion object to the Dfa class. This allows us to call the method fromPrenex without instantiating the Dfa class beforehand.
// You can think of the methods of this object like static methods of the Dfa class
object Dfa {
  def fromPrenex(str: String): Dfa[Int] = {

    // gets nfa
    val nfa = Nfa.fromPrenex(str)

    nfaToDfa(nfa).map(x => x.toInt)
  }

  def nfaToDfa(nfa: Nfa[Int]): Dfa[Long] = {
    // gets alphabet
    val alphabet: List[String] = nfa.getTransitions.foldRight(List(): List[String])((elem, acc) => elem._2.keySet.toList ++ acc)
      .toSet.filter(elem => !elem.equals("eps")).toList

    // builds the elements for dfa
    val startStateSubset = buildStartState(nfa)
    val startStateDfa = convertSetToIntState(startStateSubset)
    var endStatesDfa = Set(): Set[Long]
    var statesDfa = Set(): Set[Long]

    var transitionsDfaWithSubsets: List[(Set[Int], (String, Set[Int]))] = List()

    // gets all transitions by traversing all the possible states (accessible from startState) and all the alphabet

    var elemsToProcess: List[Set[Int]] = List(startStateSubset)
    var processedElems: List[Set[Int]] = List()

    while(elemsToProcess.nonEmpty) {
      var elem = elemsToProcess.apply(0);
      elemsToProcess = elemsToProcess.drop(1)
      processedElems = processedElems ++ List(elem)

      for(str <- alphabet) {
        val nextGroup = getNextGroup(nfa, elem, str)
        if(!elemsToProcess.contains(nextGroup) && !processedElems.contains(nextGroup))
          elemsToProcess = elemsToProcess ++ List(nextGroup)

        if(nextGroup.toList.length > 0)
          transitionsDfaWithSubsets = transitionsDfaWithSubsets ++ List((elem, (str, nextGroup)))
      }
    }

    // build endStates and states from the traversed states
    endStatesDfa = processedElems.filter(elem => elem.contains(nfa.getEndState.head)).map(elem => convertSetToIntState(elem)).toSet
    statesDfa = processedElems.map(elem => convertSetToIntState(elem)).toSet

    val transitionsDfa = transitionsDfaWithSubsets.map(elem => convertTransition(elem))

    new Dfa[Long](statesDfa, transitionsDfa, startStateDfa, endStatesDfa, 0, nfa.map(x => x.toLong))
  }

  // builds the start state by getting the start state from nfa and getting al eps-transitions from it
  def buildStartState(nfa: Nfa[Int]): Set[Int] = {
    var startStateSubset: Set[Int] = nfa.getEpsTransition(nfa.getStartState)
    var output = startStateSubset ++ Set(nfa.getStartState)

    while(!output.equals(startStateSubset)) {
      startStateSubset = output

      for(elem <- startStateSubset) {
        output = output ++ nfa.getEpsTransition(elem)
      }
    }

    output
  }

  // gets the next transition group (the group of states a group of states will get into)
  def getNextGroup(nfa: Nfa[Int], curr: Set[Int], str: String): Set[Int] = {

    // computes the next group for a state
    def op(state: Int): Set[Int] = {
        var nextStates = nfa.next(state, str.charAt(0))
        var output: Set[Int] = nextStates

        for(elem <- nextStates) {
          output = output ++ nfa.getEpsTransition(elem)
        }

        while(!nextStates.equals(output)) {
          nextStates = output
          for(elem <- nextStates) {
            output = output ++ nfa.getEpsTransition(elem)
          }
        }
        output
      }

      // computes op for every state
       curr.foldRight(Set[Int]())((state, acc) => acc ++ op(state))
  }

  def getNextGroupForLexer(nfa: Nfa[Int], curr: Set[Int], str: String): Set[Int] = {

    def op(state: Int): Set[Int] = {
      var nextStates = nfa.next(state, str.charAt(0)) ++ nfa.getEpsTransition(state)
      var output: Set[Int] = nextStates

      for(elem <- nextStates) {
        output = output ++ nfa.getEpsTransition(elem)
      }

      while(!nextStates.equals(output)) {
        nextStates = output
        for(elem <- nextStates) {
          output = output ++ nfa.getEpsTransition(elem)
        }
      }
      output
    }

    curr.foldRight(Set[Int]())((state, acc) => acc ++ op(state))
  }

  // converts a subst into an its using powers of 2 (binary -> 1 means it includes the state, 0 not)
  def convertSetToIntState(set: Set[Int]): Long = {
    set.foldRight(0)((elem, acc) => acc + Math.pow(2, elem).toInt)
  }

  def convertIntStateToSet(state: Long): Set[Int] = {
    var binaryString = state.toBinaryString
    var outputSet: Set[Int] = Set()

    for(i <- Range(0, binaryString.length)) {
      if(binaryString.reverse.apply(i).equals('1'))
        outputSet = outputSet ++ Set(i)
    }

    outputSet
  }

  // converts a transition from subsets type states to int type states
  def convertTransition(transition: (Set[Int], (String, Set[Int]))): (Long, (String, Long)) = {
    (convertSetToIntState(transition._1), (transition._2._1, convertSetToIntState(transition._2._2)))
  }
}
