#! /bin/bash
#rm clear.con
echo "#! /bin/bash">rmjunk.sh
dir=$PWD
echo "cd ${dir}" >> rmjunk.sh
echo "
rm -rf out/*
rm -rf list/*
rm -rf log/*
rm -rf csh/*
rm -rf LocalLibraries.*
rm -rf err/*
rm -rf *.dataset
rm -rf *.session.*
" >>rmjunk.sh
chmod u+x  rmjunk.sh
echo "
Universe       = vanilla
Notification   = never
Requirements   = (CPU_Type != \"crs\") && (CPU_Experiment == \"star\")
+Experiment     = \"star\"
Priority        = +10
+Job_Type       = \"cas\"
GetEnv          = true
" >rmjunk.con
echo "Initialdir = ${dir}">>rmjunk.con
echo "Executable = rmjunk.sh" >>rmjunk.con
echo "Queue" >>rmjunk.con

condor_submit rmjunk.con
