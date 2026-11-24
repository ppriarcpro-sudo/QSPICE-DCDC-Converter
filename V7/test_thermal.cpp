#include "thermal_supervisor.h"
#include <cassert>
#include <cstdio>
int main(){
 ThermalIO t; double dt=20e-6;
 for(int i=0;i<300;i++)t.step(48,48,1,96,2,dt);
 assert(t.io.ctrl.state==Boost::SOFT_START);
 t.io.ctrl.xv=9;
 t.step(48,60,2,96,1.5,dt);
 assert(t.io.ctrl.cfg.i_max==5 && t.io.ctrl.iref<=5 && t.io.ctrl.xv<=5);
 t.step(48,60,2,96,-1,dt);
 assert(t.io.ctrl.fault==7 && t.io.duty==0 && t.io.pending==0);
 t.step(48,60,2,96,2,dt); assert(t.io.ctrl.fault==7 && t.io.duty==0);
 ThermalIO s; s.step(48,48,1,96,0,dt); assert(s.io.ctrl.state==Boost::STOP);
 puts("PASS thermal derating, trip latch, stop");
}
