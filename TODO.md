- Add en passant support to Game::discoveryCheck
- Do for each piece the method threat(Piece* piece)
  V Knight
  V Rook
  V Bishop
  . Queen (working)
  X King
  X Pawn


- Finish run() function to handle real game
- Construct Game::filterMoves(Piece* piece, vector<vector<int>> moves)
- Construct Game::updateChecks(Piece* piece, vector<int> move)
- Construct Game::isMate()
- For En Passant, eat the ennemy pawn next to the pawn when moving
- Add Roque
- Add 'Save and quit' service
- Add 'Offer a draw' and 'Give up'
- Add Boule de crystal (Smart opponent)
