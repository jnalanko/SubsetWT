#include "NewSubsetWT.hh"
#include "RRR_generalization.hh"
#include "SDSL_WT.hh"   

int main(){

    // Create a random subset sequence
    int64_t n = 1e1;
    vector<vector<char> > sets;
    int64_t sigma = 4; // Alphabet size
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

        cout << i << ": "; for(char c : set) cout << (int)c << ","; cout << endl;
        
        sets.push_back(set);
    }

    // Check all queries

    //SubsetWT<RRR_Generalization<4>, RRR_Generalization<3>> sswt(sets);
    SubsetWT<SDSL_WT<sdsl::wt_hutu<>, 4>, SDSL_WT<sdsl::wt_hutu<>, 3>> sswt(sets);
    bool all_correct = true;
    for(char c = 0; c < sigma; c++){
        for(int64_t i = 0; i < n; i++){
            if(c == 0 && i == 7){
                cout << "Break" << endl;
            }
            int64_t r = sswt.rank(i, c);
            if(r != right_answers[c][i]){
                cerr << "Wrong answer for " << i << " " << (int)c << ": " << r << " vs " << right_answers[c][i] << endl;
                all_correct = false;
            }
        }
    }
    if(all_correct) cerr << "All answers were correct" << endl;
}