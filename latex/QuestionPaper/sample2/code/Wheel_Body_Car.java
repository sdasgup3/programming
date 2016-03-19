class Wheel implements ICarElement {
  private String name;

  public Wheel(String name) { this.name = name; }
  public String getName() { return this.name; }

  @Override
  void accept(ICarElementVisitor visitor) {
    %* \CodeToDoGeneral{} *) 





    }
}

class Body implements ICarElement {
  @Override
  void accept(ICarElementVisitor visitor) {
    %* \CodeToDoGeneral{} *)






  }
}

class Car implements ICarElement {
  ICarElement[] elmnts;

  public Car() {
    this.elmnts = new ICarElement[] {new Body(),
      new Wheel("front left"), new Wheel("front right"),
      new Wheel("back left") , new Wheel("back right")};
  }

  @Override
  void accept(ICarElementVisitor visitor) {
    %* \CodeToDoGeneral{} *)
    %* \CodeIn{// VISIT "elmnts" BEFORE "this"} *)










  }
}
