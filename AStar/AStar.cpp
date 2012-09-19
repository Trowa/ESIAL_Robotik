//      AStar.cpp
//      
//      Copyright 2011 Guillaume DI FRANCO <guillaume.di-franco@esial.net>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <stdio.h>
#include <stack>
#include <stdlib.h>
#include <allegro.h>

//#define DEBUG //à réctiver pour ajouter les prompt débug
using namespace std;

	    int  tableWidth=3000,
        tableHeight=2100,
        nbColonneMax=15,
        nbLigneMax=11,
        colonneMax=14,
        ligneMax=10;


typedef struct point_struct {
    unsigned char destX;
    unsigned char destY;

} Point;


typedef struct position_struct {
    Point* pointsAtteignables;
    struct position_struct* father;
    unsigned int nbPoints;
    unsigned char selectionable;
    unsigned char inClosedList;
    unsigned char inOpenedList;
    unsigned char posX, posY;
    //Rappel Poids=Poids pour aller jusqu'a l'etape precedente + dist2 + cout2
    int poids;
    //dist est la distance euclidienne LAISSEE au CARRE
    int dist2;
    //le cout du trajet jusqu'a la position
    int cout;


} Position;



void setPoids(Position *a,int b) {
    a->poids=b;
}

int calculatePoids(Position *a, Point b, int currentPoids) {

    Position pos=*a;

    //calcul de la distance euclidienne LAISSEE au CARRE
#ifdef DEBUG
    printf("\npos.posX vaut: %d\n",pos.posX);
    printf("pos.posY vaut: %d\n",pos.posY);
    printf("pos.poids vaut: %d\n",pos.poids);
    printf("pos.cout vaut: %d\n",pos.cout);
    printf("b.destX vaut: %d\n",b.destX);
    printf("b.destY vaut: %d\n",b.destY);
    
#endif


    pos.dist2=(pos.posX-b.destX)*(pos.posX-b.destX)+(pos.posY-b.destY)*(pos.posY-b.destY);


    //On en profite pour calculer le poids

    //Calcul du coût du déplacement
    //cout2 est le cout du déplacement (plus cher si diagonale) mis AU CARRE
    int cout2=1;
    if (pos.posX!=b.destX && pos.posY!=b.destY) {
		
		//On est sur un point dans la diagonale
		
        //Maj du cout de déplacement
        cout2=2;
    } else {
		//cout2 déjà à 1
    }
    
    *a=pos;
#ifdef DEBUG
    printf("pos.dist2 vaut %d\n", (pos.dist2));
    printf("cout2+pos.dist2+pos.poids+currentPoids vaut %d\n\n", (cout2+pos.dist2+pos.poids+currentPoids));
#endif
    return cout2+pos.dist2+pos.poids+currentPoids;

}

int getPoids(Position *a) {
    return a->poids;
}




