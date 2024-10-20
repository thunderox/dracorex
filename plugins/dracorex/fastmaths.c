
#include "fastmaths.h"


//------------------------------------------------------------------------------------------------
// APPROX POW

double fast_pow(double a, double b) {
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}

//------------------------------------------------------------------------------------------------
// APPROX ATAN

double fast_atan( double x )
{
    return (x / (1.0 + 0.28 * (x * x)));
}




//===============================================================

float p4fast(float in)
{
  long *lp,l;

  lp=(long *)(&in);
  l=*lp;

  l-=0x3F800000l; /* un-bias */
  l<<=2;          /* **4 */
  l+=0x3F800000l; /* bias */
  *lp=l;

  /* compiler will read this from memory since & operator had been used */
  return in;
}

//===============================================================

float fastishP2F (float pitch)
{
long convert;
float *p=(float *)&convert;
float fl,fr,warp,out;

pitch *=0.0833333; //pitch scaling. remove this line for pow(2,a)
fl = floor(pitch);
fr = pitch - fl;
float fr2 = fr*fr;
warp = fr*0.696 + fr2*0.225 + fr*fr2*0.079;  // chebychev approx
//warp = fr*0.65 + fr*fr*0.35; // chebychev approx

out = fl+warp;
out *= 8388608.0; //2^23;
out += 127.0 * 8388608.0; //2^23;

convert = out; //magic

return *p;
}




