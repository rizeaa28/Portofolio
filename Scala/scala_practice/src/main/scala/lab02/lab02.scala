package lab02

object Main extends App {

  //Exercise 1: foldWith
  def foldWith (op: (Int,Int) => Int)(start: Int, stop: Int): Int = {
    def tail_fold(crt: Int, acc: Int): Int  = {
      if (crt == stop) acc
      else tail_fold(crt + 1, op(crt, acc))
    }
    tail_fold(start, 0)
  }

  printf("Sum of first 6 numbers: " + foldWith(_ + _)(1, 6) + "\n")

  // Exercise 2: foldConditional

  def foldConditional(op: (Int,Int) => Int, p: Int => Boolean)(start: Int, stop: Int): Int = {
    def tail_fold(crt: Int, acc: Int): Int  = {
      if (crt == stop) acc
      else if (p(crt)) tail_fold(crt + 1, op(crt, acc))
      else tail_fold(crt+1, acc)
    }
    tail_fold(start, 0)
  }

  printf("Sum of the even numbers between 2 and 12: " + foldConditional(_ + _, _ % 2 == 0)(2, 12) + "\n")

  // Exercise 4: foldMap
  def foldMap(op: (Int,Int) => Int, f: Int => Int)(start: Int, stop: Int): Int = {
    def tail_fold(crt: Int, acc: Int): Int  = {
      if (crt == stop) op(f(crt), acc)
      else tail_fold(crt + 1, op(f(crt), acc))
    }
    tail_fold(start, 0)
  }

  printf("Sum of squares until 6: " + foldMap(_ + _, x => x * x)(0, 6) + "\n")

  // Exercise 5: sumSquares
  def sumSquares(n: Int): Int = foldMap(_ + _, x => x * x)(0, n)

  // Exercise 6: hasDivisor
  def hasDivisor(k: Int, start: Int, stop: Int): Boolean = {
    foldMap(_ + _, x => if (x % k == 0) 1 else 0)(start, stop) > 0
  }

  printf("Does (5, 10) have a multiple of 3?: " + hasDivisor(3, 5, 10) + "\n")

  
}
