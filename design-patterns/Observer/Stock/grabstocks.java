public class grabstocks {
  public static void main (String[] args) {
  
    stockgrabber stg = new stockgrabber();
    stockobserver sto_1 = new stockobserver (stg);

    stg.setibmprice(100.00);
    stg.setappleprice(200.00);
    stg.setgoogleprice(300.00);


    stockobserver sto_2 = new stockobserver (stg);

    stg.setibmprice(1000.00);
    stg.setappleprice(2000.00);
    stg.setgoogleprice(3000.00);



  }

}
