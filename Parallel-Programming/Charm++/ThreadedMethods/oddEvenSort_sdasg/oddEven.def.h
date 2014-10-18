



/* ---------------- method closures -------------- */
#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */


/* ---------------- method closures -------------- */
#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY

    struct Closure_Worker::sendSmaller_2_closure : public SDAG::Closure {
      int impl_noname_0;


      sendSmaller_2_closure() {
        init();
      }
      sendSmaller_2_closure(CkMigrateMessage*) {
        init();
      }
      int & getP0() { return impl_noname_0;}
      void pup(PUP::er& __p) {
        __p | impl_noname_0;
        packClosure(__p);
      }
      virtual ~sendSmaller_2_closure() {
      }
      PUPable_decl(SINGLE_ARG(sendSmaller_2_closure));
    };
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY

    struct Closure_Worker::run_3_closure : public SDAG::Closure {
      

      run_3_closure() {
        init();
      }
      run_3_closure(CkMigrateMessage*) {
        init();
      }
            void pup(PUP::er& __p) {
        packClosure(__p);
      }
      virtual ~run_3_closure() {
      }
      PUPable_decl(SINGLE_ARG(run_3_closure));
    };
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY

    struct Closure_Worker::dump_4_closure : public SDAG::Closure {
      

      dump_4_closure() {
        init();
      }
      dump_4_closure(CkMigrateMessage*) {
        init();
      }
            void pup(PUP::er& __p) {
        packClosure(__p);
      }
      virtual ~dump_4_closure() {
      }
      PUPable_decl(SINGLE_ARG(dump_4_closure));
    };
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY

    struct Closure_Worker::barrierH_5_closure : public SDAG::Closure {
      

      barrierH_5_closure() {
        init();
      }
      barrierH_5_closure(CkMigrateMessage*) {
        init();
      }
            void pup(PUP::er& __p) {
        packClosure(__p);
      }
      virtual ~barrierH_5_closure() {
      }
      PUPable_decl(SINGLE_ARG(barrierH_5_closure));
    };
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */


/* DEFS: readonly int numelements;
 */
extern int numelements;
#ifndef CK_TEMPLATES_ONLY
extern "C" void __xlater_roPup_numelements(void *_impl_pup_er) {
  PUP::er &_impl_p=*(PUP::er *)_impl_pup_er;
  _impl_p|numelements;
}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: readonly CProxy_Main mainproxy;
 */
extern CProxy_Main mainproxy;
#ifndef CK_TEMPLATES_ONLY
extern "C" void __xlater_roPup_mainproxy(void *_impl_pup_er) {
  PUP::er &_impl_p=*(PUP::er *)_impl_pup_er;
  _impl_p|mainproxy;
}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: readonly CProxy_Worker workerarray;
 */
extern CProxy_Worker workerarray;
#ifndef CK_TEMPLATES_ONLY
extern "C" void __xlater_roPup_workerarray(void *_impl_pup_er) {
  PUP::er &_impl_p=*(PUP::er *)_impl_pup_er;
  _impl_p|workerarray;
}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: message myMsg;
 */
#ifndef CK_TEMPLATES_ONLY
void *CMessage_myMsg::operator new(size_t s){
  return myMsg::alloc(__idx, s, 0, 0);
}
void *CMessage_myMsg::operator new(size_t s, int* sz){
  return myMsg::alloc(__idx, s, sz, 0);
}
void *CMessage_myMsg::operator new(size_t s, int* sz,const int pb){
  return myMsg::alloc(__idx, s, sz, pb);
}
void *CMessage_myMsg::operator new(size_t s, const int p) {
  return myMsg::alloc(__idx, s, 0, p);
}
void* CMessage_myMsg::alloc(int msgnum, size_t sz, int *sizes, int pb) {
  CkpvAccess(_offsets)[0] = ALIGN_DEFAULT(sz);
  return CkAllocMsg(msgnum, CkpvAccess(_offsets)[0], pb);
}
CMessage_myMsg::CMessage_myMsg() {
myMsg *newmsg = (myMsg *)this;
}
void CMessage_myMsg::dealloc(void *p) {
  CkFreeMsg(p);
}
void* CMessage_myMsg::pack(myMsg *msg) {
  return (void *) msg;
}
myMsg* CMessage_myMsg::unpack(void* buf) {
  myMsg *msg = (myMsg *) buf;
  return msg;
}
int CMessage_myMsg::__idx=0;
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: mainchare Main: Chare{
Main(CkArgMsg* impl_msg);
void done(CkReductionMsg* impl_msg);
};
 */
