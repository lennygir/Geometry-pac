#include <SFML/Graphics.hpp>
#include <math.h>
#include <string.h>
#include <time.h>

#define TAILLE_TERRAIN 20
#define TAILLE_CASE 45
#define LARGEUR_FENETRE 900
#define LONGUEUR_FENETRE 900
#define PAS_PAC 2
#define POSI_PIECE 15
#define TAILLE_PIECE 5
#define PAS_MONSTRE 2
#define POSITION_M_DEP_X 540
#define POSITION_M_DEP_Y 540


using namespace sf;


void affichageTerrain(RenderWindow * fenetre,int terrain[TAILLE_TERRAIN][TAILLE_TERRAIN])
{
    int i,j,x,y;
    for (i=0; i<TAILLE_TERRAIN; i++)
    {
        for(j=0; j<TAILLE_TERRAIN; j++)
        {
            x=j*TAILLE_CASE;
            y=i*TAILLE_CASE;
            if (terrain[i][j]==1)
            {
                RectangleShape rectangle(Vector2f(TAILLE_CASE,TAILLE_CASE));
                rectangle.setPosition(x,y);
                fenetre->draw(rectangle);
            }

        }
    }
}

void affichageObjet(RenderWindow * fenetre,int terrain[TAILLE_TERRAIN][TAILLE_TERRAIN])
{
    int i,j,x,y;
    for (i=0; i<TAILLE_TERRAIN; i++)
    {
        for(j=0; j<TAILLE_TERRAIN; j++)
        {
            x=j*TAILLE_CASE;
            y=i*TAILLE_CASE;
            if (terrain[i][j]==3)
            {
                CircleShape piece(TAILLE_PIECE);
                piece.setPosition(x+POSI_PIECE,y+POSI_PIECE);
                piece.setFillColor(Color::Yellow);
                fenetre->draw(piece);
            }
            if (terrain[i][j]==4)
            {
                CircleShape invi(TAILLE_PIECE);
                invi.setPosition(x+POSI_PIECE,y+POSI_PIECE);
                invi.setFillColor(Color::Red);
                fenetre->draw(invi);
            }

        }
    }

}

void collisionPacObjet(int positionX,int positionY,Sprite entiter,int *collision,int terrain[TAILLE_TERRAIN][TAILLE_TERRAIN],RenderWindow *fenetre,int *score,Thread *temp,int threadOn)
{
    int caseX,caseY,j,i;
    caseX=round(positionX/TAILLE_CASE);
    caseY=round(positionY/TAILLE_CASE);
    RectangleShape rectangle(Vector2f(TAILLE_CASE,TAILLE_CASE));
    CircleShape cerclePiece(TAILLE_PIECE);
    CircleShape cercleInvi(TAILLE_PIECE);
    for (i=caseY; i<caseY+2; i++)
    {
        for(j=caseX; j<caseX+2; j++)
        {
            if (terrain[i][j]==1)
            {
                rectangle.setPosition(j*TAILLE_CASE,i*TAILLE_CASE);
                if (entiter.getGlobalBounds().intersects(rectangle.getGlobalBounds()))
                {
                    *collision=1;
                }
            }
            if (terrain[i][j]==3)
            {
                cerclePiece.setPosition(j*TAILLE_CASE+POSI_PIECE,i*TAILLE_CASE+POSI_PIECE);
                if (entiter.getGlobalBounds().intersects(cerclePiece.getGlobalBounds()))
                {
                    terrain[i][j]=2;
                    affichageObjet(&*fenetre,terrain);
                    *score+=5;
                }
            }
            if (terrain[i][j]==4)
            {
                cercleInvi.setPosition(j*TAILLE_CASE+POSI_PIECE,i*TAILLE_CASE+POSI_PIECE);
                if (entiter.getGlobalBounds().intersects(cercleInvi.getGlobalBounds()))
                {
                    if (threadOn==0)
                    {
                        terrain[i][j]=2;
                        affichageObjet(&*fenetre,terrain);
                        temp->launch();
                    }

                }


            }



        }
    }
}


