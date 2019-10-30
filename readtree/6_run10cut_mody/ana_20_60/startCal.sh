dir=$PWD
cd rescaleEmbed
./plotPhoeff.py

cd $dir
root -b -q HFev2.C
root plotNPEv2.C
