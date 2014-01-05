class Body implements ICarElement {
    public void accept(ICarElementVisitor visitor) {
        System.out.println("In accept of Body ");
        visitor.visit(this);
    }
}
