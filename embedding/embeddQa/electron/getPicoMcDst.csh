#!/bin/csh
# rm miniMc.list
# ls /star/u/starembd/embedding/production_pAu200_2015/Piplus_200_20180601/*/*/*/*/*.minimc.root > miniMc.list
ls /star/u/starembd/embedding/production_pAu200_2015/Piplus_100_20180601/*/*/*/*/*.minimc.root > miniMc.list
# /star/embed/embedding/AuAu200_production2_2016/PiplusSST_201_20183302/ 
# ls /star/embed/reco/2015/pau200_phys/*.root > muDst.list
# get_file_list.pl -keys path,filename -cond production=P16ij,trgsetupname=AuAu_200_production2_2016,filetype=daq_reco_muDst,filename~st_physics,storage=local -limit 0 -delim / -distinct
