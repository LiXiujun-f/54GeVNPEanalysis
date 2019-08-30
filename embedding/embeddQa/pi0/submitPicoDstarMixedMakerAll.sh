#!/bin/bash
# cons
mkdir log err production production_MC csh report out list
# star-submit-template -template submitPicoDstarMixedMakerAll.xml -entities listOfFiles=test.list
dir=$PWD
cd $dir
echo $dir
star-submit-template -template submitPicoDstarMixedMakerAll.xml -entities listOfFiles=muDst.list,dir=$dir
star-submit-template -template submitPicoDstarMixedMakerAll.MC.xml -entities listOfFiles=miniMc.list,dir=$dir
