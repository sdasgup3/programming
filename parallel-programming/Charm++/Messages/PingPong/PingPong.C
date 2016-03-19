#include "PingPong.decl.h"

class startMsg: public CMessage_startMsg {
  private:

  public:
    int totalInterations;
    startMsg(int i) : totalInterations(i) {}
};


class pingPongMsg: public CMessage_pingPongMsg {
  private:

  public:
    int left;
    pingPongMsg(int i) : left(i) {}
};

class Main: public CBase_Main {
  public:
    Main(CkArgMsg *msg) {
      if(msg->argc != 2) {
        CkAbort("Insufficient params: arg: integer");    
      }
      int n = atoi(msg->argv[1]);

      CProxy_Worker arr = CProxy_Worker::ckNew(2);
      arr[0].start(new startMsg(n));
      delete msg;
    }
};


class Worker: public CBase_Worker {
  public:
    Worker() { }
    Worker(CkMigrateMessage*) {}
    void start(startMsg* msg) {
      pingPongMsg* m = new pingPongMsg(msg->totalInterations);
      thisProxy[1].ping(m);
      delete msg;
    }
    void ping(pingPongMsg* msg) {
      CkPrintf("Left: %d\n", msg->left);
      thisProxy[0].pong(msg);
    }
    void pong(pingPongMsg* msg) {
      if(msg->left == 0) {
        delete msg;
        CkExit();
      } else {
        msg->left --;
        thisProxy[1].ping(msg);
      }
    }
};

#include "PingPong.def.h"
