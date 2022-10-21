package lab01

object Main extends App {

  // Exercise 1: factorial function
  def fact (n: Int): Int = {
    def aux_fact(n: Int, acc: Int): Int =
      if (n == 1) acc
      else aux_fact(n-1, acc*n)
    aux_fact(n,1)
  }

  printf("6! is: " + fact(6) + "\n")

  // Exercise 2: greatest common divisor
  def gcd(a: Int, b: Int): Int = {
    if (a == b) a
    else if (a > b) gcd(a-b, b)
    else gcd(a, b-a)
  }

  printf("Greatest common divisor of 24 and 32: " + gcd(24, 32) + "\n")

  // Exercise 3: Sum of squares
  def sumSquares(n: Int): Int = {
    def addSquare(n: Int, acc: Int): Int = {
      if (n == 0) acc
      else addSquare(n-1, acc + n*n)
    }
    addSquare(n, 0)
  }

  printf("Sum of squares from 0 to 6 is: " + sumSquares(6) + "\n")

  // Newton's square root method
  def improve(xn: Double, a: Double): Double = {
    (xn + (a/xn)) / 2
  }

  def nth_guess(n: Int, a: Double): Double = {
    var x : Double = 1
    for(i <- Range.Int.inclusive(0,n,1)) {
      x = improve(x, a)
    }
    x
  }

  def acceptable(xn: Double, a: Double): Boolean = {
    Math.abs(xn*xn - a) <= 0.001
  }

  def mySqrt(a: Double): Double = {
    def improve(xn: Double): Double = (0.5) * (xn + a/xn)
    def acceptable(xn: Double): Boolean = Math.abs(xn*xn - a) <= 0.001

    def tailSqrt(estimate: Double): Double = {
      if(acceptable(estimate)) estimate
      else tailSqrt(improve(estimate))
    }

    tailSqrt(1)
  }

  printf("Squareroot of 121 is: " + mySqrt(121) + "\n")
}
