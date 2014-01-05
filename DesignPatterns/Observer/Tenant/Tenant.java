class Tenant extends Listener {
  public void updateOnStart() {}
  public void updateOnFinish() {}

  Tenant (Washer w) {
    registerForWasher(w);
  }
}
