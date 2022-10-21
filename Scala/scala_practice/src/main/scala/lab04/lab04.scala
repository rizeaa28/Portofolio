package lab04

object Main extends App {

  // Exercise 1: Nats
  trait Nat {}
  case object Zero extends Nat {}
  case class Succ(n: Nat) extends Nat {}

  def add(n: Nat, m: Nat): Nat = {
    n match {
      case Zero => m
      case Succ(x) => add(x, Succ(m))
    }
  }

  def toInt(n: Nat): Int = {
    n match {
      case Zero => 0
      case Succ(x) => 1 + toInt(x)
    }
  }

  def fromInt(i: Int): Nat = {
    i match {
      case 0 => Zero
      case _ => Succ(fromInt(i - 1))
    }
  }

  // Exercise 2: Binary Search Trees
  trait ITree {}
  case object Empty extends ITree
  case class INode(key: Int, left: ITree, right: ITree) extends ITree

  val tree = INode(5, INode(2, INode(1, Empty, Empty), INode(3, Empty, Empty)), INode(7, Empty, INode(9, Empty, Empty)))

  def size(t: ITree): Int = {
    t match {
      case Empty => 0
      case INode(_, left, right) => 1 + size(left) + size(right)
    }
  }

  def contains(x: Int, t: ITree): Boolean = {
    t match {
      case Empty => false
      case INode(k, left, right) => (k == x) || contains(x, left) || contains(x, right)
    }
  }

  def insert(x: Int, t: ITree): ITree = {
    t match {
      case Empty => INode(x, Empty, Empty)
      case INode(k, right, left) => {
        if (k >= x)
          INode(k, insert(x, left), right)
        else
          INode(k, left, insert(x, right))
      }
    }
  }

  def flatten(t: ITree): List[Int] = {
    t match {
      case Empty => List()
      case INode(k, left, right) => flatten(left) ::: List(k) ::: flatten(right)
    }
  }

  def depth(t: ITree): Int = {
    t match {
      case Empty => 0
      case INode(_, left, right) => 1 + depth(left).max(depth(right))
    }
  }

  def minimum(t: ITree): Int = {
    if (depth(t) == 0) -1
    else t match {
      case INode(x, left, _) => if (depth(left) == 0) x else minimum(left)
    }
  }

  def maximum(t: ITree): Int = {
    if (depth(t) == 0) -1
    else t match {
      case INode(x, _, right) => if (depth(right) == 0) x else maximum(right)
    }
  }

}
