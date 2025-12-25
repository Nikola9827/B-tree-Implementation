///////////////////////////////////////////////////////////////////////////////////////////////////h file

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

///////////////////////////////////////////////////////////////////////////////////////////////////cpp file

#include <iostream>
#include "dz2p1.h"
#include <fstream>
#include <queue>
#include <iomanip>
#include <cmath>

using namespace std;

int max_br_cifara_kljuca = 1;									//update pri ubacivanju kljuca - uradjeno

int Br_cifara(int b) {
	return static_cast<int>(floor(log10(b)) + 1);
}

int Cvor::Br_grananja_predaka()const {
	Cvor* tmp = otac;
	int b = 1;
	while (tmp != nullptr) {
		b *= (tmp->br_k + 1);
		tmp = tmp->otac;
	}
	return b;
}

Bstablo::~Bstablo() {
	if (koren != nullptr) {
		Cvor* tmp = koren;
		queue <Cvor*> red;
		red.push(tmp);
		while (!red.empty()) {
			tmp = red.front();
			red.pop();
			if (!tmp->list) for (int i = 0; i <= tmp->br_k; i++) red.push(tmp->p[i]);
			delete tmp;
		}
	}
	koren = nullptr;
}

int Bstablo::Visina_stabla()const {
	int h = 0;
	Cvor* tmp = koren;
	while (tmp != nullptr) { 
		tmp = tmp->p[0];
		h++;
	}
	if (h != 0)h--;
	return h;
}

ostream& operator<<(ostream& os, const Cvor& cvor){
	os << "|" << cvor.K[0];
	for (int i = 1; i < cvor.br_k; i++) os << " " << cvor.K[i];
	os << "|";
	return os;
}

ostream& operator<<(ostream& os, const Bstablo& stablo) {
	if (stablo.koren != nullptr) {
		int sirina_stabla = static_cast<int>(0.6*(pow(stablo.red, stablo.Visina_stabla()) * ((stablo.red * 1.0 - 1) * max_br_cifara_kljuca * 1.0 + stablo.red * 1.0)));
		Cvor * tmp = stablo.koren;
		queue <Cvor*> red;
		red.push(tmp);
		int m = 1;
		//bool u = true;
		while (!red.empty()) {
			//u = false;
			int mp = m;
			m = 0;
			for (int i = 0; i < mp; i++) {
				tmp = red.front();
				red.pop();
				double dostupno_mesta = (sirina_stabla * 1.0) / (tmp->Br_grananja_predaka() * 1.0);
				int sirina_cvora = max_br_cifara_kljuca * tmp->br_k + tmp->br_k + 1;
				os << setw(static_cast<int>((dostupno_mesta - sirina_cvora) / 2)) << " " << setw(sirina_cvora) << *tmp << setw(static_cast<int>((dostupno_mesta - sirina_cvora) / 2 + 1)) << " ";
				if (!tmp->list) {
					for (int i = 0; i <= tmp->br_k; i++) {
						red.push(tmp->p[i]);
						m++;
					}
				}
			}
			os << endl << endl << endl << endl;
		}
	} else os << " Stablo je prazno." << endl << endl;
	return os;
}

void Bstablo::Ispis_stabla()const {
	int u;
	cout << " Ispis stabla:\n 0. U datoteci\n 1. Na ekranu\n\n Izaberite jednu od opcija navodeci njen broj: ";
	cin >> u;
	cout << endl;
	if (u) {
		cout << *this << endl << endl;
		cout << " Stablo je ispisano na ekranu." << endl << endl;
	}
	else {
		string ime;
		fstream datoteka;
		cout << " Unesite ime datoteke za ispis: ";
		cin >> ime;
		cout << endl;
		datoteka.open(ime, ios::out | ios_base::app);
		datoteka << *this << endl << endl;
		cout << " Stablo je ispisano u datoteci." << endl << endl;
		datoteka.close();
	}
}

int Bstablo::Broj_kljuceva()const {
	int brk = 0;
	if (koren != nullptr) {
		Cvor* tmp = koren;
		queue <Cvor*> red;
		red.push(tmp);
		while (!red.empty()) {
			tmp = red.front();
			red.pop();
			if (!tmp->list) for (int i = 0; i <= tmp->br_k; i++) red.push(tmp->p[i]);
			brk += tmp->br_k;
		}
	}
	return brk;
}

bool Bstablo::Pretraga(int k, Cvor*& p)const {
	Cvor* tmp = koren;
	p = nullptr;
	while (tmp != nullptr) {
		if (k > tmp->K[tmp->br_k - 1]) {
			p = tmp;
			tmp = tmp->p[tmp->br_k];
		}
		else {
			for (int i = 0; i < tmp->br_k; i++) {
				if (k < tmp->K[i]) {
					p = tmp;
					tmp = tmp->p[i];
					break;
				}
				else if (k == tmp->K[i]) {
					p = tmp;
					return true;
				}

			}
		}
		
	}
	return false;
}

