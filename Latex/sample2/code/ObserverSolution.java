public void notifyFinish() {  
  for (Listener l: listeners) {
    l.updateOnFinish();
  }
}
