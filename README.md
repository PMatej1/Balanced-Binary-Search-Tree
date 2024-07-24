# Balansiranje BST

## Opis Projekta

Ovaj projekt se fokusira na ispravljanje potencijalnih problema balansa u binarnim stablima pretrage, koji mogu nastati prilikom umetanja ili brisanja elemenata. Cilj je osigurati da stablo ostane balansirano i da sve operacije pretrage, umetanja i brisanja ostanu efikasne.

## Opis Rješenja

Za rješavanje problema balansa, u strukturu čvora su dodani novi atributi:
- Broj elemenata lijevo ispod čvora
- Broj elemenata desno ispod čvora
- Visina čvora

Atributi za broj elemenata lijevo i desno se ažuriraju prilikom umetanja ili brisanja elemenata. Visina čvora se ažurira pomoću posebne funkcije.

### Balansiranje Stabla

Koristeći nekoliko pomoćnih funkcija, balansiranje se vrši na sledeći način:
1. Ako se pronađe čvor na visini od najmanje 3, i ako broj elemenata lijevo ili desno prelazi dvostruko, taj dio stabla se izdvaja u poseban vektor.
2. Elementi iz vektora se ponovo umeću u stablo rekurzivno koristeći srednji element kao korijen, čime se garantuje balans.


## Kako Pokrenuti

1. Preuzmite ili klonirajte repozitorij.
2. Kompajlirajte i pokrenite kod prema vašem razvojnom okruženju.
3. Koristite priložene testne slučajeve za verifikaciju ispravnosti funkcionalnosti.

## Autor

Matej Panić
