/*Sebastien Saez et Nicolas Ducarre
Projet du 29 mai 2013 de programmation C
Le but de ce programme sera de générer une facture de client à partir d'une liste de produits */

/*Directives de préprocesseur*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Déclaration des constantes*/
#define LGL 64
#define MAX 128

/*Constantes pour les booleans*/
#define VRAI 1
#define FAUX 0

/*Constante utilisée dans les recherches*/
#define NOTFOUND -2

/*Déclaration des structures */
/*Structure pour stocker le nom et le prénom de l'utilisateur*/
struct personne {
    char nom [LGL];
    char prenom [LGL];
};

/*Structure pour stocker les données sur le produit*/
struct produit {
    unsigned int noArt;
    char libelleArt [LGL];
    char marqueArt [LGL];
    float prix;
    unsigned int quantiteArt;
};

/*Saisie du nom et prenom*/
void saisieNom (struct personne *nouveau);

/*Saisie d'une valeur entière au clavier*/
int saisieInt ();

/*Recherche un entier dans un struct*/
int rechercheInt (struct produit p[MAX], int no);

/*Fonction qui imprime à l'écran ce qui a été commandé*/
void printProduct (struct produit c[MAX], int limite);

/*Fonction qui génère la facture au format .html */
void genHtml (struct personne n, struct produit p[MAX], int nbArt);

