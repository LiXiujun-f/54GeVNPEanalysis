#include <fstream>
void readPre(){
  TFile* file = new TFile("prev2.root","recreate");
  double x1[14],y1[14],err1[14],errsys1[14];
  double y2[14],err2[14],errsys2[14];
  double y3[14],err3[14],errsys3[14];
  ifstream in;
  in.open("Previousv2.txt");
  for (int i=0;i<14;i++)
  {
    in>>x1[i]>>y1[i]>>err1[i]>>errsys1[i]>>y2[i]>>err2[i]>>errsys2[i]>>y3[i]>>err3[i]>>errsys3[i]; 
  }
  
  double x10[14],y10[14],err10[14],errsys10[14];
  double x11[14],y11[14],err11[14],errsys11[14];
  double x12[14],y12[14],err12[14],errsys12[14];
  for (int i=0;i<14;i++)
  {
    in>>x10[i]>>y10[i]>>err10[i]>>errsys10[i]>>y11[i]>>err11[i]>>errsys11[i]>>y12[i]>>err12[i]>>errsys12[i]; 
  }
  double x4[6],y4[6],err4[6],errsys4[6];
  for (int i=0;i<6;i++)
  {
    in>>x4[i]>>y4[i]>>err4[i]>>errsys4[i];
  }
  double x5[6],y5[6],err5[6],errsys5[6];
  for (int i=0;i<6;i++)
  {
    in>>x5[i]>>y5[i]>>err5[i]>>errsys5[i];
  }
  double x6[6],y6[6],err6[6],errsys6[6];
  for (int i=0;i<6;i++)
  {
    in>>x6[i]>>y6[i]>>err6[i]>>errsys6[i];
  }
  double x7[6],y7[6],err7[6],errsys7[6];
  for (int i=0;i<6;i++)
  {
    in>>x7[i]>>y7[i]>>err7[i]>>errsys7[i];
  }
  double x8[6],y8[6],err8[6],errsys8[6];
  for (int i=0;i<6;i++)
  {
    in>>x8[i]>>y8[i]>>err8[i]>>errsys8[i];
  }
  in.close();
  double x9[12],y9[12];
  in.open("xyscan.txt");
  for (int i=0;i<12;i++){
    in>>x9[i]>>y9[i];
    cout<<x9[i]<<endl;
    y9[i]/=100.;
  }
  in.close();
  TGraphErrors* g1=new TGraphErrors(14,x1,y1,0,err1);
  g1->SetName("10_20");
  g1->Write();
  TGraphErrors* g1sys=new TGraphErrors(14,x1,y1,0,errsys1);
  g1sys->SetName("10_20_sys");
  g1sys->Write();
  TGraphErrors* g2=new TGraphErrors(14,x1,y2,0,err2);
  g2->SetName("30_40");
  g2->Write();
  TGraphErrors* g2sys=new TGraphErrors(14,x1,y2,0,errsys2);
  g2sys->SetName("30_40_sys");
  g2sys->Write();
  TGraphErrors* g3=new TGraphErrors(14,x1,y3,0,err3);
  g3->SetName("50_60");
  g3->Write();
  TGraphErrors* g3sys=new TGraphErrors(14,x1,y3,0,errsys3);
  g3sys->SetName("50_60_sys");
  g3sys->Write();
   TGraphErrors* g4sys=new TGraphErrors(6,x4,y4,0,err4);
  g4sys->SetName("0_10_sys_62");

  g4sys->Write();
  TGraphErrors* g5sys=new TGraphErrors(6,x5,y5,0,err5);
  g5sys->SetName("10_20_sys_62");
  g5sys->Write();
  TGraphErrors* g6sys=new TGraphErrors(6,x6,y6,0,err6);
  g6sys->SetName("20_30_sys_62");
  g6sys->Write();
  TGraphErrors* g7sys=new TGraphErrors(6,x7,y7,0,err7);
  g7sys->SetName("30_40_sys_62");
  g7sys->Write();
  TGraphErrors* g8sys=new TGraphErrors(6,x8,y8,0,err8);
  g8sys->SetName("40_50_sys_62");
  g8sys->Write();
  TGraph* g9 = new TGraph(12,x9,y9);
  g9->SetName("30_40_star62");
  g9->Write();
  TGraph* g10 = new TGraph(14,x10,y10);
  g10->SetName("10_20_star27");
  g10->Write();
  TGraph* g11 = new TGraph(14,x10,y11);
  g11->SetName("30_40_star27");
  g11->Write();
  TGraph* g12 = new TGraph(14,x10,y12);
  g12->SetName("50_60_star27");
  g12->Write();
//pion plus
Double_t pt_bin_center13[10] = {0.285,0.495,0.675,0.885,1.065,1.275,1.545,1.935,2.355,2.835};
Double_t v2_values13[10] = {0.0117471,0.0228517,0.0329,0.042002,0.0487612,0.0553591,0.0595984,0.0637105,0.0678853,0.066606};
Double_t v2_stat_error13[10] = {0.00012301,0.000147786,0.00020224,0.000287824,0.00041135,0.000586629,0.000681158,0.00139804,0.00267615,0.00466203};
TGraphErrors* g13 = new TGraphErrors(10,pt_bin_center13,v2_values13,0,v2_stat_error13);
g13->SetName("pionplus_0_10_27");
g13->Write();
Double_t pt_bin_center14[13] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.665,1.875,2.085,2.355,2.805,3.465};
Double_t v2_values14[13] = {0.0118138,0.0229824,0.0337497,0.0432688,0.0511433,0.0577776,0.0629835,0.0666356,0.0688837,0.0707009,0.0758873,0.0793376,0.0710317};
Double_t v2_stat_error14[13] = {7.97649e-05,9.40754e-05,0.000127369,0.000178608,0.000252476,0.000355599,0.00050783,0.000718169,0.000987558,0.00132172,0.00141911,0.0022635,0.00528366};
TGraphErrors* g14 = new TGraphErrors(13,pt_bin_center14,v2_values14,0,v2_stat_error14);
g14->SetName("pionplus_0_10_39");
g14->Write();
Double_t pt_bin_center15[13] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.665,1.875,2.085,2.355,2.835,3.495};
Double_t v2_values15[13] = {0.0105363,0.0219187,0.0341168,0.0437394,0.0526304,0.0597022,0.0662818,0.0699636,0.0716711,0.0734173,0.0782672,0.0791601,0.0842406};
Double_t v2_stat_error15[13] = {0.000105537,0.000122407,0.000164809,0.000228831,0.000319575,0.000446836,0.000629297,0.000882756,0.00122416,0.00165379,0.00173078,0.00262822,0.00572041};
TGraphErrors* g15 = new TGraphErrors(13,pt_bin_center15,v2_values15,0,v2_stat_error15);
g15->SetName("pionplus_0_10_62");
g15->Write();
//10-40
Double_t pt_bin_center16[15] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.695,1.875,2.085,2.295,2.475,2.685,2.955,3.495};
Double_t v2_values16[15] = {0.0246568,0.0488236,0.0724498,0.0933431,0.111199,0.125826,0.137535,0.145903,0.153263,0.159387,0.161891,0.1649,0.164148,0.164993,0.163504};
Double_t v2_stat_error16[15] = {5.5493e-05,6.57157e-05,8.89935e-05,0.000124016,0.000173554,0.000241259,0.000338897,0.000471602,0.000648294,0.000870067,0.00114163,0.00149154,0.00194562,0.00201045,0.00292764};

