#ifndef PARTICLE_H
#define PARTICLE_H

class Particle  {
public:
    double x;
    double y;
    unsigned char R, G, B;

    Particle() { }
    Particle(double _x, double _y, unsigned char _R, unsigned char _G, unsigned _B): 
      x(_x), y(_y), R(_R), G(_G), B(_B){ }

    void pup(PUP::er &p){
      p|x;
      p|y;
      p|R;
      p|G;
      p|B;
    }

};

#endif
