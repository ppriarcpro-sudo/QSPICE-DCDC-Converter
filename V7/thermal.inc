* Thermal analogy: V = deg C rise, I = W; signed terminal power
Rjc tj_rise tc_rise 0.8
Cj tj_rise 0 0.02
Rcs tc_rise th_rise 0.4
Cc tc_rise 0 0.2
Rha th_rise 0 4
Ch th_rise 0 8
Bheat 0 tj_rise I=V(p_mos)
Btj tj 0 V=V(tamb)+V(tj_rise)
Btc tc 0 V=V(tamb)+V(tc_rise)
Bth th 0 V=V(tamb)+V(th_rise)
* Existing enable pin carries coded thermal supervision: 0 stop, -1 trip, 1..2 active
Bfactor thermal_factor 0 V=min(1,max(0,(140-V(tj))/30))
Benable enable 0 V=if(V(tj)>=140,-1,if(V(enable_request)<0.5,0,1+V(thermal_factor)))
