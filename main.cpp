#include "NewSubsetWT.hh"
#include "RRR_generalization.hh"

int main(){

    RRR_Generalization<4> test({2,1,0,1,1,1,2});
    cout << test.rankpair(5, 1) << endl;

    vector<vector<char> > sets = {{'A', 'C'}, {'G'}, {}, {'T', 'R'}, {'G', 'T'}, {'G'}, {'A'}};
    SubsetWT<RRR_Generalization<4>, RRR_Generalization<3>> sswt(sets);
    cout << sswt.rank(5, 'G') << endl;
}