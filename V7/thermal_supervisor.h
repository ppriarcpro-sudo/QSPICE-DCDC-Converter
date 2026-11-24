#ifndef THERMAL_SUPERVISOR_H
#define THERMAL_SUPERVISOR_H
#include "digital_io.h"
struct ThermalIO {
 DigitalIO io;
 double nominal_current;
 ThermalIO():nominal_current(io.ctrl.cfg.i_max){}
 void step(double vin,double vo,double il,double target,double code,double dt){
  if(!Boost::finite_value(code) || code>2 || (code>0 && code<1)){
   io.ctrl.trip(Boost::INVALID_SENSOR); io.pending=io.duty=0; return;
  }
  if(code<0 || code==1){
   io.ctrl.trip(7); io.pending=io.duty=0; return;
  }
  double f=code==0?1:Boost::clamp(code-1,0,1);
  io.ctrl.cfg.i_max=nominal_current*f;
  io.ctrl.xv=Boost::clamp(io.ctrl.xv,0,io.ctrl.cfg.i_max);
  io.step(vin,vo,il,target,code==0?0:1,dt);
 }
};
#endif
