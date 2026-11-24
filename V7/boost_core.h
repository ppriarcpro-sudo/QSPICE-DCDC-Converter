#ifndef BOOST_CORE_H
#define BOOST_CORE_H
#include <cmath>
#include <float.h>
struct BoostConfig {
 double kp_v,ki_v,kp_i,ki_i,i_max,d_max,ramp,ov,oc,uv,ov_in,ready_time,init_timeout;
 BoostConfig():kp_v(.2),ki_v(80),kp_i(.08),ki_i(30),i_max(10),d_max(.75),ramp(2400),ov(115),oc(15),uv(35),ov_in(60),ready_time(.001),init_timeout(.035){}
};
struct Boost {
 enum { INIT, SOFT_START, RUN, STOP, FAULT };
 enum { NONE, OUTPUT_OV, OVERCURRENT, INPUT_UV, INPUT_OV, START_TIMEOUT, INVALID_SENSOR };
 BoostConfig cfg;
 double xv,xi,duty,iref,ref,elapsed,ready;
 int state,fault;
 Boost():xv(0),xi(0),duty(0),iref(0),ref(48),elapsed(0),ready(0),state(INIT),fault(NONE){}
 static bool finite_value(double x){return x >= -DBL_MAX && x <= DBL_MAX;}
 static double clamp(double x,double lo,double hi){return x<lo?lo:(x>hi?hi:x);}
 void off(){duty=iref=xv=xi=0;}
 void trip(int code){fault=code;state=FAULT;off();}
 void step(double vin,double vo,double il,double target,double enable,double dt){
  if(state==FAULT){off();return;}
  if(!finite_value(vin)||!finite_value(vo)||!finite_value(il)||!finite_value(target)||!finite_value(enable)||!finite_value(dt)||dt<=0){trip(INVALID_SENSOR);return;}
  if(vo>cfg.ov){trip(OUTPUT_OV);return;}
  if(vin>cfg.ov_in||vin<0){trip(INPUT_OV);return;}
  if(enable<.5){state=STOP;off();elapsed=ready=0;return;}
  if(state==STOP){state=INIT;elapsed=ready=0;}
  elapsed+=dt;
  if(state==INIT){
   off();ref=vin;
   // Passive charging through boost diode; wait for settled voltage/current.
   if(vin>=40 && vo>=.95*vin && vo<=1.05*vin && il>=-1 && il<=3)ready+=dt;else ready=0;
   if(elapsed>=.005 && ready>=cfg.ready_time){state=SOFT_START;ref=vo; xv=clamp(il,0,cfg.i_max);}
   else if(elapsed>=cfg.init_timeout)trip(START_TIMEOUT);
   return;
  }
  if(il>cfg.oc||il<-5){trip(OVERCURRENT);return;}
  if(vin<cfg.uv){trip(INPUT_UV);return;}
  target=clamp(target,vin,100);
  ref+=clamp(target-ref,-cfg.ramp*dt,cfg.ramp*dt);
  if(state==SOFT_START && std::fabs(ref-target)<.05 && std::fabs(vo-target)<2)state=RUN;
  double ev=ref-vo, raw=cfg.kp_v*ev+xv;
  if((raw<cfg.i_max||ev<0)&&(raw>0||ev>0))xv=clamp(xv+cfg.ki_v*ev*dt,0,cfg.i_max);
  iref=clamp(cfg.kp_v*ev+xv,0,cfg.i_max);
  double ei=iref-il,ff=1-vin/(vo>vin?vo:vin);
  raw=ff+cfg.kp_i*ei+xi;
  if((raw<cfg.d_max||ei<0)&&(raw>0||ei>0))xi=clamp(xi+cfg.ki_i*ei*dt,-.2,.2);
  duty=clamp(ff+cfg.kp_i*ei+xi,0,cfg.d_max);
 }
};
#endif
