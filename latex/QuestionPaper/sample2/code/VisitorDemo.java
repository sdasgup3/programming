// this is changed to accept the visitor
interface ICarElement {
  void accept(ICarElementVisitor visitor);
}

interface ICarElementVisitor {
  void visit(Wheel wheel);
  void visit(Body body);
  void visit(Car car);
}

class CarElementPrintVisitor implements ICarElementVisitor {
  public void visit(Wheel wheel) {
    System.out.println("Printing " + wheel.getName() + " wheel");
  }

  public void visit(Body body) {
    System.out.println("Printing body");
  }

  public void visit(Car car) {
    System.out.println("Printing car");
  }
}

public class VisitorDemo {
  public static void main(String[] args) {
    ICarElement car = new Car();
    car.accept(new CarElementPrintVisitor());
  }
}
