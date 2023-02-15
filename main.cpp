#include <iostream>
#include <vector>
#include <limits>

struct node {
    int heuristic;
    int **table;
    node *next;
    int holeMoved;
};

int **startTable();

void printTable(int **table, int southPoints, int northPoints);

bool movement(int **table, bool isAI, int hole, int &points);

bool checkTable(int **table, int &pointsIa, int &pointsPlayer);

void clearTable(int **table);

void clearMemory(int **table);

int heuristicTable(int **table, bool ai);

int createTree(int **table);

int **copyTable(int **table);

int main() {
    int northPoints = 0;
    int southPoints = 0;
    int **actualTableKalah = startTable();
    std::cout << "Bienvenido al juego de kalah!\n";
    bool AIMoment = false;//true to iA, false to player
    printTable(actualTableKalah, southPoints, northPoints);
    do {
        int hole;
        if (AIMoment) {
            //here is the heuristic and that stuff
            AIMoment = movement(actualTableKalah, true, createTree(actualTableKalah), northPoints);
        } else {
            std::cout << "Inserte la posicion desde la cual quiere iniciar a mover sus semillas a las derecha\n";
            std::cin >> hole;
            if (movement(actualTableKalah, false, hole, southPoints) == false) {
                AIMoment = true;
            }
        }
        printTable(actualTableKalah, southPoints, northPoints);

    } while (checkTable(actualTableKalah, northPoints, southPoints));
    clearTable(actualTableKalah);
    printTable(actualTableKalah, southPoints, northPoints);
    if (northPoints > southPoints) {
        std::cout << "Has perdido!!";
    } else {
        std::cout << "Has Ganado!! ";
    }
    clearMemory(actualTableKalah);
    return 0;
}

void clearMemory(int **table) {
    for (int i = 0; i < 2; i++) {
        delete[] table[i];
    }
    delete[] table;
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
    std::cout << "\t\tNorth AI Points====>" << northPoints << std::endl;
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
    std::cout << "\t\tSouth Player Points====>" << southPoints << std::endl;
}

bool movement(int **table, bool isAI, int hole, int &points) {
    bool anotherMove = false;
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
                lastMoveHole[0] = -1;
                lastMoveHole[1] = -1;
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
                    if (ballsLeft == 1 && lastMoveHole[1] == 0) {
                        anotherMove = true;
                    }
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
                    lastMoveHole[0] = 1;
                    lastMoveHole[1] = i;
                }
                if (ballsLeft > 0) {
                    points = points + 1;
                    if (ballsLeft == 1 && lastMoveHole[1] == 5) {
                        anotherMove = true;
                    }
                    ballsLeft = ballsLeft - 1;
                }
                limit = 0;
                startIn = 0;
            } else {
                for (int i = 5; i >= limit && ballsLeft > 0; --i) {
                    table[0][i] = table[0][i] + 1;
                    ballsLeft = ballsLeft - 1;
                }
                lastMoveHole[0] = -1;
                lastMoveHole[1] = -1;
                limit = 5;
            }
        }
    }
    if ((lastMoveHole[0] != -1 && lastMoveHole[1] != -1) && table[lastMoveHole[0]][lastMoveHole[1]] == 1) {
        if (lastMoveHole[0] == 0) {
            points = points + table[lastMoveHole[0]][lastMoveHole[1]] + table[1][lastMoveHole[1]];
            table[lastMoveHole[0]][lastMoveHole[1]] = 0;
            table[1][lastMoveHole[1]] = 0;
        } else {
            points = points + table[lastMoveHole[0]][lastMoveHole[1]] + table[0][lastMoveHole[1]];
            table[lastMoveHole[0]][lastMoveHole[1]] = 0;
            table[0][lastMoveHole[1]] = 0;
        }
    }
    //it returns if the player has another movement
    return anotherMove;
}

