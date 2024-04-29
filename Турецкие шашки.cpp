#include <iostream>
#include <vector>
#include <cmath>
#include <Windows.h>

using namespace std;

const int BOARD_SIZE = 8;

enum Piece {
    EMPTY,
    WHITE,
    BLACK,
    WHITE_KING,
    BLACK_KING
};

struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
};

class TurkishCheckers {
private:
    Piece board[BOARD_SIZE][BOARD_SIZE];

public:
    TurkishCheckers() {
        initializeBoard();
    }

        void initializeBoard() {
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                if (row == 2 || row == 1) board[row][col] = WHITE;
                else if (row == 5 || row == 6) board[row][col] = BLACK;
                else board[row][col] = EMPTY;
            }
        }
    }

    void printBoard() {
        cout << "  0 1 2 3 4 5 6 7\n";
        for (int row = 0; row < BOARD_SIZE; ++row) {
            cout << row << " ";
            for (int col = 0; col < BOARD_SIZE; ++col) {
                switch (board[row][col]) {
                case EMPTY:
                    cout << ". ";
                    break;
                case WHITE:
                    cout << "W ";
                    break;
                case BLACK:
                    cout << "B ";
                    break;
                case WHITE_KING:
                    cout << "WK";
                    break;
                case BLACK_KING:
                    cout << "BK";
                    break;
                }
            }
            cout << endl;
        }
    }

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece player) {
        // Проверка правильности хода
        if (toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE) {
            return false; // Целевая позиция находится за пределами доски
        }

        if (board[toRow][toCol] != EMPTY) {
            return false;  // Целевая позиция уже занята другой фигурой
        }

        int rowDiff = toRow - fromRow;
        int colDiff = toCol - fromCol;

        if (player == WHITE) {
            // Для белых: ход вниз по вертикали на 1 клетку или ход по горизонтали на 1 клетку
            if ((rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && abs(colDiff) == 1)) {
                return true;
            }
        }
        else if (player == BLACK) {
            // Для черных: ход вверх по вертикали на 1 клетку или ход по горизонтали на 1 клетку
            if ((rowDiff == -1 && colDiff == 0) || (rowDiff == 0 && abs(colDiff) == 1)) {
                return true;
            }
        }

        return false;  // Ход недопустим
    }

    bool isCaptureMove(int fromRow, int fromCol, int toRow, int toCol, Piece player) {
        // Проверка правильности хода
        if (toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE) {
            return false; // Целевая позиция находится за пределами доски
        }

        if (board[toRow][toCol] != EMPTY) {
            return false;  // Целевая позиция уже занята другой фигурой
        }

        int rowDiff = toRow - fromRow;
        int colDiff = toCol - fromCol;

        if (player == WHITE) {
            // Для белых: ход вниз по вертикали на 2 клетки и битие шашки противника
            if (rowDiff == 2 && colDiff == 0 && board[fromRow + 1][fromCol] == BLACK) {
                return true;
            }
            // Для белых: ход по горизонтали на 2 клетки и битие шашки противника
            if (rowDiff == 0 && colDiff == 2 && board[fromRow][fromCol + 1] == BLACK) {
                return true;
            }
            if (rowDiff == 0 && colDiff == -2 && board[fromRow][fromCol - 1] == BLACK) {
                return true;
            }
        }
        else if (player == BLACK) {
            // Для черных: ход вверх по вертикали на 2 клетки и битие шашки противника
            if (rowDiff == -2 && colDiff == 0 && board[fromRow - 1][fromCol] == WHITE) {
                return true;
            }
            // Для черных: ход по горизонтали на 2 клетки и битие шашки противника
            if (rowDiff == 0 && colDiff == 2 && board[fromRow][fromCol + 1] == WHITE) {
                return true;
            }
            if (rowDiff == 0 && colDiff == -2 && board[fromRow][fromCol - 1] == WHITE) {
                return true;
            }
        }

        return false;  // Ход недопустим
    }

        void makeMove(int fromRow, int fromCol, int toRow, int toCol) {
        Piece piece = board[fromRow][fromCol];
        board[fromRow][fromCol] = EMPTY;
        board[toRow][toCol] = piece;

        // Проверка превращения шашки в дамку
        if (piece == WHITE && toRow == BOARD_SIZE - 1) {
            board[toRow][toCol] = WHITE_KING;
        }
        else if (piece == BLACK && toRow == 0) {
            board[toRow][toCol] = BLACK_KING;
        }
    }