void createTable(Position ***table) {

    //////////////////////////
    // CREATION DE LA TABLE


// Y^             __ __ __ __ __ __ __ __ __ __ __ __ __ __
//    |  10.     |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    9.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    8.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    7.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    6.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    5.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    4.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    3.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    2.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    1.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    |    0.    |__|__|__|__|__|__|__|__|__|__|__|__|__|__|
//    -----------0--1--2--3--4--5--6--7--8--9-10-11-12-13-14---------> X


    ///////////////////////////////////////////////////
    //ATTENTION !!! la table est en mode "table[X][Y]"
    //
    ///////////////////////////////////////////////////
#ifdef DEBUG
    FILE* log = fopen("log_createTable.txt","w");
    fprintf(log,"Début création table\n");
    fflush(stdout);
#endif

    //~ extern int  tableWidth,
        //~ tableHeight,
        //~ nbColonneMax,
        //~ nbLigneMax,
        //~ colonneMax,
        //~ ligneMax;

#ifdef DEBUG
    fprintf(log,"Début allocation mémoire table\n");
    fprintf(log,"Step1, alloc colonnes\n");
    fflush(stdout);
#endif

    Position **tab;
    tab = (Position**) malloc( sizeof(Position *) * nbColonneMax );
    if (tab==NULL) {
		printf("erreur malloc Position**");
        return;
    }

#ifdef DEBUG
    fprintf(log,"Step2, alloc lignes\n");
    fflush(stdout);
#endif



    for ( int i=0; i<nbColonneMax ; i++) {

#ifdef DEBUG
        fprintf(log,"i=%d\n",i);
        fflush(stdout);
#endif
        tab[i] = (Position*) malloc( sizeof(Position) * nbLigneMax );
        if (tab[i]==NULL) {
			printf("erreur malloc Position*");
            return;
        }
    }
#ifdef DEBUG
    fprintf(log,"Fin allocation mémoire table\n");
    fflush(stdout);
#endif


    //definion des noeuds atteignables ou non
    for ( int x=0; x<nbColonneMax; x++) {
        for ( int y=0; y<nbLigneMax; y++) {

#ifdef DEBUG
            fprintf(log,"Def du point %d,%d\n",x,y);
            fflush(stdout);
#endif

            tab[x][y].selectionable=1;
            tab[x][y].inClosedList=0;
            tab[x][y].inOpenedList=0;
            tab[x][y].posX=x;
            tab[x][y].posY=y;
        }
    }
    
    ///////////////////////////////////
    // Positionne les points non atteignable permanent
    // (les obstacles fixes)
    //////////////////////////////////
    
    
    tab[0][1].selectionable=0;
    tab[1][1].selectionable=0;
    tab[13][1].selectionable=0;
    tab[14][1].selectionable=0;
    tab[1][10].selectionable=0;
    tab[5][10].selectionable=0;
    tab[9][10].selectionable=0;
    tab[13][10].selectionable=0;

    for ( int x=0; x<=colonneMax; x++) {
        for ( int y=0; y<=ligneMax; y++) {    //the y corresponds to the depth of the table (axis y)
            //the x corresponds to the width of the table (axis x)

            if (tab[x][y].selectionable==1) {
                tab[x][y].nbPoints = 8;
                tab[x][y].pointsAtteignables = (Point*) malloc( sizeof(Point) * 8 );

                //Colonnes non atteignables avec valeur par defaut
                int k=0;
                int eviteX[2];
                eviteX[0]=512;
                eviteX[1]=512;
                //Lignes non atteignables avec valeur par defaut
                int eviteY[2];
                eviteY[0]=512;
                eviteY[1]=512;

                //Une première boucle verifie les point selectionable
                int cmptX=0;
                int cmptY=0;

                for (int i=(x-1);i<=x+1;i++) {
                    for (int j=(y-1);j<=y+1;j++) {


#ifdef DEBUG
                                    fprintf(log,"je suis en %d,%d\n",i,j);
                                    fflush(stdout);
                                   // fclose(log);
#endif

                        if ((i==x && j==y)||i<0||j<0||i>colonneMax||j>ligneMax) {
                            //on fait rien

                        } else {



                            if (tab[i][j].selectionable==0) {
                                if (i==x) {
                                    //         _______________________
                                    //    j.  |   | X |   X   | X |   |
                                    //        |   |  \    ^    /  |   |
                                    //        |___|__ \___|___/___|___|
                                    //        |   | O |--i=x--| O |   |
                                    //        |   |   /   |   \   |   |
                                    //        |___|__/____v____\__|___|
                                    //    j.  |   | X |   X   | X |   |
                                    //
                                    //on doit eviter la ligne j pour ce cas
                                    //(on ne peut tomber dans cette partie que deux fois)
                                    eviteY[cmptY]=j;
#ifdef DEBUG
                                    fprintf(log,"je dois eviter la ligne %d\n",eviteY[cmptY]);
                                    fflush(stdout);
#endif
                                    cmptY++;
                                } else if (j==y) {
                                    //on doit eviter la colonne i à ce moment là
                                    //(on ne peut tomber dans cette partie que deux fois)
                                    eviteX[cmptX]=i;
#ifdef DEBUG
                                    fprintf(log,"je dois eviter la colonne %d\n",eviteX[cmptX]);
                                    fflush(stdout);
                                    //fclose(log);
#endif
                                    cmptX++;
                                }
                            } else {
                                //Cas ou le point est selectionnable donc rien à faire
#ifdef DEBUG
                                    fprintf(log,"rien à faire \n");
                                    fflush(stdout);
                                    //fclose(log);
#endif
                            }
                        }
                    }
                }



                //Deuxième boucle
                for (int i=(x-1);i<=x+1;i++) {
                    for (int j=(y-1);j<=y+1;j++) {

                        if ((i==x&&j==y)||i<0||j<0||i>colonneMax||j>ligneMax) {
                            //on fait rien
                        } else {
                            if (i!=eviteX[0] && i!=eviteX[1] && j!=eviteY[0] && j!=eviteY[1]) {
                                //On n'est pas dans les colonnes ou les lignes a éventuellement éviter
                                if (tab[i][j].selectionable==1) {
                                    //Le point est selectionnable donc on l'ajoute à la liste
                                    tab[x][y].pointsAtteignables[k].destX=i;
                                    tab[x][y].pointsAtteignables[k].destY=j;
                                    k++;
                                }
                            }
                        }
                    }
                }
                tab[x][y].nbPoints=k;

                tab[x][y].pointsAtteignables=(Point*)realloc(tab[x][y].pointsAtteignables,tab[x][y].nbPoints * sizeof(Point));
            }

            else {
                tab[x][y].nbPoints=0;

                free(tab[x][y].pointsAtteignables);
            }

        }

    }



    //pour activer, réactiver le DEFINE DEBUT au début du prog
#ifdef DEBUG
    FILE* handler = fopen("ResultatCreaTab.txt","w");
    for (int c=0;c<nbColonneMax;c++) {
        for (int i=0;i<nbLigneMax;i++) {
            fprintf(handler,"\nLa case (%d,%d) peux acceder a %d points:\n",c,i,tab[c][i].nbPoints);
            for (unsigned int k=0;k<tab[c][i].nbPoints;k++) {
                fprintf(handler,"La case (%d,%d)\n", tab[c][i].pointsAtteignables[k].destX,tab[c][i].pointsAtteignables[k].destY) ;
                fflush(stdout) ;
            }
        }
    }
    fclose(handler);
#endif

#ifdef DEBUG
                  fclose(log);
#endif

    (*table) = tab;
}//createTable




