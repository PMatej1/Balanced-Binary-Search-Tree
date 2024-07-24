#ifndef STABLO_CPP
#define STABLO_CPP
#include "stablo.h"
#include <iostream>
using namespace std;
template <typename Tip>
Stablo<Tip>::Stablo() {
    korijen = nullptr;
    broj_elemenata = 0;
}

template <typename Tip>
pair<typename Stablo<Tip>::Cvor*,bool> Stablo<Tip>::Insert(Tip element, const bool treba_provjeravat_balans) {

  if(korijen == nullptr) {
    korijen = new Cvor(element);
    broj_elemenata = 1;
    return {korijen, true};
  }
  //prvo provjeravamo postoji taj element vec u stablu jer ako postoji visine i broj elemenata lijevo(desno) se nece mijenjat.
  Cvor *potencijalni_isti=Find(element), *trenutni = korijen, *prethodni = nullptr;
  if (potencijalni_isti!=nullptr) return {potencijalni_isti, false};
  else{
      while(trenutni != nullptr) {
        // nakon sto smo utvrdili da nemamo vec u stablu element koji bismo unijeli moramo ispraviti svakom elementu broj elemenata lijevo(desno) vrijeme.
        prethodni = trenutni;
        if(element < trenutni->element){
            trenutni->broj_elemenata_lijevo++;
            trenutni = trenutni->ld;
        }
        else {
            trenutni->broj_elemenata_desno++;
            trenutni = trenutni->dd;
        }
      }
  }
  broj_elemenata++;
  Cvor *novi = new Cvor(element,prethodni);
  if(element < prethodni->element)
    prethodni->ld = novi;
  else prethodni->dd = novi;
  //Nakon sto smo unijeli element moramo se vratiti istim putem kojim smo dosli do novog elementa da ispravimo eventualne promjene u visini.
  ObnoviVisine(novi);
  if(treba_provjeravat_balans){
      Cvor* potencijalni_neizbalansirani=korijen;
      //Nakon obnovljenih visina i promjena u broju elemenata lijevo(desno) prvi potencijalni problem u balansu se javlja u korijenu.
      //Ako nije problem u korijenu potencijalni problem u balansu se moze samo javiti u elementima kroz koje smo prolazili dok smo umetali element.
      while(potencijalni_neizbalansirani->visina>=3){
            if (!JeLiBalansirano(potencijalni_neizbalansirani)) {
                    cout<<" Nije izbalansirano, problem je uneseni element: "<<novi->element<<" neizbalansirani: "<<potencijalni_neizbalansirani->element
                    <<" broj elemenata lijevo: "<<potencijalni_neizbalansirani->broj_elemenata_lijevo<<" broj elemenata desno: "<<
                    potencijalni_neizbalansirani->broj_elemenata_desno<<" visina: "<<potencijalni_neizbalansirani->visina<<endl;
                     Balansiraj(potencijalni_neizbalansirani);
                     break;
                    }
            if (novi->element>potencijalni_neizbalansirani->element){
                potencijalni_neizbalansirani=potencijalni_neizbalansirani->dd;
        }
            else{
                potencijalni_neizbalansirani=potencijalni_neizbalansirani->ld;
            }
      }
  }
  return {novi,true};
}

template <typename Tip>
void Stablo<Tip>::InsertVector(const vector<Tip> &pomocni, int pocetak, int iza_kraja){
    // Rekurzivna funkcija koja nam sluzi da nakon sto prikupimo potrebne elemente i izbrisemo ih da ih vrati u stablo u odgovarajucem poretku.
    int sredina=pocetak+(iza_kraja-pocetak)/2;
    if (pocetak==sredina){
        Insert(pomocni[sredina], false);
        return;
    }
    else {
        Insert(pomocni[sredina], false);
        InsertVector(pomocni, pocetak, sredina);
        InsertVector(pomocni, sredina, iza_kraja);
    }

}

