#! /bin/bash

#############################
#add all of the root files under the directory#
############################
START=`date +%s`
# out="../production/getshift_$1.root"
# if [ -f "${out}" ]; then
#     echo "${out} already exit"
#     exit 0
# fi
#
rm -rf tmp
mkdir tmp
tmp="tmplist"
cd tmp
ls ../*.root > ${tmp}
# cat ${tmp}
# exit

#  root -l -q  myhadd.C\(`${out}`, `${tmp}`, `${N}`\) 

addlist () {
  #2.output name 1.filelist 
  list=${2}" " 
  file=${1}
  for i in `cat ${file}`; do
    list=${list}" "${i}
    # echo ${list}
  done
  # echo ${list}
  hadd ${list}
}

split -l $2 ${tmp} ${tmp}_
ls ${tmp}_* > splitlist.txt
((N=0))
for filelist in `cat splitlist.txt`; do
  addlist ${filelist} production_$1.$N.tree.root
  rm -rf ${filelist}
  ((N++))
done
#rm -rf splitlist.txt
# rm -rf ${tmp}
# ls ${tmp}_*.root > ${tmp}
# addlist  ${tmp} ../${out} 

END=`date +%s`;
time=$((END-START))
echo $time


# N=`cat ${tmp} | wc -l`
# ((haddloop=0))

# while ((N>100)) 
# do
#   split -l 100 ${tmp}  ${tmp}${haddloop}_
#   ls ${tmp}${haddloop}_* > splitlist.txt
#   for filelist in `cat splitlist.txt`; do
#     addlist ${filelist} ${filelist}.root
#     rm -rf ${filelist}
#   done
#   rm -rf splitlist.txt
#   rm -rf ${tmp}
#   ls ${tmp}${haddloop}_*.root >${tmp}
#   N=`cat ${tmp} | wc -l`
#   ((haddloop+=1)) 
# done
# addlist  ${tmp} ../${out} 

