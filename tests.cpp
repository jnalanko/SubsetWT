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

template<typename wt_type>
bool test_correctness(){
    int64_t n = 1e4;
    int64_t random_seed = 124;
    vector<int64_t> sigmas;
    for(int64_t i = 1; i <= 20; i++) sigmas.push_back(i);
    sigmas.push_back(127);

    cerr << endl << "sigma =" << std::flush;
    for(char sigma : sigmas){
        cerr << " " << (int64_t) sigma << std::flush;
        vector<vector<char> > sets = get_random_subset_sequence(n, sigma, random_seed);
        vector<vector<int64_t> > right_answers = get_right_answers(sets, sigma);
        wt_type WT(sets);

        for(int64_t i = 0; i <= n; i++){
            for(char c = 0; c < sigma; c++){
                if(WT.rank(i,c) != right_answers[i][c]){
                    cout << "Wrong answer: Found: " << WT.rank(i,c) << ", expected " << right_answers[i][c] << endl;
                    return false;
                }
            }
        }
    }
    cerr << endl;

    return true;
}

template<typename wt_type>
bool test_long_gaps(){
    int64_t n = 1e6;
    vector<vector<char> > sets(n);
    for(int64_t i = 0; i < n; i++){
        if(i % (int64_t)1e5 == 0) sets[i] = {0,1,2,3};
        else sets[i] = {0};
    }
    wt_type WT(sets);
    bool good = true;
    for(int64_t i = 0; i <= n; i++)
        good &= WT.rank(i, 0) == i;
    return good;
}

bool run_test(std::function<bool()> f, const string& test_name){
    cerr << "Running test " << test_name << "... ";
    bool good = f();
    cerr << (good ? "OK" : "FAIL") << endl;
    return good;
}

template<typename wt_type>
bool run_all_tests(const string& structure_name){
    bool all_good = true;

    all_good &= run_test(test_default_constructed<wt_type>, "test_default_constructed for " + structure_name);
    all_good &= run_test(test_copy_semantics<wt_type>, "test_copy_semantics for " + structure_name);
    all_good &= run_test(test_long_gaps<wt_type>, "test_long_gaps for " + structure_name);
    all_good &= run_test(test_correctness<wt_type>, "test_correctness for " + structure_name);

    return all_good;
}

int main(){

    bool all_good = true;

    all_good &= run_all_tests<nested_wt_t>("Nested WT");
    all_good &= run_all_tests<rrr_generalization_t>("RRR generalization");
    all_good &= run_all_tests<split_t>("Split Structure");
    all_good &= run_all_tests<bitmagic_t>("Bit Magic");

    cerr << (all_good ? "All tests passed" : "SOME TESTS FAILED") << endl;

}