bool Bstablo::Umetanje(int k) {
	int br_cifara_kljuca = Br_cifara(k);
	max_br_cifara_kljuca = (max_br_cifara_kljuca<br_cifara_kljuca)?br_cifara_kljuca:max_br_cifara_kljuca;
	Cvor* pozicija = nullptr;
	if (koren == nullptr) {
		int* niz = new int[1];
		niz[0] = k;
		koren = new Cvor(true, 1, nullptr, niz);
	}
	else if (!Pretraga(k, pozicija)) {
		if (pozicija->br_k < red - 1) {
			pozicija->br_k++;
			int* K_tmp = new int[pozicija->br_k];
			Cvor** p_tmp = new Cvor * [pozicija->br_k+1];
			for (int i = 0, j = 0; i < pozicija->br_k; i++) {		//nesto ne valja
				p_tmp[i] = nullptr;									//??
				if (i == j && i == pozicija->br_k - 1) {
					K_tmp[i] = k;
				}
				else if (pozicija->K[j] < k || i != j) {
					K_tmp[i] = pozicija->K[j++];
				}
				else {
					K_tmp[i] = k;
				}
			}
			p_tmp[pozicija->br_k] = nullptr;					//??
			
			int* tmp1 = pozicija->K;
			pozicija->K = K_tmp;
			K_tmp = tmp1;
			delete[]K_tmp;

			Cvor** tmp2 = pozicija->p;
			pozicija->p = p_tmp;
			p_tmp = tmp2;
			delete[]p_tmp;
		}
		else {
			int* niz = new int[pozicija->br_k + 1];
			for (int i = 0, j = 0; i < pozicija->br_k + 1; i++) {
				if (i == j && i == pozicija->br_k) {
					niz[i] = k;
				}
				else if (pozicija->K[j] < k || i != j) {
					niz[i] = pozicija->K[j++];
				}
				else {
					niz[i] = k;
				}
			}
			int indeks = static_cast<int>((pozicija->br_k*1.0 + 1) * 1.0 / 2 - 0.5);
			int pk = niz[indeks];
			int* niz0 = new int[indeks];
			int* niz1 = new int[(pozicija->br_k + 1) - indeks - 1];
			for (int i = 0; i < indeks; i++) niz0[i] = niz[i];
			for (int i = indeks+1, j=0; i < pozicija->br_k+1; niz1[j++] = niz[i++]);
			delete[]niz;
			Cvor* p0 = new Cvor(true, indeks, pozicija->otac, niz0);								//delete
			Cvor* p1 = new Cvor(true, pozicija->br_k + 1 - indeks - 1, pozicija->otac, niz1);		//delete
			Umetanje_u_unutrasnji_cvor(pk, pozicija->otac, p0, p1);
			delete pozicija;
		}
		return true;
	}
	else {
		return false;
	}	
}

