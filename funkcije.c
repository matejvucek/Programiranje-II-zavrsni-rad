#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "header.h"

int brojacKvizova = 0;

int ucitajPitanja(Pitanje pitanja[], const char* imeDatoteke) {
    FILE* fp = fopen(imeDatoteke, "r");
    if (!fp) {
        perror("Greska pri otvaranju pitanja.txt");
        return 0;
    }

    int count = 0;
    while (count < MAX_PITANJA &&
        fgets(pitanja[count].pitanje, sizeof(pitanja[count].pitanje), fp) &&
        fgets(pitanja[count].opcije[0], sizeof(pitanja[count].opcije[0]), fp) &&
        fgets(pitanja[count].opcije[1], sizeof(pitanja[count].opcije[1]), fp) &&
        fgets(pitanja[count].opcije[2], sizeof(pitanja[count].opcije[2]), fp)) {

        char linija[10];
        if (!fgets(linija, sizeof(linija), fp)) break;
        pitanja[count].tocni = toupper(linija[0]);

        pitanja[count].pitanje[strcspn(pitanja[count].pitanje, "\r\n")] = 0;
        for (int i = 0; i < 3; i++)
            pitanja[count].opcije[i][strcspn(pitanja[count].opcije[i], "\r\n")] = 0;

        count++;
    }

    fclose(fp);
    return count;
}

void odaberiNasumicna(int indeks[], int max) {
    int oznake[MAX_PITANJA] = { 0 }, brojac = 0;
    while (brojac < ODABRANA_PITANJA) {
        int rnd = rand() % max;
        if (!oznake[rnd]) {
            indeks[brojac++] = rnd;
            oznake[rnd] = 1;
        }
    }
}

