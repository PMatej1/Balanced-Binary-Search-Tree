#ifndef STABLO_H
#define STABLO_H
#include <utility>
#include <vector>
#include <iostream>
using namespace std;

template <typename Tip>
class Stablo {
  public:
  struct Cvor {
      int broj_elemenata_lijevo, broj_elemenata_desno, visina;
      Tip element;
      Cvor *ld, *dd, *rod;
      Cvor(Tip el, Cvor* rod = nullptr, Cvor* ld = nullptr, Cvor* dd = nullptr):
          element(el), rod(rod), ld(ld), dd(dd),broj_elemenata_lijevo(0), broj_elemenata_desno(0), visina(0)  {}
          // Dodani su dodatni atributi strukture cvor (broj elemenata lijevo i desno te visina cvora,
          // koji se prilikom unosa i brisanja update-uju da bi se lakse moglo provjeriti ima li problema u balansu stabla.
  };
  private:
  int broj_elemenata;
  Cvor* korijen;
  void InOrderRek(Cvor*);
  Cvor* NadjiNajmanji(Cvor*);
  Cvor* NadjiNajveci(Cvor *);
  Cvor* NadjiSljedbenika(Cvor *cvor);
  Cvor* NadjiPrethodnog(Cvor *cvor);
  public:
  int dajKorijen() const {return korijen->element;}
  int dajDesnoDijete() const {return korijen->dd->element;}
  int dajLijevoDijete() const {return korijen->ld->element;}
  int dajBrojElemenataDesno(Tip element) const {return Find(element)->broj_elemenata_desno; }
  int dajBrojElemenataLijevo (Tip element) const {return Find(element)->broj_elemenata_lijevo; }
  int dajVisinu(Tip element) const {return Find(element)->visina;}
  int dajNajveci(Tip element) const {return NadjiNajveci(Find(element))->element;}
  int dajNajmanji(Tip element) const {return NadjiNajmanji(Find(element))->element;}
  Stablo();
  pair<Cvor*,bool> Insert(Tip element,const bool treba_provjeravat_balans=true);
  void InsertVector (const vector<Tip> & pomocni, int pocetak, int iza_kraja);
  Cvor* Find(Tip element);
  void InOrderRek() { InOrderRek(korijen);}
  vector<Tip> InOrder();
  vector<Tip> PomocniInOrder(Cvor* trenutni);
  void Erase(Cvor* za_brisanje, bool treba_provjeravat_balans=true);
  int Erase(const Tip &element,const bool treba_provjeravat_balans=true);
  void Izbrisi_Od_Datog_Elementa(Cvor *);
  bool JeLiBalansirano(Cvor *);
  void ObnoviVisine(Cvor * trenutni);
  void Balansiraj(Cvor *pocetni);
  void Ispisi();

};

#include "stablo.cpp"

#endif // STABLO_H
