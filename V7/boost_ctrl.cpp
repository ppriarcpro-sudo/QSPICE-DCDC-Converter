#include "thermal_supervisor.h"
union uData { bool b; char c; unsigned char uc; short s; unsigned short us; int i; unsigned int ui; float f; double d; long long i64; unsigned long long ui64; char *str; unsigned char *bytes; };
int __stdcall DllMain(void *,unsigned int,void *){return 1;}
struct Instance { ThermalIO io; long tick; Instance():tick(-1){} };
extern "C" __declspec(dllexport) void boost_ctrl(Instance **opaque,double t,uData *data){
 if(!*opaque)*opaque=new Instance;
 Instance &s=**opaque;
 const double Ts=20e-6;
 long tick=(long)((t+1e-12)/Ts);
 if(t>=0 && tick>s.tick){s.io.step(data[0].d,data[1].d,data[2].d,data[3].d,data[4].d,Ts);s.tick=tick;}
 data[5].d=s.io.io.duty; data[6].d=s.io.io.ctrl.iref; data[7].d=s.io.io.ctrl.state; data[8].d=s.io.io.ctrl.fault; data[9].d=s.io.io.ctrl.ref;
}
extern "C" __declspec(dllexport) double MaxExtStepSize(Instance *){return 200e-9;}
extern "C" __declspec(dllexport) void Destroy(Instance *s){delete s;}
