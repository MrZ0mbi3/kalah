#include <iostream>

int **startTable();

void printTable(int **table, int southPoints, int northPoints);

void movement(int **table, bool isAI, int hole, int &points);

int main() {
    int northPoints = 0;
    int southPoints = 0;
    int input = 0;
    //std::cin>>input;
    int **tableKalah = startTable();
    printTable(tableKalah, northPoints, southPoints);
    movement(tableKalah, false, 1, southPoints);
    printTable(tableKalah, southPoints, northPoints);
    movement(tableKalah, false, 2, southPoints);
    printTable(tableKalah, southPoints, northPoints);
    movement(tableKalah, false, 3, southPoints);
    printTable(tableKalah, southPoints, northPoints);
    movement(tableKalah, false, 4, southPoints);
    printTable(tableKalah, southPoints, northPoints);
    movement(tableKalah, false, 5, southPoints);
    printTable(tableKalah, southPoints, northPoints);
    movement(tableKalah, true, 5, northPoints);
    printTable(tableKalah, southPoints, northPoints);
    movement(tableKalah, true, 4, northPoints);
    printTable(tableKalah, southPoints, northPoints);
    movement(tableKalah, true, 3, northPoints);
    printTable(tableKalah, southPoints, northPoints);
    movement(tableKalah, true, 0, northPoints);
    printTable(tableKalah, southPoints, northPoints);

    return 0;
}

int **startTable() {
    int **table = new int *[2];
    for (int i = 0; i < 2; i++) {
        table[i] = new int[6];
        for (int j = 0; j < 6; j++) {
            table[i][j] = 4;
        }
    }
    return table;
}

void printTable(int **table, int southPoints, int northPoints) {
    std::cout << northPoints << "       North AI" << std::endl;
    for (int i = -1; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            if (i == -1) {
                std::cout << "\t" << j;
            } else {
                std::cout << "\t" << table[i][j];
            }
        }
        std::cout << std::endl;
    }
    std::cout << "South     " << southPoints << std::endl;
}

void movement(int **table, bool isAI, int hole, int &points) {
    int ballsLeft = table[1][hole];
    int limit = 5;
    int startIn = hole + 1;
    int lastMoveHole[2] = {-1, -1};
    if (isAI) {
        limit = 0;
        ballsLeft = table[0][hole];
        table[0][hole] = 0;
        startIn = hole - 1;
    } else {
        table[1][hole] = 0;
    }

    while (ballsLeft > 0) {
        if (isAI) {
            if (limit == 5) {
                for (int i = 0; i <= limit && ballsLeft > 0; ++i) {
                    table[1][i] = table[1][i] + 1;
                    ballsLeft = ballsLeft - 1;
                }
                limit = 0;
            } else {
                for (int i = startIn; i >= limit && ballsLeft > 0; --i) {
                    table[0][i] = table[0][i] + 1;
                    ballsLeft = ballsLeft - 1;
                    lastMoveHole[0] = 0;
                    lastMoveHole[1] = i;
                }
                if (ballsLeft > 0) {
                    points = points + 1;
                    ballsLeft = ballsLeft - 1;
                }
                limit = 5;
                startIn = 5;
            }
        } else {
            if (limit == 5) {
                for (int i = startIn; i <= limit && ballsLeft > 0; ++i) {
                    table[1][i] = table[1][i] + 1;
                    ballsLeft = ballsLeft - 1;
                    lastMoveHole[0] = 0;
                    lastMoveHole[1] = i;
                }
                if (ballsLeft > 0) {
                    points = points + 1;
                    ballsLeft = ballsLeft - 1;
                }
                limit = 0;
                startIn = 0;

            } else {
                for (int i = 5; i >= limit && ballsLeft > 0; --i) {
                    table[0][i] = table[0][i] + 1;
                    ballsLeft = ballsLeft - 1;
                }
                limit = 5;
            }
        }
    }
    if (table[lastMoveHole[0]][lastMoveHole[1]] == 1 && (lastMoveHole[0] != -1 && lastMoveHole[1] != -1) ){
        if (lastMoveHole[0] == 0){
            points = points + table[lastMoveHole[0]][lastMoveHole[1]] + table[1][lastMoveHole[1]];
            table[lastMoveHole[0]][lastMoveHole[1]] = 0;
            table[1][lastMoveHole[1]] = 0;
        }else{
            points = points + table[lastMoveHole[0]][lastMoveHole[1]] + table[0][lastMoveHole[1]];
            table[lastMoveHole[0]][lastMoveHole[1]] = 0;
            table[0][lastMoveHole[1]] = 0;
        }
    }
}