void Bstablo::Umetanje_u_unutrasnji_cvor(int k, Cvor* gornji, Cvor* p0, Cvor* p1) {
	bool u = true;
	while (u) {
		if (gornji == nullptr) {
			u = false;
			int* niz = new int[1];
			niz[0] = k;
			gornji = new Cvor(false, 1, nullptr, niz);
			gornji->p[0] = p0;
			gornji->p[1] = p1;
			p0->otac = gornji;
			p1->otac = gornji;
			koren = gornji;
		}
		else if (gornji->br_k < red - 1) {
			u = false;
			gornji->br_k++;
			int* K_tmp = new int[gornji->br_k];
			Cvor** p_tmp = new Cvor * [gornji->br_k + 1];
			for (int i = 0, j = 0, m = 0; i < gornji->br_k; i++) {
				//p_tmp[i] = nullptr;									//??
				if (i == j && i == gornji->br_k - 1) {
					K_tmp[i] = k;
					p_tmp[i] = p0;
					p_tmp[i + 1] = p1;
					m++;
				}
				else if (gornji->K[j] < k || i != j) {
					K_tmp[i] = gornji->K[j];
					p_tmp[i + m] = gornji->p[j + m];
					j++;
				}
				else {
					K_tmp[i] = k;
					p_tmp[i] = p0;
					p_tmp[i+1] = p1;
					m++;
				}
			}
			int* tmp1 = gornji->K;
			gornji->K = K_tmp;
			K_tmp = tmp1;
			delete[]K_tmp;

			Cvor** tmp2 = gornji->p;
			gornji->p = p_tmp;
			p_tmp = tmp2;
			delete[]p_tmp;

			p0->otac = gornji;
			p1->otac = gornji;
		}
		else {
			int* K_tmp = new int[gornji->br_k + 1];
			Cvor** p_tmp = new Cvor * [gornji->br_k + 2];
			for (int i = 0, j = 0, m = 0; i < gornji->br_k + 1; i++) {
				if (i == j && i == gornji->br_k) {
					K_tmp[i] = k;
					p_tmp[i] = p0;
					p_tmp[i + 1] = p1;
					m++;
				}
				else if (gornji->K[j] < k || i != j) {
					K_tmp[i] = gornji->K[j];
					p_tmp[i + m] = gornji->p[j + m];
					j++;
				}
				else {
					K_tmp[i] = k;
					p_tmp[i] = p0;
					p0->otac = gornji;
					p_tmp[i+1] = p1;
					p1->otac = gornji;
					m++;
				}
			}

			int indeks = static_cast<int>((gornji->br_k * 1.0 + 1) * 1.0 / 2 - 0.5);
			int pk = K_tmp[indeks];
			int* K_tmp0 = new int[indeks];
			Cvor** p_tmp0 = new Cvor * [indeks + 1];
			int* K_tmp1 = new int[(gornji->br_k + 1) - indeks - 1];
			Cvor** p_tmp1 = new Cvor * [(gornji->br_k + 1) - indeks - 1 + 1];
			for (int i = 0; i < indeks; i++) {
				K_tmp0[i] = K_tmp[i];
				p_tmp0[i] = p_tmp[i];
			}
			p_tmp0[indeks] = p_tmp[indeks];							//ocevi!!!!!!!
			p_tmp1[0] = p_tmp[indeks+1];
			for (int i = indeks + 1, j = 0; i < gornji->br_k + 1; i++, j++) {
				K_tmp1[j] = K_tmp[i];
				p_tmp1[j+1] = p_tmp[i+1];
			}
			delete[]p_tmp;
			delete[]K_tmp;
			Cvor* p0_tmp = new Cvor(false, indeks, gornji->otac, K_tmp0);
			Cvor* p1_tmp = new Cvor(false, (gornji->br_k + 1) - indeks - 1, gornji->otac, K_tmp1);

			Cvor** tmp0 = p0_tmp->p;
			p0_tmp->p = p_tmp0;
			p_tmp0 = tmp0;
			delete[]p_tmp0;

			Cvor** tmp1 = p1_tmp->p;
			p1_tmp->p = p_tmp1;
			p_tmp1 = tmp1;
			delete[]p_tmp1;

			for (int i = 0; i < p0_tmp->br_k + 1; i++) {
				p0_tmp->p[i]->otac = p0_tmp;
			}

			for (int i = 0; i < p1_tmp->br_k + 1; i++) {
				p1_tmp->p[i]->otac = p1_tmp;
			}

			//p0->otac = p0_tmp;
			//p1->otac = p1_tmp;

			k = pk;
			Cvor* tmp2 = gornji;
			gornji = gornji->otac;
			delete tmp2;
			p0 = p0_tmp;
			p1 = p1_tmp;
		}
	}
}

void Bstablo::Pomocno_umetanje() {
	int u;
	cout << " Umetanje u stablo:\n 0. Iz datoteke\n 1. Sa tastature\n\n Izaberite jednu od opcija navodeci njen broj: ";
	cin >> u;
	cout << endl;
	if (u) {
		cout << " Unesite kljuceve koji se umecu (da bi se unos zavrsio potrebno je uneti negativan broj): " << endl;
		while (true) {
			int k;
			cout << " ";
			cin >> k;
			if (k < 0) break;
			bool p = Umetanje(k);
		}
		cout << " Umetanje je zavrseno." << endl << endl;
	}
	else {
		string ime;
		fstream datoteka;
		cout << " Unesite ime datoteke za unos: ";
		cin >> ime;
		cout << endl;
		datoteka.open(ime);
		int k;
		while (datoteka>>k) {
			if (k < 0) break;
			bool p = Umetanje(k);
		}
		cout << " Umetanje je zavrseno." << endl << endl;
		datoteka.close();
	}
}