template <typename Tip>
typename Stablo<Tip>::Cvor* Stablo<Tip>::Find(Tip element) {
  Cvor* trenutni = korijen;
  while(trenutni != nullptr) {
    if(element == trenutni->element)
      return trenutni;
    if(element < trenutni->element)
      trenutni = trenutni->ld;
    else trenutni = trenutni->dd;
  }
  return nullptr;
}

template <typename Tip>
void Stablo<Tip>::InOrderRek(Cvor* cvor) {
  if(cvor==nullptr)
    return;
  InOrderRek(cvor->ld);
  cout<<cvor->element<<" ";
  InOrderRek(cvor->dd);
}

template <typename Tip>
vector<Tip> Stablo<Tip>::InOrder() {
  if (korijen==nullptr) throw "Stablo nema elemenata";
  // Funkcija InOrder nam umjesto ispisa sluzi za prikupljanje elemenata ispod korijena u vektor. Vrijeme O(n).
  vector<Tip> rezultat;
  Cvor* trenutni = NadjiNajmanji(korijen);
  while(trenutni != nullptr) {
    rezultat.push_back(trenutni->element);
    trenutni = NadjiSljedbenika(trenutni);
  }
  return rezultat;
}

template <typename Tip>
vector<Tip> Stablo<Tip>::PomocniInOrder(Cvor* pocetni){
  // Funkcija PomocniInOrder nam radi slicnu stvar kao i InOrder samo je jedina razlika to sto prikupljamo elemente ispod proslijedjenog elementa
  // ukljucujuci i njega. Vrijeme O(k) gdje nam k predstavlja broj elemenata ispod proslijedjenog elementa ukljucujuci i njega.
  vector <Tip> rezultat;
  if (pocetni==nullptr) throw "Nema trazenog elementa";
  if (pocetni->element==korijen->element) return InOrder();
  else if (pocetni->element<korijen->element){
    Cvor* pomocni=NadjiNajmanji(pocetni);
    while (pomocni!=pocetni->rod){
        rezultat.push_back(pomocni->element);
        pomocni=NadjiSljedbenika(pomocni);
    }
    }
    else {
        Cvor* pomocni=NadjiNajveci(pocetni);
        while (pomocni!=pocetni->rod){
        rezultat.push_back(pomocni->element);
        pomocni=NadjiPrethodnog(pomocni);
    }
    }

    return rezultat;
}

template <typename Tip>
typename Stablo<Tip>::Cvor* Stablo<Tip>::NadjiNajmanji(Stablo<Tip>::Cvor *cvor) {
  if(cvor == nullptr)
    return nullptr;
  Cvor *trenutni = cvor;
  while(trenutni->ld != nullptr)
    trenutni = trenutni->ld;
  return trenutni;
}
template <typename Tip>
typename Stablo<Tip>::Cvor* Stablo<Tip>::NadjiNajveci(Stablo<Tip>::Cvor *cvor) {
  if(cvor == nullptr)
    return nullptr;
  Cvor *trenutni = cvor;
  while(trenutni->dd != nullptr)
    trenutni = trenutni->dd;
  return trenutni;
}

template <typename Tip>
typename Stablo<Tip>::Cvor* Stablo<Tip>::NadjiSljedbenika(Stablo<Tip>::Cvor *cvor) {
  if(cvor->dd != nullptr)
    return NadjiNajmanji(cvor->dd);
  Cvor* trenutni = cvor;
  while(trenutni != korijen && trenutni == trenutni->rod->dd)
    trenutni = trenutni->rod;
  if(trenutni == korijen)
    return nullptr;
  else return trenutni->rod;
}
template <typename Tip>
typename Stablo<Tip>::Cvor* Stablo<Tip>::NadjiPrethodnog(Stablo<Tip>::Cvor *cvor){
    if (cvor->ld!=nullptr)
        return NadjiNajveci(cvor->ld);
    Cvor * trenutni= cvor;
    while (trenutni!=korijen&&trenutni==trenutni->rod->ld)
        trenutni=trenutni->rod;
    if (trenutni==korijen)
        return nullptr;
    else return trenutni->rod;


}

