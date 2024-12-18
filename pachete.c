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
    int cod;
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
    int i, j;

    //getc(stdin);
    scanf("%d", nrC); //Citim numarul de cartiere si de postasi
    *cart = malloc (sizeof(cartier) * (*nrC));

    for (i = 0; i < *nrC; i++) {
        (*cart)[i].id = i;
        scanf("%s", numetmp); //Folosim un sir de caractere temporar pentru a stii
        //exact cata memorie sa ii alocam numelui cartierului.
        (*cart)[i].nume = malloc(sizeof(char) * (strlen(numetmp)+1));
        strcpy((*cart)[i].nume, numetmp);
    }

    scanf("%d", nrP); //Citim numarul de pachete
    *pac = malloc (sizeof(pachet) * (*nrP));

    for (i = 0; i < *nrP; i++) {
        (*pac)[i].id = i;
        for (j = 0; j < 18; j++) {
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

    int i;
    pac->idCartier=0;
    pac->strada=0;
    pac->numar=0;

    for (i = 0; i < 18; i++) {
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

    *post = malloc (sizeof(postas) * nrC);
    int i, j;

    for (i = 0; i < nrC; i++) {
        (*post)[i].id = i;
        (*post)[i].nrPachete = 0;
        (*post)[i].pachete = malloc(nrP * sizeof(pachet));

        for (j = 0; j < nrP; j++){
            //printf("%d %d\n", (*post)[i].id, pac[j].idCartier);
            if ((*post)[i].id == pac[j].idCartier) {
                (*post)[i].pachete[(*post)[i].nrPachete] = pac[j];
                (*post)[i].nrPachete++;
                //printf("%d %d\n", (*post)[i].id, (*post)[i].nrPachete);
            }
        }
    }
    //Sunt destul de sigur ca merge bine chestia asta
}

//Cerinta I 4.
void ordonare_pachete(postas* post, int nrP, int nrC) {
    pachet temp;
    int i, l;
    //Bubble sort pentru organizarea in functie de prioritate
    for (l = 0; l < post->nrPachete-1; l++) {
        for (i = 0; i < post->nrPachete-1; i++) {
            if (post->pachete[i].prioritate < post->pachete[i+1].prioritate) {
                temp = post->pachete[i];
                post->pachete[i] = post->pachete[i+1];
                post->pachete[i+1] = temp;
            }
        }
    }
    //Bubble sort pentru origanizarea in functie de greutate a pachetelor cu acelasi grad de prioritate
    for (l = 0; l < post->nrPachete-1; l++) {
        for (i = 0; i < post->nrPachete-1; i++) {
            if ((post->pachete[i].prioritate == post->pachete[i+1].prioritate) && (post->pachete[i].greutate < post->pachete[i+1].greutate)) {
                temp = post->pachete[i];
                post->pachete[i] = post->pachete[i+1];
                post->pachete[i+1] = temp;
            }
        }
    }
}

//Cerinta I 5.
void formatare_mesaj(char* mesaj){

    mesaj[strlen(mesaj)-1] = '\0';
    char* p = strtok(mesaj," ,.;:?!");
    char a[100][100];

    int cnt=0, i;
    while (p != NULL) {
      strcpy(a[cnt], p);
      p = strtok(NULL, " ,.;:?!");
      cnt++;
    }
    mesaj[0]='\0';
    for (i = cnt-1; i >= 0; i--) {
      strcat(mesaj, a[i]);
    }
    //printf("%s\n",mesaj);
}

void cod_mesaj(pachet* pac) {
    int i;
    pac->cod = 0;
    for (i = 0; pac->mesaj[i] != '\0'; i++) {
        pac->cod += ((int)pac->mesaj[i] * i);
    }
    pac->cod = pac->cod % ((pac->strada * pac->numar) + 1);
}

//Cerinta II 6.
void alterare_cod(int *id) {

}
void alege_cod(postas* post) {
    int id, i, tempcode;
    scanf("%d", &id);

    while (id % 10) {
        for (i = 0; i < post[id].nrPachete; i++) {
            tempcode = post[id].pachete[i].cod;
            while (tempcode) {
                if (tempcode % 10 == id % 10) {
                    alterare_cod(&post[id].pachete[i].cod);
                    break;
                }
                tempcode /= 10;
            }
        }
        id /= 10;
    }
}

int main(void) {
    int nrC, nrP, nrCerinta, i, j;
    cartier* cart;
    pachet* pac;
    postas* postas;

    scanf("%d", &nrCerinta);
    input_cartiere_pachete(&nrC, &nrP, &cart, &pac);

    if (nrCerinta == 1) {
        for (i = 0; i < nrC; i++) {
            printf("%d %s\n", i, cart[i].nume);
        }
        for (i = 0; i < nrP; i++) {
            printf("%d\n", pac[i].id);

            for (j = 0; j < 18; j++) {
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

    for (i = 0; i < nrP; i++) { //for pentru a calcula la toate cartierele(altfel nu merg functiile urmatoare)
        pachet_info(&pac[i]);
    }

    if (nrCerinta == 2) {
        for (i = 0; i < nrP; i++) {
            printf("%d\n", pac[i].id);
            printf("%d %d %d\n", pac[i].idCartier, pac[i].strada, pac[i].numar);
        }
    }

    distribuire_pachete(pac, &postas, nrP, nrC);

    if (nrCerinta == 3) {
        distribuire_pachete(pac, &postas, nrP, nrC);
        for (i = 0; i < nrC; i++) {
            printf("%d %d\n", postas[i].id, postas[i].nrPachete);
            if (postas[i].nrPachete != 0) {
                for (j = 0; j < postas[i].nrPachete-1; j++) {
                    printf("%d ", postas[i].pachete[j].id);
                }
                printf("%d", postas[i].pachete[postas[i].nrPachete-1].id);
                printf("\n");
            }
        }
    }

    for (i = 0; i < nrC; i++) {
        ordonare_pachete(&postas[i], nrP, nrC);
    }

    if (nrCerinta == 4) {
        for (i = 0; i < nrC; i++) {
            printf("%d %d\n", postas[i].id, postas[i].nrPachete);
            if (postas[i].nrPachete != 0) {
                for (j = 0; j < postas[i].nrPachete-1; j++) {
                    printf("%d ", postas[i].pachete[j].id);
                }
                printf("%d", postas[i].pachete[postas[i].nrPachete-1].id);
                printf("\n");
            }
        }
    }
    for (j = 0; j < nrC; j++) {
        for (i = 0; i < postas[j].nrPachete; i++) {
            formatare_mesaj(postas[j].pachete[i].mesaj);
            cod_mesaj(&postas[j].pachete[i]);
        }
    }

    if (nrCerinta == 5) {
        for (i = 0; i < nrC; i++) {
            printf("%d %d\n", postas[i].id, postas[i].nrPachete);
            for (j = 0; j < postas[i].nrPachete; j++) {
                if (postas[i].nrPachete != 0) {
                    printf("%d %d\n", postas[i].pachete[j].id, postas[i].pachete[j].cod);
                }
            }
        }
    }
}