bool Bstablo::Brisanje(int k) {
	Cvor* pozicija = nullptr;
	if (!Pretraga(k, pozicija)) {
		return false;
	}
	else {
		if (!pozicija->list) {
			int n = 0;
			for (int i = 0; i < pozicija->br_k; i++) {
				if (pozicija->K[i] == k) {
					n = i;
					break;
				}
			}
			Cvor* tmp = pozicija->p[n + 1];
			while (tmp->p[0] != nullptr)tmp = tmp->p[0];
			int sledbenik = tmp->K[0];
			pozicija->K[n] = sledbenik;
			tmp->K[0] = k;
			//k = sledbenik;
			pozicija = tmp;
		}
		if (pozicija->br_k - 1 >= static_cast<int>(red / 2.0 - 0.5) || pozicija == koren) {
			if (pozicija->br_k == 1 && pozicija == koren) {
				koren = nullptr;
				delete pozicija;
			}
			else {
				int* K_tmp = new int[pozicija->br_k - 1];
				for (int i = 0, j = 0; i < pozicija->br_k; i++) {
					if (pozicija->K[i] != k) K_tmp[j++] = pozicija->K[i];
				}
				Cvor* tmp_cvor = new Cvor(true, pozicija->br_k - 1, pozicija->otac, K_tmp);
				Cvor* otac = pozicija->otac;
				if (otac != nullptr) {
					for (int i = 0; i < otac->br_k + 1; i++) {
						if (otac->p[i] == pozicija) otac->p[i] = tmp_cvor;
					}
				}
				if (pozicija == koren) {
					koren = tmp_cvor;
				}
				delete pozicija;
			}
		}
		else if (Brat(true, pozicija)) {
			int n;														//otac->p[n] pokazuje na poziciju
			Cvor* otac = pozicija->otac;
			Cvor* desni_brat = Brat(true, pozicija, n);
			int razdvojni_kljuc = otac->K[n];								//razdvojni je indeks onog p koji pokazuje na pozicija
			int zajam = desni_brat->K[0];
			

			int* K0_tmp = new int[pozicija->br_k];								//delete - uradio
			for (int i = 0, m = 0; i < pozicija->br_k; i++) {
				if (i == pozicija->br_k - 1) K0_tmp[i] = razdvojni_kljuc;
				else if (pozicija->K[i] != k) {
					K0_tmp[i] = pozicija->K[i+m];
				}
				else {
					K0_tmp[i] = pozicija->K[i+1];
					m++;
				}
			}
			Cvor* novi_pozicija = new Cvor(true, pozicija->br_k, otac, K0_tmp);
			otac->p[n] = novi_pozicija;
			delete pozicija;
			
			otac->K[n] = zajam;

			int* K1_tmp = new int[desni_brat->br_k - 1];								//delete - uradio
			for (int i = 0; i < desni_brat->br_k - 1; i++) {
				K1_tmp[i] = desni_brat->K[i + 1];
			}
			Cvor* novi_desni_brat = new Cvor(true, desni_brat->br_k - 1, otac, K1_tmp);
			otac->p[n+1] = novi_desni_brat;
			delete desni_brat;
		}
		else if (Brat(false, pozicija)) {
			int n;														//otac->p[n] pokazuje na poziciju
			Cvor* otac = pozicija->otac;
			Cvor* levi_brat = Brat(false, pozicija, n);
			int razdvojni_kljuc = otac->K[n-1];								//razdvojni je indeks onog p koji pokazuje na pozicija
			int zajam = levi_brat->K[levi_brat->br_k-1];


			int* K0_tmp = new int[levi_brat->br_k-1];								//delete - uradio
			for (int i = 0; i < levi_brat->br_k-1; i++) {
				K0_tmp[i] = levi_brat->K[i];
			}
			Cvor* novi_levi_brat = new Cvor(true, levi_brat->br_k-1, otac, K0_tmp);
			otac->p[n - 1] = novi_levi_brat;
			delete levi_brat;

			otac->K[n-1] = zajam;

			int* K1_tmp = new int[pozicija->br_k];								//delete - uradio
			K1_tmp[0] = razdvojni_kljuc;
			for (int i = 1, m = 1; i < pozicija->br_k; i++) {
				if (pozicija->K[0] == k)m = 0;
				if (pozicija->K[i] != k)  {
					K1_tmp[i] = pozicija->K[i - m];
				}
				else {
					K1_tmp[i] = pozicija->K[i-1];
					m--;
				}
			}
			Cvor* novi_pozicija = new Cvor(true, pozicija->br_k, otac, K1_tmp);
			otac->p[n] = novi_pozicija;
			delete pozicija;
		}
		else {
			int n;
			Cvor* brat = Brat_postoji(true, pozicija, n);					//p[n]-> je pozicija, K[n] razdvaja
			if (brat == nullptr) {
				brat = Brat_postoji(false, pozicija, n);
				Cvor* tmp = pozicija;
				pozicija = brat;
				brat = tmp;
				n--;
			}
			int br_k_novi = 2 * static_cast<int>(red / 2.0 + 0.5) - 2;
			int* K_tmp = new int[br_k_novi];									//delete - uradio
			Cvor** p_tmp = new Cvor * [br_k_novi+1];
			Cvor* otac = pozicija->otac;
			int j = 0;
			for (int i = 0; i < pozicija->br_k; i++) {
				if (pozicija->K[i] != k) {
					K_tmp[j++] = pozicija->K[i];
				}
			}
			K_tmp[j++] = otac->K[n];
			for (int i = 0; i < brat->br_k; i++) {
				if (brat->K[i] != k) {
					K_tmp[j++] = brat->K[i];
				}
			}
			Cvor* p = new Cvor(true, br_k_novi, otac, K_tmp);
			delete pozicija;
			delete brat;
			//delete[] K_tmp;
			Brisanje_iz_unutrasnjeg_cvora(otac->K[n], otac, p);

		}
		return true;
	}
}

