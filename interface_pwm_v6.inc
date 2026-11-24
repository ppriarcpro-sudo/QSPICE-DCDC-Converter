* Driver and signed power probes (1 V represents 1 W)
Vcarrier carrier 0 PULSE(0 1 0 19.9u 0.1u 0 20u)
Bgate gate 0 V=if(V(duty)>V(carrier),12,0)
Rgate gate gmos 4.7
Vdsense sw md 0
Bil il_sense 0 V=I(Lboost)
Bpin p_in 0 V=-V(vin)*I(Vinput)
Bpload p_load 0 V=V(out)*V(out)/48+V(out,loadsw)*V(out,loadsw)/96
Bpmos p_mos 0 V=V(md)*I(Vdsense)+V(gmos)*V(gate,gmos)/4.7
Bpgate p_driver 0 V=V(gate)*V(gate,gmos)/4.7
Bprgate p_rgate 0 V=V(gate,gmos)*V(gate,gmos)/4.7
Bpcoil p_coil 0 V=0.05*I(Lboost)*I(Lboost)
Bpd p_diode 0 V=V(sw,out)*(I(Lboost)-I(Vdsense))
Bpsload p_sload 0 V=V(loadsw)*V(out,loadsw)/96