void deroule(Position*** table, Point* posInit, Point* cible, stack<int>* pile) {

    Position** tab=*table;
    Point init=*posInit, target=*cible;
    stack<int> maPile=*pile;
    
#ifdef DEBUG
    FILE* handler3 = fopen("log_deroule.txt","w");
    fprintf(handler3,"log_deroule\n");
    fprintf(handler3,"Sens inverse: Je pars de (%d,%d) et vais en (%d,%d)\n", init.destX, init.destY, target.destX, target.destY) ;
    fflush(handler3);
#endif

    int currentX, currentY, xInit, yInit;
    currentX=target.destX;
    currentY=target.destY;
    xInit=init.destX;
    yInit=init.destY;

    maPile.push(currentY);
    maPile.push(currentX);

    while (currentX!=xInit || currentY!=yInit) {

        int tmpX, tmpY;
        tmpX=tab[currentX][currentY].father->posX;
        tmpY=tab[currentX][currentY].father->posY;

        //On ajoute les coordonn&#65533;es dans la stack


#ifdef DEBUG
        fprintf(handler3,"qui viens de (%d,%d)\n",tmpX,tmpY);
        fflush(handler3);
     
#endif

        currentX=tmpX;
        currentY=tmpY;
        maPile.push(currentY);
        maPile.push(currentX);
    }
    
    #ifdef DEBUG
        fclose(handler3);
#endif

    *pile=maPile;
}//deroule()

void giveOrder(stack<int> *pile, BITMAP* buffer) {

    //recupere les coordonnees de la prochaine destination

    stack<int> maPile= *pile;
    //Donne le X
    int x=maPile.top();
    printf("va en x %d",x);
    maPile.pop();
    //donne le Y
    int y=maPile.top();
    printf(" y %d\n",y);
    maPile.pop();

    //////////////////////////////////////////////////////////////////////
    //
    //il faudra implementer la commande qui discute avec l'asservissement
    //
    //////////////////////////////////////////////////////////////////////
    circlefill(buffer, (int)x*SCREEN_W/nbColonneMax+6, (int)y*SCREEN_H/nbLigneMax+6, 5, makecol(0,255,0));

    //MaJ de la pile
    *pile=maPile;
}//giveOrder()


