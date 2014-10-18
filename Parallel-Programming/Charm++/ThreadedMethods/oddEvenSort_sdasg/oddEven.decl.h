#ifndef _DECL_oddEven_H_
#define _DECL_oddEven_H_
#include "charm++.h"
#include "envelope.h"
#include <memory>
#include "sdag.h"
/* DECLS: readonly int numelements;
 */

/* DECLS: readonly CProxy_Main mainproxy;
 */

/* DECLS: readonly CProxy_Worker workerarray;
 */

/* DECLS: message myMsg;
 */
class myMsg;
class CMessage_myMsg:public CkMessage{
  public:
    static int __idx;
    void* operator new(size_t, void*p) { return p; }
    void* operator new(size_t);
    void* operator new(size_t, int*, const int);
    void* operator new(size_t, int*);
#if CMK_MULTIPLE_DELETE
    void operator delete(void*p, void*){dealloc(p);}
    void operator delete(void*p){dealloc(p);}
    void operator delete(void*p, int*, const int){dealloc(p);}
    void operator delete(void*p, int*){dealloc(p);}
#endif
    void operator delete(void*p, size_t){dealloc(p);}
    static void* alloc(int,size_t, int*, int);
    static void dealloc(void *p);
    CMessage_myMsg();
    static void *pack(myMsg *p);
    static myMsg* unpack(void* p);
    void *operator new(size_t, const int);
#if CMK_MULTIPLE_DELETE
    void operator delete(void *p, const int){dealloc(p);}
#endif
    static void __register(const char *s, size_t size, CkPackFnPtr pack, CkUnpackFnPtr unpack) {
      __idx = CkRegisterMsg(s, pack, unpack, dealloc, size);
    }
};

/* DECLS: mainchare Main: Chare{
Main(CkArgMsg* impl_msg);
void done(CkReductionMsg* impl_msg);
};
 */
 class Main;
 class CkIndex_Main;
 class CProxy_Main;
/* --------------- index object ------------------ */
class CkIndex_Main:public CkIndex_Chare{
  public:
    typedef Main local_t;
    typedef CkIndex_Main index_t;
    typedef CProxy_Main proxy_t;
    typedef CProxy_Main element_t;

    static int __idx;
    static void __register(const char *s, size_t size);
    /* DECLS: Main(CkArgMsg* impl_msg);
     */
    // Entry point registration at startup
    
    static int reg_Main_CkArgMsg();
    // Entry point index lookup
    
    inline static int idx_Main_CkArgMsg() {
      static int epidx = reg_Main_CkArgMsg();
      return epidx;
    }

    
    static int ckNew(CkArgMsg* impl_msg) { return idx_Main_CkArgMsg(); }
    
    static void _call_Main_CkArgMsg(void* impl_msg, void* impl_obj);
    
    static void _call_sdag_Main_CkArgMsg(void* impl_msg, void* impl_obj);
    /* DECLS: void done(CkReductionMsg* impl_msg);
     */
    // Entry point registration at startup
    
    static int reg_done_CkReductionMsg();
    // Entry point index lookup
    
    inline static int idx_done_CkReductionMsg() {
      static int epidx = reg_done_CkReductionMsg();
      return epidx;
    }

    
    inline static int idx_done(void (Main::*)(CkReductionMsg* impl_msg) ) {
      return idx_done_CkReductionMsg();
    }


    
    static int done(CkReductionMsg* impl_msg) { return idx_done_CkReductionMsg(); }
    
    static void _call_done_CkReductionMsg(void* impl_msg, void* impl_obj);
    
    static void _call_sdag_done_CkReductionMsg(void* impl_msg, void* impl_obj);
};
/* --------------- element proxy ------------------ */
class CProxy_Main:public CProxy_Chare{
  public:
    typedef Main local_t;
    typedef CkIndex_Main index_t;
    typedef CProxy_Main proxy_t;
    typedef CProxy_Main element_t;

    CProxy_Main(void) {};
    CProxy_Main(CkChareID __cid) : CProxy_Chare(__cid){  }
    CProxy_Main(const Chare *c) : CProxy_Chare(c){  }

