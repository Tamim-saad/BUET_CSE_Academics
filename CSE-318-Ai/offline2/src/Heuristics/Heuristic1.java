package Heuristics;
import Board.MancalaBoard;

public class Heuristic1 extends Heuristic {
  @Override
  public int getHeuristicValue(MancalaBoard board) {
    int curr = 0;
    int other = 1;
    int stones_in_my_storage = board.stonesInStorage(curr);
    int stones_in_opponents_storage = board.stonesInStorage(other);

    return stones_in_my_storage - stones_in_opponents_storage;
    
  }
}
