#include <bits/stdc++.h>
using namespace std;

const vector<pair<int, int>> direction = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

struct BoardState {
  vector<vector<int>> board;
  int blankX;
  int blankY;
  int g;
  int h;

  BoardState(const vector<vector<int>> &board, int x, int y, int g, int h) {
    this->board = board;
    this->blankX = x;
    this->blankY = y;
    this->g = g;
    this->h = h;
  }
};

// Functor to compare nodes based on the `cost` (ascending order)
struct Compare {
  bool operator()(const BoardState &a, const BoardState &b) {
    return (a.g + a.h) > (b.g + b.h); // Min-heap based on cost
  }
};

pair<int, int> getBlankCell(vector<vector<int>> board) {
  int boardSize = board.size();
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (board[i][j] == 0) {
        return make_pair(i, j);
      }
    }
  }
  return make_pair(-1, -1);
}

int getInversionCount(vector<vector<int>> board) {
  vector<int> flatBoard;
  int rowZero;
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      int x = board[i][j];
      if (x == 0)
        rowZero = (3 - i);
      flatBoard.push_back(board[i][j]);
    }
  }

  int inversionCount = 0;
  for (int i = 0; i < flatBoard.size() - 1; i++) {
    int first = flatBoard[i];
    for (int j = i + 1; j < flatBoard.size(); j++) {
      int second = flatBoard[j];
      if (first && second && first > second) {
        inversionCount++;
      }
    }
  }
  return inversionCount;
}

bool isSolvable(BoardState state) {
  vector<vector<int>> board = state.board;
  int inversionCount = getInversionCount(board);
  if (board.size() % 2 == 1) {
    if (inversionCount % 2 == 0)
      return true;
    return false;
  }
  if ((inversionCount % 2 == 1 && state.blankX % 2 == 0) ||
      (inversionCount % 2 == 0 && state.blankX % 2 == 1))
    return true;
  return false;
}

int calculateManhattanDistance(vector<vector<int>> board) {
  int boardSize = board.size();
  int manhattan = 0;
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      int cellValue = board[i][j];
      if (cellValue != 0) {
        int x = (cellValue - 1) / boardSize;
        int y = (cellValue - 1) % boardSize;
        manhattan += abs(i - x) + abs(j - y);
      }
    }
  }
  return manhattan;
}

int calculateHammingDistance(vector<vector<int>> board) {
  int boardSize = board.size();
  int hamming = 0, counter = 1;
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (board[i][j] && board[i][j] != counter)
        hamming++;
      counter++;
    }
  }
  return hamming;
}

void printBoard(BoardState state) {
  vector<vector<int>> board = state.board;

  cout << "Cost: " << state.g << ", Heuristics: " << state.h << endl;
  int length = board.size();
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length; j++) {
      if (board[i][j] == 0)
        cout << "* ";
      else
        cout << board[i][j] << " ";
    }
    cout << endl;
  }
}

void solve(vector<vector<int>> board, int f, bool &solvable) {
  pair<int, int> blankCell = getBlankCell(board);
  BoardState start(board, blankCell.first, blankCell.second, 0,
                   f == 0 ? calculateManhattanDistance(board)
                          : calculateHammingDistance(board));
  solvable = isSolvable(start);
  if (!solvable) {
    cout << "not solvable" << endl;
    return;
  }

  int boardSize = board.size();

  map<vector<vector<int>>, int> mp;
  mp.clear();

  priority_queue<BoardState, vector<BoardState>, Compare> pq;
  pq.push(start);
  int explored = 1;
  int expanded = 0;
  // int counter = 0;
  while (!pq.empty()) {
    BoardState current = pq.top();
    pq.pop();
    expanded++;

    cout << (f == 0 ? "Manhattan" : "Hamming") << ", Expanded: " << expanded
         << ", Explored: " << explored << " ";

    printBoard(current);

    if (current.h == 0) {
      cout << " Reached Goal State with Cost: " << current.g << endl;
      return;
    }
    mp[current.board] = 1;

    for (auto change : direction) {
      pair<int, int> newCell;
      newCell.first = current.blankX + change.first;
      newCell.second = current.blankY + change.second;

      if (0 <= newCell.first && newCell.first < boardSize &&
          0 <= newCell.second && newCell.second < boardSize) {
        vector<vector<int>> newBoard = current.board;
        swap(newBoard[current.blankX][current.blankY],
             newBoard[newCell.first][newCell.second]);

        if (!mp[newBoard]) {
          pair<int, int> blankCell = getBlankCell(newBoard);
          BoardState newState(newBoard, blankCell.first, blankCell.second,
                              1 + current.g,
                              f == 0 ? calculateManhattanDistance(newBoard)
                                     : calculateHammingDistance(newBoard));
          pq.push(newState);
          explored++;
        }
      }
    }
  }
}

int main() {
  int g = 0;
  while (g++ < 10) {
    cout << "Enter board size: ";
    int boardSize;
    cin >> boardSize;
    vector<vector<int>> board(boardSize, vector<int>(boardSize)), board2;
    cout << "Enter Initial Board State serially:(0 for blank) \n";

    for (int i = 0; i < boardSize; i++) {
      for (int j = 0; j < boardSize; j++) {
        cin >> board[i][j];
      }
    }
    bool solvable = false;
    solve(board, 0, solvable);
    if (solvable) {
      cout << "\n====================================================\n";
      solve(board, 1, solvable);
    }
  }
}
