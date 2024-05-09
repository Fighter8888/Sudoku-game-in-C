//
// Created by Fighter88 on 1/30/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>


#define E 36                    //ثابت اعداد حذفی جدول
int default_board[9][9];        //جدول
int solved_board[9][9];         //جدول حل شده

#include "Generator.c"



//تعریف تابع پرینت
void sudoku_printer(int board[9][9]);


//بررسی تکرار اعداد
bool check_repeat(int select[9], int num) {
    for (int i = 0; i < 9; i++) {
        if (select[i] == num) return true;
    }
return false;
}


//بررسی درست بودن اعداد وارد شده
bool valid_input(int board[9][9], int i, int j, int input) {
    int select[9];

//بررسی عمودی
    memcpy(select, board[i], sizeof(select));
    if (check_repeat(select, input)) return false;

//بررسی افقی
    for (int k = 0; k < 9; k++) {
    select[k] = board[k][j];
    }
    if (check_repeat(select, input)) return false;

//بررسی در هر مربع 3*3
    int i_from, i_to;
    int j_from, j_to;
    i_from = i - i % 3;
    i_to = i_from + 3;
    j_from = j - j % 3;
    j_to = j_from + 3;
    int x = 0;
    for (int k = i_from; k < i_to; k++) {
        for (int l = j_from; l < j_to; l++) {
            select[x++] = (int) board[k][l];
        }
    }
return !check_repeat(select, input);
}


//بررسی صفر نبودن (null) اعداد جدول
bool null_check(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) return false;
        }
    }
return true;
}


//صفر کردن یا خالی کردن تمام خانه ها
void do_null(int arr[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            arr[i][j] = (int) NULL;
        }
    }
}


//بررسی حل شدن جدول
bool check_solved(int arr[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (arr[i][j] != solved_board[i][j]) return false;
        }
    }
return true;
}


//پاک کردن مکان های خالی در رشته
char *cleaner(char *str){
    char *end;

    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

return str;
}


//تابع دریافت دستور از کابر که مانند پنل کاربری عمل میکند
void input() {
    int i, j;
    int nums[3], e = E;
    int board[9][9];
    bool table_made = false, validNumbers = false;


    while (1) {
        char user_input[10];
        validNumbers = false;
        printf("\nplease select:");
        printf("\n%s", (!table_made ? "-start-  " : "-new game-  -reset-  -solve-  -hint-  -(row,col,number)-  "));
        printf("-help-  -exit-\n");
        fgets(user_input, 10, stdin);

        for (i = 0; i < strlen(user_input) - 1; i++)
            user_input[i] = tolower(user_input[i]);
            strcpy(user_input , cleaner(user_input));

        if (user_input[1] == ',' && user_input[3] == ',') {
            validNumbers = true;
            for (int i = 0; i < 5; i += 2) {
                j = (int) (user_input[i] - '0');
                if (j < 10 && j > 0)
                    nums[(i / 2)] = j;
                else validNumbers = false;
            }
        }
        if ((!strcmp(user_input, "start") && !table_made) || (!strcmp(user_input, "new game") && table_made)) {
            do_null(default_board);
            do_null(board);
            do_null(solved_board);
            fill_Values(e);
            table_made = true;
            memcpy(board, default_board, sizeof(board));
            sudoku_printer(board);
        }
    else if (validNumbers && table_made) {
        if (default_board[nums[0] - 1][nums[1] - 1] == 0) {
            (board[nums[0] - 1][nums[1] - 1]) = nums[2];
            sudoku_printer(board);
            if (null_check(board)) {
                if (check_solved(board)) {
                    printf("\a\nYou solved sudoku.!!!\n!!!congratulation!!!\n");
                    do_null(default_board);
                    do_null(board);
                    do_null(solved_board);
                    table_made = false;
                }
            }
        }
        else{
            printf("You can't change this.(i=%d,j=%d,number=%d)", nums[0], nums[1],default_board[nums[0] - 1][nums[1] - 1]);
        }
    }
    else if (!strcmp(user_input, "hint") && table_made) {
        sudoku_printer(solved_board);
        for (i = 3; i > 0; i--) {
            printf("%d\r", i);
            sleep(1);
        }
        sudoku_printer(board);
    }
    else if (!strcmp(user_input, "solve") && table_made) {
        sudoku_printer(solved_board);
    }
    else if (!strcmp(user_input, "reset") && table_made) {
        memcpy(board, default_board, sizeof(board));
        sudoku_printer(board);
    }
        else if (!strcmp(user_input, "help")) {
            printf("\nWelcome to the guide menu:\n\n");
            printf("-missing digits for game by default:%d.\n", E);
            if (table_made) {
                printf("-new game: Generate new sudoku table.\n");
                printf("-reset: Resets the board.\n");
                printf("-solve: Solves the sudoku puzzle.\n");
                printf("-hint: Show solved sudoku for 3 second.\n");
                printf("-Entering row,col,number - marks that square with the number.\n");
            }
            else {
                printf("-start: Generate sudoku puzzle.\n");
            }
            printf("-exit: Exit the program.\n");
        }
        else if (!strcmp(user_input, "exit")) {
            printf("end of the program");
            sleep(2);
            exit(1);
        }
        else{
        printf("choose valid command.(%s)\n", user_input);
        }
    }
}


//تابع اصلی
int main() {
    srand(time(0));     //هربار اعداد تصادفی جدید ایجاد میکنه
    printf("made as a university project by Abolfazl Kazemi\n");
    printf("number of missing digits = %d\n",E);
    input();
return 1;
}


//تابع چاپ جدول سودوکو
void sudoku_printer(int board[9][9]) {
    printf("\n\n\n\n\n\n\n\n\n");
    char line[] = "  ||---+---+---||---+---+---||---+---+---||";
    printf("     1   2   3    4   5   6    7   8   9");
    for (int i = 0; i < 9; i++) {
        if((i % 3) == 0) {
        printf("\n  ||===========||===========||===========||\n");
        }
        else
            printf("\n%s\n",line);
        printf("%d ",i+1);
        for (int j = 0; j < 9; j++) {
            if((j % 3) == 0){
                printf("||");
            }
            else
                printf("|");
            if(board[i][j] != 0){
                if(default_board[i][j] == 0) {
                    int prv = board[i][j];
                    board[i][j] = 0;
                    if(!valid_input(board , i , j,prv)){
                    }
                    else {
                    }
                    board[i][j] = prv;
                    printf(" %d ", board[i][j]);
                }
                else
                    printf(" %d ", board[i][j]);
            }
            else printf("   ");
        }
        printf("||");
    }
    printf("\n  ||===========||===========||===========||\n");
}