TGraphErrors* g16 = new TGraphErrors(15,pt_bin_center16,v2_values16,0,v2_stat_error16);
g16->SetName("pionplus_10_40_62");
g16->Write();
Double_t pt_bin_center17[14] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.665,1.875,2.085,2.295,2.475,2.835,3.465};
Double_t v2_values17[14] = {0.0236042,0.0474268,0.0701121,0.0900108,0.106989,0.120903,0.132197,0.140385,0.146647,0.152065,0.151277,0.157466,0.158328,0.155246};
Double_t v2_stat_error17[14] = {4.24396e-05,5.10583e-05,6.97025e-05,9.83866e-05,0.000139269,0.000195935,0.000277684,0.000392379,0.000541797,0.00072417,0.000959308,0.00127277,0.00122297,0.00272259};
TGraphErrors* g17 = new TGraphErrors(14,pt_bin_center17,v2_values17,0,v2_stat_error17);
g17->SetName("pionplus_10_40_39");
g17->Write();

Double_t pt_bin_center18[13] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.665,1.875,2.085,2.355,2.805,3.465};
Double_t v2_values18[13] = {0.0235111,0.046407,0.0682154,0.0871932,0.103428,0.116498,0.12745,0.134928,0.141224,0.146375,0.151377,0.154863,0.158313};
Double_t v2_stat_error18[13] = {6.51565e-05,7.9295e-05,0.00010925,0.000156206,0.000223992,0.000319636,0.000452404,0.000645997,0.000917705,0.0012787,0.00142484,0.0024869,0.00608305};

