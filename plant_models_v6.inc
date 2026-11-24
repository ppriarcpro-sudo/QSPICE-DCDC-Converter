* V6 illustrative 200 V class silicon VDMOS, NOT a manufacturer model
.model MOS200_DEMO VDMOS(Rd=12m Rs=8m Rg=1 Vto=3.5 Kp=35 Cgdmax=700p Cgdmin=50p Cgs=2n Cjo=1n Is=1n Rb=20m BV=200)
.model DFAST D(Is=1n N=1 Rs=0.02 Cjo=50p Tt=10n BV=200)
.model SWMOD SW(Ron=0.01 Roff=100Meg Vt=2.5 Vh=0.1)
