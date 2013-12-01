class Manager implements Listener {
        public void updateonStart() {}
        public void updateonFinish() {}

        private Washer W;
        Manager (Washer w) {
          this.W = w;
          W.addListener(this);
        }
      }
