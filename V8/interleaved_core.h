#ifndef INTERLEAVED_CORE_H
#define INTERLEAVED_CORE_H
#include <float.h>
#include <cmath>
struct Config {
 double kp_v,ki_v,kp_i,ki_i,i_phase,oc,ov,uv,ramp,ts;
 bool shedding;
 Config():kp_v(.2),ki_v(80),kp_i(.08),ki_i(30),i_phase(10),oc(15),ov(115),uv(35),ramp(2400),ts(20e-6),shedding(false){}
};
struct Inputs { double vin,vo,i1,i2,target,enable,t1,t2; };
struct Outputs { double d1,d2,r1,r2,ref,limit1,limit2; int state,fault,phases; };
struct Controller {
 enum {INIT,SOFT_START,RUN,STOP,FAULT};
 Config cfg; Outputs y; double xv,xi[2],elapsed,ready,lowtime; bool single;
 Controller():y(),xv(0),elapsed(0),ready(0),lowtime(0),single(false){xi[0]=xi[1]=0;y.state=INIT;}
 static bool finite(double x){return x>=-DBL_MAX&&x<=DBL_MAX;}
 static double clamp(double x,double l,double h){return x<l?l:(x>h?h:x);}
 static double thermal(double t){return clamp((140-t)/30,0,1);}
 void off(){y.d1=y.d2=y.r1=y.r2=xv=xi[0]=xi[1]=0;y.phases=0;single=false;lowtime=0;}
 void trip(int n){y.state=FAULT;y.fault=n;off();}
 double pi(double e,double &x,double ff,double kp,double ki,double lo,double hi,double dt){
  double raw=ff+kp*e+x;
  if((raw<hi||e<0)&&(raw>lo||e>0))x=clamp(x+ki*e*dt,lo-ff,hi-ff);
  return clamp(ff+kp*e+x,lo,hi);
 }
 void step(const Inputs &u,double dt){
  if(y.state==FAULT){off();return;}
  if(!finite(u.vin)||!finite(u.vo)||!finite(u.i1)||!finite(u.i2)||!finite(u.target)||!finite(u.enable)||!finite(u.t1)||!finite(u.t2)||!finite(dt)||dt<=0){trip(6);return;}
  y.limit1=cfg.i_phase*thermal(u.t1); y.limit2=cfg.i_phase*thermal(u.t2);
  if(u.t1>=140||u.t2>=140){trip(7);return;}
  if(u.vo>cfg.ov){trip(1);return;}
  if(u.vin>60||u.vin<0){trip(4);return;}
  if(u.enable<.5){y.state=STOP;off();elapsed=ready=0;return;}
  if(y.state==STOP){y.state=INIT;elapsed=ready=0;}
  elapsed+=dt;
  if(y.state==INIT){
   off();y.ref=u.vin;
   if(u.vin>=40&&u.vo>=.95*u.vin&&u.vo<=1.05*u.vin&&std::fabs(u.i1)<3&&std::fabs(u.i2)<3)ready+=dt;else ready=0;
   if(elapsed>=.005&&ready>=.001){y.state=SOFT_START;y.ref=u.vo;xv=clamp(u.i1+u.i2,0,y.limit1+y.limit2);}
   else if(elapsed>=.035)trip(5);
   return;
  }
  if(u.i1>cfg.oc||u.i2>cfg.oc||u.i1<-5||u.i2<-5){trip(2);return;}
  if(u.vin<cfg.uv){trip(3);return;}
  double target=clamp(u.target,u.vin,100);
  y.ref+=clamp(target-y.ref,-cfg.ramp*dt,cfg.ramp*dt);
  if(y.state==SOFT_START&&std::fabs(y.ref-target)<.05&&std::fabs(u.vo-target)<2)y.state=RUN;
  double total_limit=y.limit1+y.limit2;
  xv=clamp(xv,0,total_limit);
  double total=pi(y.ref-u.vo,xv,0,cfg.kp_v,cfg.ki_v,0,total_limit,dt);
  // Optional shedding only in RUN. Hysteresis and dwell; no fault isolation.
  if(!cfg.shedding||y.state!=RUN||total>2.5||total>y.limit1||std::fabs(y.ref-u.vo)>2){single=false;lowtime=0;}
  else if(total<1.5&&std::fabs(y.ref-u.vo)<.5&&total<y.limit1){lowtime+=dt;if(lowtime>=.02)single=true;}
  else lowtime=0;
  y.phases=single?1:2;
  y.r1=single?clamp(total,0,y.limit1):clamp(total*.5,0,y.limit1);
  y.r2=single?0:clamp(total-y.r1,0,y.limit2);
  if(!single)y.r1=clamp(total-y.r2,0,y.limit1);
  double ff=1-u.vin/(u.vo>u.vin?u.vo:u.vin);
  y.d1=pi(y.r1-u.i1,xi[0],ff,cfg.kp_i,cfg.ki_i,0,.75,dt);
  if(single){xi[1]=0;y.d2=0;}
  else y.d2=pi(y.r2-u.i2,xi[1],ff,cfg.kp_i,cfg.ki_i,0,.75,dt);
 }
};
#endif
