import ROOT as rt
from array import array

infilename = 'higgsCombinelikelihood_r1exp2p_r1_mavariable_wodR_all_ST_rebin_Run2_ma125_allsyst_ExpXExp2p_MT700GeV.MultiDimFit.mH120.root'
infile = rt.TFile(infilename, 'READ')
tree = infile.Get('limit')

rs    = []
dnlls = []

for e in tree:
    if e.deltaNLL > 1e9: continue
    rs.append(e.r)
    dnlls.append(e.deltaNLL)
    

rs = rs[1:]
dnlls = dnlls[1:]
graph = rt.TGraph(len(rs), array('d', rs), array('d', dnlls))

c = rt.TCanvas()
graph.Draw('ALP')
c.SaveAs('LikelihoodScan.pdf')
c.SaveAs('LikelihoodScan.png')
