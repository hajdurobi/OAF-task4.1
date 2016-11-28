#include "maxsearch.hpp"
#include "seqinfileenumerator.hpp"
#include "iostream"
 
using namespace std;
 
class MyMaxSearch : public MaxSearch<int> {
    protected:
        int func(const int& e) const { return e; }
        bool cond(const int& e) const { return e%2!=0; }
};
 
int main() {
    MyMaxSearch pr;
    SeqInFileEnumerator<int> file_enum("input.txt");
    pr.addEnumerator(&file_enum);
   
    pr.run();
   
    if(pr.found()) cout << "A legnagyobb páratlan szám:" << pr.optElem();
    else cout << "Nincs páratlan szám";
   
    return 0;
}