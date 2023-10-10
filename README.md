# chess
Note:
- Game begins as white only
- Grid is (0,0) at top left, (7,7) at bottom right
- No checking, pawn promotion, castling, en passant
- Game ends when king is taken 
- Minimax algorithm implemented with alpha-beta pruning, searches 5 moves deep 
- Search tree optimized by implementing MVVLVA (Most Valuable Victim, Least Valuable Attacker) i.e. sort moves to prune faster
