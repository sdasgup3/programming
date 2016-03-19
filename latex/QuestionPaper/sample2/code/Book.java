class FictionBook {
  private double weight;
  public FictionBook(double weight) {
    this.weight = weight;
  }
  public double getWeight() {
    return weight;
  }
  public void printType() {
    System.out.println("Fiction");
  }
}
class NonFictionBook {
  private double weight;
  public NonFictionBook(double weight) {
    this.weight = weight;
  }
  public double getWeight() {
    return weight;
  }
  public void printType() {
    System.out.println("Non-Fiction");
  }
}
