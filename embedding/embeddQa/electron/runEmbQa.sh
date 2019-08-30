#! /bin/bash
# root4star -l -b -q doEmbeddingQAMaker.C'(2014,"P16id","test.list","embedding.test.root")'
root4star -l -b -q doEmbeddingQAMaker.C'(2015,"P16id","miniMc.list","embeddingQa.mc.root",1,30,0,1000,15)'
# root4star -l -b -q doEmbeddingQAMaker.C'(2015,"P16id","muDst.list","embeddingQa.rd.root",0,30,0,1000,15)'
