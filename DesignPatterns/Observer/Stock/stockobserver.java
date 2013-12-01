public class stockobserver implements observer {

  private double ibmprice;
  private double appleprice;
  private double googleprice;


  private static int observerIDTracker = 0 ;
  private int ID;
  private subject stockgrabber; 

  public stockobserver(subject stockgrabber) {
    this.stockgrabber = stockgrabber;
    this.ID = ++observerIDTracker;
    System.out.println("New Observer" + this.ID);

    stockgrabber.register(this);
  }

  public void update(double ibmprice, double appleprice, double googleprice) {
    this.ibmprice =  ibmprice;
    this.appleprice =  appleprice;
    this.googleprice =  googleprice;

    System.out.println("\nIbmprice " + this.ibmprice  + "\nApple Price " + this.appleprice + "\nGoogle Price "  + this.googleprice + "\n\n");
  }
}
