* V8 two phase plant; separate heat sinks; nominal same load as V7
.include plant_models_v6.inc
Vcarrier1 carrier1 0 PULSE(0 1 0 19.9u 0.1u 0 20u)
* Phase 2 begins 10 us later, held at 1 before first cycle by PWL offset alternative
Vcarrier2 carrier2 0 PULSE(1 0 9.9u 0.1u 19.9u 0 20u)
L1 vin nl1 470u
Rcoil1 nl1 sw1 50m
D1 sw1 out DFAST
Vds1 sw1 mp1 0
Brt1 mp1 md1 R=0.0001+0.00008*max(0,V(tj1)-25)
M1 md1 gm1 0 MOS200_DEMO
Bg1 gate1 0 V=if(V(duty1)>V(carrier1),12,0)
Rg1 gate1 gm1 4.7
Bi1 il1 0 V=I(L1)
Bpm1 pmos1 0 V=V(mp1)*I(Vds1)+V(gm1)*V(gate1,gm1)/4.7
Bpd1 pd1 0 V=V(sw1,out)*(I(L1)-I(Vds1))
Bpc1 pc1 0 V=0.05*I(L1)*I(L1)
Bpg1 pg1 0 V=V(gate1,gm1)*V(gate1,gm1)/4.7
Bdrv1 pdrv1 0 V=V(gate1)*V(gate1,gm1)/4.7
Rjc1 jr1 cr1 0.8
Cj1 jr1 0 0.02
Rcs1 cr1 hr1 0.4
Cc1 cr1 0 0.2
Rha1 hr1 0 4
Ch1 hr1 0 8
Bheat1 0 jr1 I=V(pmos1)
Btj1 tj1 0 V=V(tamb1)+V(jr1)
Btc1 tc1 0 V=V(tamb1)+V(cr1)
Bth1 th1 0 V=V(tamb1)+V(hr1)
L2 vin nl2 470u
Rcoil2 nl2 sw2 50m
D2 sw2 out DFAST
Vds2 sw2 mp2 0
Brt2 mp2 md2 R=0.0001+0.00008*max(0,V(tj2)-25)
M2 md2 gm2 0 MOS200_DEMO
Bg2 gate2 0 V=if(V(duty2)>V(carrier2),12,0)
Rg2 gate2 gm2 4.7
Bi2 il2 0 V=I(L2)
Bpm2 pmos2 0 V=V(mp2)*I(Vds2)+V(gm2)*V(gate2,gm2)/4.7
Bpd2 pd2 0 V=V(sw2,out)*(I(L2)-I(Vds2))
Bpc2 pc2 0 V=0.05*I(L2)*I(L2)
Bpg2 pg2 0 V=V(gate2,gm2)*V(gate2,gm2)/4.7
Bdrv2 pdrv2 0 V=V(gate2)*V(gate2,gm2)/4.7
Rjc2 jr2 cr2 0.8
Cj2 jr2 0 0.02
Rcs2 cr2 hr2 0.4
Cc2 cr2 0 0.2
Rha2 hr2 0 4
Ch2 hr2 0 8
Bheat2 0 jr2 I=V(pmos2)
Btj2 tj2 0 V=V(tamb2)+V(jr2)
Btc2 tc2 0 V=V(tamb2)+V(cr2)
Bth2 th2 0 V=V(tamb2)+V(hr2)

Cout out 0 470u
Rload out 0 48
Rstep out loadsw 96
Sload loadsw 0 step 0 SWMOD
Bitotal itotal 0 V=V(il1)+V(il2)
Bimb imbalance 0 V=V(il1)-V(il2)
Bpin p_in 0 V=-V(vin)*I(Vinput)
Bpload p_load 0 V=V(out)*V(out)/48+V(out,loadsw)*V(out,loadsw)/96
Bpsload p_sload 0 V=V(loadsw)*V(out,loadsw)/96