template <typename Tip>
int Stablo<Tip>::Erase(const Tip &element, bool balans) {
  Cvor *za_brisanje = korijen, *potencijalni=Find(element);
  //Ako nema elementa kojeg trebamo obrisati visine i broj elemenata lijevo(desno) se ne trebaju mijenjati.
  if (potencijalni==nullptr) throw "Nema elementa koji zelite brisati";
  else{
    //Nakon sto smo utvrdili da element stvarno postoji moramo ispraviti broj elemenata lijevo(desno).
    while(za_brisanje->element!=element){
        if (za_brisanje->element<element){
            za_brisanje->broj_elemenata_desno--;
            za_brisanje=za_brisanje->dd;
        }
        else {
           za_brisanje->broj_elemenata_lijevo--;
           za_brisanje=za_brisanje->ld;

        }
  }

  Erase(za_brisanje, balans);
  return 1;
  }
}
template <typename Tip>
void Stablo<Tip>::Erase(Cvor *za_brisanje, bool treba_provjeravat_balans) {
  if (za_brisanje==nullptr) throw "nema elementa za brisanje";
  Tip pomocni_za_brisanje=za_brisanje->element;
  if(za_brisanje->dd == nullptr) {  //ako nema desnog djeteta
    if(za_brisanje->ld == nullptr) { //ako nema ni lijevog
        if(za_brisanje->rod != nullptr) { //ako ima roditelja
            //Ako je element list onda se promjene u broju elemenata lijevo(desno) mijenjaju samo iznad njega.
            if(za_brisanje->rod->dd == za_brisanje){
                za_brisanje->rod->dd = nullptr;
                if (za_brisanje->rod->ld!=nullptr)
                za_brisanje->rod->visina=za_brisanje->rod->ld->visina+1; // jer roditelj nema vise desne djece visina mu je visina lijevih elemenata +1
                else  za_brisanje->rod->visina=0;
                ObnoviVisine(za_brisanje->rod);
            }
            else {
                za_brisanje->rod->ld = nullptr;
                if (za_brisanje->rod->dd!=nullptr)
                za_brisanje->rod->visina=za_brisanje->rod->dd->visina+1; // jer roditelj nema vise lijeve djece visina mu je visina desnih elemenata +1
                else za_brisanje->rod->visina=0;
                ObnoviVisine(za_brisanje->rod);
                }
        }
        else korijen = nullptr;         //ako nema roditelja, tada je korijen
    }
    else {  //ako ima lijevo dijete
        if(za_brisanje->rod != nullptr) { //ako ima roditelja
            if(za_brisanje->rod->dd == za_brisanje)
                za_brisanje->rod->dd = za_brisanje->ld;
            else za_brisanje->rod->ld = za_brisanje->ld;
            za_brisanje->ld->rod = za_brisanje->rod;
        }
        else { //ako nema roditelja, tada je korijen
            korijen = za_brisanje->ld;
            za_brisanje->ld->rod = nullptr;
        }
        ObnoviVisine(za_brisanje);  // jer visina onog koji je preuzeo mjesto ovog za brisanje je ista kao i prije samo se mijenja za njegove roditelje
    }
  }
  else {
    Cvor* sljedbenik = NadjiSljedbenika(za_brisanje);
    if(sljedbenik->rod == za_brisanje) {
        sljedbenik->rod = za_brisanje->rod;
        sljedbenik->broj_elemenata_lijevo=za_brisanje->broj_elemenata_lijevo; // jer se desno dijete preusmjerava na staro mjesto
        if(za_brisanje->rod != nullptr) {                                     // i preuzima broj elemenata lijevo a njegov broj elemenata desno ostaje isti
            if(za_brisanje->rod->dd == za_brisanje)
                za_brisanje->rod->dd = sljedbenik;
            else za_brisanje->rod->ld = sljedbenik;
        }
        else korijen = sljedbenik;
        if(za_brisanje->ld != nullptr){
            za_brisanje->ld->rod = sljedbenik;
            sljedbenik->visina=max(za_brisanje->ld->visina+1,za_brisanje->dd->visina);
        }

        sljedbenik->ld = za_brisanje->ld;
        ObnoviVisine(sljedbenik);

    }

    else {
       sljedbenik->rod->ld = sljedbenik->dd;
       sljedbenik->broj_elemenata_lijevo=za_brisanje->broj_elemenata_lijevo;
       sljedbenik->broj_elemenata_desno=za_brisanje->broj_elemenata_desno-1;

       if(sljedbenik->dd != nullptr){
         sljedbenik->dd->rod = sljedbenik->rod;
         ObnoviVisine(sljedbenik->dd);
        Cvor * trenutni=za_brisanje->dd;
        while(trenutni->ld!=nullptr){  //jer smo vec promijenili cvor na kojem je bio sljedbenik
            trenutni->broj_elemenata_lijevo--;
            trenutni=trenutni->ld;
       }
       }
       else {
            Cvor * trenutni=za_brisanje->dd;
            while(trenutni->ld!=nullptr){  // sljedbenik nije prvo desno dijete, pa se svakom elementu vecem od sljedbenika mijenja broj elemenata lijevo
                trenutni->broj_elemenata_lijevo--;
                trenutni=trenutni->ld;
            }
       trenutni->visina=0;
       ObnoviVisine(trenutni);  // jer sljedbenik nije imao desne djece pa visina njegovog roditelja postaje 0
       }

       sljedbenik->dd = za_brisanje->dd;
       za_brisanje->dd->rod = sljedbenik;
       sljedbenik->ld = za_brisanje->ld;
       if(za_brisanje->ld != nullptr)
         za_brisanje->ld->rod = sljedbenik;
       sljedbenik->rod = za_brisanje->rod;
       if(za_brisanje->rod != nullptr) { //ako nije korijen
          if(za_brisanje->rod->ld == za_brisanje)
            za_brisanje->rod->ld = sljedbenik;
          else za_brisanje->rod->dd = sljedbenik;
       }
       // ako je korijen
       else korijen = sljedbenik;
    }
  }
  delete za_brisanje;
  broj_elemenata--;
  if(treba_provjeravat_balans){
      Cvor* potencijalni_neizbalansirani=korijen;
      while(potencijalni_neizbalansirani->visina>=3){
            if (!JeLiBalansirano(potencijalni_neizbalansirani)) {
                    cout<<" nije balansirano , problem je obrisani element: "<<pomocni_za_brisanje<<" neizbalansirani: "<<potencijalni_neizbalansirani->element
                    <<" broj elemenata lijevo: "<<potencijalni_neizbalansirani->broj_elemenata_lijevo<<" broj elemenata desno: "<<
                    potencijalni_neizbalansirani->broj_elemenata_desno<<" visina: "<<potencijalni_neizbalansirani->visina<<endl;
                    Balansiraj(potencijalni_neizbalansirani);
                    break;
            }
            if (pomocni_za_brisanje>potencijalni_neizbalansirani->element){
                potencijalni_neizbalansirani=potencijalni_neizbalansirani->dd;
            }
            else{
                potencijalni_neizbalansirani=potencijalni_neizbalansirani->ld;
            }
      }
  }

}