/*************************************************************
DEBUT DE LA FONCTION PRINCIPALE
*************************************************************/
int main () {
    /*Déclaration des variables*/
    struct personne client1 ;
    struct produit vente1[MAX] ;
    FILE *monfichier;
    int existe;
    char ligne [LGL];
    int n,
        i,
        choixMenu,
        choix1 = FAUX,	/*car la personne n'a pas encore choisi le menu 1*/
        choix2 = FAUX,	/*car la personne n'a pas encore choisi le menu 2*/
        noArt,
        quantite,
        posArt,        /*position d'article*/
        nbArt;

    /*Ouvre le fichier produit.txt en lecture seule*/
    monfichier = fopen ("produit.txt", "r");

    /*Vérifie que le fichier produit.txt ait bien été trouvé*/
    if (monfichier == NULL) {
        puts ("Fichier 'produit.txt' introuvable");
    } else {
        /*Boucle qui permet d'extraire le contenu de la liste de produit et de le mettre dans le struct vente1*/
        i = 0 ;
        nbArt = 0 ;
        existe = VRAI ;
        fgets (ligne, LGL * 4 , monfichier);		/*lit la 1ère ligne*/

        while (!feof(monfichier) && existe) {

            /*Vérifie qu'il y a bien les infos souhaitées*/
            n = sscanf (ligne, "%d %s %s %f",
                        &vente1[i].noArt,
                        vente1[i].libelleArt,
                        vente1[i].marqueArt,
                        &vente1[i].prix);
            vente1[i].quantiteArt = 0 ;	/*mets la qté a zéro pour chaque article extrait de la liste de produit*/
            /*Si une ligne de la liste de produit contient une erreur, le programme s'arrête*/
            i++;
            if (n != 4) {
                printf ("La ligne %d contient une erreur, le progamme va fermer\n", i);
                existe = FAUX;
                break;
            }
            fgets (ligne, LGL * 4 , monfichier);		/*lit la prochaine ligne de la liste de produit*/
            nbArt ++;	/*Variable qui indique le nombre d'article dans la liste*/
        }

        /*Ferme le fichier une fois qu'on a fini d'extraire son contenu ou qu'il y a une erreur*/
        fclose(monfichier);
    }

    /*On entre dans le menu que si la liste de produit est complète*/
    if (existe) {
        puts("*****************************************************************");
        puts("**************************BIENVENUE******************************");
        puts("*****************************************************************");
        puts ("Que voulez-vous faire ? ");
        puts ("1. Saisir le nom et prenom");
        puts ("2. Commander un produit");
        puts ("3. Afficher la liste des produits choisis");
        puts ("4. Generer la facture");
        puts ("0. Pour quitter le programme");
        puts("");
        printf ("Votre choix: ");
        choixMenu = saisieInt ();
    } else {
        choixMenu = 0 ;
    }

    /*Boucle qui détermine si on reste dans le menu ou pas*/
    while (choixMenu != 0 && existe) {
        switch (choixMenu) {
        case 1 :/*Saisie du nom et prénom*/
            saisieNom (&client1);
            /*Indique que le nom et prénom a été saisi (prérequis pour le choix du menu 2)*/
            choix1 = VRAI;
            break;
        case 2 : /*Saisie du produit*/
            /*Vérifie que le nom a été saisi*/
            if (choix1) {
                printf ("Numero de produit: ");
                noArt = saisieInt ();
                /*Recherche l'article dans le tableau et vérifie son existance*/
                posArt = rechercheInt (vente1, noArt);
                if (posArt == NOTFOUND) {  	/*si l'article n'existe pas*/

                    puts ("L'infortune vous touche, ce produit n'existe point");
                } else {	/*Si l'article existe*/
                    printf ("Quantite: ");
                    quantite = saisieInt ();
                    vente1[posArt].quantiteArt += quantite;
                    /*Afiche une confirmation à l'écran*/
                    printf ("Commande de %d %s %s, prix unitaire de CHF %0.2f, prix total CHF %0.2f",
                            quantite,
                            vente1[posArt].marqueArt,
                            vente1[posArt].libelleArt,
                            vente1[posArt].prix,
                            vente1[posArt].prix * quantite);
                    /*Confirme que l'on a bien exécuté le choix2*/
                    choix2 = VRAI;
					puts ("");
                }
            } else {
                /*Dans le cas où la personne n'as pas encore saisi son nom et prenom*/
                puts ("Entrez d'abord votre nom et votre prenom");
            }
            break;
        case 3:
            /*Détermine s'il y a déjà des article commandé ou pas*/
            if (choix2) {
                /*Affiche les articles qui ont été saisis*/
                printProduct (vente1, nbArt);
            } else {
                puts ("Aucun article n'a encore ete commande");
            }
            break;
        case 4:
            /*Génération du fichier en .html*/
            if (choix1 && choix2) {
                genHtml (client1, vente1, nbArt);
                puts ("La facture a ete correctement generee");
            } else {
                puts ("Il manque des informations, la facture ne peut pas etre generee");
            }
            break;
        default :
            /*Message d'erreur */
            printf ("Non, non, non! Il faut choisir entre 0, 1, 2, 3 et 4!\n");
        }
        puts ("");
        puts ("Que voulez-vous faire ? ");
        puts ("1. Saisir le nom et prenom");
        puts ("2. Commander un produit");
        puts ("3. Afficher la liste des produits choisis");
        puts ("4. Generer la facture");
        puts ("0. Pour quitter le programme");
        puts("");
        printf ("Votre choix: ");
        choixMenu = saisieInt ();
    }

    puts ("Bonne journee et au revoir!");
    system ("pause");
}
/**********************************************************
FIN DE LA FONCTION PRINCIPALE
**********************************************************/

/*Fonction qui saisit le nom et le prénom de l'utilisateur*/
void saisieNom (struct personne *nouveau) {
    char ligne [LGL];
    int n;
    printf ("Nom et prenom: ");
    fgets (ligne, LGL * 2, stdin);
    n = sscanf (ligne, "%49s %49s",
                nouveau->nom,
                nouveau->prenom);
    /*Tant que la saisie n'a pas reçu deux mots */
    while (n != 2) {

        puts ("Erreur, il faut un nom et un prenom");
        fgets (ligne, LGL, stdin);
        n = sscanf (ligne, "%49s %49s",
                    nouveau->nom,
                    nouveau->prenom);
    }
    printf ("Vous vous appellez donc %s %s\n",
            nouveau->nom,
            nouveau->prenom);
    puts ("Si votre nom est incorrect: tapez 1");
}

