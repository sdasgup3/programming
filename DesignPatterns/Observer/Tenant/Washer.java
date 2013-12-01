import java.util.List;
class Washer {
        private List<Listener> listeners;
        public void addListener(Listener l) {
          listeners.add(l);
        }
        public void  notifystart() {
           for (Listener l : listeners) {
             l.updateonStart();
           }
        }
        public void notifyfinish() {  
           for (Listener l : listeners) {
             l.updateonFinish();
           }
        }
      }
