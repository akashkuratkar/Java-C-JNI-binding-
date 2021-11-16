import java.util.Random;

public class MatrixGeneratorUtil {

 public static int[][] generateMatrix(int rows, int columns) {

  // output array to store the matrix values
  int[][] result = new int[rows][columns];

  // TO generate a random integer.
  Random random = new Random();

  // adding values at each index.
  for (int i = 0; i < rows; i++) {
   for (int j = 0; j < columns; j++) {

    result[i][j] = random.nextInt(100) * 10;

   }
  }

  // returning output.
  return result;
 }

 // to print the matrix
 public static void print(double[][] matrix) {

  System.out.println();

  int rows = matrix.length;
  int columns = matrix[0].length;

  for (int i = 0; i < rows; i++) {
   for (int j = 0; j < columns; j++) {
    System.out.print(matrix[i][j] + "  ");
   }
   System.out.println();
  }

 }

}