#ifndef CK_TEMPLATES_ONLY
 int CkIndex_Main::__idx=0;
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
/* DEFS: Main(CkArgMsg* impl_msg);
 */

CkChareID CProxy_Main::ckNew(CkArgMsg* impl_msg, int impl_onPE)
{
  CkChareID impl_ret;
  CkCreateChare(CkIndex_Main::__idx, CkIndex_Main::idx_Main_CkArgMsg(), impl_msg, &impl_ret, impl_onPE);
  return impl_ret;
}

void CProxy_Main::ckNew(CkArgMsg* impl_msg, CkChareID* pcid, int impl_onPE)
{
  CkCreateChare(CkIndex_Main::__idx, CkIndex_Main::idx_Main_CkArgMsg(), impl_msg, pcid, impl_onPE);
}

  CProxy_Main::CProxy_Main(CkArgMsg* impl_msg, int impl_onPE)
{
  CkChareID impl_ret;
  CkCreateChare(CkIndex_Main::__idx, CkIndex_Main::idx_Main_CkArgMsg(), impl_msg, &impl_ret, impl_onPE);
  ckSetChareID(impl_ret);
}

// Entry point registration function

int CkIndex_Main::reg_Main_CkArgMsg() {
  int epidx = CkRegisterEp("Main(CkArgMsg* impl_msg)",
      _call_Main_CkArgMsg, CMessage_CkArgMsg::__idx, __idx, 0);
  CkRegisterMessagePupFn(epidx, (CkMessagePupFn)CkArgMsg::ckDebugPup);
  return epidx;
}


