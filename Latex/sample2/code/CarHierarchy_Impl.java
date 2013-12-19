interface ICarElement {
  public void display();
}

class Wheel implements ICarElement {
  public void display() {
    System.out.println("Printing Wheel");
  }
}
