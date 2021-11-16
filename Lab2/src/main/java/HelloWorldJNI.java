public class HelloWorldJNI {

    static {
        System.loadLibrary("native");
    }
    
    public static void main(String[] args) {
    	int[][] m1 = MatrixGeneratorUtil.generateMatrix(2000, 2000);
    	int[][] m2 = MatrixGeneratorUtil.generateMatrix(2000, 2000);
    	double[][] result = new double[m1.length][m2[0].length];
       new HelloWorldJNI().matmulti(2000,m1,m2);
		/*
		 * Date start = new Date();
		 * 
		 * 
		 * 
		 * ParallelThreadFactory.multiply(m1, m2, result);
		 * 
		 * Date end = new Date(); System.out.println("\nTime taken in milli seconds: " +
		 * (end.getTime() - start.getTime()));
		 */
        
    }

    // Declare a native method sayHello() that receives no arguments and returns void
    public native int[][] matmulti(int n,int[][] m1,int[][] m2);
}