    int ckIsDelegated(void) const
    { return CProxy_Chare::ckIsDelegated(); }
    inline CkDelegateMgr *ckDelegatedTo(void) const
    { return CProxy_Chare::ckDelegatedTo(); }
    inline CkDelegateData *ckDelegatedPtr(void) const
    { return CProxy_Chare::ckDelegatedPtr(); }
    CkGroupID ckDelegatedIdx(void) const
    { return CProxy_Chare::ckDelegatedIdx(); }

    inline void ckCheck(void) const
    { CProxy_Chare::ckCheck(); }
    const CkChareID &ckGetChareID(void) const
    { return CProxy_Chare::ckGetChareID(); }
    operator const CkChareID &(void) const
    { return ckGetChareID(); }

    void ckDelegate(CkDelegateMgr *dTo,CkDelegateData *dPtr=NULL)
    {       CProxy_Chare::ckDelegate(dTo,dPtr); }
    void ckUndelegate(void)
    {       CProxy_Chare::ckUndelegate(); }
    void pup(PUP::er &p)
    {       CProxy_Chare::pup(p); }

    void ckSetChareID(const CkChareID &c)
    {      CProxy_Chare::ckSetChareID(c); }
    Main *ckLocal(void) const
    { return (Main *)CkLocalChare(&ckGetChareID()); }
/* DECLS: Main(CkArgMsg* impl_msg);
 */
    static CkChareID ckNew(CkArgMsg* impl_msg, int onPE=CK_PE_ANY);
    static void ckNew(CkArgMsg* impl_msg, CkChareID* pcid, int onPE=CK_PE_ANY);
    CProxy_Main(CkArgMsg* impl_msg, int onPE=CK_PE_ANY);

/* DECLS: void done(CkReductionMsg* impl_msg);
 */
    
    void done(CkReductionMsg* impl_msg);

};
PUPmarshall(CProxy_Main)
typedef CBaseT1<Chare, CProxy_Main> CBase_Main;

/* DECLS: array Worker: ArrayElement{
Worker(void);
sync myMsg* sendSmaller(int impl_noname_0);
threaded void run(void);
void dump(void);
void barrierH(void);
Worker(CkMigrateMessage* impl_msg);
};
 */
 class Worker;
 class CkIndex_Worker;
 class CProxy_Worker;
 class CProxyElement_Worker;
 class CProxySection_Worker;
/* --------------- index object ------------------ */
class CkIndex_Worker:public CkIndex_ArrayElement{
  public:
    typedef Worker local_t;
    typedef CkIndex_Worker index_t;
    typedef CProxy_Worker proxy_t;
    typedef CProxyElement_Worker element_t;
    typedef CProxySection_Worker section_t;

    static int __idx;
    static void __register(const char *s, size_t size);
    /* DECLS: Worker(void);
     */
    // Entry point registration at startup
    
    static int reg_Worker_void();
    // Entry point index lookup
    
    inline static int idx_Worker_void() {
      static int epidx = reg_Worker_void();
      return epidx;
    }

    
    static int ckNew(void) { return idx_Worker_void(); }
    
    static void _call_Worker_void(void* impl_msg, void* impl_obj);
    
    static void _call_sdag_Worker_void(void* impl_msg, void* impl_obj);
    /* DECLS: sync myMsg* sendSmaller(int impl_noname_0);
     */
    // Entry point registration at startup
    
    static int reg_sendSmaller_marshall2();
    // Entry point index lookup
    
    inline static int idx_sendSmaller_marshall2() {
      static int epidx = reg_sendSmaller_marshall2();
      return epidx;
    }

    
    inline static int idx_sendSmaller(myMsg* (Worker::*)(int impl_noname_0) ) {
      return idx_sendSmaller_marshall2();
    }


    
    static int sendSmaller(int impl_noname_0) { return idx_sendSmaller_marshall2(); }
    
    static void _call_sendSmaller_marshall2(void* impl_msg, void* impl_obj);
    
    static void _call_sdag_sendSmaller_marshall2(void* impl_msg, void* impl_obj);
    
    static void _marshallmessagepup_sendSmaller_marshall2(PUP::er &p,void *msg);
    /* DECLS: threaded void run(void);
     */
    // Entry point registration at startup
    
    static int reg_run_void();
    // Entry point index lookup
    
    inline static int idx_run_void() {
      static int epidx = reg_run_void();
      return epidx;
    }

    
    inline static int idx_run(void (Worker::*)(void) ) {
      return idx_run_void();
    }


    
    static int run(void) { return idx_run_void(); }
    
