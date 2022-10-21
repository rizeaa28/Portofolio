package lab03

import scala.collection.mutable.ListBuffer

object Main extends App {

  // Exercise 1
  def atLeastk(k: Int, l: List[Int]): Boolean =
    if (k == 0) true
    else l.length >= k

  //Exercise 2
  def take(n: Int, l: List[Int]): List[Int] = {
    if (!atLeastk(n, l)) l
    else {
      var out: List[Int] = List()
      for(i <- Range.inclusive(0, n-1)) {
        out = l.apply(i) :: out
      }
      out.reverse
    }
  }

  printf("First 3 elements of (1, 2, 3, 4, 5): " + take(3, List(1, 2, 3, 4, 5)) + "\n")

  // Exercise 3
  def drop(n: Int, l: List[Int]): List[Int] = {
    if (!atLeastk(n, l)) List()
    else {
      var out: List[Int] = l
      for(i <- Range.inclusive(0, n-1)) {
        out = out.tail
      }
      out
    }
  }

  printf("Last 3 elements of (1, 2, 3, 4, 5): " + drop(2, List(1, 2, 3, 4, 5)) + "\n")

  // Exercise 4
  def takeP(p: Int => Boolean)(l: List[Int]): List[Int] =
    l.foldRight(List():List[Int])((elem, acc) => if (p(elem)) acc.::(elem) else acc)

  printf("Even elements of (1, 2, 3, 4, 5, 6, 7, 8): " + takeP(_ % 2 == 0)(List(1, 2, 3, 4, 5, 6, 7, 8)) + "\n")

  // Exercise 5
  def part(p: Int => Boolean)(l: List[Int]): (List[Int], List[Int]) =
    (takeP(p)(l), takeP(x => !p(x))(l))

  // Exercise 6: Gradebook functions
  type Gradebook = List[(String,Int)]

  def increment(g: Gradebook): Gradebook =
    g.map(student => if (student._2 >= 5) (student._1, student._2 + 1) else student)

  def average(g: Gradebook): Double = g.foldRight(0:Double)((student, acc) => acc + student._2) / g.length

  def percentage(g: Gradebook): (Double,Double) = {
    val passed: Double = g.foldRight(0)((student, acc) => if (student._2 >= 5) acc + 1 else acc)
    val failed: Double = g.length - passed
    ((failed * 100) / g.length, (passed * 100) / g.length)
  }

  def pass(g: Gradebook): List[String] = g.filter(student => student._2 >= 5).map(student => student._1)

  def mergeSort(l: Gradebook): Gradebook = {
    def merge(u: Gradebook, v: Gradebook): Gradebook = {
      var i = 0
      var j = 0
      val out: ListBuffer[(String, Int)] = ListBuffer()

      while (i < u.length && j < v.length) {
        if (u.apply(i)._2 > v.apply(j)._2) {
          out.addOne(v.apply(j))
          j = j + 1
        } else {
          out.addOne(u.apply(i))
          i = i + 1
        }
      }

      while (i < u.length) {
        out.addOne(u.apply(i))
        i = i + 1
      }

      while (j < v.length) {
        out.addOne(v.apply(j))
        j = j + 1
      }

      out.toList
    }

    if (l.length == 1) l
    else merge(mergeSort(l.take(l.length / 2)), mergeSort(l.drop(l.length / 2)))
  }

  val gradebook = List(("G",3), ("F", 10), ("M",6), ("P",4))
  printf(mergeSort(gradebook) + "\n")

  def honorsList(g: Gradebook): List[String] = pass(mergeSort(g)).reverse
}

