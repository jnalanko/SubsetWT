#include "NewSubsetWT.hh"
#include "RRR_generalization.hh"
#include "SplitStructure.hh"
#include "BitMagic.hh"
#include "SDSL_WT.hh"   

template<typename subsetwt_t>
bool run_test(int64_t n, int64_t sigma){

    cerr << "Running test for n = " << n << ", sigma = " << sigma << endl;

    // Create a random subset sequence
    vector<vector<char> > sets;
    vector<vector<int64_t>> right_answers(sigma, vector<int64_t>(n+1));
    srand(123);
    for(int64_t i = 0; i < n; i++){
        vector<char> set;
        for(int64_t c = 0; c < sigma; c++){
            if(rand() % sigma == 0){
                set.push_back(c);
                right_answers[c][i+1]++;
            }
            right_answers[c][i+1] += right_answers[c][i];
        }
        
        sets.push_back(set);
    }

    // Check all queries

    subsetwt_t sswt(sets);
    bool all_correct = true;
    for(char c = 0; c < sigma; c++){
        for(int64_t i = 0; i < n; i++){
            int64_t r = sswt.rank(i, c);
            if(r != right_answers[c][i]){
                cerr << "Wrong answer for " << i << " " << (int)c << ": " << r << " vs " << right_answers[c][i] << endl;
                exit(1);
                all_correct = false;
            }
        }
    }
    return all_correct;
}

int main(){

    typedef SubsetWT<SDSL_WT<sdsl::wt_hutu<>, 4>, SDSL_WT<sdsl::wt_hutu<>, 3>> nested_wt_t;
    typedef SubsetWT<RRR_Generalization<4>, RRR_Generalization<3>> rrr_generalization_t;
    typedef SubsetWT<SplitStructure<4>, SplitStructure<3>> split_t;
    typedef SubsetWT<BitMagic<4>, BitMagic<3>> bitmagic_t;

    int64_t n = 1e4;
    bool all_correct = true;

    vector<int64_t> sigmas;
    for(int64_t i = 1; i <= 20; i++) sigmas.push_back(i);
    sigmas.push_back(127);

    cerr << "Testing with nested WT" << endl;
    for(int64_t sigma : sigmas){
        all_correct &= run_test<nested_wt_t>(n + rand()%n, sigma);
    }
    cerr << "Testing with RRR Generalization" << endl;
    for(int64_t sigma : sigmas){
        all_correct &= run_test<rrr_generalization_t>(n + rand()%n, sigma);
    }
    //cerr << "Testing with SplitStructure" << endl;
    //for(int64_t sigma : sigmas){
    //    all_correct &= run_test<split_t>(n + rand()%n, sigma);
    //}
    cerr << "Testing with BitMagic" << endl;
    for(int64_t sigma : sigmas){
        all_correct &= run_test<bitmagic_t>(n + rand()%n, sigma);
    }

    if(all_correct) cerr << "All answers were correct" << endl;

}