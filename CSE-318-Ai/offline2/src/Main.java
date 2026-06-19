import Board.MancalaBoard;
import Heuristics.*;
import java.util.Scanner;

public class Main {
  MancalaBoard board;

  public static void main(String[] args) {

    Scanner input = new Scanner(System.in);
    System.out.println("Select :\n1. Human vs AI\n2. AI vs AI\n3. AI vs AI "
                       + "100 play Win Ratio");
    int option = input.nextInt();

    int stepSize1, stepSize2;
    stepSize1 = 6;
    stepSize2 = 7;

    if (option == 1 || option == 2) {

      System.out.println("Select Heuristic for player 1 from 1 to 4");
      int heuristicNo1 = input.nextInt();
      System.out.println("Select Heuristic for player 2 from 1 to 4");
      int heuristicNo2 = input.nextInt();

      MancalaBoard board = new MancalaBoard(stepSize1, stepSize2);
      board.setHeuristic(0, getHeuristic(heuristicNo1));
      board.setHeuristic(1, getHeuristic(heuristicNo2));

      board.currPlayer = 0;
      int moveNo = 0;
      if (option == 1) {
        System.out.println("Want to make 1st move?\n1.Yes\n2.No");
        moveNo = input.nextInt();
        moveNo--;
      }
      board.print();
      int[] moves;
      moves = new int[2];

      while (true) {
        if (board.isGameOver(board.currPlayer)) {
          break;
        }

        int move = 0;
        if (option == 1) {

          if (board.currPlayer == moveNo) {
            move = input.nextInt();

          } else {
            move = board.getMove(board.currPlayer);
          }
        } else {
          move = board.getMove(board.currPlayer);
        }

        System.out.println("Player= " + (board.currPlayer + 1) + " choose " +
                           move + " no bin .Moves: " + moves[board.currPlayer]);
        board = board.move(board.currPlayer, move);
        moves[board.currPlayer]++;
        board.print();
      }

      System.out.println(
          "----------------------------------------------------");
      int diff = board.getTotalStones(0) - board.getTotalStones(1);
      if (diff > 0) {
        System.out.println("Player 1 win");
      } else if (diff < 0) {
        System.out.println("Player 2 win");
      } else {
        System.out.println("Draw Match");
      }

      System.out.println("Player 1: " + board.getTotalStones(0));
      System.out.println("Player 2: " + board.getTotalStones(1));

      System.out.println("Summary:");
      System.out.println("\t\t\tPlayer 1\tPlayer 2");
      System.out.println("Total moves\t\t" + moves[0] + "\t\t" + moves[1]);
      System.out.println("Extra Moves\t\t" + board.extraMoves[0] + "\t\t" +
                         board.extraMoves[1]);
      System.out.println("Captured Stones\t\t" + board.capturedStones[0] +
                         "\t\t" + board.capturedStones[1]);
    } else if (option == 3) {
      for (int h1 = 1; h1 < 4; h1++) {
        for (int h2 = h1 + 1; h2 <= 4; h2++) {
          int heuristicNo1 = h1;
          int heuristicNo2 = h2;
          int winner1 = 0;
          int winner2 = 0;
          int draw = 0;
          for (int k = 1; k <= 100; k++) {

            MancalaBoard board = new MancalaBoard(stepSize1, stepSize2);
            board.setHeuristic(0, getHeuristic(heuristicNo1));
            board.setHeuristic(1, getHeuristic(heuristicNo2));

            board.currPlayer = 0;
            int moveNo = 0;
            int[] moves;
            moves = new int[2];

            while (true) {
              if (board.isGameOver(board.currPlayer)) {
                break;
              }

              int move = 0;
              move = board.getMove(board.currPlayer);

              board = board.move(board.currPlayer, move);
              moves[board.currPlayer]++;
            }

            int diff = board.getTotalStones(0) - board.getTotalStones(1);
            if (diff > 0) {
              winner1++;
            } else if (diff < 0) {
              winner2++;
            } else {
              draw++;
            }
          }

          System.out.println("Heuristic " + heuristicNo1 + " vs "
                             + "Heuristic" + heuristicNo2 + " :");
          System.out.println("Player 1 win: " + winner1);
          System.out.println("Player 2 win: " + winner2);
          System.out.println("Draw: " + draw + "\n");
        }
      }
    } else {
      System.out.println("Invalid option");
    }
  }

  public static Heuristic getHeuristic(int heuristicNo) {
    if (heuristicNo == 1) {
      return new Heuristic1();
    } else if (heuristicNo == 2) {
      return new Heuristic2();
    } else if (heuristicNo == 3) {
      return new Heuristic3();
    } else if (heuristicNo == 4) {
      return new Heuristic4();
    }
    return null;
  }
}