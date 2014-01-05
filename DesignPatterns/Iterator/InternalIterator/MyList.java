class MyList {
        static class Node { Node next; Object value; }
        Node listHeader;
        void apply(Command c) {
          for (Node current = listHeader; current != null; current = current.next) {
            c.apply(current.value);
          }
        }
      }
