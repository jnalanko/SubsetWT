#include "NewSubsetWT.hh"
#include "RRR_generalization.hh"

int main(){
    vector<vector<char> > sets = {{'A', 'C'}, {'G'}, {'T', 'R'}, {'G', 'T'}, {'G'}, {'A'}};
    SubsetWT<RRR_Generalization, RRR_Generalization> sswt(sets);
    cout << sswt.rank(4, 'G') << endl;
}