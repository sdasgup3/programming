class Manager extends Listener {
        public void updateOnStart() {}
        public void updateOnFinish() {}

        Manager (Washer w) {
          registerForWasher(w);
        }
      }
