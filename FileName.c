#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<conio.h>
#include<string.h>
#include <stdlib.h> 
#include <time.h> 

typedef struct vector2
{
	int x;
	int y;
}Vector2;

typedef struct field
{
	int opened;
	int mined;
	int adjacent;
	int marked;
}Field;

void GetInput();
void MenuUpdate(char input);
void GameUpdate(char input);
void LoseUpdate(char input);
void WinUpdate(char input);
void RecusveOpenNeighbours(Vector2 field);
void DrawGame();
void SetupGame();
void RandomBoard();
void SetupNeighbours();
void MarkField();
void OpenField();
void DrawMenu();
void DrawButton(int size, char text[], int selected);
void StartGame();

int slimAsciiTable[] = {
	201, // Top-left Corner ╔
	205, // Horizontal Line ═
	187, // Top-right Corner ╗
	186, // Vertical Line ║
	200, // Bottom-left Corner ╚
	188  // Bottom-right Corner ╝
};
int thickAsciiTable[] = {
	219, // Top-left Corner ╔
	219, // Horizontal Line ═
	219, // Top-right Corner ╗
	219, // Vertical Line ║
	219, // Bottom-left Corner ╚
	219  // Bottom-right Corner ╝
};
int colourBackgroundTable[] = {
	0,   //black
	106, //cyan
	46,	 //light blue
	104, //blue
	44,	 //dark blue
	42,  //green
	43,  //yellow
	101, //pink
	45,  //purplr
	41   //red
};
int colourTable[] = {
	90,   //black
	95,  //cyan
	92,	 //green
	92,  //yellow
	94,	 //blue
	91,  //red
	95,  //purpur
	97,  //white
	34,  //dark blue
	90   //gray
};
int selectedButton = 0;
void (*update)(char input);
Vector2 boardSize = { 52,52 };
int mines = 52;
Vector2 pointer = { 0,0 };
Field board[100][100];
int openedFirstField = 0;
time_t timeStart;
int flagsPlaced = 0;
int loseTime = 0;


void main()
{
	DrawMenu();
	update = MenuUpdate;
	GetInput(0);
}

void GetInput()
{
	char input = NULL;

	while (1)
	{
		input = _getch();

		update(input);
	}
}

void MenuUpdate(char input)
{

	/**/switch (input)
	{
	case 'w':
	{
		if (selectedButton-- <= 0)selectedButton = 3;
		DrawMenu();
		break;
	}
	case 's':
	{
		if (selectedButton++ >= 3)selectedButton = 0;
		DrawMenu();
		break;
	}
	case 13:
	case 32:
	{
		StartGame();
		break;
	}
	case 0:
	{
		selectedButton = 0;
		DrawMenu();
		break;
	}
	default:

		break;
	}

}

void StartGame()
{
	system("@cls||clear");

	switch (selectedButton)
	{
	case 0:
	{
		boardSize = (Vector2){ 9,9 };
		mines = 10;
		break;
	}
	case 1:
	{
		boardSize = (Vector2){ 16,16 };
		mines = 40;
		break;
	}
	case 2:
	{
		boardSize = (Vector2){ 30,16 };
		mines = 99;
		break;
	}
	case 3:
	{
		system("@cls||clear");
		while (1)
		{
			int x, y, m;
			printf("uneti dimenzije minskog polja\n\nX-sirina\nY-visina\nM-broj mina\n\nu formatu (X Y M)\n");
			if (scanf(" %d %d %d", &x, &y, &m) != 3) { system("@cls||clear"); printf("unos nije lepo formairan\n\n"); while (getchar() != '\n'); continue; }
			if (x < 2 || x >= 100) { system("@cls||clear"); printf("sirina mora da bude veca od 2 i manja od 100\n\n");	continue; }
			if (y < 2 || y >= 100) { system("@cls||clear"); printf("visina mora da bude veca od 2 i manja od 100\n\n");	continue; }
			if (m < 0) { system("@cls||clear"); printf("mora da bude barem jedna mina\n\n");	continue; }
			boardSize = (Vector2){ x,y };
			mines = m;
			break;
		}
		break;
	}
	default:
		printf("selectGameError");
		break;
	}
	pointer = (Vector2){ boardSize.x / 2,boardSize.y / 2 };
	SetupGame();
	DrawGame();
	update = GameUpdate;
}

