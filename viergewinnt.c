/* viergewinnt.c 
Vier gewinnt Spiel mit Computergegner in C
v1.0 vom 14.12.2008 */ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define RESET		0
#define BRIGHT 		1
#define DIM		2
#define BLACK 		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define	WHITE		7
char feld [7] [6]; /*Array zur Darstellung des Spielfeldes*/
int anzahlv [7]; /*Array zur Darstellung der noch vertikalen freien Plätze*/
int anzahlh [6]; /*Array zur Darstellung der noch horizontal freien Plätze*/
int bewertung[11]; /*Bewertungsarray für den PC - je höher die Zahl pro Spalte desto höher die Priorität*/
int debug=-1;
void textcolor(int attr, int fg, int bg)/*Funktion zum Setzen der Farben in der Konsole*/
{	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}
int Bewertung (int h, int g, int j, int i, int o, int mode, int zahlj, int zahli, char c)/*Funktion zur Bewertung für den PC*/
{
/*j,i als Bezugspunkt für die Prüfung, h,g als Variablen zum Verschieben der Koordinaten, o als Zähler, Mode gibt an auf wieviele Steine geprüft wird, zahlj und zahli die Verschiebungsrichtung und c zum merken welches Zeichen möglicherweise bereits gefunden wurde*/
	if ((feld [h] [g] == feld [j] [i] && feld [j] [i]!='.') || (feld [h] [g]==feld [j+zahlj] [i+zahli] && feld [j+zahlj] [i+zahli]!='.' && feld [j] [i]=='.' && (feld [j+zahlj] [i+zahli]==c || c=='0' || c=='.' || c=='1')))
	{
		o++;
		if (o==mode) 
/*Je nach Prüfungsmodus wird geprüft ob 4 Steine in einer Reihe liegen oder ob 1,2 oder 3 Steine bereits in einer Reihe liegen (PC-Prüfung)*/
		{
			if (mode==3)/*Die Bewertung folgt je Modus analog*/
			{
				o=0;
				if (feld [j] [i]=='O')
/*Wird bei einer normalen Prüfung ein Viererpaar gefunden, so wird der Asciiwert des Zeichens zurückgegeben und die Koordinaten und die momentane Verschiebung werden im Array Bewertung gespeichert*/
				{
					bewertung [8]=zahlj;
					bewertung [9]=zahli;
					bewertung [10]=j;
					bewertung [11]=i;
					return 79;
				}
				if (feld [j] [i]=='X')
				{
					bewertung [8]=zahlj;
					bewertung [9]=zahli;
					bewertung [10]=j;
					bewertung [11]=i;
					return 88;
				}
				if (feld [j] [i]=='.')/*Ist das Ausgangsfeld leer so prüft der PC und eine Bewertung wird abgegeben*/
				{
					if (feld [j+zahlj] [i+zahli]=='X')
						o+=120450;
					else
						o+=17207;
				}
			}
			if (mode==2)/*Bei Mode 2 und 1 prüft ebenfalls der PC*/
			{
				o=0;
				if ((j+(3*zahlj)>=0 && i+(3*zahli)>=0 && j+(3*zahlj)<7 && i+(3*zahli)<7) || (j-(3*zahlj)>=0 && j-(3*zahlj)<7 && i+(3*zahli)>=0 && i+(3*zahli)<7) || (j-(2*zahlj)>=0 && j-(2*zahlj)<7 && i-(2*zahli)>=0 && i+(3*zahli)<7))
				{
					if ((feld [j+(3*zahlj)] [i+(3*zahli)]=='.' && ((j+(3*zahlj))==anzahlv[(j+(3*zahlj))]-1) && c!='O' && c!='X' && c!='.') || (feld [j-(3*zahlj)] [i-(3*zahli)]=='.' && ((j-(3*zahlj))==anzahlv[(j-(3*zahlj))]-1) && c!='X' && c!='O' && c!='.') || (feld [j-(2*zahlj)] [i-(2*zahli)]=='.'  && feld [j+(2*zahlj)] [i+(2*zahli)]=='.' && (c=='O' || c=='X'))) /*Prüfung auf Gewinnmöglichkeit des Gegners oder eigene Gewinnmöglcihkeit im nächsten Zug bei 2 gelegten Steinen*/
					{
						if (feld [j+zahlj] [i+zahli]=='O')
							o+=17207;
						if (feld [j+zahlj] [i+zahli]=='X')
 							o+=5735;
					}
				}
				else if (feld [j+zahlj] [i+zahli]=='O')
					o+=2458;
				else if (feld [j+zahlj] [i+zahli]=='X')
					o+=351;
			}
			if (mode==1)
			{
				o=0;
				if (feld [j+zahlj] [i+zahli]=='O')
					o+=50;
				if (feld [j+zahlj] [i+zahli]=='X')
					o+=7;
			}
		}
	}
	else /*Wird ein anderer Stein gefunden wird h auf 8 gesetzt und die Schleife abgebrochen*/
		o=-1;
return o;
}

