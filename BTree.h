#ifndef DZ2P_H
#define DZ2P_H

#include <iostream>

using namespace std;

struct Cvor {
	int br_k;
	bool list;
	Cvor* otac;
	int* K;
	Cvor** p;
	Cvor(bool l, int brk = 1, Cvor* otac1 = nullptr, int* nk = nullptr) :br_k(brk), list(l), K(nk), p(new Cvor* [brk + 1]), otac(otac1) {
		for (int i = 0; i < br_k + 1; i++)p[i] = nullptr;
	}
	Cvor(Cvor&) = delete;
	Cvor(Cvor&&) = delete;
	~Cvor() {
		/*if (otac != nullptr) {
			for (int i = 0; i < otac->br_k + 1; i++) {
				if (otac->p[i] == this) {
					otac->p[i] = nullptr;
					break;
				}
			}
		}*/
		delete[]K;
		delete[]p;
	}
	friend ostream& operator<<(ostream&, const Cvor&);
	int Br_grananja_predaka()const;
};

class Bstablo {
public:
	
	Bstablo(int r = 0) :red(r), koren(nullptr) {}
	Bstablo(Bstablo*) = delete;
	Bstablo(Bstablo**) = delete;
	~Bstablo();

	void Postavi_red(int r) { red = r; }
	Cvor* Dohvati_koren()const { return koren; }
	int Visina_stabla()const;
	friend ostream& operator<<(ostream&, const Bstablo&);
	void Ispis_stabla()const;
	int Broj_kljuceva()const;
	bool Pretraga(int, Cvor*&)const;
	bool Umetanje(int);
	void Umetanje_u_unutrasnji_cvor(int, Cvor*, Cvor*, Cvor*);
	void Pomocno_umetanje();
	bool Brisanje(int);
	Cvor* Brat(bool, Cvor*, int&);
	bool Brat(bool, Cvor*);
	Cvor* Brat_postoji(bool, Cvor*, int&);
	void Brisanje_iz_unutrasnjeg_cvora(int, Cvor*, Cvor*);

private:
	int red;
	Cvor* koren;
};

#endif // !DZ2P_H