#ifndef BOTBASELINE
#define BOTBASELINE

#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include "config.h"

using namespace std;

Point player_rand(int board_game[][WIDTH], int player_id);
Point check_win(int board_game[][WIDTH], int player_id);
Point check_n_tile(int board_game[][WIDTH], int player_id, int n);
Point march(int board_game[][WIDTH], Point move, Point dir, int length);
vector<vector<int>> score_ready(vector<vector<int>> scorecol);
vector<int> sum_sumcol_values(vector<vector<int>> sumcol);
vector<int> score_of_row(int board_game[][WIDTH], Point cordi, Point dir, Point cordf, int player_id);
vector<vector<int>> score_of_col_one(int board_game[][WIDTH], int player_id, Point move);
vector<Point> possible_moves(int board_game[][WIDTH]);
int defend(int board_game[][WIDTH], int player_id, Point move);
int attack(int board_game[][WIDTH], int player_id, Point move);
int stupid_score(int board_game[][WIDTH], int player_id, int antiplayer_id, Point move);
int winning_situation(vector<vector<int>> sumcol);
Point player_baseline(int board_game[][WIDTH], int player_id);
Point defend1(int board_game[][WIDTH], int player_id);
Point attack1(int board_game[][WIDTH], int player_id);
Point player_baseline1(int board_game[][WIDTH], int player_id);

Point player_rand(int board_game[][WIDTH], int player_id) {
    int row, col;
    row = rand() % HEIGHT;
    col = rand() % WIDTH;
    return Point(row, col);
}

Point check_win(int board_game[][WIDTH], int player_id) {
    return check_n_tile(board_game, player_id, 4);
}

Point check_n_tile(int board_game[][WIDTH], int player_id, int n) {
    int check_6h = 1, check_3h = 1, check_5h = 1, check_1h = 1;
    Point temp(-1, -1);
    Point posible_moves[8];
    int p_moves = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board_game[i][j] != player_id) continue;

            check_6h = 1, check_3h = 1, check_5h = 1, check_1h = 1;
            for (int k = 1; k < n; k++) {
                if (board_game[i][j] == board_game[i + k][j]) check_6h++;
                if (board_game[i][j] == board_game[i][j + k]) check_3h++;
                if (board_game[i][j] == board_game[i + k][j + k]) check_5h++;
                if (board_game[i][j] == board_game[i - k][j + k]) check_1h++;
            }

            if (check_6h == n) {
                if (n == 3) {
                    if (board_game[i - 1][j] == 0 && board_game[i + n][j] == 0) return Point(i - 1, j);
                }
                if (board_game[i - 1][j] == 0) {
                    posible_moves[p_moves] = Point(i - 1, j);
                    p_moves++;
                }
                if (board_game[i + n][j] == 0) {
                    posible_moves[p_moves] = Point(i + n, j);
                    p_moves++;
                }

            }
            if (check_3h == n) {
                if (n == 3) {
                    if (board_game[i][j - 1] == 0 && board_game[i][j + n] == 0) return Point(i, j - 1);
                }

                if (board_game[i][j - 1] == 0) {
                    posible_moves[p_moves] = Point(i, j - 1);
                    p_moves++;
                }
                if (board_game[i][j + n] == 0) {
                    posible_moves[p_moves] = Point(i, j + n);
                    p_moves++;
                }

            }
            if (check_5h == n) {
                if (n == 3) {
                    if (board_game[i - 1][j - 1] == 0 && board_game[i + n][j + n] == 0) return Point(i - 1, j - 1);
                }

                if (board_game[i - 1][j - 1] == 0) {
                    posible_moves[p_moves] = Point(i - 1, j - 1);
                    p_moves++;
                }
                if (board_game[i + n][j + n] == 0) {
                    posible_moves[p_moves] = Point(i + n, j + n);
                    p_moves++;
                }

            }
            if (check_1h == n) {
                if (n == 3) {
                    if (board_game[i + 1][j - 1] == 0 && board_game[i - n][j + n] == 0) return Point(i + 1, j - 1);
                }
                if (board_game[i + 1][j - 1] == 0) {
                    posible_moves[p_moves] = Point(i + 1, j - 1);
                    p_moves++;
                }
                if (board_game[i - n][j + n] == 0) {
                    posible_moves[p_moves] = Point(i - n, j + n);
                    p_moves++;
                }

            }

            if (p_moves > 0) {
                return posible_moves[rand() % p_moves];
            }
        }
    }
    return Point(-1, -1);
}

Point march(int board_game[][WIDTH], Point move, Point dir, int length) {
    Point f = Point(move.x + length * dir.x, move.y + length * dir.y);
    while (!(0 <= f.x && f.x < HEIGHT && 0 <= f.y && f.y < WIDTH)) {
        f.x -= dir.x;
        f.y -= dir.y;
    }
    return f;
}

