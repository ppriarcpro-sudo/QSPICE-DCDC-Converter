#include "interleaved_digital.h"
#include <cassert>
#include <cstdio>
int main(){
 Digital d; Inputs u={48,48,1,1,96,1,25,25};double ts=20e-6;
 for(int i=0;i<300;i++)d.step(u,ts);
 assert(d.ctrl.y.state==Controller::SOFT_START);
 u.vo=80;u.i1=3;u.i2=1;
 for(int i=0;i<100;i++)d.step(u,ts);
 assert(d.ctrl.y.d1<d.ctrl.y.d2);
 assert(d.ctrl.y.r1==d.ctrl.y.r2);
 u.t1=125;d.step(u,ts);assert(d.ctrl.y.limit1==5&&d.ctrl.y.r1<=5);
 u.t2=140;d.step(u,ts);assert(d.ctrl.y.fault==7&&d.d1==0&&d.d2==0);
 u.t2=25;d.step(u,ts);assert(d.ctrl.y.state==Controller::FAULT);
 Digital e;u.t1=25;u.vo=48;u.i1=u.i2=1;
 for(int i=0;i<300;i++)e.step(u,ts);
 u.i2=16;e.step(u,ts);assert(e.ctrl.y.fault==2&&e.d1==0&&e.d2==0);
 Controller c;c.cfg.shedding=true;c.y.state=Controller::RUN;c.y.ref=96;c.xv=1;
 Inputs s={48,96,.5,.5,96,1,25,25};
 for(int i=0;i<1100;i++)c.step(s,ts);
 assert(c.y.phases==1&&c.y.d2==0);
 s.vo=90;c.step(s,ts);assert(c.y.phases==2);
 puts("PASS sharing, thermal limits, latched trip, OC, phase shedding");
}
