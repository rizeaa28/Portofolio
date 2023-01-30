import java.io.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Tema2 {
    static int[] nrOfProductsPerBigWorker;

    public static void main(String[] args) {

        String inputFolder = args[0];
        int nrOfThreads = Integer.valueOf(args[1]);

        nrOfProductsPerBigWorker = new int[nrOfThreads];

        ExecutorService resolveOrders = Executors.newFixedThreadPool(nrOfThreads);

        BufferedReader ordersReader;
        try {
            FileReader ordersreads = new FileReader("" + inputFolder + "/orders.txt");
            ordersReader = new BufferedReader(ordersreads);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }

        BufferedWriter productWriter;
        try {
            FileWriter productswrites = new FileWriter("order_products_out.txt");
            productWriter = new BufferedWriter(productswrites);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        BufferedWriter ordersWriter;
        try {
            FileWriter orderswrites = new FileWriter("orders_out.txt");
            ordersWriter = new BufferedWriter(orderswrites);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        Thread[] bigWorkers = new Thread[nrOfThreads];
        for(int i = 0; i < nrOfThreads; i++) {
            bigWorkers[i] = new Thread(new BigWorker(ordersReader, resolveOrders, i, inputFolder, productWriter, ordersWriter));
            bigWorkers[i].start();
        }

        for(int i = 0; i < nrOfThreads; i++) {
            try {
                bigWorkers[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        resolveOrders.shutdown();
        try {
            ordersReader.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        try {
            productWriter.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        try {
            ordersWriter.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public synchronized static int getMinOfOrders() {
        int min = nrOfProductsPerBigWorker[0];

        for(int i = 1; i < nrOfProductsPerBigWorker.length; i++) {
            if(nrOfProductsPerBigWorker[i] < min)
                min = nrOfProductsPerBigWorker[i];
        }

        return min;
    }

    public synchronized static int getNrOfProductsOfBigWorker(int id) {
        return nrOfProductsPerBigWorker[id];
    }
}