int Pruefung (int j,int i, int mode, int pc)/*Funktion zur Überprüfung sowohl ob 4er Paare vorliegen als auch für den PC ob 3er, 2er... usw. Paare vorliegen*/
{/*j,i als Koordinaten, mode als Anzahl der zu prüfenden Steine, pc um zu ermitteln um auf Unentschieden geprüft werden muss*/
	int o=0,h,g=i,zahl=0,summe=0;
	char c='0';
	if (i<(6-mode)) /*Wenn in der Spalte j weniger als 4 Steine liegen, kann es vertikal kein 4er Paar geben*/
	{
		while (g>=0 && g<5 && g<(i+mode))/*Liegen mehr als 4 Steine in der Spalte, so wird zunächst geprüft ob unter dem gelegten Stein weitere gleiche Steine liegen*/
		{
			g++;
			zahl=Bewertung(j,g,j,i,o,mode,0,1,c);
			if (zahl!=-1)
			{
				o++;
				if (zahl==79 || zahl==88)
					return zahl;
				else if (zahl>6)
				{
					summe+=zahl;
					g=8;
				}
			}
			else 
				g=8;
		}
	}
	if ((anzahlh[i]-pc)<(6-mode))/*Prüfung auf horizontale 4er Paare: Wenn weniger als 4 Steine in der Zeile liegen kann kein 4er Paar entstehen*/
	{
		o=0;
		h=j;
		while (h>=0 && h<6 && h<(j+mode))/*Liegen mehr als 4 Steine in der Zeile, so wird geprüft ob rechts neben dem gelegten Stein weitere gleiche Steine liegen*/
		{
			h++;
			zahl=Bewertung(h,i,j,i,o,mode,1,0,c);
			if (zahl!=-1)
			{
				o++;
				if (zahl==79 || zahl==88)
					return zahl;
				else if (zahl>6)
				{
					summe+=zahl;
					h=8;
				}
			}
			else /*Wird ein anderer Stein gefunden wird h auf 8 gesetzt und die Schleife abgebrochen*/
			{
				if (feld [h] [g]=='.')
					c='.';
				h=8;
			}
		}
		h=j;
		if (o>0 && feld [j-1] [i] == feld [j+1] [i])/*Wenn ein oder mehrere Stein(e) gefunden wurde(n) und sie mit dem Stein gegenüber übereinstimmen bleibt der Zähler o bestehen*/
			c=feld[j+1] [i];
		else if (c!='.')/*Andernfalls wird er zurückgesetzt*/
		{
			c='1';
			o=0;
		}
		while (h>0 && h<7 && h>(j-mode))/*Prüfung ob links neben dem gelegten Stein weitere gleiche Steine liegen*/
		{
			h--;
			zahl=Bewertung(h,i,j,i,o,mode,-1,0,c);
			if (zahl!=-1)
			{
				o++;
				if (zahl==79 || zahl==88)
					return zahl;
				else if (zahl>6)
				{
					summe+=zahl;
					h=8;
				}
			}
			else /*Wird ein anderer Stein gefunden wird h auf 8 gesetzt und die Schleife abgebrochen*/
				h=8;
		}
	}
	o=0;
	h=j;
	g=i;
	c='0';
	while (h>=0 && h<6 && h<(j+mode) && g<5 && g>=0 && g<(i+mode)) /*Prüfung auf die rechte untere Diagonale*/
	{
		h++;
		g++;
		zahl=Bewertung(h,g,j,i,o,mode,1,1,c);
		if (zahl!=-1)
		{
			o++;
			if (zahl==79 || zahl==88)
				return zahl;
			else if (zahl>6)
			{
					summe+=zahl;
					h=8;
			}
			}
		else /*Wird ein anderer Stein gefunden wird h auf 8 gesetzt und die Schleife abgebrochen*/
		{
			if (feld [h] [g]=='.')
				c='.';
			h=8;
		}
	}
	h=j;
	g=i;
	if (o>0 && feld [j+1] [i+1]==feld [j-1] [i-1])/*Wenn ein oder mehrere Stein(e) gefunden wurde(n) und sie mit dem Stein gegenüber übereinstimmen bleibt der Zähler o bestehen*/
		c=feld[j+1] [i+1];
	else if (c!='.')/*Andernfalls wird er zurückgesetzt*/
	{
		c='1';
		o=0;
	}
	while (h>0 && h<7 && h>(j-mode) && g<6 && g>0 && g>(i-mode)) /*Prüfung auf die linke obere Diagonale*/
	{
		h--;
		g--;
		zahl=Bewertung(h,g,j,i,o,mode,-1,-1,c);
		if (zahl!=-1)
		{
			o++;
			if (zahl==79 || zahl==88)
				return zahl;
			else if (zahl>6)
			{
					summe+=zahl;
					h=8;
			}
		}
		else /*Wird ein anderer Stein gefunden wird h auf 8 gesetzt und die Schleife abgebrochen*/
			h=8;
	}
	o=0;
	h=j;
	g=i;
	c='0';
	while (h>=0 && h<6 && h<(j+mode) && g<6 && g>0 && g>(i-mode)) /*Prüfung auf die rechte obere Diagonale*/
	{
		h++;
		g--;
		zahl=Bewertung(h,g,j,i,o,mode,1,-1,c);
		if (zahl!=-1)
		{
			o++;
			if (zahl==79 || zahl==88)
				return zahl;
			else if (zahl>6)
			{
					summe+=zahl;
					h=8;
			}
		}
		else /*Wird ein anderer Stein gefunden wird h auf 8 gesetzt und die Schleife abgebrochen*/
		{
			if (feld [h] [g]=='.')
				c='.';
			h=8;
		}
	}
	h=j;
	g=i;
	if (o>0 && feld [j+1] [i-1]==feld [j-1] [i+1])/*Wenn ein oder mehrere Stein(e) gefunden wurde(n) und sie mit dem Stein gegenüber übereinstimmen bleibt der Zähler o bestehen*/
		c=feld[j+1] [i-1];
	else if (c!='.')/*Andernfalls wird er zurückgesetzt*/
	{
		c='1';
		o=0;
	}
	while (h>0 && h<7 && h>(j-mode) && g<5 && g>=0 && g<(i+mode)) /*Prüfung auf die linke untere Diagonale*/
	{
		h--;
		g++;
		zahl=Bewertung(h,g,j,i,o,mode,-1,1,c);
		if (zahl!=-1)
		{
			o++;
			if (zahl==79 || zahl==88)
				return zahl;
			else if (zahl>6)
			{
					summe+=zahl;
					h=8;
			}
		}
		else /*Wird ein anderer Stein gefunden wird h auf 8 gesetzt und die Schleife abgebrochen*/
			h=8;
	}
	if (pc==0)/*Wenn die Prüfung nicht vom PC vorgenommen wird, so wird geprüft ob alle Spalten voll sind und ein Unentschieden vorliegt*/
	{
	o=0;
	h=0;
	while (o<7)
	{
		if (anzahlv[o]==0)
			h++;
		o++;	
	}
	if (h==7)/*Sind alle Spalten gefüllt wird der Wert 86 für Unentschieden zurückgegeben*/
		return 86;
	}
	return summe;
}

