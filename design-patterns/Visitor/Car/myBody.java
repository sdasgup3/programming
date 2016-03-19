class myBody extends Body {
      public void accept(ICarElementVisitor visitor) {
        System.out.println("In accept of myBody ");
                visitor.visit(this);
                    }
}