void Bstablo::Brisanje_iz_unutrasnjeg_cvora(int k, Cvor* gornji, Cvor* p) {
	bool u = true;
	int iteration = 0;
	while (u) {
		if (gornji->br_k - 1 >= static_cast<int>(red / 2.0 - 0.5) || gornji == koren) {
			u = false;
			if (gornji->br_k == 1 && gornji == koren) {
				koren = p;
				delete gornji;
				p->otac = nullptr;
			}
			else {
				int* K_tmp = new int[gornji->br_k - 1];
				Cvor** p_tmp = new Cvor * [gornji->br_k];
				for (int i = 0, j = 0, m = 0; i < gornji->br_k; i++) {
					if (gornji->K[i] != k) {
						K_tmp[j] = gornji->K[i];
						p_tmp[j + m] = gornji->p[i + m];
						j++;
					}
					else {
						p_tmp[j] = p;
						m++;
					}
				}/////////////////////////////////
				//Cvor* tmp_gornji = new Cvor(true, gornji->br_k - 1, gornji->otac, K_tmp);
				gornji->br_k--;

				Cvor** pomocna0 = gornji->p;
				gornji->p = p_tmp;
				p_tmp = pomocna0;
				delete[]p_tmp;

				int* pomocna1 = gornji->K;
				gornji->K = K_tmp;
				K_tmp = pomocna1;
				delete[]K_tmp;
				//Cvor* otac = gornji->otac;
			}
		}
		else if (Brat(true, gornji)) {
			u = false;
			int n;														//otac->p[n] pokazuje na poziciju
			Cvor* otac = gornji->otac;
			Cvor* desni_brat = Brat(true, gornji, n);
			int razdvojni_kljuc = otac->K[n];								//razdvojni je indeks onog p koji pokazuje na pozicija
			int zajam = desni_brat->K[0];
			int* K0_tmp = new int[gornji->br_k];								//delete - uradio
			Cvor** p0_tmp = new Cvor * [gornji->br_k + 1];
			for (int i = 0, m = 0; i < gornji->br_k; i++) {
				if (gornji->K[i] != k) {
					if (i == gornji->br_k - 1) {
						K0_tmp[i] = razdvojni_kljuc;
						p0_tmp[i] = gornji->p[i + 1];
						p0_tmp[i + 1] = desni_brat->p[0];
					}
					else {
						K0_tmp[i] = gornji->K[i + m];
						p0_tmp[i] = gornji->p[i + m];
					}
				}
				else {
					if (i == gornji->br_k - 1) {
						K0_tmp[i] = razdvojni_kljuc;
						p0_tmp[i] = p;
						p0_tmp[i + 1] = desni_brat->p[0];
					}
					else {
						
						K0_tmp[i] = gornji->K[i + 1];
						p0_tmp[i] = p;
						m++;
					}
				}
			}

			int* tmp0 = gornji->K;
			gornji->K = K0_tmp;
			K0_tmp = tmp0;
			delete[]K0_tmp;

			Cvor** tmp1 = gornji->p;
			gornji->p = p0_tmp;
			p0_tmp = tmp1;
			delete[]p0_tmp;

			//for(int i=0)

			otac->K[n] = zajam;

			int* K1_tmp = new int[desni_brat->br_k - 1];								//delete - uradio
			Cvor** p1_tmp = new Cvor * [desni_brat->br_k];
			for (int i = 0; i < desni_brat->br_k - 1; i++) {
				K1_tmp[i] = desni_brat->K[i + 1];
				p1_tmp[i] = desni_brat->p[i + 1];
			}
			p1_tmp[desni_brat->br_k - 1] = desni_brat->p[desni_brat->br_k];
			/*Cvor* novi_desni_brat = new Cvor(true, desni_brat->br_k - 1, otac, K1_tmp);
			otac->p[n + 1] = novi_desni_brat;
			delete desni_brat;*/

			int* tmp2 = desni_brat->K;
			desni_brat->K = K1_tmp;
			K1_tmp = tmp2;
			delete[]K1_tmp;

			Cvor** tmp3 = desni_brat->p;
			desni_brat->p = p1_tmp;
			p1_tmp = tmp3;
			delete[]p1_tmp;

			desni_brat->br_k--;
		}
		else if (Brat(false, gornji)) {
			u = false;
			int n;														//otac->p[n] pokazuje na poziciju
			Cvor* otac = gornji->otac;
			Cvor* levi_brat = Brat(false, gornji, n);
			int razdvojni_kljuc = otac->K[n-1];								//razdvojni je indeks onog p koji pokazuje na pozicija
			int zajam = levi_brat->K[levi_brat->br_k-1];
			
			int* K0_tmp = new int[gornji->br_k];								//delete - uradio
			Cvor** p0_tmp = new Cvor * [gornji->br_k + 1];
			K0_tmp[0] = razdvojni_kljuc;
			p0_tmp[0] = levi_brat->p[levi_brat->br_k];
			if (gornji->K[0] == k) {
				p0_tmp[1] = p;
				for (int i = 1; i < gornji->br_k; i++) {
					K0_tmp[i] = gornji->K[i];
					p0_tmp[i + 1] = gornji->p[i + 1];
				}
			}
			else {
				for (int i = 1, m = 1, j = 0; i < gornji->br_k; i++) {
					if (gornji->K[i] != k) {
						K0_tmp[i] = gornji->K[i - m];
						p0_tmp[i + j] = gornji->p[i - m + j];
					}
					else {
						K0_tmp[i] = gornji->K[i - 1];
						p0_tmp[i] = gornji->p[i - 1];
						p0_tmp[i + 1] = p;
						m--;
						j++;
					}
				}
			}
			
			int* tmp0 = gornji->K;
			gornji->K = K0_tmp;
			K0_tmp = tmp0;
			delete[]K0_tmp;

			Cvor** tmp1 = gornji->p;
			gornji->p = p0_tmp;
			p0_tmp = tmp1;
			delete[]p0_tmp;

			otac->K[n - 1] = zajam;

			int* K1_tmp = new int[levi_brat->br_k - 1];								//delete - uradio
			Cvor** p1_tmp = new Cvor * [levi_brat->br_k];
			for (int i = 0; i < levi_brat->br_k - 1; i++) {
				K1_tmp[i] = levi_brat->K[i];
				p1_tmp[i] = levi_brat->p[i];
			}
			p1_tmp[levi_brat->br_k - 1] = levi_brat->p[levi_brat->br_k - 1];

			int* tmp2 = levi_brat->K;
			levi_brat->K = K1_tmp;
			K1_tmp = tmp2;
			delete[]K1_tmp;

			Cvor** tmp3 = levi_brat->p;
			levi_brat->p = p1_tmp;
			p1_tmp = tmp3;
			delete[]p1_tmp;

			levi_brat->br_k--;
		}
		else {
			int n;
			Cvor* brat = Brat_postoji(true, gornji, n);					//p[n]-> je pozicija, K[n] razdvaja
			if (brat != nullptr) {
				int br_k_novi = 2 * static_cast<int>(red / 2.0 + 0.5) - 2;
				int* K_tmp = new int[br_k_novi];									//delete - uradio
				Cvor* otac = gornji->otac;
				Cvor** p_tmp = new Cvor * [br_k_novi + 1];
				int j = 0;
				//int ip;
				for (int i = 0, m = 0; i < gornji->br_k; i++) {
					if (gornji->K[i] != k) {
						if (i == gornji->br_k - 1) K_tmp[j] = otac->K[n];
						else K_tmp[j] = gornji->K[i + m];
						p_tmp[j] = gornji->p[i + m];
						j++;
					}
					else {
						if (i == gornji->br_k - 1) K_tmp[j] = otac->K[n];
						else K_tmp[j] = gornji->K[i + 1];
						p_tmp[j] = p;
						j++;
						m++;
						//ip = i;
					}
				}
				p_tmp[gornji->br_k] = brat->p[0];
				//K_tmp[j] = otac->K[n];
				//p_tmp[j] = gornji->p[ip];
				//j++;
				for (int i = 0; i < brat->br_k; i++) {
					K_tmp[j] = brat->K[i];
					p_tmp[j + 1] = brat->p[i + 1];
					j++;
				}
				//p_tmp[j] = brat->p[brat->br_k];
				bool list = iteration ? false : true;
				Cvor* new_p = new Cvor(false, br_k_novi, otac, K_tmp);

				Cvor** p0_tmp = new_p->p;
				new_p->p = p_tmp;
				p_tmp = p0_tmp;
				delete[]p_tmp;

				Cvor* pomocna = gornji;
				delete pomocna;
				delete brat;

				for (int i = 0; i < new_p->br_k + 1; i++) new_p->p[i]->otac = new_p;

				k = otac->K[n];
				gornji = otac;
				p = new_p;
				//delete[] K_tmp;
				//Brisanje_iz_unutrasnjeg_cvora(otac->K[n], otac, p);		////////////////////////////////
			}
			else {
				brat = Brat_postoji(false, gornji, n);
				int br_k_novi = 2 * static_cast<int>(red / 2.0 + 0.5) - 2;
				int* K_tmp = new int[br_k_novi];									//delete - uradio
				Cvor* otac = gornji->otac;
				Cvor** p_tmp = new Cvor * [br_k_novi + 1];
				int j = 0;
				for (int i = 0; i < brat->br_k; i++) { 
					K_tmp[j] = brat->K[i];
					p_tmp[j] = brat->p[i];
					j++;
				}
				//K_tmp[j] = otac->K[n-1];
				//p_tmp[j] = brat->p[brat->br_k];
				//j++;
				for (int i = 0, m = 1, x = 0; i < gornji->br_k; i++) {
					if (gornji->K[i] != k) {
						if (i == 0) {
							K_tmp[j] = otac->K[n - 1];
							p_tmp[j] = brat->p[brat->br_k];
						}
						else {
							K_tmp[j] = gornji->K[i - m];
							p_tmp[j + x] = gornji->p[i - m + x];
						}
						j++;
					}
					else {
						if (i == 0) {
							K_tmp[j] = otac->K[n - 1];
							p_tmp[j] = brat->p[brat->br_k];
						}
						else {
							K_tmp[j] = gornji->K[i - 1];
							p_tmp[j] = gornji->p[i - 1];
						}
						p_tmp[j + 1] = p;
						j++;
						m--;
						x++;
					}
				}

				bool list = iteration ? false : true;
				Cvor* new_p = new Cvor(false, br_k_novi, otac, K_tmp);

				Cvor** p0_tmp = new_p->p;
				new_p->p = p_tmp;
				p_tmp = p0_tmp;
				delete[]p_tmp;

				Cvor* pomocna = gornji;
				delete pomocna;
				delete brat;

				for (int i = 0; i < new_p->br_k + 1; i++) new_p->p[i]->otac = new_p;

				k = otac->K[n - 1];
				gornji = otac;
				p = new_p;
				//delete[] K_tmp;
				//Brisanje_iz_unutrasnjeg_cvora(otac->K[n], otac, p);
			}
		}
	}
	iteration++;
}

