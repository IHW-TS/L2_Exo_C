// ldd: -lncurses
#include <ncurses.h> // API pour le developpement d'interface
#include <time.h>
#include <stdlib.h>

void INIT_SCREEN(void)
{
    initscr();            // fonction qui permet d'initialiser un programme utilisant ncurses.h
    raw();                // permet que les caractère soient disponible des que les utiliseateurs les tapes...
    keypad(stdscr, TRUE); // permet la lecture des touches de fonction telles que F1, F2, les touches fléchées etc...
    noecho();             // fonction noecho() désactive l'affichage sur l'écran d'une touche frappée au clavier
}

void DONE_SCREEN(void)
{
    endwin(); // fin du programme utilisant ncurses.h
    exit(0);  //sortit du terminal
}

int hauteur (int size_row){

    printw(" Entrer le nombre de ligne du tableau = ");
    scanw(%u, &size_row);

    if (pl>4 && pl<16){
       
    }else{
        prinw("Entrer un nombre valide");
    }

}


int largeur (int size_col){



    printw(" Entrer le nombre colonne du tableau = ");
    scanw(%u, &size_col);
    if (pl>4 && pl<26){
       
    }else{
        prinw("Entrer un nombre valide");
    }


}

int joueur(int pl){


    printw(" Entrer le nombre de joueur entre 2 = ");
    scanw(%u, &pl);
    if (pl>2 && pl<8){
        char *players = (char *)malloc(pl * sizeof(char));
        free(players);
    }else{
        prinw("Entrer un nombre valide");
    }

}

#define MAX_PLAYERS joueur()

char CHIP[MAX_PLAYERS] = "XOAZERTY";

#define MAX_SIZE_ROW largeur()
#define MAX_SIZE_COL hauteur()

char board[MAX_SIZE_ROW][MAX_SIZE_COL];

/*
void PLAYERS(void)
{
    int pl;

    printw(" Entrer le nombre de joueur = ");
    scanw(%d, &pl);
    char *players = (char *)malloc(pl * sizeof(char));
    free(players);
}
*/
/*
void define_board(int x, int y)
{
    printw(" Entrer le nombre de ligne du tableau = ");
    scanw(%d, &x);
    printw(" Entrer le nombre colonne du tableau = ");
    scanw(%d, &y);

    char *board = (char *)malloc(x * y * sizeof(char));
    free(board);
}
*/

void init_board(void)
{

    int row, col;

    for (row = 0; row < MAX_SIZE_ROW; row++)
    {
        for (col = 0; col < MAX_SIZE_COL; col++)
        {
            board[row][col] = ' ';
        }
    }
}

void draw_board(void)
{
    int row, col;
    // prend place a l'origine du repère
    move(0, 0);

    for (row = MAX_SIZE_ROW - 1; row >= 0; row--)
    {
        for (col = 0; col < MAX_SIZE_COL; col++)
        {
            printw("|%c", board[row][col]);
        }
        printw("|\n");
    }
    for (col = 0; col < MAX_SIZE_COL; col++)
    {
        printw("+-");
    }
    printw("+\n");
    for (col = 0; col < MAX_SIZE_COL; col++)
    {
        printw(" %c", 'A' + col);
    }
    printw("\n");
    refresh();
}

char get_col(void)
{
    int key;
    while (1)
    {
        key = getch(); //renvoie valeur ASCII
        if (key == KEY_BACKSPACE)
        {
            return -1;
        }
        else if ('A' <= key && key <= 'A' + MAX_SIZE_COL)
        {
            return key - 'A';
        }
        else if ('a' <= key && key <= 'a' + MAX_SIZE_COL)
        {
            return key - 'a';
        }
    }
}

int add_coin(int col, int player)
{
    for (int row = 0; row < MAX_SIZE_ROW; row++)
    {
        if (board[row][col] == ' ')
        {
            board[row][col] = CHIP[player];
            return (player + 1) % MAX_PLAYERS;
        }
    }
    return player;
}

int game_over(void)
{
    int full = 1;
    for (int row = 0; row < MAX_SIZE_ROW; row++)
    {
        for (int col = 0; col < MAX_SIZE_COL; col++)
        {
            if (board[row][col] == ' ')
            {
                full = 0;
                continue;
            }
            if (col < MAX_SIZE_COL - 4 // teste une ligne
                && board[row][col] == board[row][col + 1] && board[row][col] == board[row][col + 2] && board[row][col] == board[row][col + 3])
            {
                return board[row][col];
            }
            if (row < MAX_SIZE_ROW - 4 // teste une colonne
                && board[row][col] == board[row + 1][col] && board[row][col] == board[row + 2][col] && board[row][col] == board[row + 3][col])
            {
                return board[row][col];
            }
            if (row < MAX_SIZE_ROW - 4 && col < MAX_SIZE_COL - 4 // teste une diagonale à droite
                && board[row][col] == board[row + 1][col + 1] && board[row][col] == board[row + 2][col + 2] && board[row][col] == board[row + 3][col + 3])
            {
                return board[row][col];
            }
            if (row < MAX_SIZE_ROW - 4 && col > 3 // teste une diagonale à gauche
                && board[row][col] == board[row + 1][col - 1] && board[row][col] == board[row + 2][col - 2] && board[row][col] == board[row + 3][col - 3])
            {
                return board[row][col];
            }
        }
    }
    return full;
}

void play(void)
{
    int turn = 0;
    int col;
    int winner = 0;
    while (!(winner = game_over()))
    {
        draw_board();
        printw("\nplayer %u? ", turn + 1);
        if ((col = get_col()) == -1)
        {
            winner = -1;
            break;
        }
        turn = add_coin(col, turn);
    }
    draw_board();
    if (winner == -1)
    {
        printw("\n== no winner\n");
    }
    else
    {
        printw("\n== %c wins!!\n", winner);
    }
    printw("(hit a key)\n");
    refresh();
}

int main(void)
{
    INIT_SCREEN();
    init_board();
    play();
    getch();
    DONE_SCREEN();
}
