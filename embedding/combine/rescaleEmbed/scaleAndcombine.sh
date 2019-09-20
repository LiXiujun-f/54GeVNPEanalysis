#! /bin/bash
outDir="rescaleFile"
[ -d $outDir ] || mkdir $outDir

echo "scale pi0"
pi0DalitzBranch="0.01174"
pi0Embeddfile="data/embeddQa_pi0_0918.root"
pi0Rescalefile="$outDir/rescale_pi0.root"
root -b -q generateRecaleHists.C\(\"$pi0Embeddfile\",\"$pi0Rescalefile\",$pi0DalitzBranch\)

echo "scale eta"
etaDalitzBranch="0.48*0.0069"
etaEmbeddfile="data/embeddQa_eta_0918.root"
etaRescalefile="$outDir/rescale_eta.root"
root -b -q generateRecaleHists.C\(\"$etaEmbeddfile\",\"$etaRescalefile\",$etaDalitzBranch\)

echo "scale gamma"
gammaDalitzBranch="1"
gammaEmbeddfile="data/embeddQa_gamma_0918.root"
gammaRescalefile="$outDir/rescale_gamma.root"
root -b -q generateRecaleHists.C\(\"$gammaEmbeddfile\",\"$gammaRescalefile\",$gammaDalitzBranch\)

hadd rescale_combine.root $pi0Rescalefile $etaRescalefile $gammaRescalefile 
