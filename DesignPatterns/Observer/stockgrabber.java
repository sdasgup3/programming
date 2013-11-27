import java.util.ArrayList;

public class stockgrabber implements subject {

  private ArrayList<observer> observers;
  private double ibmprice;
  private double appleprice;
  private double googleprice;

  public stockgrabber() {
    observers = new ArrayList<observer>();
  }

  public void register(observer newbserver) {
    observers.add(newbserver);
  }
  public void unregister(observer delobserver) {
    int observerIndex = observers.indexOf(delobserver);
    System.out.println("Observer" + (observerIndex + 1) + "deleted");
    observers.remove(observerIndex);
  }
  public void notifyobserver() {
    for (observer obs: observers) {
      obs.update(ibmprice, appleprice, googleprice);
    }
  }

  public void setibmprice(double price) {
    this.ibmprice = price;
    notifyobserver();
  }
  public void setappleprice(double price) {
    this.appleprice = price;
    notifyobserver();
  }
  public void setgoogleprice(double price) {
    this.googleprice = price;
    notifyobserver();
  }
}