template<typename Tip>
void Stablo<Tip>::Izbrisi_Od_Datog_Elementa(Cvor *pocetni){
    if (pocetni==nullptr) throw "nema elementa od kojeg hocemo da brisemo";
    // Ova funkcija nam sluzi da nakon sto prikupimo u vektor sve elemente ispod elementa koji nije izbalansiran obrisemo te elemente da bismo
    // ih mogli ponovo unijeti u odgovarajucem poretku. vrijeme O(k*logn) gdje je k broj elemenata koji zelimo izbrisati.
    if (pocetni->element<=korijen->element){
        // Ova funkcija radi na za elemente manje i jednake korijenu jer uzimamo najmanji element ispod datog elementa i
        // idemo do njegovog roditelja preko njegovih sjedbenika (ako je korijen roditelj je nullptr pa nam funkcija radi u ovom slucaju)
        Cvor* trenutni=NadjiNajmanji(korijen), *pomocni=trenutni;
        while(trenutni!=pocetni->rod){
            pomocni=NadjiSljedbenika(trenutni);
            Erase(trenutni->element, false);
            trenutni=pomocni;
        }
    }
    else {
        //ovdje uzimamo najveci element ispod datog elementa i preko njegovih prethodnika idemo do njegovog roditelja
        Cvor* trenutni=NadjiNajveci(korijen), *pomocni=trenutni;
        while(trenutni!=pocetni->rod){
            pomocni=NadjiPrethodnog(trenutni);
            Erase(trenutni->element, false);
            trenutni=pomocni;
        }

    }

}
template <typename Tip>
void Stablo<Tip>::ObnoviVisine(Cvor * pomocni){
    // ova funkcija radi tako sto se vratimo istim putem kojim smo dosli do elementa nakon umetanja ili brisanja tako sto eventuane
    //promjene u visini prepravimo tako sto uzmemo maksimum izmedju visine njegove desne i lijeve djece + 1.
    if (pomocni==nullptr) throw "visine";
    Cvor * trenutni=pomocni->rod;
    while (trenutni!=nullptr){
        if (trenutni->ld==nullptr) trenutni->visina=trenutni->dd->visina+1;
        else if (trenutni->dd==nullptr) trenutni->visina=trenutni->ld->visina+1;
        else trenutni->visina=max(trenutni->ld->visina, trenutni->dd->visina)+1;
        trenutni=trenutni->rod;

    }


}