void DrawMenu()
{
	system("@cls||clear");

	printf("\n\n\n\n __   __  ___   __    _  _______  _______  _     _  _______  _______  _______  _______  ______   	\n");
	printf("|  |_|  ||   | |  |  | ||       ||       || | _ | ||       ||       ||       ||       ||    _ |  	\n");
	printf("|       ||   | |   |_| ||    ___||  _____|| || || ||    ___||    ___||    _  ||    ___||   | ||  	\n");
	printf("|       ||   | |       ||   |___ | |____  |       ||   |___ |   |___ |   |_| ||   |___ |   |_||_ 	\n");
	printf("|       ||   | |  _    ||   ___ || _____ ||       ||    ___||    ___||    ___||    ___||    __  |	\n");
	printf("| ||_|| ||   | | | |   ||   |___  _____| ||   _   ||   |___ |   |___ |   |    |   |___ |   |  | |	\n");
	printf("|_|   |_||___| |_|  |__||_______||_______||__| |__||_______||_______||___|    |_______||___|  |_|	\nw/s - select button\nEnter - push button\n\n");



	DrawButton(30, "easy   :D", selectedButton == 0 ? 1 : 0);
	DrawButton(30, "novice :)", selectedButton == 1 ? 1 : 0);
	printf("\033[1;31m");
	DrawButton(30, "HARD  >:(", selectedButton == 2 ? 1 : 0);
	printf("\033[0m");
	DrawButton(30, "custom dx", selectedButton == 3 ? 1 : 0);

	printf("by: \n   -klipik_rez (aka. Nikola Jankovic)");

}

void DrawButton(int size, char text[], int selected)
{
	int* asciiTable = slimAsciiTable;

	switch (selected)
	{
	case 0:
	{
		asciiTable = slimAsciiTable;
		break;
	}
	case 1:
	{
		asciiTable = thickAsciiTable;
		break;
	}
	default:
		printf("asciiTableMenuError\nasciiTableMenuError\nasciiTableMenuError\nasciiTableMenuError\nasciiTableMenuError\nasciiTableMenuError\n");
		break;
	}
	printf("%s", "                              ");
	printf("%c", asciiTable[0]);

	for (int i = 0; i < size - 2; i++)
	{
		printf("%c", asciiTable[1]);

	}
	printf("%c\n", asciiTable[2]);


	printf("%s", "                              ");
	printf("%c", asciiTable[3]);
	for (int i = 0; i < size - 2; i++)
	{
		printf("%c", ' ');

	}
	printf("%c\n", asciiTable[3]);



	printf("%s", "                              ");
	int textLenth = strlen(text);
	printf("%c", asciiTable[3]);
	for (int i = 0; i < size - 2; i++)
	{
		if (((double)(size - 2) - textLenth) / 2 - 1 > i || (size - 2) - ((double)(size - 2) - textLenth) / 2 - 1 < i)
		{
			printf("%c", ' ');
		}
		else
		{
			printf("%c", text[i - ((size - 2) - textLenth) / 2]);
		}
	}
	printf("%c\n", asciiTable[3]);


	printf("%s", "                              ");
	printf("%c", asciiTable[3]);
	for (int i = 0; i < size - 2; i++)
	{
		printf("%c", ' ');

	}
	printf("%c\n", asciiTable[3]);


	printf("%s", "                              ");
	printf("%c", asciiTable[4]);
	for (int i = 0; i < size - 2; i++)
	{
		printf("%c", asciiTable[1]);

	}
	printf("%c\n\n\n", asciiTable[5]);


}

void GameUpdate(char input)
{
	switch (input)
	{
	case 'w':
	{
		if (pointer.y-- <= 0)pointer.y++;
		DrawGame();
		break;
	}
	case 'a':
	{
		if (pointer.x-- <= 0)pointer.x++;
		DrawGame();
		break;
	}
	case 's':
	{
		if (pointer.y++ >= boardSize.y - 1)pointer.y--;
		DrawGame();
		break;
	}
	case 'd':
	{
		if (pointer.x++ >= boardSize.x - 1)pointer.x--;
		DrawGame();
		break;
	}
	case 'm':
	{


		MarkField();
		DrawGame();
		break;

	}
	case 32:
	case 13:
	{
		OpenField();
		DrawGame();
		openedFirstField = 1;
		break;
	}
	case 0:
	{
		DrawGame();
		break;
	}
	default:
		printf("error game  %d", input);
		break;
	}
}

