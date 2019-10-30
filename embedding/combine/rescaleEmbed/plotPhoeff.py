#! /usr/bin/Python
import ROOT
import os
import subprocess
def run_calPhoeff(rootfile,pdfname,outname):
    command="root -b -q calPhoeff_combine.C\(\\\"%s\\\",\\\"%s\\\",\\\"%s\\\"\)"%(rootfile,pdfname,outname)
    # print command
    subprocess.call(command,shell=True)
    return 1

name = ["comb","pi0","eta","gamma2pi0","gamma2eta","dirpho"]
# run_calPhoeff("rescale_combine_0.7.root","pdf\/PhoEff_"+name[0]+"_0.7.pdf","output\/PhoEff_"+name[0]+"_0.7.root")
# run_calPhoeff("rescale_combine_1.3.root","pdf\/PhoEff_"+name[0]+"_1.3.pdf","output\/PhoEff_"+name[0]+"_1.3.root")
run_calPhoeff("rescale_combine.root","pdf\/PhoEff_"+name[0]+".pdf","output\/PhoEff_"+name[0]+".root")

# run_calPhoeff("rescale_combine_0.85.root","pdf\/PhoEff_"+name[0]+"_0.85.pdf","output\/PhoEff_"+name[0]+"_0.85.root")
# run_calPhoeff("rescale_combine_1.15.root","pdf\/PhoEff_"+name[0]+"_1.15.pdf","output\/PhoEff_"+name[0]+"_1.15.root")
# run_calPhoeff("rescale_combine.root","pdf\/PhoEff_"+name[0]+".pdf","output\/PhoEff_"+name[0]+".root")
# run_calPhoeff("data\/embeddQa_pi0_1016.root","pdf\/PhoEff_"+name[1]+".pdf","output\/PhoEff_"+name[1]+".root")
# run_calPhoeff("data\/embeddQa_eta_1016.root","pdf\/PhoEff_"+name[2]+".pdf","output\/PhoEff_"+name[2]+".root")
#
# # run_calPhoeff("data\/embeddQa_pi0_1008.root","pdf\/PhoEff_"+name[1]+".pdf","output\/PhoEff_"+name[1]+".root")
# # run_calPhoeff("data\/embeddQa_eta_1008.root","pdf\/PhoEff_"+name[2]+".pdf","output\/PhoEff_"+name[2]+".root")
# # # run_calPhoeff("data\/embeddQa_gamma_pi0_1008.root","pdf\/PhoEff_"+name[3]+".pdf","output\/PhoEff_"+name[3]+".root")
# # # run_calPhoeff("data\/embeddQa_gamma_eta_1008.root","pdf\/PhoEff_"+name[4]+".pdf","output\/PhoEff_"+name[4]+".root")
# # # run_calPhoeff("data\/embeddQa_gamma_dirpho_1008.root","pdf\/PhoEff_"+name[5]+".pdf","output\/PhoEff_"+name[5]+".root")
# # run_calPhoeff("data\/embeddQa_gamma_pi0_1014.root","pdf\/PhoEff_"+name[3]+".pdf","output\/PhoEff_"+name[3]+".root")
# # run_calPhoeff("data\/embeddQa_gamma_eta_1014.root","pdf\/PhoEff_"+name[4]+".pdf","output\/PhoEff_"+name[4]+".root")
# # run_calPhoeff("data\/embeddQa_gamma_dirpho_1014.root","pdf\/PhoEff_"+name[5]+".pdf","output\/PhoEff_"+name[5]+".root")

# run_calPhoeff("data\/embeddQa_gamma_pi0_1016.root","pdf\/PhoEff_"+name[3]+".pdf","output\/PhoEff_"+name[3]+".root")
# run_calPhoeff("data\/embeddQa_gamma_eta_1016.root","pdf\/PhoEff_"+name[4]+".pdf","output\/PhoEff_"+name[4]+".root")
# run_calPhoeff("data\/embeddQa_gamma_dirpho_1016.root","pdf\/PhoEff_"+name[5]+".pdf","output\/PhoEff_"+name[5]+".root")
# run_calPhoeff("rescaleFile\/rescale_gamma.root","pdf\/PhoEff_"+"gamma"+".pdf","output\/PhoEff_"+"gamma"+".root")
# run_calPhoeff("data\/embeddQa_gamma_1016.root","pdf\/PhoEff_"+"gamma"+".pdf","output\/PhoEff_"+"gamma"+".root")

# command="root -b -q plotComponent.C\(\\\"output\/PhoEff_\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",0\)"%(name[0],name[1],name[2],name[3],name[4],name[5])
command="root -b -q plotComponent.C\(\\\"output\/PhoEff_\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",1\)"%(name[0],name[1],name[2],"gamma",name[4],name[5])
# command="root -b -q plotComponent.C\(\\\"output\/PhoEff_\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",1\)"%(name[0]+"_0.85",name[0],name[0]+"_1.15",name[0],name[1],name[2])
subprocess.call(command,shell=True)