template<typename Tip>
bool Stablo<Tip>::JeLiBalansirano(Cvor* trenutni){
    if (trenutni->visina<3) return true;
    else{
        if (trenutni->broj_elemenata_desno>=2*trenutni->broj_elemenata_lijevo) return false;
        else if (trenutni->broj_elemenata_lijevo>=2*trenutni->broj_elemenata_desno) return false;
    }
    return true;
}

template <typename Tip>
void Stablo<Tip>::Balansiraj(Cvor * pocetni){
    if (pocetni==nullptr) throw "nema elemenata za balansiranje";
    else if (JeLiBalansirano(pocetni)) throw "ovaj dio stabla je vec balansiran";
    cout<<"zapocinjem balansiranje"<<endl;
    vector <Tip> pomocni=PomocniInOrder(pocetni);
    Izbrisi_Od_Datog_Elementa(pocetni);
    InsertVector(pomocni, 0, pomocni.size());
    cout<<"stablo nakon balansiranja: "<<endl;
    Ispisi();
    // Moze raditi i bez funkcije Ispisi(), ali to je samo za laksu provjeru.

}
template<typename Tip>
void Stablo<Tip>::Ispisi(){
    if (korijen==nullptr) throw "nema elemenata za ispisati";
    Cvor* trenutni=NadjiNajmanji(korijen);
    while (trenutni!=nullptr){
       cout<<"element: "<<trenutni->element<<" lijevo: "<<trenutni->broj_elemenata_lijevo<<" desno: "
       <<trenutni->broj_elemenata_desno<<" visina: "<<trenutni->visina;
        if (trenutni->ld!=nullptr) cout<<" lijevo dijete: "<<trenutni->ld->element;
        else cout<<" nema lijevog djeteta ";
        if (trenutni->dd!=nullptr) cout<<" desno dijete: "<<trenutni->dd->element;
        else cout<<" nema desnog djeteta ";
        if (trenutni!=korijen) cout<<" roditelj: "<<trenutni->rod->element<<endl;
        else cout<<" ovaj element je korijen stabla"<<endl;
        trenutni=NadjiSljedbenika(trenutni);
}
}


#endif // STABLO_CPP
