class Wheel implements ICarElement {
  public void accept(ICarElementVisitor visitor) {
    visitor.visit(this);
  }
}

class Body implements ICarElement {
  public void accept(ICarElementVisitor visitor) {
    visitor.visit(this);
  }
}

class Car implements ICarElement {
  public void accept(ICarElementVisitor visitor) {
    for (ICarElement elem: elmnts) {
      elem.accept(visitor);
    }
    visitor.visit(this);
  }
}