// this scan the table and return if there are movements left and add the points if any player can not do a movement
bool checkTable(int **table, int &pointsIa, int &pointsPlayer) {
    bool iaHasMovements = false;
    bool playerHasMovements = false;
    bool keepPlaying = false;
    int iaSidePoints = 0;
    int playerSidePoints = 0;
    for (int i = 0; i <= 5; ++i) {
        if (table[0][i] != 0) {
            iaSidePoints += table[0][i];
            iaHasMovements = true;
        }
        if (table[1][i] != 0) {
            playerSidePoints += table[1][i];
            playerHasMovements = true;
        }
    }
    if (iaHasMovements && playerHasMovements) {
        keepPlaying = true;
    } else {
        if (iaHasMovements) {
            pointsIa += iaSidePoints;
        }
        if (playerHasMovements) {
            pointsPlayer += playerSidePoints;
        }
    }
    return keepPlaying;
}

void clearTable(int **table) {
    for (int i = 0; i <= 5; ++i) {
        table[0][i] = 0;
        table[1][i] = 0;
    }
}

// I will count the number of movement that the adversary has at that instant
// and the number of movements that the actual player can give to the adversary
int heuristicTable(int **table, bool ai) {
    int movements = 0;
    bool giveMovement = false;
    bool oneMoveMore = false;
    if (ai) {
        for (int i = 0; i <= 5; ++i) {
            //adversary movement
            if (table[1][i] != 0) {
                movements += 1;
            }
            // last movement get one more movement
            if (table[1][i] - (5 - i) == 1) {
                oneMoveMore = true;
            }
            // ai can give a move
            if ((table[0][i] - (i + 1)) > 0) {
                giveMovement = true;
            }
        }
    } else {
        for (int i = 0; i <= 5; ++i) {
            //    AI Movements
            if (table[0][i] != 0) {
                movements += 1;
            }
            // last movement get one more movement
            if ((table[0][i] - i) == 1) {
                oneMoveMore = true;
            }
            // player can give a move
            if ((table[1][i] - (i + 1)) > 0) {
                giveMovement = true;
            }
        }
    }
    if (giveMovement) {
        movements += 1;
    }
    if (oneMoveMore) {
        movements += 1;
    }
    return movements;
}

int **copyTable(int **table){
    int **newTable = new int *[2];
    for (int i = 0; i < 2; i++) {
        newTable[i] = new int[6];
        for (int j = 0; j < 6; j++) {
            newTable[i][j] = table[i][j];
        }
    }
    return newTable;
}

int createTree(int **table){
    std::cout<<"IA Jugando"<<std::endl;
    node root = {};
    int **nextMoveTable;
    int points = 0;
    int minimum = std::numeric_limits<int>::max();
    int maximum = std::numeric_limits<int>::min();
    int holeSelected;
    root.table = table;
    root.next = new node[6];
    //creating possible movements of the AI
    for (int i = 0; i < 6; ++i) {
        nextMoveTable = copyTable(table);
        movement(nextMoveTable, false,i,points);
        node son = {};
        son.table = nextMoveTable;
        son.holeMoved = i;
        root.next[i] = son;
    }
    // creating possible movements for the player

    for (int i = 0; i < 6; ++i) {
        root.next[i].next = new node[6];
        for (int j = 0; j < 6; ++j) {
            node grandSon = {};
            nextMoveTable = copyTable(root.next[i].table);
            movement(nextMoveTable, true, j, points);
            grandSon.table = nextMoveTable;
            grandSon.holeMoved = j;
            grandSon.heuristic = heuristicTable(grandSon.table, true);
            root.next[i].next[j] = grandSon;
        }
    }

    //give heuristic to the son
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (root.next[i].next[j].heuristic < minimum){
                minimum = root.next[i].next[j].heuristic;
            }
        }
        root.next[i].heuristic = minimum;
        minimum = std::numeric_limits<int>::max();
    }

    //give heuristic to the desicion to take
    do {
        for (int i = 0; i < 6; ++i) {
            if (root.next[i].heuristic > maximum){
                maximum = root.next[i].heuristic;
                holeSelected = i;
            }
        }
        root.heuristic = maximum;
        if (table[0][holeSelected] == 0){
            root.next[holeSelected].heuristic = 0;
            maximum = std::numeric_limits<int>::min();
        }
    } while (table[0][holeSelected] == 0);
    return holeSelected;
}