#! /bin/bash
#this shell is to draw the efficiency for single embedding result
root -b -q calPhoeff.C\(\"data/embeddQa_pi0_0918.root\",\"pdf/PhoErecoEff_pi0.pdf\",\"output/RecoEff_pi0.root\"\)
root -b -q calPhoeff.C\(\"data/embeddQa_eta_0918.root\",\"pdf/PhoErecoEff_eta.pdf\",\"output/RecoEff_eta.root\"\)
root -b -q calPhoeff.C\(\"data/embeddQa_gamma_0918.root\",\"pdf/PhoErecoEff_gamma.pdf\",\"output/RecoEff_gamma.root\"\)
