class Book {
  private double weight;
  private String type;
  public Book(double weight, String type) {
    this.weight = weight;
    this.type = type;
  }
  public double getWeight() {
    return weight;
  }
  public void printType() {
    System.out.println(type);
  }
}
class FictionBook extends Book {
  public FictionBook(double weight) {
    super(weight, "Fiction");
  }
}
class NonFictionBook extends Book {
  public NonFictionBook(double weight) {
    super(weight, "Non-Fiction");
  }
}
