#ifndef COMPLEX_H
#define COMPLEX_H

#include "Include.h"





class complex {
  private:
  protected:
  public:
    float a, b;
    static unsigned int additions, multiplications;
    complex();
    complex(float a, float b);
    complex conj();
    complex operator*(const complex& c) const;
    complex operator+(const complex& c) const;
    complex operator-(const complex& c) const;
    complex operator-() const;
    complex operator*(const float c) const;
    complex& operator=(const complex& c);
    static void reset();
};



struct vertex_ocean {
	float   x, y, z; // vertex
	float  nx, ny, nz; // normal
	unsigned long color;
	float   a, b, c; // htilde0
	float  _a, _b, _c; // htilde0mk conjugate
	float  ox, oy, oz; // original position
};

struct complex_vector_normal {	// structure used with discrete fourier transform
	complex h;		// wave height
	_vec2 D;		// displacement
	_vec3 n;		// normal
};


#endif