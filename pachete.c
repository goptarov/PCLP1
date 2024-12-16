#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int id;
    char*nume;
}cartier;

typedef struct {
    int id;
    int adresa[18]; //adresa (reprezentata binar)
    int idCartier;
    int strada;
    int numar;
    int prioritate; //valoare de la 1 la 5. 5 fiind prioritatea maxima.
    float greutate;
    char* mesaj; //mesajul se termina cu '\n'
    int codificare;
}pachet;

typedef struct{
    int id; //intreg din [0, 31] (maxim 32 de postasi)
    int nrPachete; //intreg din [0, 49] (maxim 50 de pachete)
    pachet* pachete; //vector cu pachetele pe care le are de distrubuit
}postas;

//Postas sef(redistribuie pachetele postasilor simpli)

//Cerinta I 1.
void input_cartiere_pachete(int *nrC, int *nrP, cartier** cart, pachet** pac) {

    char numetmp[20], mesajtmp[100];

    //getc(stdin);
    scanf("%d", nrC); //Citim numarul de cartiere si de postasi
    *cart = malloc (sizeof(cartier) * (*nrC));

    for (int i = 0; i < *nrC; i++) {
        (*cart)[i].id = i;
        scanf("%s", numetmp); //Folosim un sir de caractere temporar pentru a stii
        //exact cata memorie sa ii alocam numelui cartierului.
        (*cart)[i].nume = malloc(sizeof(char) * (strlen(numetmp)+1));
        strcpy((*cart)[i].nume, numetmp);
    }

    scanf("%d", nrP); //Citim numarul de pachete
    *pac = malloc (sizeof(pachet) * (*nrP));

    for (int i = 0; i < *nrP; i++) {
        (*pac)[i].id = i;
        for (int j = 0; j < 18; j++) {
            scanf("%d", &(*pac)[i].adresa[j]);
        }
        scanf("%d", &(*pac)[i].prioritate);
        scanf("%f", &(*pac)[i].greutate);
        getc(stdin); //Citim un caracter care ne bloca citirea urmatoare
        fgets(mesajtmp, sizeof(mesajtmp), stdin); //Folosim un sir de caractere temporar pentru a stii
                                                     //exact cata memorie sa ii alocam mesajului.
        (*pac)[i].mesaj = malloc(sizeof(char) * (strlen(mesajtmp)+1));
        strcpy((*pac)[i].mesaj, mesajtmp);
    }
}

//Cerinta I 2.
void pachet_info(pachet* pac) {

    pac->idCartier=0;
    pac->strada=0;
    pac->numar=0;

    for (int i = 0; i < 18; i++) {
        if (i <= 4) {
            pac->idCartier += (pac->adresa[i]) * pow(2, 4-i);
        }
        if (i > 4 && i <= 9) {
            pac->strada += (pac->adresa[i]) * pow(2, 9-i);
        }
        if (i > 9) {
            pac->numar += (pac->adresa[i]) * pow(2, 17-i);
        }
    }
    /*
    printf("%d \n", pac->idCartier);
    printf("%d \n", pac->strada);
    printf("%d ", pac->numar);
    */
}

//Cerinta I 3.
void distribuire_pachete(pachet* pac, postas** post, int nrP, int nrC) {

    for (int i = 0; i < nrC; i++) {
        (*post)[i].id = i;
        (*post)[i].nrPachete = 0;
        (*post)[i].pachete = '\0';

        for (int j = 0; j < nrP; j++){
            if ((*post)[i].id == pac[j].idCartier) {

                (*post)[i].pachete = realloc((*post)[i].pachete, sizeof(pachet)*((*post)[i].nrPachete+1));//Alocam memorie pentru pachet de fiecare data cand postasul primeste unul.
                (*post)[i].pachete[(*post)[i].nrPachete] = pac[j];
                (*post)[i].nrPachete++;
            }

        }
    }
    //Sunt destul de sigur ca merge bine chestia asta
}

//Cerinta I 4.
void ordonare_pachete(pachet* pac, int nrP) {
    int temp;
    for (int i = 0; i < nrP-1; i++) {
        if (pac[i].prioritate > pac[i+1].prioritate) {
            temp = pac[i].prioritate;
            pac[i].prioritate = pac[i+1].prioritate;
            pac[i+1].prioritate = temp;
        }
    }
    for (int i = 0; i < nrP-1; i++) {
        if (pac[i].prioritate == pac[i+1].prioritate && pac[i].greutate <= pac[i+1].greutate) {
            temp = pac[i].greutate;
            pac[i].greutate = pac[i+1].greutate;
            pac[i+1].greutate = temp;
        }
    }
    //Si asta cred ca e ok(asta nici nu e testata)
}

//Cerinta I 5.
void codificare_mesaj(char* mesaj){

    mesaj[strlen(mesaj)-1] = '\0';
    char* p = strtok(mesaj," ,.;:?!");
    char a[100][100];

    int cnt=0;
    while (p != NULL) {
      strcpy(a[cnt], p);
      p = strtok(NULL, " ,.;:?!");
      cnt++;
    }
    mesaj[0]='\0';
    for (int i = cnt-1; i >= 0; i--) {
      strcat(mesaj, a[i]);
    }
    //test: printf("%s\n",mesaj);
}

void cryptare_mesaj(){}

int main(void) {
    int nrC, nrP, nrCerinta;
    cartier* cart;
    pachet* pac;
    postas* postas;

    scanf("%d", &nrCerinta);
    input_cartiere_pachete(&nrC, &nrP, &cart, &pac);

    if (nrCerinta == 1) {
        for (int i = 0; i < nrC; i++) {
            printf("%d %s\n", i, cart[i].nume);
        }
        for (int i = 0; i < nrP; i++) {
            printf("%d\n", pac[i].id);

            for (int j = 0; j < 18; j++) {
                if (j == 17) {
                    printf("%d", pac[i].adresa[j]);
                    break;
                }
                printf("%d ", pac[i].adresa[j]);
            }
            printf("\n");
            printf("%d %.3f\n", pac[i].prioritate, pac[i].greutate);
            printf("%s", pac[i].mesaj);
        }
    }

    for (int i = 0; i < nrP; i++) { //for pentru a calcula la toate cartierele(altfel nu merg functiile urmatoare)
        pachet_info(&pac[i]);
    }

    if (nrCerinta == 2) {
        for (int i = 0; i < nrP; i++) {
            printf("%d\n", pac[i].id);
            printf("%d %d %d\n", pac[i].idCartier, pac[i].strada, pac[i].numar);
        }
    }

    distribuire_pachete(pac, &postas, nrC, nrP);

    if (nrCerinta == 3) {
        for (int i = 0; i < nrC; i++) {
            printf("%d %d\n", postas[i].id, postas[i].nrPachete);
            for (int j = 0; j < postas[i].nrPachete; j++) {
                if (j == postas[i].nrPachete - 1) {
                    printf("%d", postas[i].pachete[j].id);
                    break;
                }
                printf("%d ", postas[i].pachete[j].id);
            }
            printf("\n");
        }
    }

    for (int i = 0; i < nrP; i++) {
        codificare_mesaj(pac[i].mesaj);
    }
}