int postojiKorisnik(const char* ime) {
    FILE* f = fopen("data/korisnici.txt", "r");
    if (!f) return 0;

    char linija[50];
    while (fgets(linija, sizeof(linija), f)) {
        linija[strcspn(linija, "\r\n")] = 0;
        if (strcmp(linija, ime) == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void dodajKorisnika(const char* ime) {
    if (!ime || strlen(ime) == 0) {
        printf("Neispravno ime korisnika.\n");
        return;
    }

    if (postojiKorisnik(ime)) {
        printf("Korisnik '%s' vec postoji.\n", ime);
        return;
    }

    FILE* f = fopen("data/korisnici.txt", "a");
    if (!f) {
        perror("Greska pri otvaranju korisnici.txt za pisanje");
        return;
    }

    fprintf(f, "%s\n", ime);
    fclose(f);

    printf("Korisnik '%s' uspjesno dodan.\n", ime);
}

void obrisiKorisnika(const char* ime) {
    FILE* f = fopen("data/korisnici.txt", "r");
    if (!f) { perror("Greska pri otvaranju korisnici.txt"); return; }

    FILE* temp = fopen("data/temp.txt", "w");
    if (!temp) { perror("Greska pri otvaranju temp.txt"); fclose(f); return; }

    char linija[50];
    int pronadjen = 0;

    while (fgets(linija, sizeof(linija), f)) {
        linija[strcspn(linija, "\r\n")] = 0;
        if (strcmp(linija, ime) != 0) {
            fprintf(temp, "%s\n", linija);
        }
        else {
            pronadjen = 1;
        }
    }

    fclose(f); fclose(temp);
    remove("data/korisnici.txt");
    if (rename("data/temp.txt", "data/korisnici.txt") != 0) {
        perror("Greska pri preimenovanju datoteke");
    }

    if (pronadjen)
        printf("Korisnik '%s' obrisan.\n", ime);
    else
        printf("Korisnik '%s' nije pronaden.\n", ime);
}

void azurirajKorisnika(const char* staroIme, const char* novoIme) {
    FILE* f = fopen("data/korisnici.txt", "r");
    if (!f) { perror("Greska pri otvaranju korisnici.txt"); return; }
    FILE* temp = fopen("data/temp.txt", "w");
    if (!temp) { perror("Greska pri otvaranju temp.txt"); fclose(f); return; }

    char linija[50];
    int pronadjen = 0;

    while (fgets(linija, sizeof(linija), f)) {
        linija[strcspn(linija, "\r\n")] = 0;
        if (strcmp(linija, staroIme) == 0) {
            fprintf(temp, "%s\n", novoIme);
            pronadjen = 1;
        }
        else {
            fprintf(temp, "%s\n", linija);
        }
    }

    fclose(f); fclose(temp);
    remove("data/korisnici.txt");
    if (rename("data/temp.txt", "data/korisnici.txt") != 0) {
        perror("Greska pri preimenovanju datoteke");
    }

    if (pronadjen) printf("Ime korisnika promijenjeno.\n");
    else printf("Korisnik '%s' nije pronaden.\n", staroIme);
}

// ======= SORTIRANJE I ISPIS KORISNIKA =======
int usporediKorisnike(const void* a, const void* b) {
    const char* ka = *(const char**)a;
    const char* kb = *(const char**)b;
    return strcmp(ka, kb);
}

void prikaziKorisnike() {
    FILE* f = fopen("data/korisnici.txt", "r");
    if (!f) { perror("Greska pri otvaranju korisnici.txt"); return; }

    char** korisnici = NULL;
    int n = 0;
    char linija[50];

    while (fgets(linija, sizeof(linija), f)) {
        linija[strcspn(linija, "\r\n")] = 0;

        char** tmp = realloc(korisnici, (n + 1) * sizeof(char*));
        if (!tmp) {
            perror("Greska realloc");
            for (int i = 0; i < n; i++) free(korisnici[i]);
            free(korisnici);
            fclose(f);
            return;
        }
        korisnici = tmp;

        korisnici[n] = malloc(strlen(linija) + 1);
        if (!korisnici[n]) {
            perror("Greska malloc");
            for (int i = 0; i < n; i++) free(korisnici[i]);
            free(korisnici);
            fclose(f);
            return;
        }
        strcpy(korisnici[n], linija);
        n++;
    }
    fclose(f);

    if (n == 0) { printf("Nema korisnika.\n"); return; }

    qsort(korisnici, n, sizeof(char*), usporediKorisnike);

    printf("\n--- Korisnici (sortirani) ---\n");
    for (int i = 0; i < n; i++) {
        printf("%s\n", korisnici[i]);
        free(korisnici[i]);
    }
    free(korisnici);
}

void traziKorisnika(const char* ime) {
    if (!ime || strlen(ime) == 0) {
        printf("Neispravno ime za pretragu.\n");
        return;
    }

    FILE* f = fopen("data/korisnici.txt", "r");
    if (!f) { perror("Greska pri otvaranju data/korisnici.txt"); return; }

    char linija[50];
    int pronadjen = 0;
    while (fgets(linija, sizeof(linija), f)) {
        linija[strcspn(linija, "\r\n")] = 0;
        if (strcmp(linija, ime) == 0) {
            pronadjen = 1;
            break;
        }
    }
    fclose(f);

    if (pronadjen)
        printf("Korisnik '%s' pronaden.\n", ime);
    else
        printf("Korisnik '%s' nije pronaden.\n", ime);
}

void pokreniKviz(Pitanje pitanja[], int brojPitanja, const char* imeIgraca) {
    brojacKvizova++;

    int odabrani[ODABRANA_PITANJA], bodovi = 0;
    odaberiNasumicna(odabrani, brojPitanja);
    char odgovor;
    FILE* rez = fopen("data/rezultati.txt", "a");
    if (!rez) { perror("Greska pri otvaranju rezultati.txt"); return; }

    fprintf(rez, "=== Kviz igraca: %s ===\n", imeIgraca);

    for (int i = 0; i < ODABRANA_PITANJA; i++) {
        int idx = odabrani[i];
        printf("\n%d. %s\n%s\n%s\n%s\n", i + 1, pitanja[idx].pitanje,
            pitanja[idx].opcije[0], pitanja[idx].opcije[1], pitanja[idx].opcije[2]);

        do {
            printf("Odgovor (A, B, C): ");
            if (scanf(" %c", &odgovor) != 1) {
                printf("Greska pri unosu.\n");
                fclose(rez);
                return;
            }
            odgovor = toupper(odgovor);
        } while (odgovor != 'A' && odgovor != 'B' && odgovor != 'C');

        if (odgovor == pitanja[idx].tocni) bodovi++;

        fprintf(rez, "%d. %s | Vas odgovor: %c | Točno: %c | %s\n",
            i + 1, pitanja[idx].pitanje, odgovor, pitanja[idx].tocni,
            (odgovor == pitanja[idx].tocni) ? "Tocno" : "Netocno");
    }

    float prosjek = (float)bodovi / ODABRANA_PITANJA;
    printf("\nOsvojeni bodovi: %d/%d\n", bodovi, ODABRANA_PITANJA);

    fprintf(rez, "Rezultat: %d/%d za %s (prosjek: %.2f)\n\n",
        bodovi, ODABRANA_PITANJA, imeIgraca, prosjek);
    fclose(rez);
}

void pokaziRezultate(const char* imeKorisnika) {
    FILE* f = fopen("data/rezultati.txt", "r");
    if (!f) { perror("Greska pri otvaranju rezultati.txt"); return; }

    char linija[512];
    int pronadjen = 0;

    printf("\n--- Rezultati za korisnika: %s ---\n", imeKorisnika);

    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, "Rezultat:") && strstr(linija, imeKorisnika)) {
            printf("%s", linija);
            pronadjen = 1;
        }
    }

    if (!pronadjen) {
        printf("Nema rezultata za korisnika '%s'.\n", imeKorisnika);
    }

    fclose(f);
}

