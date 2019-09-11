#! /bin/bash
if [ $# -eq 0 ]; then
    echo
   	echo "Please input 2 arguments!"
  	echo "--------- first: number of jobs"
    echo "--------- second: submit(1) or not(0)"
    echo
    exit
elif [ $# -eq 1 ]; then
    echo
    echo "Please input one more arguments!"
   	echo "--------- first: number of jobs"
    echo "--------- second: submit(1) or not(0)"
    echo
	exit
fi

runshellname="run.sh"
pythiashellname="runpythia.sh"
N="1e6"
currenttime=0
currenttime=`date +%s`
((currenttime=currenttime%100000))
echo $currenttime
touch run.con
dir=$PWD
echo "
Universe       = vanilla
Notification   = never
Requirements   = (CPU_Type != \"crs\") && (CPU_Experiment == \"star\")
+Experiment     = \"star\"
Priority        = +10
+Job_Type       = \"cas\"
GetEnv          = true
Initialdir      = $dir 
" > run.con

echo "write run.con"
./clear.sh

if [ ! -e csh ]; then
  mkdir csh
fi
if [ ! -e out ]; then
  mkdir out
fi
if [ ! -e log ]; then
  mkdir log
fi

chmod u+x $runshellname
for jobId in `seq 1 $1`; do
  cd  csh
  mkdir $jobId
  cd ../
  cp -r simv2.C myFunction.h data/ $runshellname  csh/$jobId
  rdnb=$currenttime$jobId
  echo "Executable = csh/$jobId/$runshellname" >> run.con
  echo "Arguments = $PWD $jobId $N $rdnb" >> run.con
  echo "Output = log/$jobId.out" >> run.con 
  echo "Error = log/$jobId.err">>run.con
  echo "Log = log/$jobId.log">>run.con
  echo "Queue " >> run.con
  echo " " >> run.con
  ((jobId++))
done

if [ $2 -eq 1 ]; then
  condor_submit run.con
fi