int PC (void)/*Funktion für die Handlungen des PC*/
{/*j ist Zähler für die Spalten, i für die zu prüfenden Steine, l gibt die Spalte mit dem höchsten Wert an, bmax gibt die höchsten Bewertung an und ergebnis ist die Rückgabevariable*/
	int j=0,bmax=-100000,i,l,ergebnis=0;
	printf("\nJetzt ist der Computer an der Reihe\n");
	while (j<7)/*In den Spalten 0-6 wird die Prüfung durchgeführt und die Bewertungen notiert*/
	{
		i=3;
		if (anzahlv[j]>0)/*Wenn die Spalte noch nicht voll ist wird sie bewertet*/
		{
			bewertung[j]=0;
			while (i>0)/*i gibt die Prüfung an (auf 3 Steine, 2 Steine, einen Stein...)*/
			{
				bewertung[j]+=(Pruefung(j,(anzahlv[j]-1),i,1));/*Das Ergebnis wird in bewertung [j] gespeichert*/
				if (bewertung[j]<17207)/*Wenn bewertung[j] nicht jeweils die höchste Bewertungsstufe hat wird geprüft ob links oder rechts der Spalte mehr Steine liegen als in Spalte [j]*/
				{
					if (((anzahlv[j-1]<=(anzahlv[j]-1) || anzahlv[j+1]<=(anzahlv[j]-1)) && (i==3 && anzahlv[j]>1)))
					{/*Ist dies der Fall wird die Prüfung wiederholt und von der Bewertung abgezogen*/
						ergebnis=Pruefung(j,(anzahlv[j]-2),i,1);
						bewertung[j]-=ergebnis;
						if (ergebnis>=120450 && ergebnis%120450==0 && anzahlv[j]>2)/*Wurde 120450 oder ein vielfaches davon abgezogen und ist noch 2 Stellen über der einzuwerfenden Platz, so wird die Prüfung für diese Stelle ebenfalls noch einmal durchgeführt*/
						{
							ergebnis=Pruefung(j,(anzahlv[j]-3),i,1);
							if(ergebnis>=120450);
								bewertung[j]=17206;
							if (ergebnis<120450 && ergebnis>=17207)
								bewertung[j]=-99000;
						}	
					}/*Ist dies der Fall wird die Bewertung für das nächst höhere Loch ermittelt und abgezogen bzw. addiert*/
				}
				else
					i=1;
				i--;

			}
			if (bewertung[j]<-99100)
				bewertung[j]=-99100;
			if (bewertung[j]>bmax)/*In bmax wird die höchste Bewertung des Bewertungsarrays gespeichert*/
			{
				bmax=bewertung[j];
				l=j;/*Die Spaltennummer der höchsten Bewertung wird in l abgelegt*/
				if (bmax>=120450)/*Wenn bmax größer oder gleich der größten Bewertung ist wird die Prüfung abgebrochen*/
					j=6;
			}
		}
		else
			bewertung[j]=-1000000;/*Ist die Spalte voll bekommt sie die niedrigste Bewertung*/
		j++;
	}
	while (bmax==0)/*Ist bmax=0 so wurde noch kein Stein in eines der Felder geworfen (am Anfang des Spiels). Die Randomfunktion entscheidet dann über die Spalten 3,4 oder 5 weil sie die besten Chancen für einen Gewinn liefern*/
	{
		l=rand()%2;
		l+=3;
		if (anzahlv[l]>0)
			bmax=1;
	}
	(anzahlv[l])--;
	feld [l] [(anzahlv[l])]='X';
	(anzahlh[(anzahlv[l])])--;
	ergebnis=Pruefung(l, anzahlv[l],3,0);/*Nachdem der Zug getätigt wurde wird die Prüfungsfunktion aufgerufen und das Ergebnis an Spiel() zur�ckgegeben*/
	return ergebnis;
}
int Eingabe (char n [20], int s)/*Eingabefunktion für den/die Spieler*/
{
int eingabe=8,ergebnis;
char c;
	printf("\n%s du bist dran!\n",n);
	do /*Bei korrekter Eingabe wird überprüft ob man noch in die gewählte Spalte einwerfen kann*/
	{
		do /*Eingabeüberprüfung*/
		{
			if (scanf("%d", &eingabe) > 0)/*Scanf gibt den Wert 0 bei fehlerhafter Eingabe zurück*/
			{
				if (eingabe==0)/*Eingabe 0 soll das Spiel mit Abfrage abbrechen*/
				{
					printf("\nMöchten Sie das Spiel wirklich abbrechen? (j/n)\n");
					do
					{
						if (scanf("%c", &c)>0)
						{
							if (c=='j')
								return 87;
							if (c=='n')
								break;	
						}
						else
							scanf("%*s");
					}
					while (c!='y' || c!='n');
				}
				if(eingabe<1 || eingabe>7)/*Ist t>7 oder kleiner als 0 so ist die Eingabe fehlerhaft*/
				{
					printf("\nBitte eine Zahl zwischen 1 und 7 eingeben!\n");
					eingabe=8;
				}
			}
			else
			{
				printf("\nBitte eine Zahl zwischen 1 und 7 eingeben!\n"); /*Ist die Eingabe fehlerhaft so wird der Eingabepuffer geleert*/
				scanf("%*s");
			}
		}
		while (eingabe==8);
		eingabe--;
		if (anzahlv[eingabe]>0)/*Wenn in der Spalte noch Platz ist, so darf ein Stein eingeworfen werden*/
		{
			(anzahlv[eingabe])--;
			if (s==1)/*Prüfung auf Spieler 1 oder Spieler 2*/
				feld [eingabe] [(anzahlv[eingabe])]='O';
			else
				feld [eingabe] [(anzahlv[eingabe])]='X';
			(anzahlh[(anzahlv[eingabe])])--;
		}
		else /*Andernfalls wird eine Fehlermeldung ausgegeben*/
		{
			printf("\nDie gewählte Spalte ist bereits voll!\n\n");
			eingabe=8;
		}
	} 
	while (eingabe==8);
	ergebnis=Pruefung(eingabe,anzahlv[eingabe],3,0);
	return ergebnis;
}

