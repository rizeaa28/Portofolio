import java.io.*;
import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Semaphore;

public class BigWorker implements Runnable{
    ExecutorService resolveOrders;
    ArrayList<Order> orders = new ArrayList<Order>();
    BufferedReader ordersReader;
    BufferedWriter productsWriter;
    BufferedWriter ordersWriter;
    int id;
    String inputFolder;

    public BigWorker(BufferedReader ordersReader, ExecutorService resolveOrders, int id, String inputFolder,
                     BufferedWriter productsWriter, BufferedWriter ordersWriter) {
        this.ordersReader = ordersReader;
        this.resolveOrders = resolveOrders;
        this.id = id;
        this.inputFolder = inputFolder;
        this.productsWriter = productsWriter;
        this.ordersWriter = ordersWriter;
    }

    @Override
    public void run() {

        while(true) {
            try {
                if (!ordersReader.ready()) break;
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            if(Tema2.getMinOfOrders() == Tema2.nrOfProductsPerBigWorker[this.id]) {
                try {
                    String orderLine = ordersReader.readLine();

                    if(orderLine != null) {
                        String[] orderDetails =  orderLine.split(",");
                        if(Integer.parseInt(orderDetails[1]) != 0) {
                            this.orders.add(new Order(orderDetails[0], orderDetails[1], this.id));
                            Tema2.nrOfProductsPerBigWorker[id] = Tema2.nrOfProductsPerBigWorker[id] + Integer.parseInt(orderDetails[1]);
                        }
                    }
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }


            }
        }

        try {
            for(int i = 0; i < this.orders.size(); i++) {
                BufferedReader productsReader = null;
                FileReader productsreads = new FileReader("" + inputFolder + "/order_products.txt");
                productsReader = new BufferedReader(productsreads);

                for(int j = 0; j < this.orders.get(i).nrOfProducts; j++) {
                    resolveOrders.submit(new SmallWorker(this.orders.get(i), productsWriter, productsReader));
                }
            }
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }


        while(Tema2.getNrOfProductsOfBigWorker(id) >= 0) {
            boolean hasWritten = true;
            while(hasWritten) {
                hasWritten = false;
                for(int i = 0; i < this.orders.size(); i++) {
                    if(this.orders.get(i).isFinished() && !this.orders.get(i).hasBeenWritten) {
                        try {
                            ordersWriter.write(this.orders.get(i).name + "," + this.orders.get(i).nrOfProducts + ",shipped\n");
                            this.orders.get(i).hasBeenWritten = true;
                            hasWritten = true;
                        } catch (IOException e) {
                            throw new RuntimeException(e);
                        }
                    }
                }
            }
            if(Tema2.getNrOfProductsOfBigWorker(id) == 0)
                Tema2.nrOfProductsPerBigWorker[id]--;

        }

        boolean hasWritten = true;
        while(hasWritten) {
            hasWritten = false;
            for(int i = 0; i < this.orders.size(); i++) {
                if(this.orders.get(i).isFinished() && !this.orders.get(i).hasBeenWritten) {
                    try {
                        ordersWriter.write(this.orders.get(i).name + "," + this.orders.get(i).nrOfProducts + ",shipped\n");
                        this.orders.get(i).hasBeenWritten = true;
                        hasWritten = true;
                    } catch (IOException e) {
                        throw new RuntimeException(e);
                    }
                }
            }
        }

    }
}