void Astar(Position ***table, Point *maPos, Point *maCible) {

    Position**tab=*table;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //pour activer, reactiver le DEFINE DEBUT au debut du prog
#ifdef DEBUG
    FILE* handler = fopen("ResultatAStar.txt","w");
    for (int c=0;c<15;c++) {
        for (int i=0;i<11;i++) {
            fprintf(handler,"\nLa case (%d,%d) peux acceder a %d points:\n",c,i,tab[c][i].nbPoints);
            for (unsigned int k=0;k<tab[c][i].nbPoints;k++) {
                fprintf(handler,"La case (%d,%d)\n", tab[c][i].pointsAtteignables[k].destX,tab[c][i].pointsAtteignables[k].destY) ;
                fflush(stdout) ;
            }
        }
    }
    fclose(handler);
    printf("log cree : ResultatAStar.txt\n");

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //On part de maPos, on veut aller a maCible
    int posX, posY, finalX, finalY;

    //initialisation avec les coordonnees de maPos
    posX = maPos->destX;
    posY= maPos->destY;
    tab[posX][posY].inClosedList=1;

    //les coordonnees de maCible
    finalX = maCible->destX;
    finalY= maCible->destY;


#ifdef DEBUG
    FILE* handler2 = fopen("log_AStar.txt","w");
    fprintf(handler,"debut %d,%d  cible %d,%d\n",posX, posY, finalX, finalY);
    fflush(stdout);
#endif


    /*la boucle ne s'arrete qu'une fois la position finale atteinte par
    *l'algo de recherche
    */
    int k=0, currentPoids=0,previousPoids=0;

    while ((posX!=finalX || posY!=finalY)) {

        //Initialisation de la variable bestPoids
        int bestPoids=9999;

        //On recupere le nombre de points atteignable depuis la Position actuelle
        int nbPoints=tab[posX][posY].nbPoints;

#ifdef DEBUG
        fprintf(handler2,"\ndebut %d,%d  a %d points atteignables\n",posX, posY, nbPoints);
        fflush(stdout);
#endif

        //On sauvegardera les coordonnees de la meilleur position atteignable
        //depuis la position actuelle

        int currentX, currentY, bestX, bestY;

        for (int i=0; i<nbPoints; i++) {

            //On recupere les coordonnees du point atteignable

            currentX=tab[posX][posY].pointsAtteignables[i].destX;
            currentY=tab[posX][posY].pointsAtteignables[i].destY;

#ifdef DEBUG
            fprintf(handler2,"le current point teste est %d,%d  cible %d,%d\n",currentX, currentY, finalX, finalY);
            fprintf(handler2,"inClosedList=%d\n",tab[currentX][currentY].inClosedList);
            fflush(stdout);
#endif

            //Test present dans liste fermee

            if (tab[currentX][currentY].inClosedList==0) {

#ifdef DEBUG
                fprintf(handler2,"Le point n'est pas dans la liste fermee\n");
                fflush(stdout);
#endif

                //Cas ou le point n'est pas dans la liste ferme

                //Calcul du poids du "voyage" vers ce point

                currentPoids=calculatePoids(&tab[currentX][currentY], *maCible, previousPoids);

#ifdef DEBUG
                fprintf(handler2,"currentPoids du point %d,%d vaut %d\n",currentX,currentY,currentPoids);
                fflush(stdout);
#endif

                //On cherche le point atteignable ayant le meilleur poids
                if (currentPoids<bestPoids) {
                    bestX=currentX;
                    bestY=currentY;
                    bestPoids=currentPoids;
                };

                //test si le point est dans la liste ouverte

                if (tab[currentX][currentY].inOpenedList==0) {

#ifdef DEBUG
                    fprintf(handler2,"Le point n'est pas dans la liste ouvert\n");
                    fflush(stdout);
#endif

                    //Cas ou le point n'est pas dans la liste ouverte
                    //On l'y met
                    tab[currentX][currentY].inOpenedList=1;

                    //et on met renseigne son poids
                    tab[currentX][currentY].cout=currentPoids;

                    //on met &#65533; jour l'info sur le p&#65533;re
                    tab[currentX][currentY].father=&tab[posX][posY];
                }

                else {

#ifdef DEBUG
                    fprintf(handler2,"Le point est dans la liste ouvert\n");
                    fflush(stdout);
#endif

                    //si il est dans l'Openlist on verifie son poids

                    if (tab[currentX][currentY].cout>currentPoids) {

                        //si le coup est inferieur alors
                        //on met a jour le poids et le pere
                        tab[currentX][currentY].father=&tab[posX][posY];
                        tab[currentX][currentY].cout=currentPoids;
                        
                    } else {//on laisse telle quel
                    }
                }
            } else {//on fait rien puisque on passe deja; par la ou qu'on a deja essaye;

#ifdef DEBUG
                fprintf(handler2,"le point %d,%d est dans la liste ferm&#65533;e\n",currentX,currentY);
                fflush(stdout);
#endif

            }
        }


        //On se positionne sur le meilleur point
        posX=bestX;
        posY=bestY;
        previousPoids=bestPoids;
        //On place cette position dans la liste fermee
        //on met a jour le poids et le pere
        tab[posX][posY].inClosedList=1;

#ifdef DEBUG
        fprintf(handler2,"\nOn Choisi le point %d,%d\n\n",posX,posY);
        fflush(stdout);
        
#endif

        k++;

    }

#ifdef DEBUG

        fclose(handler2);
#endif


    (*table) = tab;


    return;
}



