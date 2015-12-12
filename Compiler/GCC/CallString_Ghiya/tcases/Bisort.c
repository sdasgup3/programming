#define CONST_m1 10000
#define CONST_b 31415821
#define RANGE 100
#define NULL 0

struct node { 
  int value;
  struct node *left;
  struct node *right;
  };
int flag=0,foo=0;
int __NumNodes,__NDim;
int dealwithargs(int argc, char *argv[])
{
  int size;

  if (argc > 3)
    flag = atoi(argv[3]);
  else
    flag = 0;

  if (argc > 2) 
    __NumNodes = atoi(argv[2]);
  else 
    __NumNodes = 4;

  if (argc > 1)
    size = atoi(argv[1]);
  else
    size = 16;
  __NDim = mylog(__NumNodes);
  return size;
  
}
int mylog(int num)
{
  int j=0,k=1;
  
  while(k<num) { k*=2; j++; }
  return j;
} 
void InOrder(struct node *h)
{
struct node *l, *r;
if ((h != NULL))
  {
   l = h->left;
   r = h->right;
   InOrder(l);
//    chatting("%d @ 0x%x\n",h->value,h);
   InOrder(r);
  }
}
int mult(int p, int q)
{
	int p1, p0, q1, q0;
	
	p1=p/CONST_m1; p0=p%CONST_m1;
	q1=q/CONST_m1; q0=q%CONST_m1;
	return (((p0*q1+p1*q0) % CONST_m1)*CONST_m1+p0*q0);
}

int skiprand(int seed, int n)
/* Generate the nth random # */
{
  for (; n; n--) seed=random(seed);
  return seed;
}

int random(int seed)
{
  int tmp;
  tmp = (mult(seed,CONST_b)+1);
  return tmp;
}

struct node* RandTree(int n,int seed,int node,int level)
{
  int next_val,my_name;
//   future_cell_int f_left, f_right;
  struct node *h;
  my_name=foo++;
  if ((n > 1))
    {
      int newnode;
      if (level<__NDim)
	{
	  newnode = node + (1 <<  (__NDim-level-1));
	}
      else
	newnode = node;
      seed = random(seed);
      next_val=seed % RANGE;
//       NewNode(h,next_val,node);
// #ifdef FUTURES
//       FUTURE((n/2),seed,newnode,level+1,RandTree,&f_left);
//       FUTURE((n / 2),skiprand(seed,(n)+1),node,level+1,RandTree,&f_right);
//       TOUCH(&f_left);
// #else
//       f_left.value=(int) RandTree((n/2),seed,newnode,level+1);
//       f_right.value=(int) RandTree((n/2),skiprand(seed,(n)+1),node,level+1);
// #endif
//       h->left = (struct node *) f_left.value;
// #ifdef FUTURES
//       TOUCH(&f_right);
// #endif
//       h->right = (struct node *) f_right.value;
    }
  else 
    h = NULL;
  return(h);
} 

void SwapValue(struct node *l,struct node *r)
{ int temp,temp2;
  
  temp = l->value;
  temp2 = r->value;
// //   NOTEST();
  r->value = temp;
//   RETEST();
//   l->value = temp2;
} 