int Ausgabe (g,h,k,l) /*Ausgabefunktion zur Darstellung des Spielfeldes nach jedem Zug*/
{/*k und l sind die Koordinaten des letzten Zuges bei Gewinn des Spieles und g und h die Verschiebung zur Ermittlung des Gewinners*/
	int i=0,j=0;/*Zählervariablen*/
	char c='a';
	if (debug==1)/*Im Debugmodus werden die Bewertungen der Spalten mit ausgegeben*/
	{
		printf("\n  "); 
		while (j<7)
		{
			printf("%d ",bewertung[j]);
			j++;
		}
		j=0;
		printf("\n");
	}
	printf("\n  1  2  3  4  5  6  7");
	while (i<6)
	{
		printf("\n%c ",c);
		c++;
		while (j<7)
		{
			textcolor(BRIGHT, BLUE, BLACK);/*Die Farbgebungsfunktion wird je nach benötigtem Fall aufgerufen*/
			if (feld [j] [i]=='X')
				textcolor(BRIGHT, RED, BLACK);
			if (feld [j] [i]=='O')
				textcolor(BRIGHT, YELLOW, BLACK);
			if (k!=-1)
			{
				if (j==k && i==l)
					textcolor(BRIGHT, GREEN, BLACK);/*Die grüne Farbgebung dient dazu die Gewinnersteine einzufärben*/
				else if (((j==(k+g) && i==(l+h)) || (j==(k+(2*g)) && i==(l+(2*h))) || (j==(k+(3*g)) && i==(l+(3*h) ))) && feld [j] [i] == feld [k] [l] && feld [j-g] [i-h] == feld [k] [l])
					textcolor(BRIGHT, GREEN, BLACK);
				else if ((((j==(k-g)) && i==(l-h)) || (j==(k-(2*g)) && i==(l-(2*h))) || (j==(k-(3*g)) && i==(l-(3*h)))) && feld [j] [i] == feld [k] [l] && feld [j+g] [i+h] == feld [k] [l])
					textcolor(BRIGHT, GREEN, BLACK);
			}
			printf("%c  ",feld [j] [i]);
			textcolor(RESET, WHITE, BLACK);
			j++;
		}
		j=0;
		i++;
	}
	if (k==-1 && l==-1)/*Spiel abbrechen soll nur ausgegeben werden solange das Spiel noch läuft*/
		printf("\n\n0: Spiel abbrechen");
	printf("\n");
	return 0;
}

