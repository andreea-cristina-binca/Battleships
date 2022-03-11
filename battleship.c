#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

void initmeniu();
int optiune(int poz);
void initalegharta(int argc);
void alegharta(int **argharti, int **auxiliar, int argc);
void newgame(int **auxiliar);
void resumegame(FILE *resume);
void gameplay(int **player, int **computer, FILE *resume);
void genhartacomp(int **auxiliar);
void nave4(int **auxiliar, int nr);
void nave3(int **auxiliar, int nr);
void nave2(int **auxiliar, int nr);
void nave1(int **auxiliar, int nr);
void bordarenave(int **auxiliar);
int alegereplayer(int **computer, int *coord);
int alegerecomputer(int **player);
void randomise(int **player);
int destroy(int **player, int **computer);
int navedistruse(int **matrice);

int main(int argc, char *argv[])
{
	//verificare argumente de comanda
	if (argc < 2)
	{
		printf("[Eroare]: Nu s-au dat argumente de comanda.\n");
		return 1;
	}
	//verificare deschidere fisiere
	int i;
	FILE * harta;
	for (i = 1; i < argc; i++)
	{
		harta = fopen(argv[i], "r");
		if (harta == NULL)
		{
			printf("Fisierul %s nu poate fi deschis.\n", argv[i]);
			return 1;
		}
		fclose(harta);
	}
	//memorare harti
	int **argharti, **auxiliar, adun = 0, nrmat = argc - 1;
	argharti = (int **) malloc((nrmat *10) *sizeof(int*));
	for (i = 0; i < nrmat * 10; i++)
		argharti[i] = (int*) calloc(10, sizeof(int));
	auxiliar = (int **) malloc(10* sizeof(int*));
	for (i = 0; i < 10; i++)
		auxiliar[i] = (int*) calloc(10, sizeof(int));
	for (i = 1; i < argc; i++)
	{
		int l, c;
		harta = fopen(argv[i], "r");
		while (fscanf(harta, "%d %d", &l, &c) != EOF)
			argharti[l + adun][c] = 1;
		adun = adun + 10;
		fclose(harta);
	}
	//initializare fereastra grafica
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
	init_pair(2, COLOR_GREEN, COLOR_BLUE);
	init_pair(3, COLOR_MAGENTA, COLOR_CYAN);
	init_pair(4, COLOR_RED, COLOR_BLUE);
	bkgd(COLOR_PAIR(1));
	//deschidere meniu cu optiuni
	FILE * resume;
	int poz = 2;
	while (poz != 4)
	{
		poz = 2;
		poz = optiune(poz);
		switch (poz)
		{
			case 2:
				alegharta(argharti, auxiliar, argc);
				newgame(auxiliar);
				clear();
				break;
			case 3:
				resume = fopen("resume.txt", "r+");
				resumegame(resume);
				fclose(resume);
				clear();
				break;
			case 4:
				endwin();
				break;
		}
	}
	for (i = 0; i < nrmat * 10; i++)
		free(argharti[i]);
	free(argharti);
	for (i = 0; i < 10; i++)
		free(auxiliar[i]);
	free(auxiliar);
	return 0;
}

void initmeniu()
{
	attron(COLOR_PAIR(2) | A_BOLD);
	mvprintw(0, 5, "BATTLESHIPS\n\n");
	attroff(COLOR_PAIR(2) | A_BOLD);
	mvprintw(2, 3, " ");
	mvprintw(2, 5, "New Game\n");
	mvprintw(3, 3, " ");
	mvprintw(3, 5, "Resume Game\n");
	mvprintw(4, 3, " ");
	mvprintw(4, 5, "Quit\n");
	refresh();
}

int optiune(int poz)
{
	initmeniu();
	attron(COLOR_PAIR(2) | A_BOLD);
	mvprintw(2, 3, ">");
	mvprintw(2, 5, "New Game\n");
	attroff(COLOR_PAIR(2) | A_BOLD);
	refresh();
	int ch = getch();
	while ((char) ch != '\n')
	{
		switch (ch)
		{
			case KEY_DOWN:
				initmeniu();
				attron(COLOR_PAIR(2) | A_BOLD);
				switch (poz)
				{
					case 4:
						mvprintw(2, 3, ">");
						mvprintw(2, 5, "New Game\n");
						poz = 2;
						break;
					case 2:
						mvprintw(3, 3, ">");
						mvprintw(3, 5, "Resume Game\n");
						poz = 3;
						break;
					case 3:
						mvprintw(4, 3, ">");
						mvprintw(4, 5, "Quit\n");
						poz = 4;
						break;
				}
				attroff(COLOR_PAIR(2) | A_BOLD);
				refresh();
				break;
			case KEY_UP:
				initmeniu();
				attron(COLOR_PAIR(2) | A_BOLD);
				switch (poz)
				{
					case 3:
						mvprintw(2, 3, ">");
						mvprintw(2, 5, "New Game\n");
						poz = 2;
						break;
					case 4:
						mvprintw(3, 3, ">");
						mvprintw(3, 5, "Resume Game\n");
						poz = 3;
						break;
					case 2:
						mvprintw(4, 3, ">");
						mvprintw(4, 5, "Quit\n");
						poz = 4;
						break;
				}
				attroff(COLOR_PAIR(2) | A_BOLD);
				refresh();
				break;
			default:
				break;
		}
		ch = getch();
	}
	refresh();
	return poz;
}