void collisionPacEnnemi(int *boucleJeu,Sprite pacMan,Sprite *ennemi,int *resultat,int inviOn,int *X,int *Y)
{
    if (pacMan.getGlobalBounds().intersects(ennemi->getGlobalBounds()))
    {
        if (inviOn==0)
        {
            *resultat=0;
            *boucleJeu=1;

        }
        if (inviOn==1)
        {
           *X=POSITION_M_DEP_X;
           *Y=POSITION_M_DEP_Y;

        }
    }


}

void deplacementEnnemi(Sprite sprite,int terrain[TAILLE_TERRAIN][TAILLE_TERRAIN],int *X,int *Y,int *collision,int *direction,int *oldX,int *oldY)
{
    int caseX,caseY,j,i,iExit=0;;
    caseX=round(*X/TAILLE_CASE);
    caseY=round(*Y/TAILLE_CASE);
    RectangleShape rectangle(Vector2f(TAILLE_CASE,TAILLE_CASE));

    for (i=caseY; i<caseY+2; i++)
    {
        for(j=caseX; j<caseX+2; j++)
        {
            if (terrain[i][j]==1)
            {
                rectangle.setPosition(j*TAILLE_CASE,i*TAILLE_CASE);
                if (sprite.getGlobalBounds().intersects(rectangle.getGlobalBounds()))
                {
                    *collision=1;

                }
            }
        }
    }


        if (*direction==1)
        {
            if (*collision==1)
            {
                *Y=*oldY;
                *X=*oldX;
                *collision=0;
                for(i = 0 ; i<10000 && iExit == 0 ; i++)
                {
                    *direction=rand()%4+1;
                    if( *direction != *direction )
                        iExit = 1;
                }

            }

            else if (*collision==0)
            {
                *oldX=*X;
                *oldY=*Y;
                *X+=PAS_MONSTRE;

            }
        }

    if (*direction==2)
    {
        if (*collision==1)
        {
            *Y=*oldY;
            *X=*oldX;
            *collision=0;
            for(i = 0 ; i<10000 && iExit == 0 ; i++)
            {
                *direction=rand()%4+1;
                if( *direction != *direction )
                    iExit = 1;

            }


        }
        else if (*collision==0)
        {
            *oldX=*X;
            *oldY=*Y;
            *X-=PAS_MONSTRE;

        }
    }

    if (*direction==3)
    {
        if (*collision==1)
        {
            *Y=*oldY;
            *X=*oldX;
            *collision=0;
            for(i = 0 ; i<10000 && iExit == 0 ; i++)
            {
                *direction=rand()%4+1;
                if( *direction != *direction )
                    iExit = 1;
            }

        }
        else if (*collision==0)
        {
            *oldX=*X;
            *oldY=*Y;
            *Y-=PAS_MONSTRE;
        }
    }

    if (*direction==4)
    {
        if (*collision==1)
        {
            *Y=*oldY;
            *X=*oldX;
            *collision=0;
            for(i = 0 ; i<10000 && iExit == 0 ; i++)
            {
                *direction=rand()%4+1;
                if( *direction != *direction )
                    iExit = 1;
            }



        }
        else if (*collision==0)
        {
            *oldX=*X;
            *oldY=*Y;
            *Y+=PAS_MONSTRE;
        }


    }


}



int nombrePoint(int terrain[TAILLE_TERRAIN][TAILLE_TERRAIN])
{
    int i,j,pointTotal=0;
    for(i=0; i<TAILLE_TERRAIN; i++)
    {
        for(j=0; j<TAILLE_TERRAIN; j++)
            if (terrain[i][j]==3)
                pointTotal+=5;
    }
    return pointTotal;
}

