class Tenant implements Listener {
  public void updateonStart() {}
  public void updateonFinish() {}

  private Washer W;
  Tenant (Washer w) {
     this.W = w;
     W.addListener(this);
  }
}