void DrawGame()
{
	system("@cls||clear");
	printf("mines:%d/%d    time:%d\n", flagsPlaced, mines, timeStart == 0 ? -52 : time(NULL) - timeStart);
	printf("%c", slimAsciiTable[0]);
	for (int i = 0; i < boardSize.x; i++)
	{
		printf("%c", slimAsciiTable[1]);

	}
	printf("%c\n", slimAsciiTable[2]);

	for (int i = 0; i < boardSize.y; i++)
	{
		printf("%c", slimAsciiTable[3]);
		for (int j = 0; j < boardSize.x; j++)
		{
			if (pointer.x == j && pointer.y == i)
			{
				printf("\033[41;30m");
			}

			/*if (board[j][i].mined == 1)
			{
				printf("\033[47;30mX\033[0;0m");
			}
			else
			{
				if (pointer.x == j && pointer.y == i)
					printf("%d", board[j][i].adjacent);
				else
				{

					printf("\033[%d;29m%d\033[0;0m", colourTable[board[j][i].adjacent], board[j][i].adjacent);


				}
			}*/

			if (board[j][i].opened == 1)
			{
				if (board[j][i].mined == 1)
				{
					printf("\033[47;30mX\033[0;0m");
				}
				else
				{
					if (pointer.x == j && pointer.y == i)
						printf("%d", board[j][i].adjacent);
					else
					{
						if (board[j][i].adjacent == 0)
						{
							printf("\033[%d;29m%c\033[0;0m", colourTable[board[j][i].adjacent], ' ');
						}
						else
						{
							printf("\033[%d;29m%d\033[0;0m", colourTable[board[j][i].adjacent], board[j][i].adjacent);
						}

					}
				}
				//printf("%c", board[j][i].mined == 1 ? 'm' : ' ');

			}
			else
			{
				if (board[j][i].marked == 1)
				{
					printf("\033[0;42m%c\033[0;0m", 'T');
				}
				else
				{
					printf("%c", 177);
				}

			}

			if (pointer.x == j && pointer.y == i)
			{
				printf("\033[0;0m");
			}


		}
		printf("%c\n", slimAsciiTable[3]);
	}

	printf("%c", slimAsciiTable[4]);
	for (int i = 0; i < boardSize.x; i++)
	{
		printf("%c", slimAsciiTable[1]);

	}
	printf("%c\n\n\n", slimAsciiTable[5]);
	printf("w/a/s/d - move cursor\nSpace - reveal field\nM - mark field");
}

void SetupGame()
{
	openedFirstField = 0;
	flagsPlaced = 0;
	//pointer = (Vector2){ 0,0 };
	loseTime = 0;

	int counter = 0;
	for (int i = 0; i < boardSize.y; i++)
	{
		for (int j = 0; j < boardSize.x; j++)
		{
			if (counter < mines)
				board[j][i].mined = 1;
			else
				board[j][i].mined = 0;
			counter++;
			board[j][i].adjacent = 0;
			board[j][i].marked = 0;
			board[j][i].opened = 0;

		}
	}
	RandomBoard();
}

void RandomBoard()
{

	for (int i = 0; i < boardSize.y; i++)
	{
		for (int j = 0; j < boardSize.x; j++)
		{
			srand(time(0));
			int randX = rand() % boardSize.x;
			srand(time(0) +5252);
			int randY = rand() % boardSize.y;
			int store = board[j][i].mined;
			board[j][i].mined = board[randX][randY].mined;
			board[randX][randY].mined = store;
		}
	}
}

void SetupNeighbours()
{
	timeStart = time(NULL);
	for (int i = 0; i < boardSize.y; i++)
	{
		for (int j = 0; j < boardSize.x; j++)
		{

			for (int k = -1; k <= 1; k++)
				for (int l = -1; l <= 1; l++)
				{
					if (j + k > boardSize.x - 1) continue;
					if (i + l > boardSize.y - 1) continue;
					if (j + k < 0) continue;
					if (i + l < 0) continue;
					//if (i == 0 && j == 0)continue;
					board[j][i].adjacent += board[j + k][i + l].mined;
				}
		}
	}
}

