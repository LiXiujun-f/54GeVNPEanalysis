#! /bin/bash

#############################
#add all of the root files under the directory#
############################

START=`date +%s`
tmp="tmplist.txt"
# out="../embeddingQa.phoE.root"
# out="../embeddQa_gamma_pi0_0929.root"
out="../embeddQa_gamma_pi0_0930.root"
N=`ls *.root | wc -l`
rm -rf ${tmp}
ls *.root > ${tmp}
if [ -f "${out}" ]; then
    echo "${out} already exit"
    exit 0
fi
#  root -l -q  myhadd.C\(`${out}`, `${tmp}`, `${N}`\) 

list=${out}" "
for i in `cat ${tmp}`; do
  list=${list}" "${i}
  # echo ${list}
done
echo ${list}
hadd ${list}

END=`date +%s`;
time=$((END-START))
echo $time

