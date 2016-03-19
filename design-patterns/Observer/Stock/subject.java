//Stock marhet with thousands of stocks needs  to sens updates to objects representing individual stocks
//The subject sends many stocks to the observers
//Te observers takes the ones they want and use them

public interface subject{
  public void register(observer o);
  public void unregister(observer o);
  public void notifyobserver();
}





