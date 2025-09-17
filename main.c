#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"

int main() {
    srand((unsigned int)time(NULL));

    Pitanje pitanja[MAX_PITANJA];
    int broj = ucitajPitanja(pitanja, "data/pitanja.txt");
    if (broj == 0) {
        printf("Greška pri učitavanju pitanja.\n");
        return 1;
    }

    int izbor;
    char ime[50], novoIme[50];

    do {
        printf("\n=== KVIZ O HRVATSKOJ POVIJESTI ===\n");
        printf("1. Dodaj korisnika\n");
        printf("2. Pokreni kviz\n");
        printf("3. Obrisi korisnika\n");
        printf("4. Prikazi korisnike\n");
        printf("5. Azuriraj korisnika\n");
        printf("6. Trazi korisnika\n");
        printf("7. Prikazi rezultate korisnika\n");
        printf("8. Tablica rezultata\n");
        printf("9. Izlaz\n");
        printf("Izbor: ");
        if (scanf("%d", &izbor) != 1) {
            printf("Greška pri unosu.\n");
            return 1;
        }
        getchar(); // proguta Enter

        switch ((IzbornikOpcije)izbor) {
        case DODAJ_KORISNIKA:
            printf("Unesite ime: ");
            fgets(ime, sizeof(ime), stdin);
            ime[strcspn(ime, "\n")] = 0;
            dodajKorisnika(ime);
            break;

        case POKRENI_KVIZ:
            printf("Unesite ime: ");
            fgets(ime, sizeof(ime), stdin);
            ime[strcspn(ime, "\n")] = 0;
            if (!postojiKorisnik(ime)) {
                printf("Korisnik ne postoji.\n");
                break;
            }
            pokreniKviz(pitanja, broj, ime);
            break;

        case OBRISI_KORISNIKA:
            printf("Unesite ime: ");
            fgets(ime, sizeof(ime), stdin);
            ime[strcspn(ime, "\n")] = 0;
            obrisiKorisnika(ime);
            break;

        case PRIKAZI_KORISNIKE:
            prikaziKorisnike();
            break;

        case AZURIRAJ_KORISNIKA:
            printf("Unesite staro ime: ");
            fgets(ime, sizeof(ime), stdin);
            ime[strcspn(ime, "\n")] = 0;

            printf("Unesite novo ime: ");
            fgets(novoIme, sizeof(novoIme), stdin);
            novoIme[strcspn(novoIme, "\n")] = 0;

            azurirajKorisnika(ime, novoIme);
            break;

        case TRAZI_KORISNIKA:
            printf("Unesite ime: ");
            fgets(ime, sizeof(ime), stdin);
            ime[strcspn(ime, "\n")] = 0;
            traziKorisnika(ime);
            break;

        case POKAZI_REZULTATE:
            printf("Unesite ime: ");
            fgets(ime, sizeof(ime), stdin);
            ime[strcspn(ime, "\n")] = 0;
            pokaziRezultate(ime);
            break;

        case TABLICA_REZULTATA:
            tablicaRezultata();
            break;

        case IZLAZ:
            printf("Izlaz iz programa.\n");
            break;

        default:
            printf("Nepoznata opcija.\n");
        }

    } while (izbor != IZLAZ);

    return 0;
}
