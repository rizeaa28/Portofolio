import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;

public class SmallWorker implements Runnable {
    Order productOriginOrder;
    BufferedWriter productsWriter;
    BufferedReader productsReader;

    public SmallWorker(Order productOriginOrder, BufferedWriter productsWriter, BufferedReader productsReader) {
        this.productOriginOrder = productOriginOrder;
        this.productsWriter = productsWriter;
        this.productsReader = productsReader;
    }

    @Override
    public void run() {

        while(true) {
            try {
                if (!productsReader.ready()) break;
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            String line = "";
            try {
                line = productsReader.readLine();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            if(line.length() != 0) {
                String[] productDetails = line.split(",");

                if(productDetails[0].equals(productOriginOrder.name)) {
                    productOriginOrder.hasBeenDelivered();
                    try {
                        productsWriter.write(productOriginOrder.name + "," + productDetails[1] + ",shipped\n");
                    } catch (IOException e) {
                        throw new RuntimeException(e);
                    }
                    break;
                }
            }
        }
        productOriginOrder.hasFinishedTask();
    }
}
