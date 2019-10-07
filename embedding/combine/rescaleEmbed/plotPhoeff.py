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
# run_calPhoeff("rescale_combine1002.root","pdf\/PhoEff_"+name[0]+".pdf","output\/PhoEff_"+name[0]+".root")
# run_calPhoeff("data\/embeddQa_pi0_0930.root","pdf\/PhoEff_"+name[1]+".pdf","output\/PhoEff_"+name[1]+".root")
# run_calPhoeff("data\/embeddQa_eta_0930.root","pdf\/PhoEff_"+name[2]+".pdf","output\/PhoEff_"+name[2]+".root")
# run_calPhoeff("data\/embeddQa_gamma_pi0_0930.root","pdf\/PhoEff_"+name[3]+".pdf","output\/PhoEff_"+name[3]+".root")
# run_calPhoeff("data\/embeddQa_gamma_eta_0930.root","pdf\/PhoEff_"+name[4]+".pdf","output\/PhoEff_"+name[4]+".root")
# run_calPhoeff("data\/embeddQa_gamma_dirpho_1002.root","pdf\/PhoEff_"+name[5]+".pdf","output\/PhoEff_"+name[5]+".root")

command="root -b -q plotComponent.C\(\\\"output\/PhoEff_\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\",\\\"%s\\\"\)"%(name[0],name[1],name[2],name[3],name[4],name[5])
subprocess.call(command,shell=True)
