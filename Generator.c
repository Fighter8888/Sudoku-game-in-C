//
// Created by Fighter88 on 1/30/2023.
//

#define N 9     // ثابت تعداد سطر و ستون
#define SRN 3   // ثابت تعداد سطر و ستون در هر مربع3*3



//تابع ساخت اعداد تصادفی
int Random_Num_Generator(int num) {
    return (int) ( rand() % num ) + 1;
}


//تابع بررسی وجود مقدار در هر مربع3*3
bool UnUsed_In_Box(int rowStart, int colStart, int num) {
    for (int i = 0; i < SRN; i++)
        for (int j = 0; j < SRN; j++)
            if (default_board[rowStart + i][colStart + j] == num)
                return false;
    return true;
}


//تابع بررسی وجود مقدار در هر سطر
bool UnUsed_In_Row(int i, int num) {
    for (int j = 0; j < N; j++)
        if (default_board[i][j] == num)
            return false;
    return true;
}


//تابع بررسی مقدار در هر ستون
bool UnUsed_In_Col(int j, int num) {
    for (int i = 0; i < N; i++)
        if (default_board[i][j] == num)
            return false;
    return true;
}


//تابع بررسی برقراری قوانین سودوکو برای خانه به مختصات i*j
bool safe_check(int i, int j, int num) {
    return (UnUsed_In_Row(i, num) && UnUsed_In_Col(j, num) && UnUsed_In_Box(i - i % SRN, j - j % SRN, num));
}


//تابع پر کردن مربع ها
void box_fill(int row, int col) {
    int num;
    for (int i = 0; i < SRN; i++) {
        for (int j = 0; j < SRN; j++) {
            do {
                num = Random_Num_Generator(N);
            }while (!UnUsed_In_Box(row, col, num));

            default_board[row + i][col + j] = num;
        }
    }
}


//تابع اصلاح خانه ها
void fill_diagonal() {
    for (int i = 0; i < N; i = i + SRN)
        box_fill(i, i);
}


//تابع پر کردن خانه های باقی مانده
bool fill_Remain(int i, int j) {

    if (j >= N && i < N - 1) {
        i++;;
        j = 0;
    }

    if (i >= N && j >= N)
        return true;

    if (i < SRN) {
        if (j < SRN)
            j = SRN;
    }

    else if (i < N - SRN) {
        if (j == ((int)(i / SRN)) * SRN)
            j = j + SRN;
    }

    else {
        if (j == N - SRN) {
            i++;
            j = 0;
            if (i >= N)
                return true;
        }
    }

    for (int num = 1; num <= N; num++) {
        if (safe_check(i, j, num)) {
            default_board[i][j] = num;
            if (fill_Remain(i, j + 1))
                return true;

            default_board[i][j] = 0;
        }
    }
    return false;
}


//تابع حذف E تا عدد(به صورت تصادفی) از جدول
void remove_E_Digits(int e) {
    int count = e;
    while (count != 0) {
        int cellId = Random_Num_Generator(N * N);

        int i = (cellId / N);
        int j = cellId % 9;
        if (j != 0)
            j--;

        if (default_board[i][j] != 0) {
            count--;
            default_board[i][j] = 0;
        }
    }
}

//تابع پر کردن خانه های جدول
void fill_Values(int e) {
    fill_diagonal();
    fill_Remain(0, SRN);
    memcpy(solved_board,default_board,sizeof(solved_board));
    remove_E_Digits(e);
}
