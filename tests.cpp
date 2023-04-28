#include "SubsetWT.hpp"
#include "RRR_generalization.hpp"
#include "SplitStructure.hpp"
#include "BitMagic.hpp"
#include "SDSL_WT.hpp"


typedef SubsetWT<SDSL_WT<sdsl::wt_hutu<>, 4>, SDSL_WT<sdsl::wt_hutu<>, 3>> nested_wt_t;
typedef SubsetWT<RRR_Generalization<4>, RRR_Generalization<3>> rrr_generalization_t;
typedef SubsetWT<SplitStructure<4>, SplitStructure<3>> split_t;
typedef SubsetWT<BitMagic<4>, BitMagic<3>> bitmagic_t;

vector<vector<char> > get_random_subset_sequence(int64_t n, int64_t sigma, int64_t random_seed){
    // Create a random subset sequence
    vector<vector<char> > sets;
    srand(random_seed);
    for(int64_t i = 0; i < n; i++){
        vector<char> set;
        for(int64_t c = 0; c < sigma; c++){
            if(rand() % sigma == 0){
                set.push_back(c);
            }
        }
        sets.push_back(set);
    }
    return sets;
}

// Returns right answers to all queries.
// answers[i][c] is the answer to rank(i,c)
vector<vector<int64_t>> get_right_answers(const vector<vector<char>>& sets, int64_t sigma){
    int64_t n = sets.size();
    vector<vector<int64_t>> right_answers(n+1, vector<int64_t>(sigma));
    for(int64_t i = 0; i < n; i++){
        for(char c : sets[i]) 
            right_answers[i+1][c]++;
        for(char c = 0; c < sigma; c++)
            right_answers[i+1][c] += right_answers[i][c];
    }
    return right_answers;
}

template<typename wt_type>
bool test_default_constructed(){
    // None of the below should crash
    wt_type wt_1; // Default-constructed
    wt_type wt_2 = wt_1; // Copy constructor
    wt_2 = wt_1; // Copy assignment

    // We don't require a default-constructed tree to work

    //if(wt_1.rank(0, 'x') != 0) return false;
    //if(wt_2.rank(0, 'x') != 0) return false;

    return true;
}

template<typename wt_type>
bool test_copy_semantics(){

    int64_t sigma = 10;
    int64_t n = 1000;
    int64_t random_seed = 12345;
    vector<vector<char> > sets = get_random_subset_sequence(n, sigma, random_seed);

    wt_type* wt1 = new wt_type(sets);
    wt_type* wt2 = new wt_type(*wt1); // Copy constructor
    wt_type* wt3 = new wt_type({{1,2}, {3}});
    *wt3 = *wt2; // Copy assignment

    vector<vector<int64_t> > right_answers = get_right_answers(sets, sigma);

    delete wt1;

    // wt2 and wt3 should still work
    for(int64_t i = 0; i <= n; i++){
        for(char c = 0; c < sigma; c++){
            if(wt2->rank(i,c) != right_answers[i][c]) return false;
            if(wt3->rank(i,c) != right_answers[i][c]) return false;
        }
    }

    delete wt2;

    // wt3 should still work
    for(int64_t i = 0; i <= n; i++){
        for(char c = 0; c < sigma; c++){
            if(wt3->rank(i,c) != right_answers[i][c]) return false;
        }
    }

    delete wt3;

    return true;
}


int main(){

    test_default_constructed<nested_wt_t>();
    test_default_constructed<rrr_generalization_t>();
    test_default_constructed<split_t>();
    test_default_constructed<bitmagic_t>();

    test_copy_semantics<nested_wt_t>();
    test_copy_semantics<rrr_generalization_t>();
    test_copy_semantics<split_t>();
    test_copy_semantics<bitmagic_t>();

}