vector<vector<int>> score_ready(vector<vector<int>> scorecol) {
    vector<vector<int>> sumcol;
    sumcol.resize(7);
    for (int i = 0; i < sumcol.size(); i++) {
        sumcol.at(i).resize(4);
        for (int j = 0; j < sumcol.at(i).size(); j++)
            sumcol.at(i).at(j) = 0;
    }
    for (int i = 0; i < scorecol.size(); i++)
        for (int j = 0; j < scorecol.at(i).size(); j++)
            for (int k = 0; k < sumcol.size(); k++)
                if (scorecol.at(i).at(j) == k)
                    sumcol.at(k).at(i) += 1;
    return sumcol;
}

vector<int> sum_sumcol_values(vector<vector<int>> sumcol) {
    vector<int> temp = { 0, 0, 0, 0, 0, 0, 0 };
    for (int i = 0; i < temp.size(); i++)
        if (i == 5)
            for (int j = 0; j < sumcol.at(i).size(); j++) {
                if (sumcol.at(5).at(j) == 1)
                    temp.at(5) = 1;
            }
        else
            for (int j = 0; j < sumcol.at(i).size(); j++)
                temp.at(i) += sumcol.at(i).at(j);
    return temp;
}

vector<int> score_of_row(int board_game[][WIDTH], Point cordi, Point dir, Point cordf, int player_id) {
    vector<int> row, colscores;
    while ((cordi.x != cordf.x + dir.x) || (cordi.y != cordf.y + dir.y)) {
        row.push_back(board_game[cordi.x][cordi.y]);
        cordi.x += dir.x;
        cordi.y += dir.y;
    }

    if (row.size() >= 5)
        for (int i = 0; i < row.size() - 4; i++) {
            vector<int> tempscores = { row.at(i), row.at(i + 1), row.at(i + 2), row.at(i + 3), row.at(i + 4) };
            int blank = count(tempscores.begin(), tempscores.end(), 0);
            int filled = count(tempscores.begin(), tempscores.end(), player_id);
            if (blank + filled < 5) {
                colscores.push_back(6);
            }
            else if (blank == 5)
                colscores.push_back(0);
            else
                colscores.push_back(filled);
        }
    return colscores;
}

vector<vector<int>> score_of_col_one(int board_game[][WIDTH], int player_id, Point move) {
    vector<vector<int>> scores;
    scores.resize(4);
    scores.at(0) = score_of_row(board_game, march(board_game, move, Point(0, -1), 4), Point(0, 1), march(board_game, move, Point(0, 1), 4), player_id);
    scores.at(1) = score_of_row(board_game, march(board_game, move, Point(-1, 0), 4), Point(1, 0), march(board_game, move, Point(1, 0), 4), player_id);
    scores.at(2) = score_of_row(board_game, march(board_game, move, Point(-1, -1), 4), Point(1, 1), march(board_game, move, Point(1, 1), 4), player_id);
    scores.at(3) = score_of_row(board_game, march(board_game, move, Point(-1, 1), 4), Point(1, -1), march(board_game, move, Point(1, -1), 4), player_id);
    return score_ready(scores);
}

vector<Point> possible_moves(int board_game[][WIDTH]) {
    bool isTaken = 0;
    vector<Point> taken;
    vector<Point> coord;
    vector<Point> directions = { Point(0,1), Point(0,-1), Point(1,0), Point(-1,0), Point(1,1), Point(-1,-1), Point(-1,1), Point(1,-1) };
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            if (board_game[i][j] != 0)
                taken.push_back(Point(i, j));

    for (int i = 0; i < directions.size(); i++) {
        Point dir = directions.at(i);
        for (int j = 0; j < taken.size(); j++)
            for (int k = 1; k < 5; k++) {
                Point f = march(board_game, taken.at(j), dir, k);
                for (int m = 0; m < taken.size(); m++) {
                    if (f.x == taken.at(m).x && f.y == taken.at(m).y) {
                        isTaken = 1;
                        break;
                    }
                    else {
                        for (int n = 0; n < coord.size(); n++)
                            if (f.x == coord.at(n).x && f.y == coord.at(n).y) {
                                isTaken = 1;
                                break;
                            }
                        if (isTaken)
                            break;
                    }
                }
                if (!isTaken)
                    coord.push_back(f);
                isTaken = 0;
            }
    }
    return coord;
}

int defend(int board_game[][WIDTH], int player_id, Point move) {
    int M = 1000, dis = 0;
    board_game[move.x][move.y] = player_id;
    vector<vector<int>> sumanticol = score_of_col_one(board_game, player_id, move);
    int d = winning_situation(sumanticol);
    dis += d * (M - 100);
    vector<int> temp = sum_sumcol_values(sumanticol);
    dis += temp.at(6) + temp.at(1) + 4 * temp.at(2) + 8 * temp.at(3) + 16 * temp.at(4);
    return dis;
}

