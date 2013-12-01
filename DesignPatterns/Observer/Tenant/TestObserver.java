public class TestObserver {
  public static void main (String[] args) {
  
    Washer w = new Washer();
    Tenant t = new Tenant (w);
    Manager m = new Manager (w);

  }

}