void victoire(int pointTotal,int score,int *resultat,int *boucleJeu)
{
    if (pointTotal==score)
    {
        *resultat=1;
        *boucleJeu=1;
    }
}


void affichageTexteVariable(Font police,int score,RenderWindow *fenetre,char textEcrire[100],int x,int y,int taille)
{
    char text[100];
    Text texte;
    sprintf(text,textEcrire,score);
    texte.setFont(police);
    texte.setString(text);
    texte.setCharacterSize(taille);
    texte.setPosition(x,y);
    texte.setColor(Color::Blue);
    fenetre->draw(texte);

}

void affichageTexte(Font police,RenderWindow *fenetre,char text[100],int x,int y,int taille)
{
    Text texte;
    texte.setFont(police);
    texte.setString(text);
    texte.setCharacterSize(taille);
    texte.setPosition(x,y);
    texte.setColor(Color::Blue);
    fenetre->draw(texte);

}


void invicible(int *threadOn)
{
    *threadOn=1;
    sleep(seconds(5));
    *threadOn=0;


}


void imageFantome(int inviOn,Sprite *monstre,Texture *normal,Texture *peur)
{
    if (inviOn==1)
    {
        monstre->setTexture(*peur);
    }
    if (inviOn==0)
    {
        monstre->setTexture(*normal);
    }
}

typedef struct
{
    int x,y,direction,oldX,oldY,collision=0;
} Perso;