TGraphErrors* g18 = new TGraphErrors(13,pt_bin_center18,v2_values18,0,v2_stat_error18);
g18->SetName("pionplus_10_40_27");
g18->Write();
//0-80%
Double_t pt_bin_center19[15] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.665,1.875,2.085,2.295,2.475,2.685,2.955,3.345};
Double_t v2_values19[15] = {0.0208012,0.041395,0.0611387,0.0783936,0.0932306,0.105414,0.115887,0.123129,0.129545,0.134079,0.136608,0.141293,0.145453,0.144127,0.147837};
Double_t v2_stat_error19[15] = {4.06899e-05,4.87531e-05,6.64991e-05,9.38308e-05,0.000132824,0.000187112,0.000265895,0.000375892,0.000515683,0.000692813,0.000918648,0.00122249,0.00162044,0.00172633,0.00303448};
TGraphErrors* g19 = new TGraphErrors(15,pt_bin_center19,v2_values19,0,v2_stat_error19);
g19->SetName("pionplus_0_80_39");
g19->Write();
Double_t pt_bin_center20[15] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.665,1.875,2.085,2.295,2.475,2.685,2.955,3.345};
Double_t v2_values20[15] = {0.0212466,0.04235,0.0632251,0.0813738,0.0970523,0.110052,0.120986,0.128628,0.134524,0.140802,0.144568,0.147086,0.152306,0.151087,0.152001};
Double_t v2_stat_error20[15] = {5.35708e-05,6.30403e-05,8.53286e-05,0.000119076,0.000166829,0.000232304,0.000327271,0.00045751,0.000629584,0.000844917,0.0011038,0.00145558,0.00188488,0.00195929,0.00327581};
TGraphErrors* g20 = new TGraphErrors(15,pt_bin_center20,v2_values20,0,v2_stat_error20);
g20->SetName("pionplus_0_80_62");
g20->Write();
Double_t pt_bin_center21[14] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.665,1.875,2.085,2.295,2.475,2.805,3.465};
Double_t v2_values21[14] = {0.0204075,0.0404926,0.0592754,0.0759822,0.0897911,0.100898,0.110136,0.117802,0.121748,0.127693,0.133237,0.13354,0.138953,0.139932};
Double_t v2_stat_error21[14] = {6.33703e-05,7.70403e-05,0.000105998,0.000151183,0.000217215,0.000309484,0.000439386,0.000629185,0.000895574,0.00124855,0.00171713,0.00236994,0.00242954,0.00593679};
TGraphErrors* g21 = new TGraphErrors(14,pt_bin_center21,v2_values21,0,v2_stat_error21);
g21->SetName("pionplus_0_80_27");
g21->Write();

Double_t pt_bin_center22[12] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.665,1.875,2.085,2.355,2.835};
Double_t v2_values22[12] = {0.028888,0.0589122,0.0884838,0.114564,0.138449,0.156339,0.173457,0.181754,0.19405,0.19931,0.210363,0.227665};
Double_t v2_stat_error22[12] = {0.000143401,0.000178332,0.000249537,0.000358916,0.000517534,0.000736543,0.00105173,0.00148376,0.00201896,0.00269214,0.00284612,0.0045173};
TGraphErrors* g22 = new TGraphErrors(12,pt_bin_center22,v2_values22,0,v2_stat_error22);
g22->SetName("pionplus_40_80_39");
g22->Write();

Double_t pt_bin_center23[13] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.695,1.875,2.085,2.295,2.475,2.835};
Double_t v2_values23[13] = {0.0302612,0.0618884,0.0931795,0.120907,0.14497,0.16514,0.181052,0.191616,0.197537,0.212764,0.210707,0.217859,0.23746};
Double_t v2_stat_error23[13] = {0.0001836,0.000225071,0.000312447,0.000444503,0.000630998,0.000892833,0.00125877,0.00155694,0.00237389,0.00312628,0.00405695,0.00530303,0.00493076};
TGraphErrors* g23 = new TGraphErrors(13,pt_bin_center23,v2_values23,0,v2_stat_error23);
g23->SetName("pionplus_40_80_62");
g23->Write();

Double_t pt_bin_center24[11] = {0.285,0.495,0.675,0.885,1.065,1.275,1.485,1.665,1.965,2.355,2.835};
Double_t v2_values24[11] = {0.0277103,0.0571959,0.0852322,0.111648,0.13395,0.148716,0.166455,0.181484,0.187652,0.210592,0.236303};
Double_t v2_stat_error24[11] = {0.000223711,0.000282544,0.00039842,0.000580809,0.000849794,0.00122565,0.00175536,0.00251897,0.00293369,0.00553557,0.00955107};
TGraphErrors* g24 = new TGraphErrors(11,pt_bin_center24,v2_values24,0,v2_stat_error24);
g24->SetName("pionplus_40_80_27");
g24->Write();


}