int Spiel (int Spiel) /*Funktion zum Aufrufen der einzelnen Spielfunktionen*/
{
	char name [20],name2 [20]; /*g ist Indikator ob bereits gewonnen wurde, name und name2 sind Arrays für die Spielernamen*/
	int i=0,j=0,g='a';
	printf("\nSpieler 1 gib deinen Namen (max 20 Zeichen) ein:\n");
	scanf("%s",name);
	if (Spiel==2)/*Wird im Hotseatmodus gespielt so wird ein zweiter Spielername abgefragt*/
	{
		printf("\nSpieler 2 gib deinen Namen (max 20 Zeichen) ein:\n");
		scanf("%s",name2);
	}
	printf("\n  1  2  3  4  5  6  7");
	while (i<6)/*Anzahlh und Anzahlv sowie das Bewertungsarray werden zur späteren Verwendung gesetzt. Ein leeres Spielfeld wird erstellt und ausgegeben*/
	{
		anzahlh[i]=6;
		printf("\n%c ",g);
		g++;
		textcolor(BRIGHT, BLUE, BLACK);
		while (j<7)
		{
			anzahlv[j]=6;
			bewertung[j]=0;
			feld [j] [i]='.';
			printf("%c  ",feld [j] [i]);
			j++;
		}
		textcolor(RESET, WHITE, BLACK);
		j=0;
		i++;
	}
	printf("\n\n0: Spiel abbrechen\n");
	g=0;
	i=0;
	while (g==0)/*Solange kein Gewinner feststeht ist g=0*/
	{
		if (i==0)/*Auslosung am Start wer das Spiel beginnen darf*/
		{
			printf("\nDie magische Maschine wird nun entscheiden wer das Spiel beginnen darf....\n\n");
			i=rand()%2;
		}
		if (i==1)
			g=Eingabe(name, i);
		i=1;
		if (Spiel==1 && g==0)
			g=PC();
		if (Spiel==2 && g==0)
		{
			i++;
			Ausgabe(0,0,-1,-1);
			g=Eingabe(name2, i);
			i=1;
		}
		if (g!=0)/*Wurde ein Gewinner ermittelt wird g entweder X für den PC oder O für den menschlichen Spieler*/
		{

			if (g==79)
			{
				Ausgabe(bewertung[8],bewertung[9],bewertung[10],bewertung[11]);/*Wurde ein Gewinner ermittelt wird die Ausgabefunktion mit den entsprechenden Koordinaten und der Verschiebung ausgeführt*/
				printf("\n%s hat gewonnen!\n",name);
			}
			if (g==86)
			{
				Ausgabe(0,0,-1,0);
				printf("\nUnentschieden!\n");
			}
			if (g==88)
			{
				Ausgabe(bewertung[8],bewertung[9],bewertung[10],bewertung[11]);
				if (Spiel==2) 
					printf("\n%s hat gewonnen!\n",name2);
				else
					printf("\nDer Computer hat gewonnen!\n");
			}
		}
		else
			Ausgabe(0,0,-1,-1);/*Ist noch kein Gewinner ermittelt, so wird die Ausgabefunktion mit Werten ausgeführt die keine Grünfärbung bewirken*/
	}
	return 0;
}

int main (void)
{
	int menu=0;/*Variable zum Abfragen der Menüpunkte*/
	srand( time(NULL) );
	textcolor(DIM, RED, BLACK);
	printf("\nVier gewinnt 1.0\nAutor: Timo Homburg\nLizensiert unter GPLv3\n");
	textcolor(RESET, WHITE, BLACK);
	do
	{
		textcolor(DIM, YELLOW, BLACK);
		printf("\n1: Neues Spiel gegen den PC\n\n2: Neues Spiel: Spieler gegen Spieler (Hotseat)\n\n3: Spielregeln\n\n4: Spiel beenden\n\n");
		textcolor(RESET, WHITE, BLACK);
		if (scanf("%d",&menu)>0)
		{
			if (menu==1)
				Spiel(1);
			if (menu==2)
				Spiel(2);
			if (menu==3)
				printf("\nVersuche gegen den Computer oder einen Mitspieler möglichst schnell 4 Steine\nvertikal, horizontal oder diagonal in eine Reihe zu setzen.\nWer dies als erstes schafft gewinnt!\n");
			if (menu==5)
				debug*=-1;
		}
		else
			scanf("%*s");
	}
	while (menu!=4);
	return 0;
}
