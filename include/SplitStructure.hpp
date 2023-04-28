#include <vector>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/rank_support_v.hpp>
#include "predecessor.hpp"

template<int64_t sigma>
class SplitStructure{
    // Base 3 (0,1,||2) does not require R_bv
    // base 4 (1,2||3,0)
public:
    sdsl::bit_vector L_bv;
    sdsl::bit_vector R_bv;
    sdsl::rank_support_v<> L_bv_rs;
    sdsl::rank_support_v<> R_bv_rs;
    Predecessor *c_predStructure = nullptr;
    uint64_t n;

    SplitStructure(){
        // c_predStructure is initialized to nullptr
    }

    SplitStructure(const vector<char>& seq){
        //assert(sigma == 4 || sigma == 3);
        if (sigma < 3 || sigma > 4) {
            std::cerr << "alphabet size = " << sigma << std::endl;
            throw std::invalid_argument("Split rank structure works only for alphabets of size 3 or 4.");
        }
        n = seq.size();
        int64_t right = 0;
        vector<uint64_t> B_v;
        if (sigma == 4){
            for(char c : seq) if (c == 0 || c == 3) {right ++;}
            L_bv.resize(n-right);
            R_bv.resize(right);
            for (int64_t i = 0, l = 0, r = 0; i < n; i++){
                if(seq[i] == 2 || seq[i] == 1 ){
                    L_bv[l] = seq[i] - 1;
                    l++;
                } else{
                    B_v.push_back(i);
                    R_bv[r] = (seq[i] == 3);
                    r++;
                }
            }
            sdsl::util::init_support(R_bv_rs, &R_bv);
        }
        else if (sigma == 3) { // sigma 3
            for(char c : seq) if (c == 2) {right ++;}  // directly update B_v?
            L_bv.resize(n-right);
            for (int64_t i = 0, l = 0; i < n; i++){
                if(seq[i] == 1 || seq[i] == 0){ // 10 || 01
                    L_bv[l] = seq[i];
                    l++;}
                else { B_v.push_back(i);} // 11 if (seq[i] == 2)
            }
        }

        sdsl::util::init_support(L_bv_rs, &L_bv);
        c_predStructure = new Predecessor(B_v);
    }

    SplitStructure(const SplitStructure& other){
        assert(&other != this); // What on earth are you trying to do?
        operator=(other);
    }

    SplitStructure& operator=(const SplitStructure& other){
        if(&other != this){
            this->n = other.n;

            this->L_bv = other.L_bv;
            this->R_bv = other.R_bv;

            this->L_bv_rs = other.L_bv_rs;
            this->R_bv_rs = other.R_bv_rs;

            this->L_bv_rs.set_vector(&(this->L_bv));
            this->R_bv_rs.set_vector(&(this->R_bv));

            delete this->c_predStructure;
            if(other.c_predStructure != nullptr){    
                c_predStructure = new Predecessor(*(other.c_predStructure));
            }

            return *this;
        } else return *this; // Assignment to self -> do nothing.
    }

    ~SplitStructure() {
        delete c_predStructure;
        c_predStructure = nullptr;
    }

    // Rank of symbol in half-open interval [0..pos)
    int64_t rank(int64_t pos, char symbol) const{
        //assert(sigma == 3 || sigma == 4);
        if (pos <= 0)[[unlikely]]{ return 0;}
        int64_t new_pos;
        pair<int64_t,bool> pred = {1,0};
        pred = c_predStructure->getPredWithJumpTable(pos);
        if (sigma == 4){
            if (symbol == 1){
                new_pos = pos - (pred.first + !pred.second);
                if (new_pos > n) new_pos = n+1;
                return new_pos - L_bv_rs(new_pos);
            }
            if (symbol == 2){
                new_pos = pos - (pred.first + !pred.second);
                return L_bv_rs(new_pos);
            }
            if (pred.second == 0) pred.first += 1;// pred.first < pos &&
            if (symbol == 0){return pred.first - R_bv_rs(pred.first);}
            return R_bv_rs.rank(pred.first);// symbol == 3
        } else { // sigma == 3
            if (symbol == 0){
                new_pos = pos - (pred.first + !pred.second);
                return new_pos - L_bv_rs(new_pos);
            }
            if (symbol == 1){
                new_pos = pos - (pred.first + !pred.second);
                return L_bv_rs(new_pos);
            }
            return (pred.second == 0) ? pred.first += 1 : pred.first; // symbol == 2
        }
    }

//  rank(pos, symbol) + rank(pos, sigma-1) == rank(pos,{01,10}) + rank(pos, 11)
    int64_t rankpair(int64_t pos, char symbol) const{ // 1,2 for base 4 || 0,1 for base 3
        //assert((sigma == 3 || sigma == 4) && symbol != sigma-1);
        if (symbol >= sigma-1){
            throw std::invalid_argument("Rank pair only works for symbols smaller than sigma - 1.");
        }
        if (pos <= 0)[[unlikely]]{ return 0;}
        pair<int64_t,bool> pred= {-1,0};
        pred = c_predStructure->getPredWithJumpTable(pos);
        
        int64_t new_pos = pos - (pred.first + !pred.second);
        if (new_pos > n) new_pos = n+1;
        int64_t rank_10 = L_bv_rs(new_pos);
        if (pred.second == 0) pred.first+=1; // after new_pos
        if (sigma == 4){
            int64_t rank_3 = R_bv_rs.rank(pred.first); // = 11
            if (symbol == 1){ //01
                return rank_3 + new_pos - rank_10;
            }
            return rank_3 + rank_10; // 10
        } else { // sigma == 3
            if (symbol == 0){ // 01
                return pred.first + new_pos - rank_10;
            }
            return pred.first + rank_10; // 10
        }
    }

    size_t size_in_bytes() const{
        size_t sz = 0;
        sz += sdsl::size_in_bytes(L_bv);
        sz += sdsl::size_in_bytes(L_bv_rs);
        sz += sdsl::size_in_bytes(R_bv);
        sz += sdsl::size_in_bytes(R_bv_rs);
        sz += c_predStructure->sizeInBytes();
        return sz;
    }
};
