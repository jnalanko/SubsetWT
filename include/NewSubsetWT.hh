#pragma once 

#include <vector>
#include <utility>
#include <iostream>

using namespace std;

template<typename base4_rank_t, typename base3_rank_t>
class SubsetWT{

public:

    static const int64_t ROOT_NONE = 0; // '00'
    static const int64_t ROOT_RIGHT = 1; // '01'
    static const int64_t ROOT_LEFT = 2; // '10'
    static const int64_t ROOT_BOTH = 3; // '11'

    static const int64_t CHILD_RIGHT = 0; // '01'
    static const int64_t CHILD_LEFT = 1; // '10'
    static const int64_t CHILD_BOTH = 2; // '11'

    vector<char> alphabet;
    vector<char> char_to_idx; // Vector of length 256 mapping characters to their indices in the alphabet
    
    base4_rank_t root;

    // children[0] and children[1] are the left and right children of the root
    // The children of children[i] are at children[2*i+2] and children[2*i + 3]
    vector<base3_rank_t> children;

    // Alphabet intervals of child nodes
    vector<pair<int64_t, int64_t> > child_intervals;    


private:

    constexpr int64_t get_left_child_idx(int64_t child_idx) const{
        return 2*child_idx+2;
    }

    constexpr int64_t get_right_child_idx(int64_t child_idx) const{
        return 2*child_idx+2 + 1;
    }

    // Helper function used in the constructor
    void init_alphabet(const vector<vector<char>>& sets){
        vector<bool> alphabet_bitmap(256, 0); // Temporary bitmap to mark the distinct characters
        char_to_idx.resize(256, -1);

        for(const vector<char>& v : sets){
            for(char c : v) alphabet_bitmap[c] = 1;
        }

        for(int64_t c = 0; c < 256; c++){
            if(alphabet_bitmap[c]){
                char_to_idx[c] = alphabet.size();
                alphabet.push_back(c);
            }
        }
    }

    // Helper function used in the constructor
    // Alphabet must be initialized before calling
    // [start, end) is a half-open interval
    void init_children_recursion(int64_t child_idx, int64_t start, int64_t end, const vector<vector<char>>& sets, vector<int64_t>&& sets_in_this_child){
        child_intervals[child_idx] = {start,end};

        char middle_char = alphabet[(start + end)/2];
        // [start, midpoint) go left, [midpoint, end) go right

        // Set indexes that go to left and to right
        vector<int64_t> sets_to_left, sets_to_right;

        vector<char> split_seq; // 0 = left, 1 = right, 2 = both

        // Split the alphabet
        for(int64_t i : sets_in_this_child){
            bool has_left = false;
            bool has_right = false;
            for(char c : sets[i]){
                if(c < middle_char){
                    has_left = true;
                } else {
                    has_right = true;
                }
            }
            if(has_left & !has_right) split_seq.push_back(CHILD_LEFT);
            else if(!has_right & has_right) split_seq.push_back(CHILD_RIGHT);
            else split_seq.push_back(CHILD_BOTH);

            if(has_left) sets_to_left.push_back(i);
            if(has_right) sets_to_right.push_back(i);
        }

        vector<int64_t>().swap(sets_in_this_child); // Free memory

        // Construct the base-3 rank support
        children[child_idx] = base3_rank_t(split_seq);

        // Recurse to children
        if(end - start >= 3){
            int64_t left_idx = get_left_child_idx(child_idx);
            int64_t right_idx = get_right_child_idx(child_idx);
            init_children_recursion(left_idx , start          , (start + end)/2, sets, std::move(sets_to_left));
            init_children_recursion(right_idx, (start + end)/2, end            , sets, std::move(sets_to_right));
        }
    }

    // Helper function used in the constructor
    // Alphabet must be initialized before calling
    void init_tree(const vector<vector<char>>& sets){
        int64_t sigma = alphabet.size();
        char middle_char = alphabet[sigma/2];

        // Initialize the root
        vector<char> root_split_seq(sets.size());
        vector<int64_t> sets_to_left_child, sets_to_right_child;
        for(int64_t i = 0; i < sets.size(); i++){
            bool has_left = false;
            bool has_right = true;
            for(char c : sets[i]){
                if(c < middle_char) has_left = true;
                else has_right = true;
            }

            if(!has_left && !has_right) root_split_seq.push_back(ROOT_NONE);
            else if(has_left && !has_right) root_split_seq.push_back(ROOT_LEFT);
            else if(!has_left && has_right) root_split_seq.push_back(ROOT_RIGHT);
            else if(has_left && has_right) root_split_seq.push_back(ROOT_BOTH);

            if(has_left) sets_to_left_child.push_back(i);
            if(has_right) sets_to_right_child.push_back(i);
        }

        root = base4_rank_t(root_split_seq);

        // Initialize the children
        child_intervals.resize(sigma); // This may have a little bit more space that required but that's not so bad
        children.resize(sigma); // This may have a little bit more space that required but that's not so bad

        init_children_recursion(0, 0, sigma/2, sets, std::move(sets_to_left_child)); // Left child of root
        init_children_recursion(1, sigma/2, sigma, sets, std::move(sets_to_right_child)); // Right child of root
    }

public:

    SubsetWT(){}

    SubsetWT(const vector<vector<char>>& sets){
        init_alphabet(sets);
        init_tree(sets);
    }

    // Count of character c in subsets up to pos, not including pos
    int64_t rank(int64_t pos, char c) const{
        int64_t char_idx = char_to_idx[c];
        if(char_idx == -1) return 0; // Character not found

        char root_sym = char_idx < alphabet.size()/2 ? ROOT_LEFT : ROOT_RIGHT;
        int64_t x = root.rankpair(pos, root_sym);

        // Traverse down the tree
        int64_t child_idx = (root_sym == ROOT_LEFT) ? 0 : 1;
        while(child_idx < children.size()){
            const auto [left, right] = child_intervals[child_idx]; // C++17 structured binding
            char child_sym = char_idx < (left + right) / 2 ? CHILD_LEFT : CHILD_RIGHT;
            x = children[child_idx].rankpair(x, child_sym);
        }

        return x;
    }

    int64_t size_in_bytes() const{
        return 0; // TODO
    }

    int64_t serialize(ostream& os) const{
        return 0; // TODO
    }

    void load(istream& is){
        return; // TODO
    }


};
