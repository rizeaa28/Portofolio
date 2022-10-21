package lab05

object Main extends App {

  trait FList[A]{ // list with elements of type A
    def length: Int
    def head: A
    def tail: FList[A]
    def map[B](f: A => B): FList[B]
    // a op (b op (c op acc))
    def foldRight[B](acc: B)(op: (A,B) => B): B
    // ((acc op a) op b) op c
    def foldLeft[B](acc: B)(op: (B,A) => B): B

    def contains(e: A):Boolean =
      this.foldRight(false)(_ == e || _)

    def indexOf(e: A): Int
    def update(e: A, pos: Int): FList[A]
    def append(l: FList[A]): FList[A]
    def reverse: FList[A]
    def last: A = this.reverse.head

    def filter(p: A => Boolean): FList[A]
  }

  case class FNil[A]() extends FList[A]{
    override def length: Int = 0
    override def head: A = throw new Exception("head on empty list")
    override def tail: FList[A] = throw new Exception("head on empty list")
    override def map[B](f: A => B): FList[B] = FNil[B]
    override def foldRight[B](acc: B)(op: (A,B) => B): B = acc
    override def foldLeft[B](acc: B)(op: (B,A) => B): B = acc

    override def indexOf(e: A): Int = 0

    override def update(e: A, pos: Int): FList[A] = FNil[A]

    override def append(l: FList[A]): FList[A] = l

    override def reverse: FList[A] = FNil[A]

    override def filter(p: A => Boolean): FList[A] = FNil[A]

}

  case class Cons[A](x:A, xs:FList[A]) extends FList[A]{
    override def length = 1 + xs.length
    override def head:A = x
    override def tail:FList[A] = xs
    override def map[B](f: A => B): FList[B] =
      Cons(f(x),xs.map(f))
    override def foldRight[B](acc: B)(op: (A,B) => B): B =
      op(x, xs.foldRight(acc)(op))
    override def foldLeft[B](acc: B)(op: (B,A) => B): B =
      xs.foldLeft(op(acc,x))(op)

    override def indexOf(e: A): Int = {
      if (e == x) 1
      else 1 + xs.indexOf(e)
    }

    override def update(e: A, pos: Int): FList[A] = {
      if (pos == 0) Cons(e, xs)
      else Cons(x, xs.update(e, pos - 1))
    }

    override def append(l: FList[A]): FList[A] = {
      Cons(x, l).append(xs)
    }

    override def reverse: FList[A] = {
      var out: FList[A] = FNil[A]
      Cons(x, xs).foldLeft(out)((acc, elem) => Cons(elem, acc))
    }

    override def filter(p: A => Boolean): FList[A] = {
      var out: FList[A] = FNil[A]
      Cons(x, xs).foldRight(out)((elem, acc) => if (p(elem)) Cons(elem, acc) else acc)
    }


  }
}