void bordarenave(int **auxiliar)
{
	int i, j;
	//colturi
	if (auxiliar[0][0] == 1)
	{
		if (auxiliar[0][1] == 0) auxiliar[0][1] = 2;
		if (auxiliar[1][1] == 0) auxiliar[1][1] = 2;
		if (auxiliar[1][0] == 0) auxiliar[1][0] = 2;
	}
	if (auxiliar[0][9] == 1)
	{
		if (auxiliar[0][8] == 0) auxiliar[0][8] = 2;
		if (auxiliar[1][8] == 0) auxiliar[1][8] = 2;
		if (auxiliar[1][9] == 0) auxiliar[1][9] = 2;
	}
	if (auxiliar[9][0] == 1)
	{
		if (auxiliar[9][1] == 0) auxiliar[9][1] = 2;
		if (auxiliar[8][1] == 0) auxiliar[8][1] = 2;
		if (auxiliar[8][0] == 0) auxiliar[8][0] = 2;
	}
	if (auxiliar[9][9] == 1)
	{
		if (auxiliar[9][8] == 0) auxiliar[9][8] = 2;
		if (auxiliar[8][8] == 0) auxiliar[8][8] = 2;
		if (auxiliar[8][9] == 0) auxiliar[8][9] = 2;
	}
	//liniile 0 si 9
	for (j = 1; j < 9; j++)
	{
		if (auxiliar[0][j] == 1)
		{
			if (auxiliar[0][j - 1] == 0) auxiliar[0][j - 1] = 2;
			if (auxiliar[0][j + 1] == 0) auxiliar[0][j + 1] = 2;
			if (auxiliar[1][j - 1] == 0) auxiliar[1][j - 1] = 2;
			if (auxiliar[1][j] == 0) auxiliar[1][j] = 2;
			if (auxiliar[1][j + 1] == 0) auxiliar[1][j + 1] = 2;
		}
		if (auxiliar[9][j] == 1)
		{
			if (auxiliar[9][j - 1] == 0) auxiliar[9][j - 1] = 2;
			if (auxiliar[9][j + 1] == 0) auxiliar[9][j + 1] = 2;
			if (auxiliar[8][j - 1] == 0) auxiliar[8][j - 1] = 2;
			if (auxiliar[8][j] == 0) auxiliar[8][j] = 2;
			if (auxiliar[8][j + 1] == 0) auxiliar[8][j + 1] = 2;
		}
	}
	//coloanele 0 si 9
	for (j = 1; j < 9; j++)
	{
		if (auxiliar[j][0] == 1)
		{
			if (auxiliar[j - 1][0] == 0) auxiliar[j - 1][0] = 2;
			if (auxiliar[j - 1][1] == 0) auxiliar[j - 1][1] = 2;
			if (auxiliar[j][1] == 0) auxiliar[j][1] = 2;
			if (auxiliar[j + 1][1] == 0) auxiliar[j + 1][1] = 2;
			if (auxiliar[j + 1][0] == 0) auxiliar[j + 1][0] = 2;
		}
		if (auxiliar[j][9] == 1)
		{
			if (auxiliar[j - 1][9] == 0) auxiliar[j - 1][9] = 2;
			if (auxiliar[j - 1][8] == 0) auxiliar[j - 1][8] = 2;
			if (auxiliar[j][8] == 0) auxiliar[j][8] = 2;
			if (auxiliar[j + 1][8] == 0) auxiliar[j + 1][8] = 2;
			if (auxiliar[j + 1][9] == 0) auxiliar[j + 1][9] = 2;
		}
	}
	//interior matrice
	for (i = 1; i < 9; i++)
		for (j = 1; j < 9; j++)
			if (auxiliar[i][j] == 1)
			{
				if (auxiliar[i - 1][j - 1] == 0) auxiliar[i - 1][j - 1] = 2;
				if (auxiliar[i - 1][j] == 0) auxiliar[i - 1][j] = 2;
				if (auxiliar[i - 1][j + 1] == 0) auxiliar[i - 1][j + 1] = 2;
				if (auxiliar[i][j - 1] == 0) auxiliar[i][j - 1] = 2;
				if (auxiliar[i][j + 1] == 0) auxiliar[i][j + 1] = 2;
				if (auxiliar[i + 1][j - 1] == 0) auxiliar[i + 1][j - 1] = 2;
				if (auxiliar[i + 1][j] == 0) auxiliar[i + 1][j] = 2;
				if (auxiliar[i + 1][j + 1] == 0) auxiliar[i + 1][j + 1] = 2;
			}
}

void nave4(int **auxiliar, int nr)
{
	int lower = 0, upper = 9, i, j, num1 = 10, num2 = 10;
	for (i = 0; i < nr; i++)
	{
		while (num1 > 6)
			num1 = (rand() % (upper - lower + 1)) + lower;
		while (num2 > 6)
			num2 = (rand() % (upper - lower + 1)) + lower;
		int var = (rand() % (1 - 0 + 1)) + 0;
		if (var == 0)
			for (j = num2; j < num2 + 4; j++)
				auxiliar[num1][j] = 1;
		else
			for (j = num1; j < num1 + 4; j++)
				auxiliar[j][num2] = 1;
		bordarenave(auxiliar);
	}
}