// #ifndef ONEONLY
void SwapTree(struct node *t1,struct node *t2)
{
  if ((t1==NULL) || (t2==NULL)) return;
  if ((t1) !=(t2))   //PID(t1) != PID(t2)
    {
      int t1val, t2val;
      struct node *t1left, *t1right, *t2left, *t2right;
  
//       chatting("Swap 0x%x,0x%x\n",t1,t2);
      t1val = t1->value;
//       NOTEST();
      t1left = t1->left;
      t1right = t1->right;
//       RETEST();
      t2val = t2->value;
//       NOTEST();
      t2left = t2->left;
      t2right = t2->right;
      t2->value = t1val;
//       RETEST();
      t1->value = t2val;
      /*chatting("values were %d,%d\n",t1val,t2val);*/
      if ((t1) != (t1left))  //PID(t1) != PID(t1left)
	{
// 	  future_cell_int fcleft;
// 	  FUTURE(t1left,t2left,SwapTree,&fcleft);
     SwapTree(t1right,t2right);
// 	  TOUCH(&fcleft); 
/*	  SwapTree(t1left,t2left);
	  SwapTree(t1right,t2right);*/
	}
      else 
	{
// 	  future_cell_int fcleft;
// 	  FUTURE(t1left,t2left,NewSwapSubTree,&fcleft);
//      NewSwapSubTree(t1right,t2right);
// 	  TOUCH(&fcleft);
/*	  NewSwapSubTree(t1left,t2left);
	  NewSwapSubTree(t1right,t2right); */
	}
    }
  else 
    {
      int tempval;
      struct node *temphandle;
//       NOTEST();
      /*chatting("I think I'm local\n");*/
      tempval=t1->value;
      t1->value=t2->value;
      t2->value=tempval;
      temphandle=t1->left;
      t1->left=t2->left;
      t2->left=temphandle;
      temphandle=t1->right;
      t1->right=t2->right;
      t2->right=temphandle;
//       RETEST();
    }
	
  /*chatting("End Swap 0x%x,0x%x\n",t1,t2);*/
}



void
/***********/
SwapValLeft(struct node *l,struct node *r,struct node *ll,struct node *rl,int lval,int rval)
{
  r->value = lval;
// #ifdef ONEONLY  
//   r->left = ll;
//   l->left = rl;
// #else
  if (rl) SwapTree(rl,ll);
// #endif  
  l->value = rval;
} 


void SwapValRight(struct node *l,struct node *r,struct node *lr,struct node *rr,int lval,int rval)
{  
  r->value = lval;
// #ifdef ONEONLY
//   r->right = lr;
//   l->right = rr;
// #else
  if (lr) SwapTree(lr,rr);
// #endif  
  l->value = rval;
  /*chatting("Swap Val Right l 0x%x,r 0x%x val: %d %d\n",l,r,lval,rval);*/
} 

int Bimerge(struct node *root, int spr_val,int dir)
{ int rightexchange;
  int elementexchange;
  int temp;
  struct node *pl,*pll,*plr;
  struct node *pr,*prl,*prr;
  struct node *rl;
  struct node *rr;
/*#ifdef  DUMB
  struct node *dummy;
#endif */ 
  int rv,lv;


  /*chatting("enter bimerge %x\n", root);*/
  rv = root->value;
//   NOTEST();
  pl = root->left;
  pr = root->right;
  rightexchange = ((rv > spr_val) ^ dir);
  if (rightexchange)
    {
      root->value = spr_val;
      spr_val = rv;
    }
//   RETEST();
  
  while ((pl != NULL))
    {
      /*chatting("pl = 0x%x,pr = 0x%x\n",pl,pr);*/
      lv = pl->value;
//       NOTEST();
      pll = pl->left;
      plr = pl->right;
//       RETEST();
      rv = pr->value;
//       NOTEST();
      prl = pr->left;
      prr = pr->right;
//       RETEST();
      elementexchange = ((lv > rv) ^ dir);
      if (rightexchange)
        if (elementexchange)
          { 
            SwapValRight(pl,pr,plr,prr,lv,rv);
            pl = pll;
            pr = prl;
          }
        else 
          { pl = plr;
            pr = prr;
          }
      else 
        if (elementexchange)
          { 
            SwapValLeft(pl,pr,pll,prl,lv,rv);
            pl = plr;
            pr = prr;
          }
        else 
          { pl = pll;
            pr = prl;
          }
    }
  if ((root->left != NULL))
    { 
//       future_cell_int f_left;
      int value;
//       NOTEST();
      rl = root->left;
      rr = root->right;
      value = root->value;
/*#ifdef DUMB
      LocalNewNode(dummy,value);
#endif */     
// #ifdef FUTURES
//       FUTURE(rl,value,dir,Bimerge,&f_left);		/*** FUTURE CALL ***/
//       spr_val=Bimerge(rr,spr_val,dir);
//       TOUCH(&f_left);
//       root->value = f_left.value;
// #else
      root->value=Bimerge(rl,value,dir);
      spr_val=Bimerge(rr,spr_val,dir);
// #endif
//       RETEST();
    }
  /*chatting("exit bimerge %x\n", root);*/
  return spr_val;
} 