void makeCaptureMove(int fromRow, int fromCol, int toRow, int toCol) {
    // Выполнение хода с битием шашки противника
    Piece piece = board[fromRow][fromCol];
    board[fromRow][fromCol] = EMPTY;
    board[toRow][toCol] = piece;

    // Удаление шашки противника
    int capturedRow = (fromRow + toRow) / 2;
    int capturedCol = (fromCol + toCol) / 2;
    board[capturedRow][capturedCol] = EMPTY;

    // Проверка превращения шашки в дамку
    if (piece == WHITE && toRow == BOARD_SIZE - 1) {
        board[toRow][toCol] = WHITE_KING;
    }
    else if (piece == BLACK && toRow == 0) {
        board[toRow][toCol] = BLACK_KING;
    }
}

bool hasCaptureMove(Piece player) {
    // Проверка наличия возможных ходов с битием шашки для игрока
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board[row][col] == player || board[row][col] == player + 2) {
                // Проверка возможных направлений для бития шашки
                int directions[4][2] = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };
                for (const auto& direction : directions) {
                    int targetRow = row + 2 * direction[0];
                    int targetCol = col + 2 * direction[1];
                    int capturedRow = row + direction[0];
                    int capturedCol = col + direction[1];
                    if (targetRow >= 0 && targetRow < BOARD_SIZE && targetCol >= 0 && targetCol < BOARD_SIZE &&
                        board[targetRow][targetCol] == EMPTY && board[capturedRow][capturedCol] != player &&
                        board[capturedRow][capturedCol] != EMPTY) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

    bool isGameEnd(Piece& winner) {
        bool whitePiecesLeft = false;
        bool blackPiecesLeft = false;

        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                if (board[row][col] == WHITE || board[row][col] == WHITE_KING) {
                    whitePiecesLeft = true;
                }
                else if (board[row][col] == BLACK || board[row][col] == BLACK_KING) {
                    blackPiecesLeft = true;
                }
            }
        }

        if (!whitePiecesLeft) {
            winner = BLACK;
            return true;
        }
        else if (!blackPiecesLeft) {
            winner = WHITE;
            return true;
        }
        else {
            return false;
        }
    }

    void play() {
        Piece winner = EMPTY;
        bool isWhiteTurn = true; // Первый ход делает белый игрок

        while (!isGameEnd(winner)) {
            system("cls");
            printBoard();

            if (isWhiteTurn) {
                cout << "Ход белых (W): ";
            }
            else {
                cout << "Ход черных (B): ";
            }

            int fromRow, fromCol, toRow, toCol;
            cin >> fromRow >> fromCol >> toRow >> toCol;

            if (isCaptureMove(fromRow, fromCol, toRow, toCol, isWhiteTurn ? WHITE : BLACK)) {
                makeCaptureMove(fromRow, fromCol, toRow, toCol);

                // Проверка возможности продолжения бития
                while (hasCaptureMove(isWhiteTurn ? WHITE : BLACK)) {
                    system("cls");
                    printBoard();
                    cout << "Продолжение бития...\n";

                    cout << "Выберите следующий ход: ";
                    int nextFromRow, nextFromCol, nextToRow, nextToCol;
                    cin >> nextFromRow >> nextFromCol >> nextToRow >> nextToCol;

                    if (isCaptureMove(nextFromRow, nextFromCol, nextToRow, nextToCol, isWhiteTurn ? WHITE : BLACK)) {
                        makeCaptureMove(nextFromRow, nextFromCol, nextToRow, nextToCol);
                    }
                    else {
                        cout << "Неверный ход. Попробуйте еще раз.\n";
                        Sleep(2000); // Задержка для отображения сообщения
                    }
                }

                isWhiteTurn = !isWhiteTurn; // Смена хода
            }
            else if (isValidMove(fromRow, fromCol, toRow, toCol, isWhiteTurn ? WHITE : BLACK)) {
                makeMove(fromRow, fromCol, toRow, toCol);
                isWhiteTurn = !isWhiteTurn; // Смена хода
            }
            else {
                cout << "Неверный ход. Попробуйте еще раз.\n";
                Sleep(2000); // Задержка для отображения сообщения
            }
        }

        system("cls");
        printBoard();

        if (winner == WHITE) {
            cout << "Победили белые (W)!\n";
        }
        else if (winner == BLACK) {
            cout << "Победили черные (B)!\n";
        }
        else {
            cout << "Ничья!\n";
        }
    }
};
int main() {
    setlocale(LC_ALL, "ru");
    TurkishCheckers game;
    game.play();
    return 0;
}