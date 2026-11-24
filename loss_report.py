"""Time-weighted signed power averages; CSV time in seconds, power in W."""
import csv, sys, math
KEYS=('p_in','p_driver','p_load','p_mos','p_diode','p_coil','p_rgate','p_sload')
def report(rows,a,b):
    if not a<b or len(rows)<2 or a<rows[0][0] or b>rows[-1][0]: raise ValueError('Window outside data')
    sums=[0.]*len(KEYS)
    for (t,x),(u,y) in zip(rows,rows[1:]):
        if u<=t: raise ValueError('Time must strictly increase')
        l,r=max(t,a),min(u,b)
        if l>=r: continue
        for j in range(len(KEYS)):
            v=x[j]+(y[j]-x[j])*(l-t)/(u-t); w=x[j]+(y[j]-x[j])*(r-t)/(u-t)
            sums[j]+=(v+w)*(r-l)/2
    m=dict(zip(KEYS,(s/(b-a) for s in sums)))
    supply=m['p_in']+m['p_driver']; loss=sum(m[k] for k in KEYS[3:]); residual=supply-m['p_load']-loss
    return m,100*m['p_load']/supply if supply>0 else float('nan'),residual
if __name__=='__main__':
    if len(sys.argv)!=4: raise SystemExit('Usage: python loss_report.py data.csv start_s end_s')
    with open(sys.argv[1],newline='') as f:
        rows=[(float(r['time']),[float(r[k]) for k in KEYS]) for r in csv.DictReader(f)]
    if not all(math.isfinite(t) and all(map(math.isfinite,x)) for t,x in rows): raise ValueError('Nonfinite data')
    m,e,r=report(rows,float(sys.argv[2]),float(sys.argv[3]))
    for k,v in m.items(): print(f'{k}: {v:.6g} W')
    print(f'Efficiency: {e:.4g}%\nEnergy balance residual: {r:.6g} W (includes stored energy change)')