// tablica rezultata
typedef struct {
    char ime[50];
    int bodovi;
    int maxBodovi;
    float prosjek;
} Rezultat;

int usporediRezultate(const void* a, const void* b) {
    const Rezultat* r1 = (const Rezultat*)a;
    const Rezultat* r2 = (const Rezultat*)b;

    if (r2->bodovi != r1->bodovi)
        return r2->bodovi - r1->bodovi;
    else
        return (r2->prosjek > r1->prosjek) - (r2->prosjek < r1->prosjek);
}

void tablicaRezultata() {
    FILE* f = fopen("data/rezultati.txt", "r");
    if (!f) { perror("Greska pri otvaranju rezultati.txt"); return; }

    Rezultat* rezultati = NULL;
    int n = 0;
    char linija[256];

    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, "Rezultat:")) {
            Rezultat r;
            if (sscanf(linija, "Rezultat: %d/%d za %49s (prosjek: %f)",
                &r.bodovi, &r.maxBodovi, r.ime, &r.prosjek) == 4) {
                Rezultat* tmp = realloc(rezultati, (n + 1) * sizeof(Rezultat));
                if (!tmp) {
                    perror("Greska realloc");
                    free(rezultati);
                    fclose(f);
                    return;
                }
                rezultati = tmp;
                rezultati[n++] = r;
            }
        }
    }
    fclose(f);

    if (n == 0) {
        printf("Nema rezultata za prikaz.\n");
        free(rezultati);
        return;
    }

    qsort(rezultati, n, sizeof(Rezultat), usporediRezultate);

    printf("\n=== TABLICA REZULTATA ===\n");
    for (int i = 0; i < n; i++) {
        printf("%2d. %-20s %d/%d (prosjek: %.2f)\n",
            i + 1, rezultati[i].ime, rezultati[i].bodovi,
            rezultati[i].maxBodovi, rezultati[i].prosjek);
    }

    free(rezultati);
}

