#include "digital_io.h"
#include <cassert>
#include <cstdio>
int main(){
 DigitalIO d; const double ts=20e-6;
 assert(d.adc(0,0,80)==0 && d.adc(90,0,80)==80);
 assert(std::fabs(d.adc(48,0,80)-48)<=80.0/4095/2);
 for(int i=0;i<300;i++)d.step(48,48,1,96,1,ts);
 assert(d.ctrl.state==Boost::SOFT_START);
 double p=d.pending; d.step(48,48,1,96,1,ts); assert(d.duty==p);
 assert(std::fabs(d.pending*1000-(long)(d.pending*1000+.5))<1e-8);
 double old=d.fv; d.step(44,48,1,96,1,ts); assert(std::fabs(d.fv-(old+d.adc(44,0,80))/2)<1e-9);
 d.step(48,48,16,96,1,ts);assert(d.ctrl.fault==Boost::OVERCURRENT&&d.duty==0&&d.pending==0);
 DigitalIO e;for(int i=0;i<300;i++)e.step(48,48,1,96,1,ts);e.step(48,48,1,96,0,ts);assert(e.ctrl.state==Boost::STOP&&e.duty==0&&e.pending==0);
 DigitalIO f;f.cfg.enabled=false; Boost b;for(int i=0;i<1000;i++){f.step(48,48,1,96,1,ts);b.step(48,48,1,96,1,ts);assert(f.duty==b.duty);}
 DigitalIO g;g.step(NAN,48,1,96,1,ts);assert(g.ctrl.fault==Boost::INVALID_SENSOR);
 puts("PASS ADC, filter, PWM quantization, one-sample delay, raw OC, STOP, ideal comparison, NaN");
}
