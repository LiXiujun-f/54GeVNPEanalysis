#!/bin/bash
# cons
Address=$PWD
echo $Address
mkdir log err production csh report out list
# star-submit-template -template submitPicoDstarMixedMakerAll.xml -entities listOfFiles=test.list
# star-submit-template -template submitPicoDstarMixedMakerAll.xml -entities listOfFiles=test.list,dir=$Address
# [ -f  submit_${1}.xml ]|| rm -f submit_${1}.xml
# sed -e "s/production/production_$1/g" submit.xml > submit_${1}.xml
# star-submit-template -template submit_$1.xml -entities listOfFiles=pico_rmbad_$1.list,dir=$Address
# star-submit-template -template submit_${1}.xml -entities listOfFiles=pico_nfs_${1}.list,dir=$Address
# star-submit-template -template submit_${1}.xml -entities listOfFiles=pico_local_xr_${1}.list,dir=$Address
star-submit-template -template submit.xml -entities listOfFiles=miniTree.list,dir=$Address