void MarkField()
{
	if (board[pointer.x][pointer.y].opened == 0)
	{
		board[pointer.x][pointer.y].marked = abs(board[pointer.x][pointer.y].marked - 1);
		flagsPlaced += board[pointer.x][pointer.y].marked == 0 ? -1 : 1;
	}

	if (flagsPlaced == mines)
	{
		int count = 0;
		for (int i = 0; i < boardSize.y; i++)
		{
			for (int j = 0; j < boardSize.x; j++)
			{
				if (board[pointer.x][pointer.y].marked && board[pointer.x][pointer.y].mined)count++;
			}
		}
		if (count >= mines)
		{
			update = WinUpdate;
		}
	}
}

void OpenField()
{
	if (board[pointer.x][pointer.y].marked == 0)
	{
		if (openedFirstField == 0)
		{
			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++)
				{
					if (j + pointer.x > boardSize.x - 1) continue;
					if (i + pointer.y > boardSize.y - 1) continue;
					if (j + pointer.x < 0) continue;
					if (i + pointer.y < 0) continue;
					if (board[pointer.x + j][pointer.y + i].mined == 1)
					{
						board[pointer.x + j][pointer.y + i].mined = 0;
						int iterations = 100;
						while (--iterations > 0)
						{
							srand(time(0));
							int randX = rand() % boardSize.x;
							srand(time(0) + 5252);
							int randY = rand() % boardSize.y;
							if (board[randX][randY].mined == 0)
							{
								board[randX][randY].mined = 1;
								break;
							}
						}

					}
				}
			mines = 0;
			for (int i = 0; i < boardSize.y; i++)
			{
				for (int j = 0; j < boardSize.x; j++)
				{
					if (board[j][i].mined)mines++;
				}
			}
			SetupNeighbours();
		}
		board[pointer.x][pointer.y].opened = 1;
		if (board[pointer.x][pointer.y].mined == 1)
		{
			//you loseeeeeeee!!!!
			loseTime = time(NULL) - timeStart;;
			update = LoseUpdate;
			update(0);
			return;
		}

		RecusveOpenNeighbours((Vector2) { pointer.x, pointer.y });

	}
}

void RecusveOpenNeighbours(Vector2 field)
{
	if (board[pointer.x][pointer.y].mined == 1)
	{
		board[pointer.x][pointer.y].opened = 0;
		return;
	}

	int openEmpty = 0;
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
		{
			if (field.x + j > boardSize.x - 1) continue;
			if (field.y + i > boardSize.y - 1) continue;
			if (field.x + j < 0) continue;
			if (field.y + i < 0) continue;
			//if (i == 0 && j == 0)continue;
			if ((board[field.x + j][field.y + i].adjacent == 0) && board[field.x + j][field.y + i].opened == 1) openEmpty++;
		}

	if (openEmpty != 0)
	{
		board[field.x][field.y].opened = 1;
		board[field.x][field.y].marked = 0;

		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
			{
				if (field.x + j > boardSize.x - 1) continue;
				if (field.y + i > boardSize.y - 1) continue;
				if (field.x + j < 0) continue;
				if (field.y + i < 0) continue;
				if (i == 0 && j == 0)continue;
				if (board[field.x + j][field.y + i].opened == 0)
				{
					RecusveOpenNeighbours((Vector2) { field.x + j, field.y + i });
				}
			}

	}
}

