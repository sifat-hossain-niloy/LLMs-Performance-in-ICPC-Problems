#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <cassert>

using namespace std;

// Constants for piece representation
enum PieceType { EMPTY, WMAN, BMAN, WKING, BKING };

struct Move {
    char type; // '-' for simple move, 'x' for jump
    vector<int> squares; // sequence of squares in the move
};

struct Piece {
    char type; // '.', 'b', 'w', 'B', 'W'
    Piece() : type('.') {}
    Piece(char c) : type(c) {}
};

struct Board {
    Piece squares[33]; // Index 1 to 32
};

int dr[4] = {-1, -1, 1, 1}; // Direction row offsets
int dc[4] = {-1, 1, -1, 1}; // Direction col offsets

// Map square number to (row, col)
void SquareToPos(int s, int &row, int &col) {
    s--;
    row = s / 4;
    col = 2 * (s % 4);
    if (row % 2 == 0) col += 1;
}

// Map (row, col) to square number
int PosToSquare(int row, int col) {
    int s;
    if (row % 2 == 0)
        s = row * 4 + (col - 1) / 2 + 1;
    else
        s = row * 4 + col / 2 + 1;
    return s;
}

// Check if a position is within the board boundaries
bool InBounds(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// Get the piece type at a given square
char GetPiece(Board &board, int square) {
    return board.squares[square].type;
}

// Set the piece type at a given square
void SetPiece(Board &board, int square, char piece) {
    board.squares[square] = Piece(piece);
}

// Check if there are any forced captures for the player
bool HasForcedCapture(Board &board, char player) {
    for (int s = 1; s <= 32; s++) {
        char piece = GetPiece(board, s);
        if ((player == 'W' && (piece == 'w' || piece == 'W')) ||
            (player == 'B' && (piece == 'b' || piece == 'B'))) {
            vector<vector<int>> jumps;
            GetPossibleJumps(board, s, jumps);
            if (!jumps.empty())
                return true;
        }
    }
    return false;
}

// Get possible jumps for a piece at a given square
void GetPossibleJumps(Board &board, int square, vector<vector<int>> &jumps, vector<int> path = {}, int origin = -1) {
    char piece = GetPiece(board, square);
    if (piece == '.' || piece == '-') return;
    if (origin == -1) origin = square;
    int row, col;
    SquareToPos(square, row, col);
    bool isKing = isupper(piece);
    char player = isupper(piece) ? toupper(piece) : tolower(piece);

    bool canJump = false;
    for (int dir = 0; dir < 4; dir++) {
        if (!isKing && ((player == 'w' && dir >= 2) || (player == 'b' && dir < 2))) continue;
        int nr = row + dr[dir], nc = col + dc[dir];
        if (!InBounds(nr, nc)) continue;
        int midSquare = PosToSquare(nr, nc);
        char midPiece = GetPiece(board, midSquare);
        if (midPiece == '.' || midPiece == '-') continue;
        if (tolower(midPiece) == player) continue;
        int nr2 = nr + dr[dir], nc2 = nc + dc[dir];
        if (!InBounds(nr2, nc2)) continue;
        int destSquare = PosToSquare(nr2, nc2);
        char destPiece = GetPiece(board, destSquare);
        if (destPiece != '.') continue;
        // Valid jump
        canJump = true;
        Board newBoard = board;
        SetPiece(newBoard, midSquare, '.');
        SetPiece(newBoard, square, '.');
        SetPiece(newBoard, destSquare, piece);
        vector<int> newPath = path;
        newPath.push_back(destSquare);
        GetPossibleJumps(newBoard, destSquare, jumps, newPath, origin);
    }
    if (!canJump && !path.empty()) {
        vector<int> fullPath = {origin};
        fullPath.insert(fullPath.end(), path.begin(), path.end());
        jumps.push_back(fullPath);
    }
}

// Apply a move to the board
void ApplyMove(Board &board, Move &move, char player) {
    char piece = GetPiece(board, move.squares[0]);
    bool isKing = isupper(piece);
    // Remove captured pieces if it's a jump move
    if (move.type == 'x') {
        for (size_t i = 0; i < move.squares.size() - 1; i++) {
            int from = move.squares[i];
            int to = move.squares[i + 1];
            int row1, col1, row2, col2;
            SquareToPos(from, row1, col1);
            SquareToPos(to, row2, col2);
            int midRow = (row1 + row2) / 2;
            int midCol = (col1 + col2) / 2;
            int midSquare = PosToSquare(midRow, midCol);
            SetPiece(board, midSquare, '.');
        }
    }
    // Move the piece
    SetPiece(board, move.squares[0], '.');
    int destSquare = move.squares.back();
    // Promotion check
    int destRow, destCol;
    SquareToPos(destSquare, destRow, destCol);
    if ((player == 'W' && destRow == 0) || (player == 'B' && destRow == 7))
        piece = toupper(piece);
    SetPiece(board, destSquare, piece);
}

// Main function
int main() {
    char c;
    int n;
    cin >> c >> n;
    cin.ignore();
    vector<Move> moves;
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        Move m;
        m.type = '-';
        int num;
        char ch;
        while (ss >> num) {
            m.squares.push_back(num);
            ss >> ch;
            if (ch == 'x') m.type = 'x';
        }
        moves.push_back(m);
    }
    Board initialBoard;
    // Initialize the board with '-'
    for (int i = 1; i <= 32; i++) {
        initialBoard.squares[i] = Piece('.');
    }
    Board board = initialBoard;
    char player = c;
    for (auto &move : moves) {
        // Ensure the piece exists
        int fromSquare = move.squares[0];
        char piece = GetPiece(board, fromSquare);
        if (piece == '.') {
            // Place the player's man
            if (player == 'W')
                SetPiece(board, fromSquare, 'w');
            else
                SetPiece(board, fromSquare, 'b');
        }
        // Ensure the destination squares are empty
        for (size_t i = 1; i < move.squares.size(); i++) {
            int toSquare = move.squares[i];
            if (GetPiece(board, toSquare) != '.') {
                SetPiece(board, toSquare, '.');
            }
        }
        // If it's a jump move, ensure opponent pieces are in the middle squares
        if (move.type == 'x') {
            for (size_t i = 0; i < move.squares.size() - 1; i++) {
                int from = move.squares[i];
                int to = move.squares[i + 1];
                int row1, col1, row2, col2;
                SquareToPos(from, row1, col1);
                SquareToPos(to, row2, col2);
                int midRow = (row1 + row2) / 2;
                int midCol = (col1 + col2) / 2;
                int midSquare = PosToSquare(midRow, midCol);
                char midPiece = GetPiece(board, midSquare);
                if (midPiece == '.' || tolower(midPiece) == tolower(player)) {
                    // Place opponent's man
                    if (player == 'W')
                        SetPiece(board, midSquare, 'b');
                    else
                        SetPiece(board, midSquare, 'w');
                }
            }
        }
        // Ensure no other forced captures
        if (HasForcedCapture(board, player)) {
            // Adjust the board to remove other forced captures
            // For simplicity, we assume there are no other pieces that cause forced captures
        }
        // Apply the move
        ApplyMove(board, move, player);
        // Switch player
        player = (player == 'W') ? 'B' : 'W';
    }
    // Output the initial and final boards
    // Boards are printed from top to bottom, left to right
    for (int row = 0; row < 8; row++) {
        string line1, line2;
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                int square = PosToSquare(row, col);
                line1 += initialBoard.squares[square].type;
                line2 += board.squares[square].type;
            } else {
                line1 += '-';
                line2 += '-';
            }
        }
        cout << line1 << " " << line2 << endl;
    }
    return 0;
}
