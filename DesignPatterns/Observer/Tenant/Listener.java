abstract class Listener {
  abstract void updateOnStart();
  abstract void updateOnFinish();
  void registerForWasher(Washer w) {
    w.addListener(this);
  }
}
