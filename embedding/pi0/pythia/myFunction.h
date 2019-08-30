
TProfile2D* pGammav2;
TF1* pi0v2[9];
TF1* pi0spectra[9];

void setDecayChannels(int const defirst,int const desecond,int const mdme);
void getKinematics(TLorentzVector& b, double const mass);
void decayAndFill(int const kf, TLorentzVector* mother, double const weight, TClonesArray& daughters,int cent);

void fill(TLorentzVector* mother,TLorentzVector* gamma, double weight, int cent);
void bookHists();
void getCentAndEP(int & cent,float & EP);