int Bisort(struct node *root,int spr_val,int dir)
{ struct node *l;
  struct node *r;
// #ifdef DUMB
//   struct node *dummy;
// #endif  
  int val;
  /*chatting("bisort %x\n", root);*/
  if ((root->left == NULL))
    { 
//      NOTEST();
     if (((root->value > spr_val) ^ dir))
        {
	  val = spr_val;
	  spr_val = root->value;
	  root->value =val;
	}
//     RETEST();
    }
  else 
    {
//       future_cell_int f_left;
      int ndir;
//       NOTEST();
      l = root->left;
      r = root->right;
      val = root->value;
      /*chatting("root 0x%x, l 0x%x, r 0x%x\n", root,l,r);*/
/*#ifdef DUMB 
      LocalNewNode(dummy,val);
#endif */     
// #ifdef FUTURES
//       FUTURE(l,val,dir,Bisort,&f_left);		/*** FUTURE CALL ***/
//       ndir = !dir;
//       spr_val = Bisort(r,spr_val,ndir);
//       TOUCH(&f_left);
//       root->value = f_left.value;
// #else
      root->value=Bisort(l,val,dir);
      ndir = !dir;
      spr_val=Bisort(r,spr_val,ndir);
// #endif
      spr_val=Bimerge(root,spr_val,dir);
//       RETEST();
    }
  /*chatting("exit bisort %x\n", root);*/
  return spr_val;
} 

void main(int argc,char *argv[])
{ struct node *h;
  unsigned long time1, time2, time3, time4;
  int sval;
  int height;
  int counter;
  int n;
  int result = 1;
   
  
// #ifdef FUTURES 
//   n = SPMDInit (argc,argv);
// #else 
  n = dealwithargs(argc,argv);
// #endif

//   chatting("Bisort with %d size on %d procs of dim %d\n",
// 	   n, __NumNodes, __NDim);
//   CMMD_node_timer_clear(0);
//   CMMD_node_timer_clear(1);
  h = RandTree(n,12345768,0,0);
  sval = random(245867) % RANGE;
  if (flag) {
    InOrder(h);
//     chatting("%d\n",sval);
   }
//   chatting("**************************************\n");
//   chatting("BEGINNING BITONIC SORT ALGORITHM HERE\n");
//   chatting("**************************************\n");

// #ifndef PLAIN
//   ClearAllStats();
// #endif

//   CMMD_node_timer_start(0);
  sval=Bisort(h,sval,0);
//   CMMD_node_timer_stop(0);
  if (flag) {
//     chatting("Sorted Tree:\n"); 
    InOrder(h);
//     chatting("%d\n",sval);
   }
  
//   CMMD_node_timer_start(1);
  sval=Bisort(h,sval,1);
//   CMMD_node_timer_stop(1);
  if (flag) {
//     chatting("Sorted Tree:\n"); 
    InOrder(h);
//     chatting("%d\n",sval);
   }
/*
  chatting("Time to sort forward = %f\n",CMMD_node_timer_elapsed(0));
  chatting("Time to sort backward = %f\n",CMMD_node_timer_elapsed(1));
  chatting("Total: %f\n",CMMD_node_timer_elapsed(0)+
           CMMD_node_timer_elapsed(1));*/
// #ifdef FUTURES
//   __ShutDown(0);
// #endif
  exit(0);
} 

