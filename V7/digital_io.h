#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H
#include "boost_core.h"
struct DigitalConfig {
 bool enabled; double alpha; int adc_levels,pwm_levels; bool delay;
 DigitalConfig():enabled(true),alpha(.5),adc_levels(4095),pwm_levels(1000),delay(true){}
};
struct DigitalIO {
 DigitalConfig cfg; Boost ctrl; bool initialized; double fv,fo,fi,pending,duty;
 DigitalIO():initialized(false),fv(0),fo(0),fi(0),pending(0),duty(0){}
 double adc(double x,double lo,double hi) {
  double q=Boost::clamp(x,lo,hi); return lo+(long)((q-lo)/(hi-lo)*cfg.adc_levels+.5)*(hi-lo)/cfg.adc_levels;
 }
 void step(double vin,double vo,double il,double target,double enable,double dt){
  // Check raw samples before clipping or filtering; sampled protection only.
  if(!Boost::finite_value(vin)||!Boost::finite_value(vo)||!Boost::finite_value(il)||!Boost::finite_value(target)||!Boost::finite_value(enable)||!Boost::finite_value(dt)||dt<=0)ctrl.trip(Boost::INVALID_SENSOR);
  else if(vo>ctrl.cfg.ov)ctrl.trip(Boost::OUTPUT_OV);
  else if(vin>ctrl.cfg.ov_in||vin<0)ctrl.trip(Boost::INPUT_OV);
  else if((ctrl.state==Boost::RUN||ctrl.state==Boost::SOFT_START)&& (il>ctrl.cfg.oc||il<-5))ctrl.trip(Boost::OVERCURRENT);
  else if((ctrl.state==Boost::RUN||ctrl.state==Boost::SOFT_START)&&vin<ctrl.cfg.uv)ctrl.trip(Boost::INPUT_UV);
  if(ctrl.state==Boost::FAULT){pending=duty=0;return;}
  if(!cfg.enabled){ctrl.step(vin,vo,il,target,enable,dt);duty=ctrl.duty;return;}
  double a=adc(vin,0,80),b=adc(vo,0,140),c=adc(il,-20,20);
  if(!initialized){fv=a;fo=b;fi=c;initialized=true;}
  else {fv+=cfg.alpha*(a-fv);fo+=cfg.alpha*(b-fo);fi+=cfg.alpha*(c-fi);}
  ctrl.step(fv,fo,fi,target,enable,dt);
  if(ctrl.state!=Boost::RUN&&ctrl.state!=Boost::SOFT_START){pending=duty=0;return;}
  double q=(long)(ctrl.duty*cfg.pwm_levels+.5)/(double)cfg.pwm_levels;
  q=Boost::clamp(q,0,ctrl.cfg.d_max);
  duty=cfg.delay?pending:q;pending=q;
 }
};
#endif
