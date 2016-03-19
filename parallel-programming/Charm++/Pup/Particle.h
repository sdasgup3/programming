#ifndef PARTICLE_H
#define PARTICLE_H

/*
*Particle object with x&y coordinate components
*/

class Particle  {
public:
    double x;
    double y;
    int size;
    int *arr;


    Particle() { }
    Particle(double a, double b, int* A, int s) { 
      x=a; y=b; 
      arr = A; 
      size = s; 
      printf("%f %f %d\n",x, y, size);
      for(int i = 0 ; i < size ; i++) {
        printf("%d ",arr[i]);
      }
      printf("\n");
    }

    void pup(PUP::er &p){
      p|x;
      p|y;
      p|size;
      int isNull = (!arr);
      p|isNull;
      if(p.isUnpacking()) {
        if(isNull) {
          arr = NULL;
        } else {
          arr = new int[size];
        }
      }
      p(arr, size);
    }

};

#endif
