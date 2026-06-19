package Heuristics;

import Board.MancalaBoard;
import java.util.Random;

public class Heuristic3 extends Heuristic {
  @Override
  public int getHeuristicValue(MancalaBoard board) {
    int curr = 0;
    int other = 1;

    int stones_on_my_side = board.getTotalStones(curr);
    int stones_on_opponents_side = board.getTotalStones(other);

    int stones_in_my_storage = board.stonesInStorage(curr);
    int stones_in_opponents_storage = board.stonesInStorage(other);

    Random random;
    random = new Random();
    int w1 = random.nextInt((range2 - range1) + 1) + range1;
    random = new Random();
    int w2 = random.nextInt((range2 - range1) + 1) + range1;
    random = new Random();
    int w3 = random.nextInt((range2 - range1) + 1) + range1;

    int addMove = board.extraMoves[curr];

    return w1 * (stones_in_my_storage - stones_in_opponents_storage) +
        w2 * (stones_on_my_side - stones_on_opponents_side) + w3 * addMove;
  }
}
