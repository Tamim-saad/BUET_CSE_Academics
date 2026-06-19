package Heuristics;
import Board.MancalaBoard;
import java.util.Random;

public abstract class Heuristic {
  public abstract int getHeuristicValue(MancalaBoard board);
  int range1 = 7;
  int range2 = 13;
}