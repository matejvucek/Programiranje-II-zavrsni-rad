#ifndef HEADER_H
#define HEADER_H

#define MAX_PITANJA 100
#define ODABRANA_PITANJA 5

typedef struct {
    char pitanje[256];
    char opcije[3][100];
    char tocni;
} Pitanje;

typedef enum {
    DODAJ_KORISNIKA = 1,
    POKRENI_KVIZ,
    OBRISI_KORISNIKA,
    PRIKAZI_KORISNIKE,
    AZURIRAJ_KORISNIKA,
    TRAZI_KORISNIKA,
    POKAZI_REZULTATE,
    TABLICA_REZULTATA,
    IZLAZ
} IzbornikOpcije;

int ucitajPitanja(Pitanje pitanja[], const char* imeDatoteke);
void odaberiNasumicna(int indeks[], int max);

int postojiKorisnik(const char* ime);
void dodajKorisnika(const char* ime);
void obrisiKorisnika(const char* ime);
void azurirajKorisnika(const char* staroIme, const char* novoIme);
void prikaziKorisnike();
void traziKorisnika(const char* ime);

void pokreniKviz(Pitanje pitanja[], int brojPitanja, const char* imeIgraca);
void pokaziRezultate(const char* imeKorisnika);
void tablicaRezultata();

#endif

