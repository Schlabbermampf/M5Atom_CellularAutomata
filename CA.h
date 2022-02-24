
// Selected Cellular-Automata
// 0 - off (all Cells alive)
// 1: 2D CA with Conway's Game-of-Life rule. With random initialization.
// 2: 2D CA with Conway's Game-of-Life rule. With glider initialization.
// 3: 1D CA with random rule. Sliding and with random initialization.
int CAType = 3;

// Universe
bool C0[5][5];

// 2D CA Universe Back-Side
bool C1[5][5];

// 2D CA Source- and Target-pointers
bool (*CS)[5];
bool (*CT)[5];

// 1D CA Rule
byte R = 30;

// Initializations
bool CGlider[5][5] = {{ 0, 1, 0, 0, 0 },
                      { 0, 0, 1, 0, 0 },
                      { 1, 1, 1, 0, 0 },
                      { 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0 }};
                      
bool COnePix[5][5] = {{ 0, 0, 1, 0, 0 },
                      { 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0 }};

// Neighbor-Iteration
int NX[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };
int NY[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };


void initCA()
{
  randomSeed( analogRead( 0 ) );

  // Random 1D CA rule
  R = random( 256 );

  // Reset 2D Source- and Target-pointers
  CS = C0;
  CT = C1;

  // Initialize Universe
  for( int x = 0; x < 5; ++x )
  for( int y = 0; y < 5; ++y )
  {
    C0[x][y] = CAType == 1 ? y == 4 ? random( 2 ) : 0
             : CAType == 2 ? random( 2 )
             : CAType == 3 ? CGlider[y][x]
             : 1;
  }
}

void updateCA2D()
{
  // Any-Change indicator
  bool z = true;

  // Iterate all Cells
  for( int x = 0; x < 5; ++x )    
  for( int y = 0; y < 5; ++y )
  {
    // Num of neighbouring Cells alive
    int nc = 0;

    // Iterate all Neighbours
    for( int n = 0; n < 8; ++n )
    {
      // Neighbour-Position
      int nx = x + NX[n];
      int ny = y + NY[n];

      // Warping border-condition
      nx += (nx >= 5) ? -5 : (nx < 0) ? 5 : 0;
      ny += (ny >= 5) ? -5 : (ny < 0) ? 5 : 0;

      // Increment number of living Cells
      nc += CS[nx][ny];
    }

    // Update new Cell state from Rule
    CT[x][y] = CS[x][y] ? (nc == 2 || nc == 3) ? 1 : 0
                        : (nc == 3)            ? 1 : 0;

    // Track if any Cell changed
    z &= CS[x][y] == CT[x][y];
  }

  if( z )
  {
    // When no change detected, re-initialize Universe
    initCA();
  }
  else
  {
    // Otherwise swap Source- and Target-pointers
    bool (*tmp)[5] = CS;
    CS = CT;
    CT = tmp;
  }
}

void updateCA1D()
{
  // Any-Change indicator
  bool z = true;

  // Slide all Cells upwards, except last row 
  for( int x = 0; x < 5; ++x )  
  for( int y = 0; y < 4; ++y )
  {
    C0[x][y] = C0[x][y+1];   
  }
  
  // Iterate all Cells in the last row
  for( int x = 0; x < 5; ++x )    
  {
    // Num of neighbouring Cells alive
    int nc = 0;

    // Iterate all Neighbours
    for( int n = -1; n <= 1; ++n )
    {
      // Neighbour position
      int nx = x + n;
      
      // Warping border condition
      nx += (nx >= 5) ? -5 : (nx < 0) ? 5 : 0;
      
      // Determine neighbour constellation
      nc = (nc << 1) + C0[nx][3];
    }

    // Receive new Cell state from Rule
    bool c = R & (1<<nc);

    // Track if any Cell changed
    z &= CS[x][4] == c;

    // Apply new Cell state
    C0[x][4] = c;    
  }

  if( z )
  {
    // When no change detected, re-initialize Universe
    initCA();
  }
}

void updateCA()
{
  // Update 1D or 2D CA depending on selected type
  switch( CAType )
  {
    case 1 :
      updateCA1D();
      break;
    case 2 :
    case 3 :
      updateCA2D();
      break;
  }
}