void WinUpdate(char input)
{
	system("@cls||clear");
	printf("mines:XX/XX    time:%d\n", loseTime);
	printf("%c", slimAsciiTable[0]);
	for (int i = 0; i < boardSize.x; i++)
	{
		printf("%c", slimAsciiTable[1]);

	}
	printf("%c\n", slimAsciiTable[2]);

	for (int i = 0; i < boardSize.y; i++)
	{
		printf("%c", slimAsciiTable[3]);
		for (int j = 0; j < boardSize.x; j++)
		{
			if (pointer.x == j && pointer.y == i)
			{
				printf("\033[41;30m");
			}


			if (board[j][i].opened == 1)
			{
				if (board[j][i].mined == 1)
				{
					printf("\033[47;30mX\033[0;0m");
				}
				else
				{
					if (pointer.x == j && pointer.y == i)
						printf("%d", board[j][i].adjacent);
					else
					{
						if (board[j][i].adjacent == 0)
						{
							printf("\033[%d;29m%c\033[0;0m", colourTable[board[j][i].adjacent], ' ');
						}
						else
						{
							printf("\033[%d;29m%d\033[0;0m", colourTable[board[j][i].adjacent], board[j][i].adjacent);
						}

					}
				}
				//printf("%c", board[j][i].mined == 1 ? 'm' : ' ');

			}
			else
			{
				if (board[j][i].marked == 1)
				{

					printf("\033[0;42m%c\033[0;0m", 'T');
				}
				else
				{
					printf("%c", 177);
				}

			}

			if (pointer.x == j && pointer.y == i)
			{
				printf("\033[0;0m");
			}


		}
		printf("%c\n", slimAsciiTable[3]);
	}

	printf("%c", slimAsciiTable[4]);
	for (int i = 0; i < boardSize.x; i++)
	{
		printf("%c", slimAsciiTable[1]);

	}
	printf("%c\n\n\n", slimAsciiTable[5]);

	printf("JUHUUUU!!! POLUPAO SI SVE MINE!\n\n:D\n\nPRITISNI Esc ZA POVRATAK NA MENI :)\n");

	printf("                   \xDB\xDB\xDB\xDB\xDB                   \n");
	printf("                   \xDB\xDB\xDB\xDB\xDB                   \n");
	printf("                   \xDB\xDB\xDB\xDB\xDB                   \n");
	printf("      \xDB\xDB\xDB\xDB   \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB   \xDB\xDB\xDB\xDB      \n");
	printf("      \xDB\xDB\xDB\xDB   \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB      \n");
	printf("         \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB         \n");
	printf("         \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB   \xDB         \n");
	printf("      \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB\xDB\xDB\xDB      \n");
	printf("      \xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB      \n");
	printf("      \xDB\xDB\xDB\xDB\xDB\xDB     \xDB\xDB\xDB\xDB\xDB\xDB   \xDB\xDB   \xDB\xDB\xDB\xDB\xDB\xDB      \n");
	printf("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB   \xDB\xDB\xDB  \xDB\xDB\xDB\xDB   \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n");
	printf("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n");
	printf("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB\xDB\xDB\xDB       \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n");
	printf("      \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB     \xDB\xDB\xDB\xDB     \xDB\xDB\xDB\xDB\xDB      \n");
	printf("      \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB   \xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB \xDB\xDB\xDB  \xDB\xDB\xDB      \n");
	printf("      \xDB\xDB\xDB\xDB\xDB\xDB\xDB   \xDB  \xDB\xDB\xDB\xDB          \xDB\xDB\xDB\xDB      \n");
	printf("         \xDB\xDB\xDB             \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB         \n");
	printf("         \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB         \n");
	printf("      \xDB\xDB\xDB\xDB   \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB\xDB      \n");
	printf("      \xDB\xDB\xDB\xDB   \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB   \xDB\xDB\xDB\xDB      \n");
	printf("                   \xDB\xDB\xDB\xDB\xDB                   \n");
	printf("                   \xDB\xDB\xDB\xDB\xDB                   \n");
	printf("                   \xDB\xDB\xDB\xDB\xDB                   \n");

	if (input == 27)
	{
		update = MenuUpdate;
		update(0);
	}
}

void LoseUpdate(char input)
{
	system("@cls||clear");
	printf("mines:XX/XX    time:%d\n", loseTime);
	printf("%c", slimAsciiTable[0]);
	for (int i = 0; i < boardSize.x; i++)
	{
		printf("%c", slimAsciiTable[1]);

	}
	printf("%c\n", slimAsciiTable[2]);

	for (int i = 0; i < boardSize.y; i++)
	{
		printf("%c", slimAsciiTable[3]);
		for (int j = 0; j < boardSize.x; j++)
		{
			if (pointer.x == j && pointer.y == i)
			{
				printf("\033[41;30m");
			}

			if (board[j][i].mined == 1)
			{
				printf("\033[47;30mX\033[0;0m");
			}
			else
			{
				if (pointer.x == j && pointer.y == i)
					printf("%d", board[j][i].adjacent);
				else
				{

					printf("\033[%d;29m%d\033[0;0m", colourTable[board[j][i].adjacent], board[j][i].adjacent);


				}
			}
		}
		printf("%c\n", slimAsciiTable[3]);
	}

	printf("%c", slimAsciiTable[4]);
	for (int i = 0; i < boardSize.x; i++)
	{
		printf("%c", slimAsciiTable[1]);

	}
	printf("%c\n\n\n", slimAsciiTable[5]);

	printf("IZGUBIO SI!!!\n\n:*(\n\nPRITISNI Esc ZA POVRATAK NA MENI :)");

	if (input == 27)
	{
		update = MenuUpdate;
		update(0);
	}
}