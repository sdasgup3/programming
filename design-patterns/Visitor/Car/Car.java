class Car implements ICarElement {
    ICarElement[] elements;
 
    public Car() {
        //create new Array of elements
        Body b  = new myBody(); 
        this.elements = new ICarElement[] { new Wheel("front left"), 
            new Wheel("front right"), new Wheel("back left") , 
            new Wheel("back right"), new Body(),b,  new Engine() };
    }
 
    public void accept(ICarElementVisitor visitor) {    
        for(ICarElement elem : elements) {
            elem.accept(visitor);
        }
        visitor.visit(this);    
    }
}
