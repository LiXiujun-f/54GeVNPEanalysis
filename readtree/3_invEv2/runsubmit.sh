#! /bin/bash
rm -rf runsubmit.con
rm -rf listname.txt
rm -rf list/miniTree_*
./split.sh 70  miniTree.list
dir=$PWD
ls $PWD/list/miniTree_*  > listname.txt
echo "
Universe       = vanilla
Notification   = never
Requirements   = (CPU_Type != \"crs\") && (CPU_Experiment == \"star\")
+Experiment     = \"star\"
Priority        = +10
+Job_Type       = \"cas\"
GetEnv          = true
" >runsubmit.con
echo "Initialdir = ${dir}">>runsubmit.con
Njob=0
for list in `cat  listname.txt`; do
  echo "Executable = job.sh" >>runsubmit.con
  echo "Arguments = $list ${dir} production/job_${Njob}" >>runsubmit.con
  echo "Output         = out/job_${Njob}.out" >> runsubmit.con
  echo "Error          = err/job_${Njob}.err" >> runsubmit.con
  echo "Log            = log/job_${Njob}.log" >> runsubmit.con
  echo "Queue" >>runsubmit.con
  ((Njob++))
done
condor_submit runsubmit.con