Cvor* Bstablo::Brat(bool desni, Cvor* trenutni, int& razdvojni) {
	Cvor* otac = trenutni->otac;
	for (int i = 0; i < otac->br_k + 1; i++) {
		if (otac->p[i] == trenutni) {
			razdvojni = i;
			break;
		}
	}
	if (desni) {
		if (razdvojni < otac->br_k && otac->p[razdvojni + 1]->br_k - 1 >= static_cast<int>(red / 2.0 - 0.5)) return otac->p[razdvojni + 1];
		return nullptr;
	}
	else {
		if (razdvojni > 0 && otac->p[razdvojni - 1]->br_k - 1 >= static_cast<int>(red / 2.0 - 0.5)) return otac->p[razdvojni - 1];
		return nullptr;
	}
}

bool Bstablo::Brat(bool desni, Cvor* trenutni) {
	Cvor* otac = trenutni->otac;
	int n;
	for (int i = 0; i < otac->br_k + 1; i++) {
		if (otac->p[i] == trenutni) {
			n = i;
			break;
		}
	}
	if (desni) {
		if (n < otac->br_k && otac->p[n + 1]->br_k - 1 >= static_cast<int>(red / 2.0 - 0.5)) return true;
		return false;
	}
	else {
		if (n > 0 && otac->p[n - 1]->br_k - 1 >= static_cast<int>(red / 2.0 - 0.5)) return true;
		return false;
	}
}