void CkIndex_Main::_call_Main_CkArgMsg(void* impl_msg, void* impl_obj_void)
{
  Main* impl_obj = static_cast<Main *>(impl_obj_void);
  new (impl_obj) Main((CkArgMsg*)impl_msg);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void done(CkReductionMsg* impl_msg);
 */

void CProxy_Main::done(CkReductionMsg* impl_msg)
{
  ckCheck();
  if (ckIsDelegated()) {
    int destPE=CkChareMsgPrep(CkIndex_Main::idx_done_CkReductionMsg(), impl_msg, &ckGetChareID());
    if (destPE!=-1) ckDelegatedTo()->ChareSend(ckDelegatedPtr(),CkIndex_Main::idx_done_CkReductionMsg(), impl_msg, &ckGetChareID(),destPE);
  }
  else CkSendMsg(CkIndex_Main::idx_done_CkReductionMsg(), impl_msg, &ckGetChareID(),0);
}

// Entry point registration function

int CkIndex_Main::reg_done_CkReductionMsg() {
  int epidx = CkRegisterEp("done(CkReductionMsg* impl_msg)",
      _call_done_CkReductionMsg, CMessage_CkReductionMsg::__idx, __idx, 0);
  CkRegisterMessagePupFn(epidx, (CkMessagePupFn)CkReductionMsg::ckDebugPup);
  return epidx;
}


void CkIndex_Main::_call_done_CkReductionMsg(void* impl_msg, void* impl_obj_void)
{
  Main* impl_obj = static_cast<Main *>(impl_obj_void);
  impl_obj->done((CkReductionMsg*)impl_msg);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
void CkIndex_Main::__register(const char *s, size_t size) {
  __idx = CkRegisterChare(s, size, TypeMainChare);
  CkRegisterBase(__idx, CkIndex_Chare::__idx);
  // REG: Main(CkArgMsg* impl_msg);
  idx_Main_CkArgMsg();
  CkRegisterMainChare(__idx, idx_Main_CkArgMsg());

  // REG: void done(CkReductionMsg* impl_msg);
  idx_done_CkReductionMsg();

}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: array Worker: ArrayElement{
Worker(void);
sync myMsg* sendSmaller(int impl_noname_0);
threaded void run(void);
void dump(void);
void barrierH(void);
Worker(CkMigrateMessage* impl_msg);
};
 */
#ifndef CK_TEMPLATES_ONLY
 int CkIndex_Worker::__idx=0;
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker(void);
 */

void CProxyElement_Worker::insert(int onPE)
{ 
  void *impl_msg = CkAllocSysMsg();
   UsrToEnv(impl_msg)->setMsgtype(ArrayEltInitMsg);
   ckInsert((CkArrayMessage *)impl_msg,CkIndex_Worker::idx_Worker_void(),onPE);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: sync myMsg* sendSmaller(int impl_noname_0);
 */

myMsg* CProxyElement_Worker::sendSmaller(int impl_noname_0, const CkEntryOptions *impl_e_opts) 
{
  ckCheck();
  //Marshall: int impl_noname_0
  int impl_off=0;
  { //Find the size of the PUP'd data
    PUP::sizer implP;
    implP|impl_noname_0;
    impl_off+=implP.size();
  }
  CkMarshallMsg *impl_msg=CkAllocateMarshallMsg(impl_off,impl_e_opts);
  { //Copy over the PUP'd data
    PUP::toMem implP((void *)impl_msg->msgBuf);
    implP|impl_noname_0;
  }
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  return (myMsg*) (ckSendSync(impl_amsg, CkIndex_Worker::idx_sendSmaller_marshall2()));
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: threaded void run(void);
 */

void CProxyElement_Worker::run(void) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckSend(impl_amsg, CkIndex_Worker::idx_run_void(),0);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void dump(void);
 */

void CProxyElement_Worker::dump(void) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckSend(impl_amsg, CkIndex_Worker::idx_dump_void(),0);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void barrierH(void);
 */

void CProxyElement_Worker::barrierH(void) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckSend(impl_amsg, CkIndex_Worker::idx_barrierH_void(),0);
}

void CkIndex_Worker::_call_redn_wrapper_barrierH_void(void* impl_msg, void* impl_obj_void)
{
  Worker* impl_obj = static_cast<Worker*> (impl_obj_void);
  char* impl_buf = (char*)((CkReductionMsg*)impl_msg)->getData();
  impl_obj->barrierH();
  delete (CkReductionMsg*)impl_msg;
}

#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker(CkMigrateMessage* impl_msg);
 */
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker(void);
 */

CkArrayID CProxy_Worker::ckNew(const CkArrayOptions &opts)
{
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ArrayEltInitMsg);
  return ckCreateArray((CkArrayMessage *)impl_msg, CkIndex_Worker::idx_Worker_void(), opts);
}

CkArrayID CProxy_Worker::ckNew(const int s1)
{
  void *impl_msg = CkAllocSysMsg();
  CkArrayOptions opts(s1);
  UsrToEnv(impl_msg)->setMsgtype(ArrayEltInitMsg);
  return ckCreateArray((CkArrayMessage *)impl_msg, CkIndex_Worker::idx_Worker_void(), opts);
}

// Entry point registration function

int CkIndex_Worker::reg_Worker_void() {
  int epidx = CkRegisterEp("Worker(void)",
      _call_Worker_void, 0, __idx, 0);
  return epidx;
}


void CkIndex_Worker::_call_Worker_void(void* impl_msg, void* impl_obj_void)
{
  Worker* impl_obj = static_cast<Worker *>(impl_obj_void);
  CkFreeSysMsg(impl_msg);
  new (impl_obj) Worker();
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: sync myMsg* sendSmaller(int impl_noname_0);
 */

// Entry point registration function

int CkIndex_Worker::reg_sendSmaller_marshall2() {
  int epidx = CkRegisterEp("sendSmaller(int impl_noname_0)",
      _call_sendSmaller_marshall2, CkMarshallMsg::__idx, __idx, 0+CK_EP_NOKEEP);
  CkRegisterMessagePupFn(epidx, _marshallmessagepup_sendSmaller_marshall2);

  return epidx;
}


void CkIndex_Worker::_call_sendSmaller_marshall2(void* impl_msg, void* impl_obj_void)
{
  Worker* impl_obj = static_cast<Worker *>(impl_obj_void);
  int impl_ref = CkGetRefNum(impl_msg), impl_src = CkGetSrcPe(impl_msg);
  CkMarshallMsg *impl_msg_typed=(CkMarshallMsg *)impl_msg;
  char *impl_buf=impl_msg_typed->msgBuf;
  /*Unmarshall pup'd fields: int impl_noname_0*/
  PUP::fromMem implP(impl_buf);
  int impl_noname_0; implP|impl_noname_0;
  impl_buf+=CK_ALIGN(implP.size(),16);
  /*Unmarshall arrays:*/
  void *impl_retMsg=(void *)   impl_obj->sendSmaller(impl_noname_0);
  CkSendToFutureID(impl_ref, impl_retMsg, impl_src);
}

void CkIndex_Worker::_marshallmessagepup_sendSmaller_marshall2(PUP::er &implDestP,void *impl_msg) {
  CkMarshallMsg *impl_msg_typed=(CkMarshallMsg *)impl_msg;
  char *impl_buf=impl_msg_typed->msgBuf;
  /*Unmarshall pup'd fields: int impl_noname_0*/
  PUP::fromMem implP(impl_buf);
  int impl_noname_0; implP|impl_noname_0;
  impl_buf+=CK_ALIGN(implP.size(),16);
  /*Unmarshall arrays:*/
  if (implDestP.hasComments()) implDestP.comment("impl_noname_0");
  implDestP|impl_noname_0;
}
PUPable_def(SINGLE_ARG(Closure_Worker::sendSmaller_2_closure));
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: threaded void run(void);
 */

void CProxy_Worker::run(void) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckBroadcast(impl_amsg, CkIndex_Worker::idx_run_void(),0);
}

// Entry point registration function

int CkIndex_Worker::reg_run_void() {
  int epidx = CkRegisterEp("run(void)",
      _call_run_void, 0, __idx, 0);
  return epidx;
}


void CkIndex_Worker::_call_run_void(void* impl_msg, void* impl_obj_void)
{
  Worker* impl_obj = static_cast<Worker *>(impl_obj_void);
  CthThread tid = CthCreate((CthVoidFn)_callthr_run_void, new CkThrCallArg(impl_msg,impl_obj), 0);
  ((Chare *)impl_obj)->CkAddThreadListeners(tid,impl_msg);
  CthResume(tid);
}

void CkIndex_Worker::_callthr_run_void(CkThrCallArg *impl_arg)
{
  void *impl_msg = impl_arg->msg;
  Worker *impl_obj = (Worker *) impl_arg->obj;
  delete impl_arg;
  CkFreeSysMsg(impl_msg);
  impl_obj->run();
}
PUPable_def(SINGLE_ARG(Closure_Worker::run_3_closure));
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void dump(void);
 */

void CProxy_Worker::dump(void) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckBroadcast(impl_amsg, CkIndex_Worker::idx_dump_void(),0);
}

// Entry point registration function

int CkIndex_Worker::reg_dump_void() {
  int epidx = CkRegisterEp("dump(void)",
      _call_dump_void, 0, __idx, 0);
  return epidx;
}


void CkIndex_Worker::_call_dump_void(void* impl_msg, void* impl_obj_void)
{
  Worker* impl_obj = static_cast<Worker *>(impl_obj_void);
  CkFreeSysMsg(impl_msg);
  impl_obj->dump();
}
PUPable_def(SINGLE_ARG(Closure_Worker::dump_4_closure));
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void barrierH(void);
 */

void CProxy_Worker::barrierH(void) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckBroadcast(impl_amsg, CkIndex_Worker::idx_barrierH_void(),0);
}

