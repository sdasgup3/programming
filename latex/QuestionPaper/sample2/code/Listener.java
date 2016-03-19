abstract class Listener {
  void registerForWasher(Washer w) {
    w.addListener(this);
  }
  public abstract void updateOnFinish();
}

class Tenant extends Listener {
  Tenant(Washer w) {
    registerForWasher(w);
  }
  public void updateOnFinish() { /* e.g., pick up laundry */ }
}

class Manager extends Listener {
  Manager(Washer w) {
    registerForWasher(w);
  }
  public void updateOnFinish() { /* e.g., charge tenant */ }
}

class Washer {
  private List<Listener> listeners;
  public void addListener(Listener l) {
    listeners.add(l);
  }
  public void notifyFinish() {
    %* \CodeToDoGeneral{} *)







  }
}