void nave3(int **auxiliar, int nr)
{
	int lower = 0, upper = 9, i, j, num1 = 10, num2 = 10;
	for (i = 0; i < nr; i++)
	{
		int verif = 0;
		while (verif == 0)
		{
			num1 = 10;
			num2 = 10;
			verif = 1;
			while (num1 > 7)
				num1 = (rand() % (upper - lower + 1)) + lower;
			while (num2 > 7)
				num2 = (rand() % (upper - lower + 1)) + lower;
			int var = (rand() % (1 - 0 + 1)) + 0;
			if (var == 0)
				if (auxiliar[num1][num2] == 0 && auxiliar[num1][num2 + 1] == 0
					&& auxiliar[num1][num2 + 2] == 0)
					for (j = num2; j < num2 + 3; j++)
						auxiliar[num1][j] = 1;
				else
					verif = 0;
			else
			if (auxiliar[num1][num2] == 0 && auxiliar[num1 + 1][num2] == 0 &&
				auxiliar[num1 + 2][num2] == 0)
				for (j = num1; j < num1 + 3; j++)
					auxiliar[j][num2] = 1;
			else
				verif = 0;
		}
		bordarenave(auxiliar);
	}
}

void nave2(int **auxiliar, int nr)
{
	int lower = 0, upper = 9, i, j, num1 = 10, num2 = 10;
	for (i = 0; i < nr; i++)
	{
		int verif = 0;
		while (verif == 0)
		{
			num1 = 10;
			num2 = 10;
			verif = 1;
			while (num1 > 8)
				num1 = (rand() % (upper - lower + 1)) + lower;
			while (num2 > 8)
				num2 = (rand() % (upper - lower + 1)) + lower;
			int var = (rand() % (1 - 0 + 1)) + 0;
			if (var == 0)
				if (auxiliar[num1][num2] == 0 && auxiliar[num1][num2 + 1] == 0)
					for (j = num2; j < num2 + 2; j++)
						auxiliar[num1][j] = 1;
				else
					verif = 0;
			else
			if (auxiliar[num1][num2] == 0 && auxiliar[num1 + 1][num2] == 0)
				for (j = num1; j < num1 + 2; j++)
					auxiliar[j][num2] = 1;
			else
				verif = 0;
		}
		bordarenave(auxiliar);
	}
}

void nave1(int **auxiliar, int nr)
{
	int lower = 0, upper = 9, i, num1 = 10, num2 = 10;
	for (i = 0; i < nr; i++)
	{
		int verif = 0;
		while (verif == 0)
		{
			num1 = 10;
			num2 = 10;
			verif = 1;
			num1 = (rand() % (upper - lower + 1)) + lower;
			num2 = (rand() % (upper - lower + 1)) + lower;
			if (auxiliar[num1][num2] == 0)
				auxiliar[num1][num2] = 1;
			else
				verif = 0;
		}
		bordarenave(auxiliar);
	}
}

void genhartacomp(int **auxiliar)
{
	int i, j;
	srand(time(0));
	//initializare cu 0
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			auxiliar[i][j] = 0;
	//nava de 4
	nave4(auxiliar, 1);
	//navele de 3
	nave3(auxiliar, 2);
	//navele de 2
	nave2(auxiliar, 3);
	//navele de 1
	nave1(auxiliar, 4);
	//schimbare borduri nave in 0
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			if (auxiliar[i][j] == 2) auxiliar[i][j] = 0;
}

