#include "library/linsearch.hpp"
#include "library/seqinfileenumerator.hpp"
#include "library/summation.hpp"
#include <iostream>
#include <string>

using namespace std;

// A hallgatók struktúrája és a beolvasó, kiíró operátorok kiterjesztése
struct Hallgato {
  string neptunkod;
  int jegy;
  double atlag;
  
  friend ifstream& operator>>(ifstream&, Hallgato&);
  friend ostream& operator<<(ifstream&, Hallgato&);
};
ifstream& operator >>(ifstream& f, Hallgato& df) {
	f >> df.neptunkod >> df.jegy;
	return f;
}
ostream& operator <<(ostream& s, Hallgato& akt) {
	s << akt.neptunkod << akt.atlag;
	return s;
}
// A feladatot megoldó Lineáris keresést az általános linkerbõl származtatjuk.
class MyLinSearch : public LinSearch<Hallgato, true> {
    protected:  
        bool cond(const Hallgato& e) const { return e.atlag >= 4; }
};

// Absztrakt felsoroló osztály
class HallgatoEnumerator : public Enumerator<Hallgato> {
	protected:
		SeqInFileEnumerator<Hallgato> *_f;
		Hallgato _current;
		bool _end;
	public:
		HallgatoEnumerator(const string &filename) {
			try { _f = new SeqInFileEnumerator<Hallgato>(filename);
			} catch (SeqInFileEnumerator<Hallgato>::Exceptions ex) {
				cout << "Nincs meg a file." ; exit(1);
			}
		}
		void first() { _f->first(); next(); }
		void next();
		bool end() const { return _end; }
		Hallgato current() const { return _current; }
};
class AvgCalc : public Summation<Hallgato,double> {
	private:
		const string neptunkod;
		int db;
		double sum;
		
	protected:
		void init() {db = 0; sum = 0;}
		void first() {}
		bool whileCond(const Hallgato &e) const { return e.neptunkod == neptunkod; }
		void add(const Hallgato &e) { sum+=e.jegy; db+= 1;  *_result=sum/db; }
	
	public:
		AvgCalc(string neptunkod) : Summation<Hallgato, double> (), neptunkod(neptunkod) {}
};
void HallgatoEnumerator::next() {
	_end = _f->end();
	if(!_end) {
		_current.neptunkod = _f->current().neptunkod;
		cout << _current.neptunkod << " " ;
		AvgCalc avg(_f -> current().neptunkod);
		avg.addEnumerator(_f);
		avg.run();
		_current.atlag = avg.result();
		cout << " atlaga : " << _current.atlag << endl;
	}
}
int main() {
    MyLinSearch pr;
    HallgatoEnumerator t("input.txt");
    pr.addEnumerator(&t);

    pr.run();
	cout << "Eredmeny: ";
    if(pr.found()) cout << "Igaz" ;
    else cout << "Hamis";
   
    return 0;
}