int attack(int board_game[][WIDTH], int player_id, Point move) {
    int M = 1000, adv = 0;
    board_game[move.x][move.y] = player_id;
    vector<vector<int>> sumcol = score_of_col_one(board_game, player_id, move);
    int a = winning_situation(sumcol);
    adv += a * M;
    vector<int> temp = sum_sumcol_values(sumcol);
    adv += temp.at(6) + temp.at(1) + 4 * temp.at(2) + 8 * temp.at(3) + 16 * temp.at(4);
    return adv;
}

int stupid_score(int board_game[][WIDTH], int player_id, int antiplayer_id, Point move) {
    int res = attack(board_game, player_id, move) + defend(board_game, antiplayer_id, move);
    board_game[move.x][move.y] = 0;
    return res;
}

int winning_situation(vector<vector<int>> sumcol) {
    for (int i = 0; i < sumcol.at(5).size(); i++)
        if (sumcol.at(5).at(i) == 1)
            return 5;
    int counts = 0;
    for (int i = 0; i < sumcol.at(4).size(); i++)
        if (sumcol.at(4).at(i) > 0)
            counts += 1;
    if ((counts >= 2) || ((counts >= 1) && (*max_element(sumcol.at(4).begin(), sumcol.at(4).end()) >= 2)))
        return 4;
    for (int i = 0; i < sumcol.at(4).size(); i++)
        if (sumcol.at(4).at(i) >= 1)
            for (int j = 0; j < sumcol.at(3).size(); j++)
                if (i != j && sumcol.at(3).at(j) >= 2)
                    return 4;
    vector<vector<int>> temp = sumcol;
    sort(temp.at(3).begin(), temp.at(3).end());
    reverse(temp.at(3).begin(), temp.at(3).end());
    counts = 0;
    for (int i = 0; i < sumcol.at(3).size(); i++)
        if (sumcol.at(3).at(i) > 0)
            counts += 1;
    if ((counts >= 2) && (sumcol.at(3).at(0) >= sumcol.at(3).at(1)) && (sumcol.at(3).at(1) >= 2))
        return 3;
    return 0;
}

Point player_baseline(int board_game[][WIDTH], int player_id) {
    Point p = check_win(board_game, player_id);
    bool is_empty = false;
    Point movecol = Point(0, 0);
    int maxscorecol = 0;
    int antiplayer_id;
    if (player_id == 1)
        antiplayer_id = -1;
    else
        antiplayer_id = 1;

    if(p.x != -1 && p.y != -1)
        return p;
    else {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++)
                if (board_game[i][j] != 0) {
                    is_empty = true;
                    break;
                }
            if (is_empty)
                break;
        }
        if (!is_empty)
            return player_rand(board_game, player_id);
        else {
            vector<Point> moves = possible_moves(board_game);
            for (int i=0; i < moves.size(); i++) {
                Point move = moves[i];
                if (maxscorecol == 0) {
                    int scorecol = stupid_score(board_game, player_id, antiplayer_id, move);
                    maxscorecol = scorecol;
                    movecol = move;
                }
                else {
                    int scorecol = stupid_score(board_game, player_id, antiplayer_id, move);
                    if (scorecol > maxscorecol) {
                        maxscorecol = scorecol;
                        movecol = move;
                    }
                }
            }
            return movecol;
        }
    }
}

Point defend1(int board_game[][WIDTH], int player_id) {
    //    Pointp p = check_n_tile(board_game, -player_id, 4);
    //    if(p.x != -1 || p.y != -1) return p;
    //    else {
    //        p = check_n_tile(board_game, -player_id, 3);
    //        if(p.x != -1 || p.y != -1) return p;
    //    }
    //    return Point(-1, -1);
    return check_n_tile(board_game, -player_id, 4);
}

Point attack1(int board_game[][WIDTH], int player_id) {
    Point p = check_n_tile(board_game, player_id, 3);
    if (p.x != -1 && p.y != -1) return p;

    p = check_n_tile(board_game, -player_id, 3);
    if (p.x != -1 && p.y != -1) return p;

    p = check_n_tile(board_game, player_id, 2);
    if (p.x != -1 && p.y != -1) return p;

    p = check_n_tile(board_game, player_id, 1);
    if (p.x != -1 && p.y != -1) return p;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board_game[i][j] == -player_id) {
                return Point(i + 1, j);
            }
        }
    }

    return Point(HEIGHT / 2, WIDTH / 2);
}

Point player_baseline1(int board_game[][WIDTH], int player_id) {
    Point p = check_win(board_game, player_id);
    if (p.x != -1 && p.y != -1) {
        return p;
    }
    else {
        p = defend1(board_game, player_id);
        if (p.x != -1 && p.y != -1) {
            return p;
        }
        else {
            return attack1(board_game, player_id);
        }
    }
}

#endif // BOTBASELINE