int main()
{
    int boucleMenu=0,boucleJeu,boucleMenuDebut;
    int gauche=0,droite=0,haut=0,bas=0,score=0,inviOn=0,resultat,pointTotal,boucleMenuFin;

    char texteScore[20]="Score : %i points";
    char textePerdu[20]="Vous avez perdu !";
    char texteVictoire[20]="Vous avez gagner !";
    char texteJouer[20]="JOUER";



    RenderWindow fenetre(VideoMode(LARGEUR_FENETRE, LONGUEUR_FENETRE), "PAC");
    //fenetre.setFramerateLimit(80);
    srand(time(NULL));

    Texture imageM1,imageM2,imageM3,imageM4,imagePeur,imagePacMan;
    imagePacMan.loadFromFile("image_pacman.png");
    imageM1.loadFromFile("image_M1.png");
    imageM2.loadFromFile("image_M2.png");
    imageM3.loadFromFile("image_M3.png");
    imageM4.loadFromFile("image_M4.png");
    imageM4.loadFromFile("image_M4.png");
    imagePeur.loadFromFile("fantome_peur.png");

    Sprite spM1,spM2,spM3,spM4,spPacMan;
    spPacMan.setTexture(imagePacMan);

    Event evenement;

    Font police;
    police.loadFromFile("arial.ttf");

    Perso pacMan,m1,m2,m3,m4;

    Thread threadTemp(&invicible,&inviOn);


    do
    {
        fenetre.clear();
        boucleMenuDebut=0;
        affichageTexte(police,&fenetre,texteJouer,260,250,100);
        fenetre.display();

        do
        {
            while (fenetre.pollEvent(evenement))
            {

                switch (evenement.type)
                {

                case Event::Closed:
                    fenetre.close();
                    break;

                case Event::KeyPressed:
                    if (evenement.key.code == Keyboard::Right)
                    {
                        boucleMenuDebut=1;

                        break;
                    }
                    if (evenement.key.code == Keyboard::Left)
                    {
                        fenetre.close();
                        break;
                    }
                }
            }

        }
        while(boucleMenuDebut==0);


        boucleJeu=0,boucleMenuFin=0;
        pacMan.x=TAILLE_CASE+1,pacMan.y=TAILLE_CASE+1;

        int terrain[TAILLE_TERRAIN][TAILLE_TERRAIN]= {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                {1,2,3,3,3,4,3,3,3,3,1,1,1,1,1,1,1,1,1,1},
                                                {1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1},
                                                {1,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1},
                                                {1,1,1,3,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1},
                                                {1,1,1,3,3,3,1,1,1,3,1,1,1,1,1,1,1,1,1,1},
                                                {1,1,1,1,1,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1},
                                                {1,1,1,1,1,3,1,1,1,1,1,3,1,1,1,1,1,1,1,1},
                                                {1,1,1,1,1,3,3,1,1,1,1,3,1,1,1,1,1,1,1,1},
                                                {1,1,1,1,1,1,3,1,1,1,1,3,3,3,3,1,1,1,1,1},
                                                {1,1,1,1,1,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1},
                                                {1,1,1,1,1,1,4,1,1,1,1,1,1,1,3,1,1,1,1,1},
                                                {1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,1,1,1,1},
                                                {1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,3,1,1,1,1},
                                                {1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,3,1,1,1,1},
                                                {1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,3,1,1,1,1},
                                                {1,1,1,1,1,1,3,3,1,1,1,1,1,1,1,3,1,1,1,1},
                                                {1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,3,1,1,1,1},
                                                {1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,4,1,1,1,1},
                                                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
                                            };

        m1.x=POSITION_M_DEP_X,m1.y=POSITION_M_DEP_Y,m1.direction=1;
        m2.x=POSITION_M_DEP_X,m2.y=POSITION_M_DEP_Y,m2.direction=2;
        m3.x=POSITION_M_DEP_X,m3.y=POSITION_M_DEP_Y,m3.direction=3;
        m4.x=POSITION_M_DEP_X,m4.y=POSITION_M_DEP_Y,m4.direction=4;

        gauche=0,droite=0,haut=0,bas=0,score=0,resultat=3;
        pointTotal=nombrePoint(terrain);


        do
        {

            fenetre.clear();
            affichageTerrain(&fenetre,terrain);
            affichageObjet(&fenetre,terrain);
            affichageTexteVariable(police,score,&fenetre,texteScore,600,200,30);

            spPacMan.setPosition(pacMan.x,pacMan.y);
            spM1.setPosition(m1.x,m1.y);
            spM2.setPosition(m2.x,m2.y);
            spM3.setPosition(m3.x,m3.y);
            spM4.setPosition(m4.x,m4.y);

            fenetre.draw(spPacMan);
            fenetre.draw(spM1);
            fenetre.draw(spM2);
            fenetre.draw(spM3);
            fenetre.draw(spM4);


            while (fenetre.pollEvent(evenement))
            {

                switch (evenement.type)
                {

                case Event::Closed:
                    fenetre.close();
                    break;

                case Event::KeyPressed:
                    if (evenement.key.code == Keyboard::Right)
                    {
                        droite=1,bas=0,haut=0,gauche=0;
                        break;
                    }
                    if (evenement.key.code == Keyboard::Left)
                    {
                        gauche=1,droite=0,bas=0,haut=0;
                        break;
                    }
                    if (evenement.key.code == Keyboard::Up)
                    {
                        haut=1,gauche=0,droite=0,bas=0;
                        break;
                    }
                    if (evenement.key.code == Keyboard::Down)
                    {
                        bas=1,haut=0,gauche=0,droite=0;
                        break;
                    }
                }
            }


            collisionPacObjet(pacMan.x,pacMan.y,spPacMan,&pacMan.collision,terrain,&fenetre,&score,&threadTemp,inviOn);

            deplacementEnnemi(spM1,terrain,&m1.x,&m1.y,&m1.collision,&m1.direction,&m1.oldX,&m1.oldY);
            deplacementEnnemi(spM2,terrain,&m2.x,&m2.y,&m2.collision,&m2.direction,&m2.oldX,&m2.oldY);
            deplacementEnnemi(spM3,terrain,&m3.x,&m3.y,&m3.collision,&m3.direction,&m3.oldX,&m3.oldY);
            deplacementEnnemi(spM4,terrain,&m4.x,&m4.y,&m4.collision,&m4.direction,&m4.oldX,&m4.oldY);

            collisionPacEnnemi(&boucleJeu,spPacMan,&spM1,&resultat,inviOn,&m1.x,&m1.y);
            collisionPacEnnemi(&boucleJeu,spPacMan,&spM2,&resultat,inviOn,&m2.x,&m2.y);
            collisionPacEnnemi(&boucleJeu,spPacMan,&spM3,&resultat,inviOn,&m3.x,&m3.y);
            collisionPacEnnemi(&boucleJeu,spPacMan,&spM4,&resultat,inviOn,&m4.x,&m4.y);

            imageFantome(inviOn,&spM1,&imageM1,&imagePeur);
            imageFantome(inviOn,&spM2,&imageM2,&imagePeur);
            imageFantome(inviOn,&spM3,&imageM3,&imagePeur);
            imageFantome(inviOn,&spM4,&imageM4,&imagePeur);

            victoire(pointTotal,score,&resultat,&boucleJeu);

            if (droite==1)
            {
                if (pacMan.collision==1)
                {
                    pacMan.y=pacMan.oldY;
                    pacMan.x=pacMan.oldX;
                    droite=0;
                    pacMan.collision=0;
                }

                else if (pacMan.collision==0)
                {
                    pacMan.oldX=pacMan.x;
                    pacMan.oldY=pacMan.y;
                    pacMan.x+=PAS_PAC;

                }




            }
            if (gauche==1)
            {
                if (pacMan.collision==1)
                {
                    pacMan.y=pacMan.oldY;
                    pacMan.x=pacMan.oldX;
                    gauche=0;
                    pacMan.collision=0;
                }
                else if (pacMan.collision==0)
                {
                    pacMan.oldX=pacMan.x;
                    pacMan.oldY=pacMan.y;
                    pacMan.x-=PAS_PAC;

                }



            }
            if (haut==1)
            {
                if (pacMan.collision==1)
                {
                    pacMan.y=pacMan.oldY;
                    pacMan.x=pacMan.oldX;
                    haut=0;
                    pacMan.collision=0;
                }
                else if (pacMan.collision==0)
                {
                    pacMan.oldY=pacMan.y;
                    pacMan.oldX=pacMan.x;
                    pacMan.y-=PAS_PAC;
                }



            }
            if (bas==1)
            {
                if (pacMan.collision==1)
                {
                    pacMan.y=pacMan.oldY;
                    pacMan.x=pacMan.oldX;
                    bas=0;
                    pacMan.collision=0;

                }
                else if (pacMan.collision==0)
                {
                    pacMan.oldY=pacMan.y;
                    pacMan.oldX=pacMan.x;
                    pacMan.y+=PAS_PAC;
                }


            }

            fenetre.display();
            sleep(milliseconds(9));

        }
        while(boucleJeu==0);

        fenetre.clear();


        if (resultat==1)
        {
            //affichageTexte(police,fenetre,texte,x,y,taille)
            affichageTexte(police,&fenetre,texteVictoire,340,300,30);
            //affichageTexteVariable(police,variable,fenetre,texte,x,y,taille)
            affichageTexteVariable(police,score,&fenetre,texteScore,350,350,30);
        }
        if (resultat==0)
        {
            affichageTexte(police,&fenetre,textePerdu,340,300,30);
            affichageTexteVariable(police,score,&fenetre,texteScore,350,350,30);
        }
        fenetre.display();
        do
        {
            while (fenetre.pollEvent(evenement))
            {

                switch (evenement.type)
                {

                case Event::Closed:
                    fenetre.close();
                    break;

                case Event::KeyPressed:
                    if (evenement.key.code == Keyboard::Right)
                    {
                        boucleMenuFin=1;

                        break;
                    }
                    if (evenement.key.code == Keyboard::Left)
                    {
                        fenetre.close();
                        break;
                    }
                }
            }



        }
        while(boucleMenuFin==0);

    }

    while(boucleMenu==0);

}