    static void _call_run_void(void* impl_msg, void* impl_obj);
    
    static void _call_sdag_run_void(void* impl_msg, void* impl_obj);
    
    static void _callthr_run_void(CkThrCallArg *);
    /* DECLS: void dump(void);
     */
    // Entry point registration at startup
    
    static int reg_dump_void();
    // Entry point index lookup
    
    inline static int idx_dump_void() {
      static int epidx = reg_dump_void();
      return epidx;
    }

    
    inline static int idx_dump(void (Worker::*)(void) ) {
      return idx_dump_void();
    }


    
    static int dump(void) { return idx_dump_void(); }
    
    static void _call_dump_void(void* impl_msg, void* impl_obj);
    
    static void _call_sdag_dump_void(void* impl_msg, void* impl_obj);
    /* DECLS: void barrierH(void);
     */
    // Entry point registration at startup
    
    static int reg_barrierH_void();
    // Entry point index lookup
    
    inline static int idx_barrierH_void() {
      static int epidx = reg_barrierH_void();
      return epidx;
    }

    
    inline static int idx_barrierH(void (Worker::*)(void) ) {
      return idx_barrierH_void();
    }


    
    static int barrierH(void) { return idx_barrierH_void(); }
    // Entry point registration at startup
    
    static int reg_redn_wrapper_barrierH_void();
    // Entry point index lookup
    
    inline static int idx_redn_wrapper_barrierH_void() {
      static int epidx = reg_redn_wrapper_barrierH_void();
      return epidx;
    }
    
    static int redn_wrapper_barrierH(CkReductionMsg* impl_msg) { return idx_redn_wrapper_barrierH_void(); }
    
    static void _call_redn_wrapper_barrierH_void(void* impl_msg, void* impl_obj_void);
    
    static void _call_barrierH_void(void* impl_msg, void* impl_obj);
    
    static void _call_sdag_barrierH_void(void* impl_msg, void* impl_obj);
    /* DECLS: Worker(CkMigrateMessage* impl_msg);
     */
    // Entry point registration at startup
    
    static int reg_Worker_CkMigrateMessage();
    // Entry point index lookup
    
    inline static int idx_Worker_CkMigrateMessage() {
      static int epidx = reg_Worker_CkMigrateMessage();
      return epidx;
    }

    
    static int ckNew(CkMigrateMessage* impl_msg) { return idx_Worker_CkMigrateMessage(); }
    
    static void _call_Worker_CkMigrateMessage(void* impl_msg, void* impl_obj);
    
