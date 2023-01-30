public class Order {
    String name;
    int nrOfProducts;
    int hasDelivered;
    int bossID;
    boolean hasBeenWritten;

    public Order(String name, String nrOfProducts, int bossID) {
        this.name = name;
        this.nrOfProducts = Integer.parseInt(nrOfProducts);

        this.hasDelivered = 0;
        this.bossID = bossID;
        this.hasBeenWritten = false;
    }

    public synchronized void hasBeenDelivered() {
        hasDelivered++;
    }

    public synchronized void hasFinishedTask() {
        Tema2.nrOfProductsPerBigWorker[bossID]--;
    }

    public synchronized boolean isFinished() {
        return hasDelivered == nrOfProducts;
    }
}
