#! /bin/bash
dir=$PWD
chmod u+x  addsperate.sh
echo "
Universe       = vanilla
Notification   = never
Requirements   = (CPU_Type != \"crs\") && (CPU_Experiment == \"star\")
+Experiment     = \"star\"
Priority        = +10
+Job_Type       = \"cas\"
GetEnv          = true
" > addtree.con
echo "Initialdir = ${dir}">>addtree.con
echo "Executable = addsperate.sh" >>addtree.con
echo "Arguments = $1 $2" >> addtree.con
echo "Queue" >> addtree.con

condor_submit addtree.con
