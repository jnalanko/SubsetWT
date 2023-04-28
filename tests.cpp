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

template<typename wt_type>
bool test_default_constructed(){
    // None of the below should crash
    wt_type wt_1; // Default-constructed
    wt_type wt_2 = wt_1; // Copy constructor
    wt_2 = wt_1; // Assignment operator

    // We don't require a default-constructed tree to work

    //if(wt_1.rank(0, 'x') != 0) return false;
    //if(wt_2.rank(0, 'x') != 0) return false;

    return true;
}

/*
template<typename wt_type>
bool test_copy_semantics(){

    vector<vector<char> > sets = get_random_subset_sequence(1000, 10, 12345);

    wt_type wt

    return true;
}
*/


int main(){

    test_default_constructed<nested_wt_t>();
    test_default_constructed<rrr_generalization_t>();
    test_default_constructed<split_t>();
    test_default_constructed<bitmagic_t>();

}