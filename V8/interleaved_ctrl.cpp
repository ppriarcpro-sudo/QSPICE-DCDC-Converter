#include "interleaved_digital.h"
union uData { bool b; char c; unsigned char uc; short s; unsigned short us; int i; unsigned int ui; float f; double d; long long i64; unsigned long long ui64; char *str; unsigned char *bytes; };
int __stdcall DllMain(void *,unsigned int,void *){return 1;}
struct Instance { Digital io; long tick; Instance():tick(-1){} };
extern "C" __declspec(dllexport) void interleaved_ctrl(Instance **opaque,double t,uData *data){
 if(!*opaque)*opaque=new Instance;
 Instance &s=**opaque;
 const double Ts=20e-6;
 long tick=(long)((t+1e-12)/Ts);
 if(t>=0&&tick>s.tick){
  Inputs u={data[0].d,data[1].d,data[2].d,data[3].d,data[4].d,data[5].d,data[6].d,data[7].d};
  s.io.step(u,Ts);s.tick=tick;
 }
 const Outputs &y=s.io.ctrl.y;
 data[8].d=s.io.d1;data[9].d=s.io.d2;data[10].d=y.r1;data[11].d=y.r2;
 data[12].d=y.state;data[13].d=y.fault;data[14].d=y.ref;
 data[15].d=y.phases;data[16].d=y.limit1;data[17].d=y.limit2;
}
extern "C" __declspec(dllexport) double MaxExtStepSize(Instance *){return 10e-9;}
extern "C" __declspec(dllexport) void Destroy(Instance *s){delete s;}