/*Saisie d'une valeur entière au clavier*/
int saisieInt () {
    char ligne [LGL];
    unsigned int valeur, n;
    fgets (ligne, LGL, stdin); /* saisir une val*/
    n = sscanf (ligne, "%u", &valeur);
    while (n != 1 || valeur < 0) {
        printf ("Erreur, entrez une valeur entiere positive svp: ");
        fgets (ligne, LGL, stdin); /*Saisir une val*/
        n = sscanf (ligne, "%u", &valeur);
    }
    return valeur;
}

/*Recherche un index de produit dans un struct*/
int rechercheInt (struct produit p[MAX], int no) {
    unsigned int position = 0;

    /*Boucle qui retourne la position de l'article si elle le trouve */
    for  (position=0; position < MAX; position++) {
        if (p[position].noArt == no) {
            return position;
        }
    }
    /*Retour de NOTFOUND si elle ne le trouve pas*/
    return NOTFOUND;
}

/*Fonction qui imprime à l'écran ce qui a été commandé*/
void printProduct (struct produit cmd[MAX], int limite) {
    int i;
	float total = 0;

	puts ("");
	//En-tête
	puts (" No  |   Marque   |    Libelle    |  Prix  |  NB  |  Total  ");
	puts ("------------------------------------------------------------");

    /*Ce qu'il faut imprimer*/
    for (i = 0 ; i < limite ; i++) {
        /*Si la quantité d'article est supérieure à zéro => il imprime */
        if (cmd[i].quantiteArt > 0) {
            printf ("%3d  | %10s | %13s | %6.2f | %4d | %7.2f ",
                    cmd[i].noArt,
                    cmd[i].marqueArt,
                    cmd[i].libelleArt,
                    cmd[i].prix,
                    cmd[i].quantiteArt,
                    cmd[i].prix * cmd[i].quantiteArt);
			total+= cmd[i].prix * cmd[i].quantiteArt;
			puts ("");
			puts ("------------------------------------------------------------");
        }
    }
	printf ("Total final:                                        %0.2f", total);
	puts ("");
}

/*Fonction qui génère le .html et qui prend en paramètres les structs de personnes
et de produits ainsi que le numero d'article pour savoir le nombre d'article à imprimer*/
void genHtml(struct personne n, struct produit p[MAX], int nbArt) {
    int i;
    float Total;
    char Commandes [LGL];
    FILE *fichier;
    strcpy (Commandes, n.nom);
    strcat (Commandes, n.prenom);
    strcat (Commandes, ".html");
    Total = 0;
    /* Ouverture du fichier et si le fichier existe déjà, il sera écrasé*/
    fichier = fopen(Commandes, "w+" );

    /*Si le fichier n'existe pas => Message d'erreur */
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier%s/n", fichier);
    } else {
    /*Syntaxe du fichier HTML*/
        fprintf (fichier, "<html>\n<head><title>Facture</title></head>");
        fprintf (fichier, "<body>\n");
        fprintf (fichier, "<h1>Facture de %s %s</h1>\n", n.nom, n.prenom);
        fprintf (fichier, "<table border>\n<tr bgcolor=\"green\">");
        fprintf (fichier, "<td>No</td><td>Marque</td><td>Ref</td><td>Prix</td><td>Nb</td><td>Total</td>");

        /*Boucle qui imprime tous les articles dans le tableau HTML*/
        for(i = 0; i < nbArt; i++) {
            if (p[i].quantiteArt > 0 ) {
                fprintf (fichier, "<tr><td>%d</td><td>%s</td><td>%s</td><td>%.2f</td><td>%d</td><td align=right>%.2f</td></tr>",
                         p[i].noArt,
                         p[i].marqueArt,
                         p[i].libelleArt,
                         p[i].prix,
                         p[i].quantiteArt,
                         p[i].prix * p[i].quantiteArt);
                Total+= p[i].prix * p[i].quantiteArt;
            }
        }
        /*Impression du total */
        fprintf (fichier, "<tr><td>Total</td><td>&nbsp</td><td>&nbsp</td><td>&nbsp</td><td>&nbsp</td><td>%.2f</td></tr>",Total);
        fprintf (fichier, "</table></body>");
        /*Fermeture du fichier */
        fclose (fichier);
    }
}
