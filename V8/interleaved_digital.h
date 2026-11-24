#ifndef INTERLEAVED_DIGITAL_H
#define INTERLEAVED_DIGITAL_H
#include "interleaved_core.h"
struct Digital {
 Controller ctrl; Inputs filtered; bool initialized; double pending1,pending2,d1,d2;
 Digital():filtered(),initialized(false),pending1(0),pending2(0),d1(0),d2(0){}
 static double adc(double x,double l,double h){return l+(long)((Controller::clamp(x,l,h)-l)*4095/(h-l)+.5)*(h-l)/4095;}
 static double pwm(double d){return (long)(d*1000+.5)/1000.;}
 void step(const Inputs &u,double dt){
  if(!Controller::finite(u.vin)||!Controller::finite(u.vo)||!Controller::finite(u.i1)||!Controller::finite(u.i2)||!Controller::finite(u.t1)||!Controller::finite(u.t2)||!Controller::finite(u.target)||!Controller::finite(u.enable)||!Controller::finite(dt)||dt<=0)ctrl.trip(6);
  else if(u.t1>=140||u.t2>=140)ctrl.trip(7);
  else if(u.vo>ctrl.cfg.ov)ctrl.trip(1);
  else if(u.vin>60||u.vin<0)ctrl.trip(4);
  else if(ctrl.y.state==Controller::RUN||ctrl.y.state==Controller::SOFT_START){
   if(u.i1>ctrl.cfg.oc||u.i2>ctrl.cfg.oc||u.i1<-5||u.i2<-5)ctrl.trip(2);
   else if(u.vin<ctrl.cfg.uv)ctrl.trip(3);
  }
  if(ctrl.y.state==Controller::FAULT){pending1=pending2=d1=d2=0;return;}
  Inputs q=u;
  q.vin=adc(u.vin,0,80);q.vo=adc(u.vo,0,140);
  q.i1=adc(u.i1,-20,20);q.i2=adc(u.i2,-20,20);
  if(!initialized){filtered=q;initialized=true;}
  else{filtered.vin=.5*(filtered.vin+q.vin);filtered.vo=.5*(filtered.vo+q.vo);filtered.i1=.5*(filtered.i1+q.i1);filtered.i2=.5*(filtered.i2+q.i2);}
  filtered.target=u.target;filtered.enable=u.enable;filtered.t1=u.t1;filtered.t2=u.t2;
  ctrl.step(filtered,dt);
  if(ctrl.y.state!=Controller::RUN&&ctrl.y.state!=Controller::SOFT_START){pending1=pending2=d1=d2=0;return;}
  d1=pending1;d2=ctrl.y.phases==1?0:pending2;
  pending1=pwm(ctrl.y.d1);pending2=pwm(ctrl.y.d2);
 }
};
#endif