int alegereplayer(int **computer, int *coord)
{
	int i = 0, j = 0, next, a, b, lin = coord[0] - 2, col = coord[1] - 35;
	//se uita inspre dreapta-jos 
	for (a = lin; a < 20; a = a + 2)
		for (b = col; b < 20; b = b + 2)
			if (computer[a][b] != 33 && computer[a][b] != 35)
			{
				i = a;
				j = b;
				a = 20;
				b = 20;
			}
	//se uita spre stanga-jos
	if (i == 0 && j == 0)
		for (a = lin; a < 20 ; a = a + 2)
			for (b = col; b >= 1; b = b - 2)
				if (computer[a][b] != 33 && computer[a][b] != 35)
				{
					i = a;
					j = b;
					a = 20;
					b = 0;
				}
	//se uita spre stanga-sus 
	if (i == 0 && j == 0)
		for (a = lin; a >= 1; a = a - 2)
			for (b = col; b >= 1; b = b - 2)
				if (computer[a][b] != 33 && computer[a][b] != 35)
				{
					i = a;
					j = b;
					a = 0;
					b = 0;
				}
	//se uita spre dreapta-sus
	if (i == 0 && j == 0)
		for (a = lin; a >= 1; a = a - 2)
			for (b = col; b < 20; b = b + 2)
				if (computer[a][b] != 33 && computer[a][b] != 35)
				{
					i = a;
					j = b;
					a = 0;
					b = 20;
				}
	//daca nu gaseste
	if(i==0 && j==0)
		for (a = 1; a < 20; a = a + 2)
			for (b = 1; b < 20; b = b + 2)
				if (computer[a][b] != 33 && computer[a][b] != 35)
				{
					i = a;
					j = b;
					a = 20;
					b = 20;
				}
	lin = i + 2;
	col = j + 35;
	attron(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
	mvprintw(lin, col, " ");
	attroff(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
	refresh();
	int ch = getch();
	while ((char) ch != '\n')
	{
		if (ch == KEY_UP || ch == KEY_DOWN ||
			ch == KEY_LEFT || ch == KEY_RIGHT)
		{
			attron(A_NORMAL);
			mvprintw(lin, col, " ");
			attroff(A_NORMAL);
		}
		switch (ch)
		{
			case KEY_DOWN:
				attron(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				next = 1;
				while (next)
				{
					if ((lin + 2) > 22)
					{
						i = 1;
						lin = 3;
					}
					else
					{
						i = i + 2;
						lin = lin + 2;
					}
					if (computer[i][j] != 33 && computer[i][j] != 35)
						next = 0;
				}
				mvprintw(lin, col, " ");
				attroff(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				refresh();
				break;
			case KEY_UP:
				attron(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				next = 1;
				while (next)
				{
					if ((lin - 2) < 3)
					{
						i = 19;
						lin = 21;
					}
					else
					{
						i = i - 2;
						lin = lin - 2;
					}
					if (computer[i][j] != 33 && computer[i][j] != 35)
						next = 0;
				}
				mvprintw(lin, col, " ");
				attroff(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				refresh();
				break;
			case KEY_LEFT:
				attron(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				next = 1;
				while (next)
				{
					if ((col - 2) < 36)
					{
						j = 19;
						col = 54;
					}
					else
					{
						j = j - 2;
						col = col - 2;
					}
					if (computer[i][j] != 33 && computer[i][j] != 35)
						next = 0;
				}
				mvprintw(lin, col, " ");
				attroff(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				refresh();
				break;
			case KEY_RIGHT:
				attron(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				next = 1;
				while (next)
				{
					if ((col + 2) > 54)
					{
						j = 1;
						col = 36;
					}
					else
					{
						j = j + 2;
						col = col + 2;
					}
					if (computer[i][j] != 33 && computer[i][j] != 35)
						next = 0;
				}
				mvprintw(lin, col, " ");
				attroff(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				refresh();
				break;
			//pt quit
			case 81:
				return -1;
			case 113:
				return -1;
			//pt randomise
			case 82:
				return 2;
			case 114:
				return 2;
			//pt destroy
			case 68:
				attron(A_NORMAL);
				mvprintw(lin, col, " ");
				attroff(A_NORMAL);
				return 3;
			case 100:
				attron(A_NORMAL);
				mvprintw(lin, col, " ");
				attroff(A_NORMAL);
				return 3;
			default:
				break;
		}
		ch = getch();
	}
	refresh();
	if (computer[i][j] == 88)
	{
		attron(COLOR_PAIR(4) | A_STANDOUT);
		mvprintw(lin, col, " ");
		attroff(COLOR_PAIR(4) | A_STANDOUT);
		refresh();
		napms(500);
		attron(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
		mvprintw(lin, col, " ");
		computer[i][j] = 33;
		attroff(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
		coord[0] = lin;
		coord[1] = col;
		return 1;
	}
	else
	{
		attron(COLOR_PAIR(1) | A_STANDOUT);
		mvprintw(lin, col, " ");
		attroff(COLOR_PAIR(1) | A_STANDOUT);
		computer[i][j] = 35;
		coord[0] = lin;
		coord[1] = col;
		return 0;
	}
}

int alegerecomputer(int **player)
{
	int num1 = 0, num2 = 0, upper = 19, lower = 1, verif = 1;
	srand(time(0));
	while (verif)
	{
		num1 = 0;
		num2 = 0;
		while (num1 % 2 == 0)
			num1 = (rand() % (upper - lower + 1)) + lower;
		while (num2 % 2 == 0)
			num2 = (rand() % (upper - lower + 1)) + lower;
		if (player[num1][num2] != 33 && player[num1][num2] != 35)
			verif = 0;
	}
	int lin = num1 + 2, col = num2 + 2;
	if (player[num1][num2] == 88)
	{
		attron(COLOR_PAIR(4) | A_STANDOUT);
		mvprintw(lin, col, " ");
		attroff(COLOR_PAIR(4) | A_STANDOUT);
		refresh();
		napms(500);
		attron(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
		mvprintw(lin, col, " ");
		player[num1][num2] = 33;
		attroff(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
		return 1;
	}
	else
	{
		attron(COLOR_PAIR(1) | A_STANDOUT);
		mvprintw(lin, col, " ");
		attroff(COLOR_PAIR(1) | A_STANDOUT);
		player[num1][num2] = 35;
		return 0;
	}
	refresh();
}

int navedistruse(int **matrice)
{
	int i, j, nr = 0;
	for (i = 1; i < 20; i = i + 2)
	{
		for (j = 1; j < 20; j = j + 2)
		{
			if (matrice[i][j] == 33)
			{
				int l = i, c = j, verif = 1;
				matrice[l][c] = 0;
				//verific daca exista nava pe verticala
				while (l < 19 && verif)
				{
					if (matrice[l + 2][c] == 33)
					{
						l = l + 2;
						matrice[l][c] = 0;
					}
					else
					{
						if (matrice[l + 2][c] == 88)
							verif = 0;
						else
							l = 20;
					}
				}
				//verific daca exista nava pe orizontala
				l = i;
				c = j;
				while (c < 19 && verif)
				{
					if (matrice[l][c + 2] == 33)
					{
						c = c + 2;
						matrice[l][c] = 0;
					}
					else
					{
						if (matrice[l][c + 2] == 88)
							verif = 0;
						else
							c = 20;
					}
				}
				if (verif == 1) nr++;
			}
			if (matrice[i][j] == 88)
			{
				int l = i, c = j, verif = 1;
				matrice[l][c] = 0;
				while (l < 19 && verif)
				{
					if (matrice[l + 2][c] == 33 || matrice[l + 2][c] == 88)
					{
						l = l + 2;
						matrice[l][c] = 0;
					}
					else
						l = 20;
				}
				l = i;
				c = j;
				while (c < 19 && verif)
				{
					if (matrice[l][c + 2] == 33)
					{
						c = c + 2;
						matrice[l][c] = 0;
					}
					else
						c = 20;
				}
			}
		}
	}
	return nr;
}

void randomise(int **player)
{
	int i, j, *nrnave, **hartanoua;
	nrnave = (int*) calloc(4, sizeof(int));
	for (i = 1; i < 20; i = i + 2)
	{
		for (j = 1; j < 20; j = j + 2)
		{
			int nrc = 0;
			if (player[i][j] == 33 || player[i][j] == 88 || player[i][j] == 34)
			{
				int l = i, c = j;
				if (player[l][c] == 88)
				{
					nrc++;
					player[l][c] = 0;
				}
				//verific daca exista nava pe verticala
				while (l < 19)
				{
					if (player[l + 2][c] == 33 || player[l + 2][c] == 88 ||
						player[l + 2][c] == 34)
					{
						l = l + 2;
						if (player[l][c] == 88)
						{
							nrc++;
							player[l][c] = 0;
						}
					}
					else l = 20;
				}
				//verific daca exista nava pe orizontala
				l = i;
				c = j;
				while (c < 19)
				{
					if (player[l][c + 2] == 33 || player[l][c + 2] == 88 ||
						player[l][c + 2] == 34)
					{
						c = c + 2;
						if (player[l][c] == 88)
						{
							nrc++;
							player[l][c] = 0;
						}
					}
					else c = 20;
				}
				if (nrc > 0)
					nrnave[nrc - 1]++;
			}
		}
	}
	//creare harta noua cu navele ramase
	srand(time(0));
	hartanoua = (int **) malloc(10* sizeof(int*));
	for (i = 0; i < 10; i++)
		hartanoua[i] = (int*) calloc(10, sizeof(int));
	nave4(hartanoua, nrnave[3]);
	nave3(hartanoua, nrnave[2]);
	nave2(hartanoua, nrnave[1]);
	nave1(hartanoua, nrnave[0]);
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			if (hartanoua[i][j] == 2) hartanoua[i][j] = 0;
	//formatare harta noua si copiere in harta playerului
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (hartanoua[i][j] == 1)
				player[i + i + 1][j + j + 1] = 88;
		}
	}
	//afisare harta noua 
	for (i = 1; i < 20; i = i + 2)
		for (j = 1; j < 20; j = j + 2)
			mvprintw(i + 2, j + 2, " ");
	for (i = 1; i < 20; i = i + 2)
	{
		for (j = 1; j < 20; j = j + 2)
		{
			if (player[i][j] == 33 || player[i][j] == 34)
			{
				attron(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
				mvprintw(i + 2, j + 2, "X");
				attroff(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
				player[i][j] = 34;
			}
				if(player[i][j]==35)
				{
					mvprintw(i + 2, j + 2, " ");
					player[i][j]=32;
				}
					if(player[i][j]==88)
					{
						attron(COLOR_PAIR(3) | A_BOLD | A_STANDOUT);
						mvprintw(i + 2, j + 2, " ");
						attroff(COLOR_PAIR(3) | A_BOLD | A_STANDOUT);
					}
		}
	}
	free(nrnave);
	free(hartanoua);
}

int destroy(int **player, int **computer)
{
	int i, j, k, num1, num2, upper = 19, lower = 1, verif, endgame;
	srand(time(0));
	for (k = 0; k < 10; k++)
	{
		//mutare player
		verif = 1;
		while (verif)
		{
			num1 = 0;
			num2 = 0;
			while (num1 % 2 == 0)
				num1 = (rand() % (upper - lower + 1)) + lower;
			while (num2 % 2 == 0)
				num2 = (rand() % (upper - lower + 1)) + lower;
			if (computer[num1][num2] != 33 && computer[num1][num2] != 35)
				verif = 0;
		}
		int lin = num1 + 2, col = num2 + 35;
		if (computer[num1][num2] == 88)
		{
			attron(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
			mvprintw(lin, col, " ");
			computer[num1][num2] = 33;
			attroff(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
		}
		else
		{
			attron(COLOR_PAIR(1) | A_STANDOUT);
			mvprintw(lin, col, " ");
			attroff(COLOR_PAIR(1) | A_STANDOUT);
			computer[num1][num2] = 35;
		}
		refresh();
		//verfic daca playerul a terminat
		endgame = 0;
		for (i = 1; i < 20; i = i + 2)
			for (j = 1; j < 20; j = j + 2)
				if (computer[i][j] == 88) endgame = 1;
		if (endgame == 0)
		{
			mvprintw(0, 23, "PLAYER won!    ");
			WINDOW * popup;
			popup = newwin(10, 48, 5, 5);
			wbkgd(popup, COLOR_PAIR(3));
			box(popup, 0, 0);
			wrefresh(popup);
			mvwprintw(popup, 1, 2, "PLAYER WON!");
			mvwprintw(popup, 3, 2, "Ships destroyed:");
			mvwprintw(popup, 4, 2, "PLAYER: 10");
			mvwprintw(popup, 5, 2, "COMPUTER: %d", navedistruse(player));
			mvwprintw(popup, 8, 2, "Press any key to go back to MENU.");
			wrefresh(popup);
			getch();
			return 1;
		}
		//mutare computer
		verif = 1;
		while (verif)
		{
			num1 = 0;
			num2 = 0;
			while (num1 % 2 == 0)
				num1 = (rand() % (upper - lower + 1)) + lower;
			while (num2 % 2 == 0)
				num2 = (rand() % (upper - lower + 1)) + lower;
			if (player[num1][num2] != 33 && player[num1][num2] != 35 &&
				player[num1][num2] != 34)
				verif = 0;
		}
		lin = num1 + 2;
		col = num2 + 2;
		if (player[num1][num2] == 88)
		{
			attron(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
			mvprintw(lin, col, " ");
			player[num1][num2] = 33;
			attroff(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
		}
		else
		{
			attron(COLOR_PAIR(1) | A_STANDOUT);
			mvprintw(lin, col, " ");
			attroff(COLOR_PAIR(1) | A_STANDOUT);
			player[num1][num2] = 35;
		}
		refresh();
		//verific daca computerul a terminat
		endgame = 0;
		for (i = 1; i < 20; i = i + 2)
			for (j = 1; j < 20; j = j + 2)
				if (player[i][j] == 88) endgame = 1;
		if (endgame == 0)
		{
			mvprintw(0, 23, "COMPUTER won!  ");
			WINDOW * popup;
			popup = newwin(10, 48, 5, 5);
			wbkgd(popup, COLOR_PAIR(3));
			box(popup, 0, 0);
			wrefresh(popup);
			mvwprintw(popup, 1, 2, "COMPUTER WON!");
			mvwprintw(popup, 3, 2, "Ships destroyed:");
			mvwprintw(popup, 4, 2, "PLAYER: %d", navedistruse(computer));
			mvwprintw(popup, 5, 2, "COMPUTER: 10");
			mvwprintw(popup, 8, 2, "Press any key to go back to MENU.");
			wrefresh(popup);
			getch();
			return 1;
		}
	}
	return 0;
}

void gameplay(int **player, int **computer, FILE *resume)
{
	int *coord, over = 1, endgame = 1, turn = 1, i, j;
	coord = (int*) malloc(2* sizeof(int));
	coord[0] = 3;
	coord[1] = 36;
	while (endgame)
	{
		over = 1;
		if (turn == 1)
		{
			//player turn
			mvprintw(0, 23, "PLAYER's turn  ");
			refresh();
			while (over)
			{
				over = alegereplayer(computer, coord);
				//verfic daca s-a apasat q
				if (over == -1)
				{
					for (i = 0; i < 21; i++)
					{
						for (j = 0; j < 21; j++)
							fprintf(resume, "%d ", player[i][j]);
						fprintf(resume, "\n");
					}
					fprintf(resume, "-1\n");
					for (i = 0; i < 21; i++)
					{
						for (j = 0; j < 21; j++)
							fprintf(resume, "%d ", computer[i][j]);
						fprintf(resume, "\n");
					}
					clear();
					endgame = 0;
					break;
				}
				//verific daca s-a apasat optiunea randomise
				if (over == 2)
				{
					randomise(player);
					refresh();
				}
				//verific daca s-a apasat optiunea destroy
				if (over == 3)
				{
					if (destroy(player, computer) == 1)
					{
						over = 0;
						endgame = 0;
						break;
					}
				}
				//verfic daca playerul a terminat
				endgame = 0;
				for (i = 1; i < 20; i = i + 2)
					for (j = 1; j < 20; j = j + 2)
						if (computer[i][j] == 88) endgame = 1;
				if (endgame == 0)
				{
					mvprintw(0, 23, "PLAYER won!    ");
					WINDOW * popup;
					popup = newwin(10, 48, 5, 5);
					wbkgd(popup, COLOR_PAIR(3));
					box(popup, 0, 0);
					wrefresh(popup);
					mvwprintw(popup, 1, 2, "PLAYER WON!");
					mvwprintw(popup, 3, 2, "Ships destroyed:");
					mvwprintw(popup, 4, 2, "PLAYER: 10");
					mvwprintw(popup, 5, 2, "COMPUTER: %d", navedistruse(player));
					mvwprintw(popup, 8, 2, "Press any key to go back to MENU.");
					wrefresh(popup);
					getch();
					over = 0;
				}
				turn = 0;
				refresh();
			}
			refresh();
		}
		else
		{
			//computer turn
			mvprintw(0, 23, "COMPUTER's turn");
			refresh();
			while (over)
			{
				napms(1000);
				if (alegerecomputer(player) == 0)
					over = 0;
				//verific daca computerul a terminat
				endgame = 0;
				for (i = 1; i < 20; i = i + 2)
					for (j = 1; j < 20; j = j + 2)
						if (player[i][j] == 88) endgame = 1;
				if (endgame == 0)
				{
					mvprintw(0, 23, "COMPUTER won!  ");
					WINDOW * popup;
					popup = newwin(10, 48, 5, 5);
					wbkgd(popup, COLOR_PAIR(3));
					box(popup, 0, 0);
					wrefresh(popup);
					mvwprintw(popup, 1, 2, "COMPUTER WON!");
					mvwprintw(popup, 3, 2, "Ships destroyed:");
					mvwprintw(popup, 4, 2, "PLAYER: %d", navedistruse(computer));
					mvwprintw(popup, 5, 2, "COMPUTER: 10");
					mvwprintw(popup, 8, 2, "Press any key to go back to MENU.");
					wrefresh(popup);
					getch();
					over = 0;
				}
				turn = 1;
				refresh();
			}
			refresh();
		}
	}
	free(coord);
}

void newgame(int **auxiliar)
{
	clear();
	//declarare matrice necesare pt a memora hartile
	int **player, **computer, i, j;
	player = (int **) malloc(21* sizeof(int*));
	for (i = 0; i < 21; i++)
		player[i] = (int*) malloc(21* sizeof(int));
	computer = (int **) malloc(21* sizeof(int*));
	for (i = 0; i < 21; i++)
		computer[i] = (int*) malloc(21* sizeof(int));
	//formatare harta player pt afisare
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (auxiliar[i][j] == 0)
				player[i + i + 1][j + j + 1] = 32;
			else
				player[i + i + 1][j + j + 1] = 88;
		}
	}
	for (i = 0; i < 21; i++)
	{
		if (i % 2 == 0)
			for (j = 0; j < 21; j++)
			{
				if (j % 2 == 0)
					player[i][j] = 43;
				else
					player[i][j] = 45;
			}
		else
			for (j = 0; j < 21; j = j + 2)
			{
				if (j % 2 == 0)
					player[i][j] = 124;
			}
	}
	//afisare harta player
	mvprintw(1, 9, "PLAYER");
	mvvline(3, 2, 0, 19);
	mvhline(2, 3, 0, 19);
	for (i = 1; i < 21; i++)
	{
		for (j = 1; j < 20; j++)
		{
			if (player[i][j] == 88)
			{
				attron(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				mvprintw(i + 2, j + 2, " ");
				attroff(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
			}
			if (i % 2 != 0 && j % 2 != 0)
			{
				mvhline(i + 3, j + 2, 0, 1);
				mvvline(i + 2, j + 3, 0, 1);
			}
		}
	}
	mvvline(3, 22, 0, 19);
	mvhline(22, 3, 0, 19);
	refresh();
	//generare harta computer
	genhartacomp(auxiliar);
	//formatare harta computer pt afisare
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (auxiliar[i][j] == 0)
				computer[i + i + 1][j + j + 1] = 32;
			else
				computer[i + i + 1][j + j + 1] = 88;
		}
	}
	for (i = 0; i < 21; i++)
	{
		if (i % 2 == 0)
			for (j = 0; j < 21; j++)
			{
				if (j % 2 == 0)
					computer[i][j] = 43;
				else
					computer[i][j] = 45;
			}
		else
			for (j = 0; j < 21; j = j + 2)
			{
				if (j % 2 == 0)
					computer[i][j] = 124;
			}
	}
	//afisare harta computer
	mvprintw(1, 42, "COMPUTER");
	mvvline(3, 35, 0, 19);
	mvhline(2, 36, 0, 19);
	for (i = 1; i < 21; i++)
	{
		for (j = 1; j < 20; j++)
		{
			if (i % 2 != 0 && j % 2 != 0)
			{
				mvhline(i + 3, j + 35, 0, 1);
				mvvline(i + 2, j + 36, 0, 1);
			}
		}
	}
	mvvline(3, 55, 0, 19);
	mvhline(22, 36, 0, 19);
	refresh();
	//afisare puteri
	mvprintw(5, 60, "POWERS:");
	mvprintw(6, 60, "R - randomise map");
	mvprintw(7, 60, "D - destroy in");
	mvprintw(8, 60, "    advance");
	mvprintw(10, 60, "Press Q to save");
	mvprintw(11, 60, "the game and exit.");
	//gameplay
	FILE * resume;
	resume = fopen("resume.txt", "w");
	gameplay(player, computer, resume);
	fclose(resume);
	for (i = 0; i < 21; i++)
	{
		free(player[i]);
		free(computer[i]);		
	}
	free(player);
	free(computer);

}

void resumegame(FILE *resume)
{
	//verificare resume nu este gol
	int v;
	if (fscanf(resume, "%d", &v) == EOF)
	{
		WINDOW * popup;
		popup = newwin(4, 37, 5, 5);
		wbkgd(popup, COLOR_PAIR(3));
		box(popup, 0, 0);
		wrefresh(popup);
		mvwprintw(popup, 1, 2, " There's no game to be resumed!!");
		mvwprintw(popup, 2, 2, "Press any key to go back to MENU.");
		wrefresh(popup);
		refresh();
		getch();
		return;
	}
	else rewind(resume);
	clear();
	//declaratii
	int **player, **computer, i, j;
	player = (int **) malloc(21* sizeof(int*));
	for (i = 0; i < 21; i++)
		player[i] = (int*) malloc(21* sizeof(int));
	computer = (int **) malloc(21* sizeof(int*));
	for (i = 0; i < 21; i++)
		computer[i] = (int*) malloc(21* sizeof(int));
	//matricea playerului si computerului
	for (i = 0; i < 21; i++)
		for (j = 0; j < 21; j++)
			fscanf(resume, "%d", &player[i][j]);
	int car;
	fscanf(resume, "%d", &car);
	for (i = 0; i < 21; i++)
		for (j = 0; j < 21; j++)
			fscanf(resume, "%d", &computer[i][j]);
	//afisare mat player si mat computer
	mvprintw(1, 9, "PLAYER");
	mvvline(3, 2, 0, 19);
	mvhline(2, 3, 0, 19);
	for (i = 1; i < 21; i++)
	{
		for (j = 1; j < 20; j++)
		{
			if (player[i][j] == 88)
			{
				attron(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
				mvprintw(i + 2, j + 2, " ");
				attroff(COLOR_PAIR(3) | A_STANDOUT | A_BOLD);
			}
			if (i % 2 != 0 && j % 2 != 0)
			{
				mvhline(i + 3, j + 2, 0, 1);
				mvvline(i + 2, j + 3, 0, 1);
			}
		}
	}
	for (i = 1; i < 20; i = i + 2)
		for (j = 1; j < 20; j = j + 2)
		{
			if (player[i][j] == 33)
			{
				attron(COLOR_PAIR(2) | A_STANDOUT | A_BOLD);
				mvprintw(i + 2, j + 2, " ");
				attroff(COLOR_PAIR(2) | A_STANDOUT | A_BOLD);
			}
			if (player[i][j] == 34)
			{
				attron(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
				mvprintw(i + 2, j + 2, "X");
				attroff(COLOR_PAIR(2) | A_BOLD | A_STANDOUT);
			}
			if (player[i][j] == 35)
			{
				attron(COLOR_PAIR(1) | A_STANDOUT);
				mvprintw(i + 2, j + 2, " ");
				attroff(COLOR_PAIR(1) | A_STANDOUT);
			}
		}
	refresh();
	mvvline(3, 22, 0, 19);
	mvhline(22, 3, 0, 19);
	refresh();
	mvprintw(1, 42, "COMPUTER");
	mvvline(3, 35, 0, 19);
	mvhline(2, 36, 0, 19);
	for (i = 1; i < 21; i++)
	{
		for (j = 1; j < 20; j++)
		{
			if (i % 2 != 0 && j % 2 != 0)
			{
				mvhline(i + 3, j + 35, 0, 1);
				mvvline(i + 2, j + 36, 0, 1);
			}
		}
	}
	mvvline(3, 55, 0, 19);
	mvhline(22, 36, 0, 19);
	refresh();
	for (i = 1; i < 20; i = i + 2)
		for (j = 1; j < 20; j = j + 2)
		{
			if (computer[i][j] == 33)
			{
				attron(COLOR_PAIR(2) | A_STANDOUT | A_BOLD);
				mvprintw(i + 2, j + 35, " ");
				attroff(COLOR_PAIR(2) | A_STANDOUT | A_BOLD);
			}
			if (computer[i][j] == 35)
			{
				attron(COLOR_PAIR(1) | A_STANDOUT);
				mvprintw(i + 2, j + 35, " ");
				attroff(COLOR_PAIR(1) | A_STANDOUT);
			}
		}
	refresh();
	rewind(resume);
	//afisare puteri
	mvprintw(5, 60, "POWERS:");
	mvprintw(6, 60, "R - randomise map");
	mvprintw(7, 60, "D - destroy in");
	mvprintw(8, 60, "    advance");
	mvprintw(10, 60, "Press Q to save");
	mvprintw(11, 60, "the game and exit.");
	//gameplay
	gameplay(player, computer, resume);
	fclose(resume);
	free(player);
	free(computer);
}

void initalegharta(int argc)
{
	int i;
	attron(COLOR_PAIR(2) | A_BOLD);
	mvprintw(0, 5, "Choose your map:\n\n");
	attroff(COLOR_PAIR(2) | A_BOLD);
	for (i = 1; i < argc; i++)
	{
		mvprintw(i + 1, 3, " ");
		mvprintw(i + 1, 5, "MAP %d\n", i);
	}
	refresh();
}

void alegharta(int **argharti, int **auxiliar, int argc)
{
	clear();
	int i, j, poz = 2;
	initalegharta(argc);
	attron(COLOR_PAIR(2) | A_BOLD);
	mvprintw(2, 3, ">");
	mvprintw(2, 5, "MAP 1\n");
	attroff(COLOR_PAIR(2) | A_BOLD);
	refresh();
	int ch = getch();
	while ((char) ch != '\n')
	{
		switch (ch)
		{
			case KEY_DOWN:
				initalegharta(argc);
				attron(COLOR_PAIR(2) | A_BOLD);
				if (poz != argc)
				{
					poz++;
					mvprintw(poz, 3, ">");
					mvprintw(poz, 5, "MAP %d\n", poz - 1);
				}
				else
				{
					poz = 2;
					mvprintw(poz, 3, ">");
					mvprintw(poz, 5, "MAP %d\n", poz - 1);
				}
				attroff(COLOR_PAIR(2) | A_BOLD);
				refresh();
				break;
			case KEY_UP:
				initalegharta(argc);
				attron(COLOR_PAIR(2) | A_BOLD);
				if (poz != 2)
				{
					poz--;
					mvprintw(poz, 3, ">");
					mvprintw(poz, 5, "MAP %d\n", poz - 1);
				}
				else
				{
					poz = argc;
					mvprintw(poz, 3, ">");
					mvprintw(poz, 5, "MAP %d\n", poz - 1);
				}
				attroff(COLOR_PAIR(2) | A_BOLD);
				refresh();
				break;
			default:
				break;
		}
		ch = getch();
	}
	refresh();
	poz = (poz - 2) *10;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			auxiliar[i][j] = argharti[i + poz][j];
}
