class Tenant implements Listener {
  public void updateonStart() {}
  public void updateonFinish() {}

  private Washer W;
  Tenant (private Washer w) {
     this.W = w;
     W.addListener(this);
  }
}
