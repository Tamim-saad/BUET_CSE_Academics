package Board;

import Heuristics.*;
import java.util.Arrays;

public class MancalaBoard {
  public Heuristic[] heuristic;
  public int[] extraMoves, capturedStones;
  public int[] steps;
  int[][] stonesIntoBin;
  public int totalStone, currPlayer;

  final int binNo = 6;
  final int stonePerBin = 4;

  public MancalaBoard(int s1, int s2) {
    stonesIntoBin = new int[2][binNo + 1];
    totalStone = binNo * stonePerBin * 2;

    for (int[] playerBins : stonesIntoBin) {
      Arrays.fill(playerBins, 1, binNo + 1, stonePerBin);
    }

    currPlayer = 0;
    heuristic = new Heuristic[2];
    steps = new int[] {s1, s2};
    extraMoves = new int[2];
    capturedStones = new int[2];
  }

  public void setHeuristic(int ind, Heuristic h) { heuristic[ind] = h; }

  public int otherPlayer(int playerNo) { return (playerNo + 1) % 2; }

  public int stonesInStorage(int playerNo) {
    return stonesIntoBin[playerNo][0];
  }

  public int getTotalStone(int playerNo) // without storage
  {
    int total = 0;
    for (int i = 1; i <= binNo; i++)
      total += stonesIntoBin[playerNo][i];

    return total;
  }

  public int getTotalStones(int playerNo) // with storage
  {
    int total = 0;
    for (int i = 0; i <= binNo; i++)
      total += stonesIntoBin[playerNo][i];

    return total;
  }

  public int stoneCaptured(int playerNo, int binNo) {
    int other = otherPlayer(playerNo);
    int stones = stonesIntoBin[other][6 - binNo + 1];
    return stones;
  }

  public int getProbableStoneCaptured(int playerNo) {
    int maxStoneCaptured = 0;

    for (int i = 1; i <= binNo; i++) {
      if (stonesIntoBin[playerNo][i] == 0) {
        int stonesCaptured = stoneCaptured(playerNo, i);
        if (stonesCaptured > maxStoneCaptured)
          maxStoneCaptured = stonesCaptured;
      }
    }
    return maxStoneCaptured;
  }

  public int getProbableExtraMove(int playerNo) {
    int count = 0;
    for (int i = 1; i <= binNo; i++) {
      if (stonesIntoBin[playerNo][i] == i)
        count++;
    }
    return count;
  }

  public boolean isGameOver(int playerNo) {
    if (getTotalStone(playerNo) == 0 ||
        getTotalStone(otherPlayer(playerNo)) == 0)
      return true;
    return false;
  }

  public MancalaBoard copy(int steps1, int steps2) {
    MancalaBoard newBoard = new MancalaBoard(steps1, steps2);
    newBoard.heuristic = heuristic;
    newBoard.currPlayer = currPlayer;
    newBoard.totalStone = totalStone;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j <= binNo; j++)
        newBoard.stonesIntoBin[i][j] = stonesIntoBin[i][j];

      newBoard.extraMoves[i] = extraMoves[i];
      newBoard.capturedStones[i] = capturedStones[i];
    }
    newBoard.steps[0] = steps1;
    newBoard.steps[1] = steps2;

    return newBoard;
  }

  public MancalaBoard move(int playerNo, int bin) {
    MancalaBoard newBoard = copy(steps[0], steps[1]);

    int stones = stonesIntoBin[playerNo][bin];
    newBoard.stonesIntoBin[playerNo][bin] = 0;
    int tem = playerNo;

    while (stones > 0) {
      bin--;
      if (bin == 0 && playerNo != tem) {
        continue;
      }
      if (bin < 0) {
        bin = binNo;
        playerNo = otherPlayer(playerNo);
      }
      newBoard.stonesIntoBin[playerNo][bin]++;
      stones--;
    }
    // extra move
    if (bin == 0 && playerNo == tem) {
      extraMoves[tem]++;
      return newBoard;
    }
    // capture stones
    if (newBoard.stonesIntoBin[playerNo][bin] == 1 && bin != 0 &&
        playerNo == tem) {

      int other = otherPlayer(playerNo);
      int bin2 = binNo - bin + 1;
      if (newBoard.stonesIntoBin[other][bin2] != 0) {
        capturedStones[currPlayer] += newBoard.stonesIntoBin[other][bin2];
        newBoard.stonesIntoBin[playerNo][0] +=
            newBoard.stonesIntoBin[other][bin2] + 1;
        newBoard.stonesIntoBin[other][bin2] = 0;
        newBoard.stonesIntoBin[playerNo][bin] = 0;
      }
    }

    newBoard.currPlayer = otherPlayer(currPlayer);
    return newBoard;
  }

  public int MiniMax(int step, int alpha, int beta) {
    if (step == 0 || isGameOver(currPlayer)) {
      return heuristic[currPlayer].getHeuristicValue(this);
    }
    int value;
    if (currPlayer == 0) {
      value = Integer.MIN_VALUE;
    } else {
      value = Integer.MAX_VALUE;
    }
    for (int i = 1; i <= binNo; i++) {
      if (stonesIntoBin[currPlayer][i] > 0) {
        MancalaBoard board = move(currPlayer, i);
        int val = board.MiniMax(step - 1, alpha, beta);
        if (currPlayer == 0) {
          value = Math.max(value, val);
          alpha = Math.max(alpha, val);
        } else {
          value = Math.min(value, val);
          beta = Math.min(beta, val);
        }
        if (beta < alpha)
          break;
      }
    }
    return value;
  }

  public int getMove(int playerNo) {
    if (isGameOver(playerNo)) {
      return -1;
    }
    int value, ans = 0;
    if (currPlayer == 0)
      value = Integer.MIN_VALUE;
    else
      value = Integer.MAX_VALUE;

    for (int i = 1; i <= binNo; i++) {
      if (stonesIntoBin[playerNo][i] > 0) {
        MancalaBoard board = move(playerNo, i);
        int val = board.MiniMax(steps[currPlayer] - 1, Integer.MIN_VALUE,
                                Integer.MAX_VALUE);
        if (currPlayer == 0) {
          if (val > value) {
            value = val;
            ans = i;
          }
        } else {
          if (val < value) {
            value = val;
            ans = i;
          }
        }
      }
    }
    return ans;
  }

  public void print() {
    System.out.println("-----------------");
    System.out.print("| |");
    for (int i = 1; i <= binNo; i++)
      System.out.print(stonesIntoBin[0][i] + "|");
    System.out.println(" |");

    System.out.print("|" + stonesIntoBin[0][0] + "|");
    System.out.print("-----------");
    System.out.println("|" + stonesIntoBin[1][0] + "|");

    System.out.print("| |");
    for (int i = binNo; i > 0; i--)
      System.out.print(stonesIntoBin[1][i] + "|");
    System.out.println(" |");
    System.out.println("-----------------");
    System.out.println();
  }
}
