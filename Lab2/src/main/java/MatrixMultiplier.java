public class MatrixMultiplier {
	 static {
	        System.loadLibrary("native");
	    }
    public static void main(String[] args) throws InterruptedException {
 
        System.loadLibrary("native");

        // Warming up JVM
        Thread.sleep(10000);
        System.out.println("Estimation output:");
        System.out.println("Warming up JVM:");
		printTimeMeasurementsFor(500);

        // if (compareResults(500)) System.out.println("Results are the same");
        // else System.err.println("Error! Different results!");

        System.out.println("N\tJNI\t\tPure Java\t\tJava Multithreaded\t\tJNI Multithreaded");
        for (int n = 500; n <= 4000; n *=2) {

            printTimeMeasurementsFor(n);
            }
    }
    
    private native int[][] multiply(int[][] a, int[][] b);
    private native void matmultithreaded(int n,int[][] a, int[][] b, int[][] res);
    
    
    private int[][] pureJavaMultiply(int[][] a, int[][] b) {
        int[][] res = new int[a.length][a.length];
        for (int i = 0; i < a.length; i++) {
            for (int j = 0; j < a.length; j++) {
                res[i][j] = 0;
                for (int k = 0; k < a.length; k++) {
                    res[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return res;
    }

    public long measureJni(int[][] a, int[][] b) {
        long tbegin = System.currentTimeMillis();
        multiply(a, b);
        return System.currentTimeMillis() - tbegin;
    }
    
    public long measureJniWithMultithreading(int n,int[][] a, int[][] b) {
        long tbegin = System.currentTimeMillis();
        multiply(a, b);
        return System.currentTimeMillis() - tbegin;
    }

    public long measurePureJava(int[][] a, int[][] b) {
        long tbegin = System.currentTimeMillis();
        pureJavaMultiply(a, b);
        return System.currentTimeMillis() - tbegin;
    }
    
    public long measurePureJavaMultithreaded(int[][] a, int[][] b,int[][] result) {
    	long tbegin = System.currentTimeMillis();
    	ParallelThreadFactory.multiply(a,b,result);
    	return System.currentTimeMillis() - tbegin;
    }
    public long measurePureJavaWithMultithreading(int n,int[][] a, int[][] b, int[][] c) {
        long tbegin = System.currentTimeMillis();
        matmultithreaded(n,a, b,c);
        return System.currentTimeMillis() - tbegin;
    }

    private static void printTimeMeasurementsFor(int n) {
        MatrixMultiplier c = new MatrixMultiplier();
        n=500;
        // Generate sample data
        int[][] a = new int[n][n];
        int[][] b = new int[n][n];
        
        int[][] res = new int[n][n];
        
        for (int i = 0; i < b.length; i++) {
            for (int j = 0; j < b.length; j++) {
                a[i][j] = (i % 10) * 10 + j;
                b[i][j] = 2 + j;
            }
        }

        // Benchmark
        int ATTEMPS = 50;
        long tJni = 0, tPureJava = 0,tMultiThreadJni=0,tPureJavaMultithreaded=0;
       for (int i = 0; i < ATTEMPS; i++) {
        long tbegin = System.currentTimeMillis();
        	tJni = c.measureJni(a,b);
            tPureJava = c.measurePureJava(a, b);
            tPureJavaMultithreaded = c.measurePureJavaMultithreaded(a, b,res);
            //tMultiThreadJni = c.measurePureJavaWithMultithreading(500,a,b,res);
       }
           // System.out.println("Back in Java");
        System.out.println(n + "\t" + ((tJni)) + "\t\t" + ((tPureJava )) + "\t\t\t" + ((tPureJavaMultithreaded)) + "\t\t\t" + (tMultiThreadJni));
    }
    
    private static boolean compareResults(int n) {
        MatrixMultiplier c = new MatrixMultiplier();
        // Generate sample data
        int[][] a1 = new int[n][n];
        int[][] b1 = new int[n][n];
        for (int i = 0; i < b1.length; i++) {
            for (int j = 0; j < b1.length; j++) {
                a1[i][j] = (i % 10) * 10 + j;
                b1[i][j] = 2 + j;
            }
        }
        int[][] res1 = c.multiply(a1, b1);
        int[][] res2 = c.pureJavaMultiply(a1, b1);
        for (int i = 0; i < res1.length; i++) {
            for (int j = 0; j < res1[i].length; j++) {
                if (res1[i][j] != res2[i][j]) {
                    System.err.println("Diff: [" + i + "," + j + "]: "
                            + res1[i][j] + " vs. " + res2[i][j]);
                    return false;
                }
            }
        }
        return true;
    }
}