// Entry point registration function

int CkIndex_Worker::reg_barrierH_void() {
  int epidx = CkRegisterEp("barrierH(void)",
      _call_barrierH_void, 0, __idx, 0);
  return epidx;
}


// Redn wrapper registration function

int CkIndex_Worker::reg_redn_wrapper_barrierH_void() {
  return CkRegisterEp("redn_wrapper_barrierH(CkReductionMsg *impl_msg)",
      _call_redn_wrapper_barrierH_void, CMessage_CkReductionMsg::__idx, __idx, 0);
}


void CkIndex_Worker::_call_barrierH_void(void* impl_msg, void* impl_obj_void)
{
  Worker* impl_obj = static_cast<Worker *>(impl_obj_void);
  CkFreeSysMsg(impl_msg);
  impl_obj->barrierH();
}
PUPable_def(SINGLE_ARG(Closure_Worker::barrierH_5_closure));
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker(CkMigrateMessage* impl_msg);
 */

// Entry point registration function

int CkIndex_Worker::reg_Worker_CkMigrateMessage() {
  int epidx = CkRegisterEp("Worker(CkMigrateMessage* impl_msg)",
      _call_Worker_CkMigrateMessage, 0, __idx, 0);
  return epidx;
}


void CkIndex_Worker::_call_Worker_CkMigrateMessage(void* impl_msg, void* impl_obj_void)
{
  Worker* impl_obj = static_cast<Worker *>(impl_obj_void);
  new (impl_obj) Worker((CkMigrateMessage*)impl_msg);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker(void);
 */
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: sync myMsg* sendSmaller(int impl_noname_0);
 */
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: threaded void run(void);
 */

void CProxySection_Worker::run(void) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckSend(impl_amsg, CkIndex_Worker::idx_run_void(),0);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void dump(void);
 */

void CProxySection_Worker::dump(void) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckSend(impl_amsg, CkIndex_Worker::idx_dump_void(),0);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void barrierH(void);
 */

void CProxySection_Worker::barrierH(void) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg();
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckSend(impl_amsg, CkIndex_Worker::idx_barrierH_void(),0);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker(CkMigrateMessage* impl_msg);
 */
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
void CkIndex_Worker::__register(const char *s, size_t size) {
  __idx = CkRegisterChare(s, size, TypeArray);
  CkRegisterBase(__idx, CkIndex_ArrayElement::__idx);
  // REG: Worker(void);
  idx_Worker_void();
  CkRegisterDefaultCtor(__idx, idx_Worker_void());

  // REG: sync myMsg* sendSmaller(int impl_noname_0);
  idx_sendSmaller_marshall2();

  // REG: threaded void run(void);
  idx_run_void();

  // REG: void dump(void);
  idx_dump_void();

  // REG: void barrierH(void);
  idx_barrierH_void();
  idx_redn_wrapper_barrierH_void();

  // REG: Worker(CkMigrateMessage* impl_msg);
  idx_Worker_CkMigrateMessage();
  CkRegisterMigCtor(__idx, idx_Worker_CkMigrateMessage());

  Worker::__sdag_register(); 
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
void Worker::run() {
  _TRACE_END_EXECUTE(); 
  if (!__dep.get()) _sdag_init();
  _slist_0();
  _TRACE_BEGIN_EXECUTE_DETAILED(-1, -1, _sdagEP, CkMyPe(), 0, NULL); 
}
#endif /* CK_TEMPLATES_ONLY */


#ifndef CK_TEMPLATES_ONLY
void Worker::run_end() {
}
#endif /* CK_TEMPLATES_ONLY */


#ifndef CK_TEMPLATES_ONLY
void Worker::_slist_0() {
  _atomic_0();
}
#endif /* CK_TEMPLATES_ONLY */


#ifndef CK_TEMPLATES_ONLY
void Worker::_slist_0_end() {
  run_end();
}
#endif /* CK_TEMPLATES_ONLY */


#ifndef CK_TEMPLATES_ONLY
void Worker::_atomic_0() {
  _TRACE_BEGIN_EXECUTE_DETAILED(-1, -1, (_sdag_idx_Worker_atomic_0()), CkMyPe(), 0, NULL); 
  { // begin serial block

      for (int i = 0 ; i < numelements; i++) {
        
        if(thisIndex % 2 == 0 && thisIndex != numelements -1 ) {
          myMsg* m = workerarray[thisIndex + 1].sendSmaller(val);
          val = m->val;
          delete m;
        } 
        barrier();
        if (thisIndex % 2 == 1 && thisIndex != numelements -1 ) {
          myMsg* m  = workerarray[thisIndex + 1].sendSmaller(val);
          val = m->val;
          delete m;
        }
        barrier();
      }

      contribute(CkCallback(CkIndex_Main::done(NULL), mainproxy)); 

      
  } // end serial block
  _TRACE_END_EXECUTE(); 
  _slist_0_end();
}
#endif /* CK_TEMPLATES_ONLY */


#ifndef CK_TEMPLATES_ONLY
void Worker::_sdag_init() {
  __dep.reset(new SDAG::Dependency(0,0));
}
#endif /* CK_TEMPLATES_ONLY */


#ifndef CK_TEMPLATES_ONLY
void Worker::__sdag_init() {
}
#endif /* CK_TEMPLATES_ONLY */


#ifndef CK_TEMPLATES_ONLY
void Worker::__sdag_pup(PUP::er &p) {
    bool hasSDAG = __dep.get();
    p|hasSDAG;
    if (p.isUnpacking() && hasSDAG) _sdag_init();
    if (hasSDAG) { __dep->pup(p); }
}
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
void Worker::__sdag_register() {
  (void)_sdag_idx_Worker_atomic_0();
  PUPable_reg(SINGLE_ARG(Closure_Worker::sendSmaller_2_closure));
  PUPable_reg(SINGLE_ARG(Closure_Worker::run_3_closure));
  PUPable_reg(SINGLE_ARG(Closure_Worker::dump_4_closure));
  PUPable_reg(SINGLE_ARG(Closure_Worker::barrierH_5_closure));
  PUPable_reg(SINGLE_ARG(Closure_Worker::sendSmaller_2_closure));
  PUPable_reg(SINGLE_ARG(Closure_Worker::run_3_closure));
  PUPable_reg(SINGLE_ARG(Closure_Worker::dump_4_closure));
  PUPable_reg(SINGLE_ARG(Closure_Worker::barrierH_5_closure));
}
#endif /* CK_TEMPLATES_ONLY */


#ifndef CK_TEMPLATES_ONLY
int Worker::_sdag_idx_Worker_atomic_0() {
  static int epidx = _sdag_reg_Worker_atomic_0();
  return epidx;
}
#endif /* CK_TEMPLATES_ONLY */


#ifndef CK_TEMPLATES_ONLY
int Worker::_sdag_reg_Worker_atomic_0() {
  return CkRegisterEp("Worker_atomic_0", NULL, 0, CkIndex_Worker::__idx, 0);
}
#endif /* CK_TEMPLATES_ONLY */



#ifndef CK_TEMPLATES_ONLY
void _registeroddEven(void)
{
  static int _done = 0; if(_done) return; _done = 1;
  CkRegisterReadonly("numelements","int",sizeof(numelements),(void *) &numelements,__xlater_roPup_numelements);

  CkRegisterReadonly("mainproxy","CProxy_Main",sizeof(mainproxy),(void *) &mainproxy,__xlater_roPup_mainproxy);

  CkRegisterReadonly("workerarray","CProxy_Worker",sizeof(workerarray),(void *) &workerarray,__xlater_roPup_workerarray);

/* REG: message myMsg;
*/
CMessage_myMsg::__register("myMsg", sizeof(myMsg),(CkPackFnPtr) myMsg::pack,(CkUnpackFnPtr) myMsg::unpack);

/* REG: mainchare Main: Chare{
Main(CkArgMsg* impl_msg);
void done(CkReductionMsg* impl_msg);
};
*/
  CkIndex_Main::__register("Main", sizeof(Main));

/* REG: array Worker: ArrayElement{
Worker(void);
sync myMsg* sendSmaller(int impl_noname_0);
threaded void run(void);
void dump(void);
void barrierH(void);
Worker(CkMigrateMessage* impl_msg);
};
*/
  CkIndex_Worker::__register("Worker", sizeof(Worker));

}
extern "C" void CkRegisterMainModule(void) {
  _registeroddEven();
}
#endif /* CK_TEMPLATES_ONLY */