    static void _call_sdag_Worker_CkMigrateMessage(void* impl_msg, void* impl_obj);
};
/* --------------- element proxy ------------------ */
 class CProxyElement_Worker : public CProxyElement_ArrayElement{
  public:
    typedef Worker local_t;
    typedef CkIndex_Worker index_t;
    typedef CProxy_Worker proxy_t;
    typedef CProxyElement_Worker element_t;
    typedef CProxySection_Worker section_t;

    CProxyElement_Worker(void) {}
    CProxyElement_Worker(const ArrayElement *e) : CProxyElement_ArrayElement(e){  }

    void ckDelegate(CkDelegateMgr *dTo,CkDelegateData *dPtr=NULL)
    {       CProxyElement_ArrayElement::ckDelegate(dTo,dPtr); }
    void ckUndelegate(void)
    {       CProxyElement_ArrayElement::ckUndelegate(); }
    void pup(PUP::er &p)
    {       CProxyElement_ArrayElement::pup(p); }

    int ckIsDelegated(void) const
    { return CProxyElement_ArrayElement::ckIsDelegated(); }
    inline CkDelegateMgr *ckDelegatedTo(void) const
    { return CProxyElement_ArrayElement::ckDelegatedTo(); }
    inline CkDelegateData *ckDelegatedPtr(void) const
    { return CProxyElement_ArrayElement::ckDelegatedPtr(); }
    CkGroupID ckDelegatedIdx(void) const
    { return CProxyElement_ArrayElement::ckDelegatedIdx(); }

    inline void ckCheck(void) const
    { CProxyElement_ArrayElement::ckCheck(); }
    inline operator CkArrayID () const
    { return ckGetArrayID(); }
    inline CkArrayID ckGetArrayID(void) const
    { return CProxyElement_ArrayElement::ckGetArrayID(); }
    inline CkArray *ckLocalBranch(void) const
    { return CProxyElement_ArrayElement::ckLocalBranch(); }
    inline CkLocMgr *ckLocMgr(void) const
    { return CProxyElement_ArrayElement::ckLocMgr(); }

    inline static CkArrayID ckCreateEmptyArray(void)
    { return CProxyElement_ArrayElement::ckCreateEmptyArray(); }
    inline static CkArrayID ckCreateArray(CkArrayMessage *m,int ctor,const CkArrayOptions &opts)
    { return CProxyElement_ArrayElement::ckCreateArray(m,ctor,opts); }
    inline void ckInsertIdx(CkArrayMessage *m,int ctor,int onPe,const CkArrayIndex &idx)
    { CProxyElement_ArrayElement::ckInsertIdx(m,ctor,onPe,idx); }
    inline void doneInserting(void)
    { CProxyElement_ArrayElement::doneInserting(); }

    inline void ckBroadcast(CkArrayMessage *m, int ep, int opts=0) const
    { CProxyElement_ArrayElement::ckBroadcast(m,ep,opts); }
    inline void setReductionClient(CkReductionClientFn fn,void *param=NULL) const
    { CProxyElement_ArrayElement::setReductionClient(fn,param); }
    inline void ckSetReductionClient(CkReductionClientFn fn,void *param=NULL) const
    { CProxyElement_ArrayElement::ckSetReductionClient(fn,param); }
    inline void ckSetReductionClient(CkCallback *cb) const
    { CProxyElement_ArrayElement::ckSetReductionClient(cb); }

    inline void ckInsert(CkArrayMessage *m,int ctor,int onPe)
    { CProxyElement_ArrayElement::ckInsert(m,ctor,onPe); }
    inline void ckSend(CkArrayMessage *m, int ep, int opts = 0) const
    { CProxyElement_ArrayElement::ckSend(m,ep,opts); }
    inline void *ckSendSync(CkArrayMessage *m, int ep) const
    { return CProxyElement_ArrayElement::ckSendSync(m,ep); }
    inline const CkArrayIndex &ckGetIndex() const
    { return CProxyElement_ArrayElement::ckGetIndex(); }

    Worker *ckLocal(void) const
    { return (Worker *)CProxyElement_ArrayElement::ckLocal(); }

    CProxyElement_Worker(const CkArrayID &aid,const CkArrayIndex1D &idx,CK_DELCTOR_PARAM)
        :CProxyElement_ArrayElement(aid,idx,CK_DELCTOR_ARGS)
    {}
    CProxyElement_Worker(const CkArrayID &aid,const CkArrayIndex1D &idx)
        :CProxyElement_ArrayElement(aid,idx)
    {}

    CProxyElement_Worker(const CkArrayID &aid,const CkArrayIndex &idx,CK_DELCTOR_PARAM)
        :CProxyElement_ArrayElement(aid,idx,CK_DELCTOR_ARGS)
    {}
    CProxyElement_Worker(const CkArrayID &aid,const CkArrayIndex &idx)
        :CProxyElement_ArrayElement(aid,idx)
    {}
/* DECLS: Worker(void);
 */
    
    void insert(int onPE=-1);
/* DECLS: sync myMsg* sendSmaller(int impl_noname_0);
 */
    
    myMsg* sendSmaller(int impl_noname_0, const CkEntryOptions *impl_e_opts=NULL) ;

/* DECLS: threaded void run(void);
 */
    
    void run(void) ;

/* DECLS: void dump(void);
 */
    
    void dump(void) ;

/* DECLS: void barrierH(void);
 */
    
    void barrierH(void) ;

/* DECLS: Worker(CkMigrateMessage* impl_msg);
 */

};
PUPmarshall(CProxyElement_Worker)
/* ---------------- collective proxy -------------- */
 class CProxy_Worker : public CProxy_ArrayElement{
  public:
    typedef Worker local_t;
    typedef CkIndex_Worker index_t;
    typedef CProxy_Worker proxy_t;
    typedef CProxyElement_Worker element_t;
    typedef CProxySection_Worker section_t;

    CProxy_Worker(void) {}
    CProxy_Worker(const ArrayElement *e) : CProxy_ArrayElement(e){  }

    void ckDelegate(CkDelegateMgr *dTo,CkDelegateData *dPtr=NULL)
    {       CProxy_ArrayElement::ckDelegate(dTo,dPtr); }
    void ckUndelegate(void)
    {       CProxy_ArrayElement::ckUndelegate(); }
    void pup(PUP::er &p)
    {       CProxy_ArrayElement::pup(p); }

    int ckIsDelegated(void) const
    { return CProxy_ArrayElement::ckIsDelegated(); }
    inline CkDelegateMgr *ckDelegatedTo(void) const
    { return CProxy_ArrayElement::ckDelegatedTo(); }
    inline CkDelegateData *ckDelegatedPtr(void) const
    { return CProxy_ArrayElement::ckDelegatedPtr(); }
    CkGroupID ckDelegatedIdx(void) const
    { return CProxy_ArrayElement::ckDelegatedIdx(); }

    inline void ckCheck(void) const
    { CProxy_ArrayElement::ckCheck(); }
    inline operator CkArrayID () const
    { return ckGetArrayID(); }
    inline CkArrayID ckGetArrayID(void) const
    { return CProxy_ArrayElement::ckGetArrayID(); }
    inline CkArray *ckLocalBranch(void) const
    { return CProxy_ArrayElement::ckLocalBranch(); }
    inline CkLocMgr *ckLocMgr(void) const
    { return CProxy_ArrayElement::ckLocMgr(); }

    inline static CkArrayID ckCreateEmptyArray(void)
    { return CProxy_ArrayElement::ckCreateEmptyArray(); }
    inline static CkArrayID ckCreateArray(CkArrayMessage *m,int ctor,const CkArrayOptions &opts)
    { return CProxy_ArrayElement::ckCreateArray(m,ctor,opts); }
    inline void ckInsertIdx(CkArrayMessage *m,int ctor,int onPe,const CkArrayIndex &idx)
    { CProxy_ArrayElement::ckInsertIdx(m,ctor,onPe,idx); }
    inline void doneInserting(void)
    { CProxy_ArrayElement::doneInserting(); }

    inline void ckBroadcast(CkArrayMessage *m, int ep, int opts=0) const
    { CProxy_ArrayElement::ckBroadcast(m,ep,opts); }
    inline void setReductionClient(CkReductionClientFn fn,void *param=NULL) const
    { CProxy_ArrayElement::setReductionClient(fn,param); }
    inline void ckSetReductionClient(CkReductionClientFn fn,void *param=NULL) const
    { CProxy_ArrayElement::ckSetReductionClient(fn,param); }
    inline void ckSetReductionClient(CkCallback *cb) const
    { CProxy_ArrayElement::ckSetReductionClient(cb); }

    static CkArrayID ckNew(void) { return ckCreateEmptyArray(); }
    // Generalized array indexing:
    CProxyElement_Worker operator [] (const CkArrayIndex1D &idx) const
    { return CProxyElement_Worker(ckGetArrayID(), idx, CK_DELCTOR_CALL); }
    CProxyElement_Worker operator() (const CkArrayIndex1D &idx) const
    { return CProxyElement_Worker(ckGetArrayID(), idx, CK_DELCTOR_CALL); }
    CProxyElement_Worker operator [] (int idx) const 
        {return CProxyElement_Worker(ckGetArrayID(), CkArrayIndex1D(idx), CK_DELCTOR_CALL);}
    CProxyElement_Worker operator () (int idx) const 
        {return CProxyElement_Worker(ckGetArrayID(), CkArrayIndex1D(idx), CK_DELCTOR_CALL);}
    CProxy_Worker(const CkArrayID &aid,CK_DELCTOR_PARAM) 
        :CProxy_ArrayElement(aid,CK_DELCTOR_ARGS) {}
    CProxy_Worker(const CkArrayID &aid) 
        :CProxy_ArrayElement(aid) {}
/* DECLS: Worker(void);
 */
    
    static CkArrayID ckNew(const CkArrayOptions &opts);
    static CkArrayID ckNew(const int s1);

/* DECLS: sync myMsg* sendSmaller(int impl_noname_0);
 */

/* DECLS: threaded void run(void);
 */
    
    void run(void) ;

/* DECLS: void dump(void);
 */
    
    void dump(void) ;

/* DECLS: void barrierH(void);
 */
    
    void barrierH(void) ;

/* DECLS: Worker(CkMigrateMessage* impl_msg);
 */

};
PUPmarshall(CProxy_Worker)
/* ---------------- section proxy -------------- */
 class CProxySection_Worker : public CProxySection_ArrayElement{
  public:
    typedef Worker local_t;
    typedef CkIndex_Worker index_t;
    typedef CProxy_Worker proxy_t;
    typedef CProxyElement_Worker element_t;
    typedef CProxySection_Worker section_t;

    CProxySection_Worker(void) {}

    void ckDelegate(CkDelegateMgr *dTo,CkDelegateData *dPtr=NULL)
    {       CProxySection_ArrayElement::ckDelegate(dTo,dPtr); }
    void ckUndelegate(void)
    {       CProxySection_ArrayElement::ckUndelegate(); }
    void pup(PUP::er &p)
    {       CProxySection_ArrayElement::pup(p); }

    int ckIsDelegated(void) const
    { return CProxySection_ArrayElement::ckIsDelegated(); }
    inline CkDelegateMgr *ckDelegatedTo(void) const
    { return CProxySection_ArrayElement::ckDelegatedTo(); }
    inline CkDelegateData *ckDelegatedPtr(void) const
    { return CProxySection_ArrayElement::ckDelegatedPtr(); }
    CkGroupID ckDelegatedIdx(void) const
    { return CProxySection_ArrayElement::ckDelegatedIdx(); }

    inline void ckCheck(void) const
    { CProxySection_ArrayElement::ckCheck(); }
    inline operator CkArrayID () const
    { return ckGetArrayID(); }
    inline CkArrayID ckGetArrayID(void) const
    { return CProxySection_ArrayElement::ckGetArrayID(); }
    inline CkArray *ckLocalBranch(void) const
    { return CProxySection_ArrayElement::ckLocalBranch(); }
    inline CkLocMgr *ckLocMgr(void) const
    { return CProxySection_ArrayElement::ckLocMgr(); }

    inline static CkArrayID ckCreateEmptyArray(void)
    { return CProxySection_ArrayElement::ckCreateEmptyArray(); }
    inline static CkArrayID ckCreateArray(CkArrayMessage *m,int ctor,const CkArrayOptions &opts)
    { return CProxySection_ArrayElement::ckCreateArray(m,ctor,opts); }
    inline void ckInsertIdx(CkArrayMessage *m,int ctor,int onPe,const CkArrayIndex &idx)
    { CProxySection_ArrayElement::ckInsertIdx(m,ctor,onPe,idx); }
    inline void doneInserting(void)
    { CProxySection_ArrayElement::doneInserting(); }

    inline void ckBroadcast(CkArrayMessage *m, int ep, int opts=0) const
    { CProxySection_ArrayElement::ckBroadcast(m,ep,opts); }
    inline void setReductionClient(CkReductionClientFn fn,void *param=NULL) const
    { CProxySection_ArrayElement::setReductionClient(fn,param); }
    inline void ckSetReductionClient(CkReductionClientFn fn,void *param=NULL) const
    { CProxySection_ArrayElement::ckSetReductionClient(fn,param); }
    inline void ckSetReductionClient(CkCallback *cb) const
    { CProxySection_ArrayElement::ckSetReductionClient(cb); }

    inline void ckSend(CkArrayMessage *m, int ep, int opts = 0)
    { CProxySection_ArrayElement::ckSend(m,ep,opts); }
    inline CkSectionInfo &ckGetSectionInfo()
    { return CProxySection_ArrayElement::ckGetSectionInfo(); }
    inline CkSectionID *ckGetSectionIDs()
    { return CProxySection_ArrayElement::ckGetSectionIDs(); }
    inline CkSectionID &ckGetSectionID()
    { return CProxySection_ArrayElement::ckGetSectionID(); }
    inline CkSectionID &ckGetSectionID(int i)
    { return CProxySection_ArrayElement::ckGetSectionID(i); }
    inline CkArrayID ckGetArrayIDn(int i) const
    { return CProxySection_ArrayElement::ckGetArrayIDn(i); } 
    inline CkArrayIndex *ckGetArrayElements() const
    { return CProxySection_ArrayElement::ckGetArrayElements(); }
    inline CkArrayIndex *ckGetArrayElements(int i) const
    { return CProxySection_ArrayElement::ckGetArrayElements(i); }
    inline int ckGetNumElements() const
    { return CProxySection_ArrayElement::ckGetNumElements(); } 
    inline int ckGetNumElements(int i) const
    { return CProxySection_ArrayElement::ckGetNumElements(i); }    // Generalized array indexing:
    CProxyElement_Worker operator [] (const CkArrayIndex1D &idx) const
        {return CProxyElement_Worker(ckGetArrayID(), idx, CK_DELCTOR_CALL);}
    CProxyElement_Worker operator() (const CkArrayIndex1D &idx) const
        {return CProxyElement_Worker(ckGetArrayID(), idx, CK_DELCTOR_CALL);}
    CProxyElement_Worker operator [] (int idx) const 
        {return CProxyElement_Worker(ckGetArrayID(), *(CkArrayIndex1D*)&ckGetArrayElements()[idx], CK_DELCTOR_CALL);}
    CProxyElement_Worker operator () (int idx) const 
        {return CProxyElement_Worker(ckGetArrayID(), *(CkArrayIndex1D*)&ckGetArrayElements()[idx], CK_DELCTOR_CALL);}
    static CkSectionID ckNew(const CkArrayID &aid, CkArrayIndex1D *elems, int nElems) {
      return CkSectionID(aid, elems, nElems);
    } 
    static CkSectionID ckNew(const CkArrayID &aid, int l, int u, int s) {
      CkVec<CkArrayIndex1D> al;
      for (int i=l; i<=u; i+=s) al.push_back(CkArrayIndex1D(i));
      return CkSectionID(aid, al.getVec(), al.size());
    } 
    CProxySection_Worker(const CkArrayID &aid, CkArrayIndex *elems, int nElems, CK_DELCTOR_PARAM) 
        :CProxySection_ArrayElement(aid,elems,nElems,CK_DELCTOR_ARGS) {}
    CProxySection_Worker(const CkArrayID &aid, CkArrayIndex *elems, int nElems) 
        :CProxySection_ArrayElement(aid,elems,nElems) {}
    CProxySection_Worker(const CkSectionID &sid)       :CProxySection_ArrayElement(sid) {}
    CProxySection_Worker(int n, const CkArrayID *aid, CkArrayIndex const * const *elems, const int *nElems, CK_DELCTOR_PARAM) 
        :CProxySection_ArrayElement(n,aid,elems,nElems,CK_DELCTOR_ARGS) {}
    CProxySection_Worker(int n, const CkArrayID *aid, CkArrayIndex const * const *elems, const int *nElems) 
        :CProxySection_ArrayElement(n,aid,elems,nElems) {}
    static CkSectionID ckNew(const CkArrayID &aid, CkArrayIndex *elems, int nElems) {
      return CkSectionID(aid, elems, nElems);
    } 
/* DECLS: Worker(void);
 */
    

/* DECLS: sync myMsg* sendSmaller(int impl_noname_0);
 */

/* DECLS: threaded void run(void);
 */
    
    void run(void) ;

/* DECLS: void dump(void);
 */
    
    void dump(void) ;

/* DECLS: void barrierH(void);
 */
    
    void barrierH(void) ;

/* DECLS: Worker(CkMigrateMessage* impl_msg);
 */

};
PUPmarshall(CProxySection_Worker)
#define Worker_SDAG_CODE                                                       \
public:                                                                        \
  void run();                                                                  \
private:                                                                       \
  void run_end();                                                              \
  void _slist_0();                                                             \
  void _slist_0_end();                                                         \
  void _atomic_0();                                                            \
public:                                                                        \
public:                                                                        \
  std::auto_ptr<SDAG::Dependency> __dep;                                       \
  void _sdag_init();                                                           \
  void __sdag_init();                                                          \
public:                                                                        \
  void __sdag_pup(PUP::er &p);                                                 \
  static void __sdag_register();                                               \
  static int _sdag_idx_Worker_atomic_0();                                      \
  static int _sdag_reg_Worker_atomic_0();                                      \

typedef CBaseT1<ArrayElementT<CkIndex1D>, CProxy_Worker> CBase_Worker;





/* ---------------- method closures -------------- */
class Closure_Main {
  public:


};

/* ---------------- method closures -------------- */
class Closure_Worker {
  public:


    struct sendSmaller_2_closure;


    struct run_3_closure;


    struct dump_4_closure;


    struct barrierH_5_closure;


};

extern void _registeroddEven(void);
extern "C" void CkRegisterMainModule(void);
#endif