Cvor* Bstablo::Brat_postoji(bool desni, Cvor* trenutni, int& razdvojni) {
	Cvor* otac = trenutni->otac;
	for (int i = 0; i < otac->br_k + 1; i++) {
		if (otac->p[i] == trenutni) {
			razdvojni = i;
			break;
		}
	}
	if (desni) {
		if (razdvojni < otac->br_k) return otac->p[razdvojni + 1];
		return nullptr;
	}
	else {
		if (razdvojni > 0) return otac->p[razdvojni - 1];
		return nullptr;
	}
}

void Promena_reda(Bstablo*& stablo) {
	if (stablo != nullptr) {
		int brk = stablo->Broj_kljuceva();
		int* niz = new int[brk];				//delete[]
		int x = 0;
		Cvor* tmp = stablo->Dohvati_koren();
		queue <Cvor*> red;
		red.push(tmp);
		while (!red.empty()) {
			tmp = red.front();
			red.pop();
			for (int i = 0; i < tmp->br_k; i++) {
				niz[x++] = tmp->K[i];
				if (tmp->p[i] != nullptr) red.push(tmp->p[i]);
			}
			if (tmp->p[tmp->br_k] != nullptr) red.push(tmp->p[tmp->br_k]);
		}
		/*cout << " Br. kljuceva je: " << brk << endl;
		for (int i = 0; i < brk; i++) {
			cout << " " << niz[i];
			if (i % 10 == 0 && i) cout << endl;
		}*/
		int novi_red;
		while (true) {
			cout << " Unesite novi red Bstabla u opsegu od 3 do 10: ";
			cin >> novi_red;
			cout << endl;
			if (novi_red >= 3 && novi_red <= 10) break;
			cout << " Vrednost nije validna, pokusajte ponovo. " << endl << endl;
		}
		Bstablo* novo = new Bstablo(novi_red);
		for (int i = 0; i < brk; i++) {
			novo->Umetanje(niz[i]);
		}
		Bstablo* tmp_s = stablo;
		stablo = novo;
		novo = tmp_s;
		delete novo;

		cout << " Red stabla je promenjen." << endl << endl;
	}
	else cout << " Stablo je prazno." << endl << endl;
}