int main(int argc, char **argv)
{
	BITMAP* buffer;
	
		
	    /////////////////////////////////////////////////////
    //Point qui devra etre donne en parametre plus tard
    //
 
    Point maPos,maCible;
    maPos.destX=0;
    maPos.destY=10;
    maCible.destX=14;
    maCible.destY=0;


//Creation of the table that the robot will use
	Position ** table;
	printf("\n///////////////////////////////\n//////    create Table   ////////\n");
	createTable(&table);
	//printf("poids du point avant: %d\n",getPoids(&table[9][0]));
		 for(int i=10;i>2;i--){
		 setPoids(&table[3][i],999999);
	 }
	 for(int i=2;i<15;i++){
		 setPoids(&table[i][3],999999);
	 }
	 //setPoids(&table[9][0],99999);
	//printf("poids du point apres: %d\n",getPoids(&table[9][0]));
	 printf("///////////////////////////////\n//////    ASTAR   ////////\n");
	 Astar(&table,&maPos,&maCible);
	 
	 
	   //TODO lancer allegro
        
  allegro_init() ;
  
    //installation du clavier et de la souris
  install_keyboard() ;
  
  if(install_mouse() == -1) {
        
    //si erreur de chargement de la souris
    allegro_message("Erreur avec la souris! %s", allegro_error) ;
    return 1 ;
  }
	 
	 
	   set_color_depth(16) ;
  if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0) {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("Impossible d’initialiser le mode vidéo !\n%s\n", allegro_error);
    return 1;
  }
  
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
  if (!buffer) {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message ("Erreur : imposible de créer le buffer!");
    return 1;
  }
  
  for(int i=0;i<nbColonneMax;i++){
	  
	
	  
		  for(int j=0;j<nbLigneMax;j++){
			  
			  if(i%2==1 && j%2==1){
					
				 //printf("j=%d est divisible par 2",j);
				 vline(buffer,(int)i*SCREEN_W/nbColonneMax+6,0,SCREEN_H, makecol(255,255,255));
				 hline(buffer,0,(int)j*SCREEN_H/nbLigneMax+6,SCREEN_W, makecol(255,255,255));
				  }
			  
			  /*  if(i%2==1){
					
				 //printf("j=%d est divisible par 2",j);
				 vline(buffer,(int)i*SCREEN_H/nbLigneMax+6,0,SCREEN_H, makecol(255,255,255));
				  
				  }
			  
			  if(j%2==1){
				  //printf("j=%d est divisible par 2",j);
				 hline(buffer,0,(int)j*SCREEN_H/nbLigneMax+6,SCREEN_W, makecol(255,255,255));
				  
				  }*/
			  if(table[i][j].selectionable){
				  
				circlefill(buffer, (int)i*SCREEN_W/nbColonneMax+6, (int)j*SCREEN_H/nbLigneMax+6, 5, makecol(255,255,255));
			}else{
				circlefill(buffer, (int)i*SCREEN_W/nbColonneMax+6, (int)j*SCREEN_H/nbLigneMax+6, 7, makecol(255,255,255));
				circlefill(buffer, (int)i*SCREEN_W/nbColonneMax+6, (int)j*SCREEN_H/nbLigneMax+6, 5, makecol(255,0,0));
				
			}
  
			}
}
	 
	 
     printf("///////////////////////////////\n//////    DEROULE   ////////\n");
     stack<int> maPile;
     deroule(&table,&maPos,&maCible,&maPile);
     
     blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H) ;
     
     printf("///////////////////////////////\n//////    Give order   ////////\n");
     while (maPile.size()>0) {
		 
         while(!key[KEY_SPACE]);
         while(key[KEY_SPACE]);
         //donne les ordres ? l'asservissement
         giveOrder(&maPile,buffer);
         blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H) ;

     }
	
	//textprintf_ex(buffer, font,0,0,	makecol(150,255, 100), -1 , "Appuyez sur ECHAP pour quitter");
	
	
	while(!key[KEY_ESC]);
	
	
	free(table);
	destroy_bitmap(buffer);
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	return 0;
}