int main() {
	Bstablo* stablo = nullptr;
	bool a = true;
	while (a) {
		int u;
		cout << "____________________________________________________________________________" << endl << endl;
		cout << " 0. Stvaranje objekta Bstabla\n 1. Odredjivanje visine stabla\n 2. Odredjivanje broja kljuceva u stablu\n 3. Pretraga kljuca\n 4. Umetanje kljuca u stablo\n 5. Brisanje kljuca iz stabla\n 6. Promena reda stabla\n 7. Ispisivanje sadrzaja stabla\n 8. Brisanje Bstabla\n 9. Zavrsetak programa\n\n Izaberite jednu od opcija navodeci njen broj: ";
		cin >> u;
		cout << endl;
		switch (u) {
		case 0: if (stablo!=nullptr) { cout << " Objekat Bstabla vec postoji." << endl; break; }
			  int m;
			  while(true) {
				  cout << " Unesite red Bstabla u opsegu od 3 do 10: ";
				  cin >> m;
				  cout << endl;
				  if (m >= 3 && m <= 10) break;
				  cout << " Vrednost nije validna, pokusajte ponovo. " << endl << endl;
			  }
			  stablo = new Bstablo(m);
			  break;
		case 1: if (stablo == nullptr) cout << " Stablo nije stvoreno." << endl << endl;
			  else cout << " Visina stabla je: " << stablo->Visina_stabla() << endl << endl;
			break;
		case 2: if (stablo == nullptr) cout << " Stablo nije stvoreno." << endl << endl;
			  else cout << " Broj kljuceva u stablu je: " << stablo->Broj_kljuceva() << endl << endl;
			break;
		case 3: if (stablo == nullptr) cout << " Stablo nije stvoreno." << endl << endl;
			  else {
			Cvor* p = nullptr;
			int k;
			cout << " Unesite kljuc koji se pretrazuje: ";
			cin >> k;
			cout << endl;
			if (stablo->Pretraga(k, p)) cout << " Kljuc " << k << " postoji u stablu." << endl << endl;
			else cout << " Kljuc " << k << " ne postoji u stablu." << endl << endl;
		}
			break;
		case 4: if (stablo == nullptr) cout << " Stablo nije stvoreno." << endl << endl;
			  else stablo->Pomocno_umetanje();
			break;
		case 5: if (stablo == nullptr) cout << " Stablo nije stvoreno." << endl << endl;
			  else {
			int k;
			cout << " Unesite kljuc koji se brise: ";
			cin >> k;
			cout << endl;
			if (stablo->Brisanje(k)) cout << " Kljuc " << k << " je uspesno izbrisan." << endl << endl;
			else cout<<" Kljuc " << k << " ne postoji u stablu." << endl << endl;
		}
			break;
		case 6: if (stablo == nullptr) cout << " Stablo nije stvoreno." << endl << endl;
			  else Promena_reda(stablo);
			break;
		case 7: if (stablo == nullptr) cout << " Stablo nije stvoreno." << endl << endl;
			  else stablo->Ispis_stabla();
			break;
		case 8: delete stablo;
			stablo = nullptr;
			cout << " Stablo je izbrisano." << endl;
			break;
		case 9: delete stablo;
			stablo = nullptr;
			a = false;
			break;
		default:cout << " Opcija koju ste izabrali ne postiji, pokusajte ponovo." << endl << endl;
		}
		cout << "____________________________________________________________________________" << endl << endl;
		system("pause");

	}
}