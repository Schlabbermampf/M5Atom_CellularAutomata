
int CAType = 0;

uint T = 0;
byte R = 30;

int NX[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };
int NY[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };

bool C0[5][5];
bool C1[5][5];

bool (*CS)[5];
bool (*CT)[5];

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
          
void initCA()
{
  randomSeed( analogRead( 0 ) );

  T = 0;
  R = random( 256 );
  
  CS = C0;
  CT = C1;

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
  bool z = true;
  
  for( int x = 0; x < 5; ++x )    
  for( int y = 0; y < 5; ++y )
  {
    int nc = 0;

    for( int n = 0; n < 8; ++n )
    {
      int nx = x + NX[n];
      nx += (nx >= 5) ? -5 : (nx < 0) ? 5 : 0;
      
      int ny = y + NY[n];
      ny += (ny >= 5) ? -5 : (ny < 0) ? 5 : 0;
      
      nc += CS[nx][ny];
    }
  
    CT[x][y] = CS[x][y] ? (nc == 2 || nc == 3) ? 1 : 0
                        : (nc == 3)            ? 1 : 0;


    z &= CS[x][y] == CT[x][y];
  }

  if( z )
  {
    initCA();
  }
  else
  {
    bool (*tmp)[5] = CS;
    CS = CT;
    CT = tmp;
  }
}

void updateCA1D()
{
  bool z = true;
    
  for( int x = 0; x < 5; ++x )  
  for( int y = 0; y < 4; ++y )
  {
    C0[x][y] = C0[x][y+1];   
  }
  
  for( int x = 0; x < 5; ++x )    
  {
    int nc = 0;

    for( int n = -1; n <= 1; ++n )
    {
      int nx = x + n;
      nx += (nx >= 5) ? -5 : (nx < 0) ? 5 : 0;
      
      nc = (nc << 1) + C0[nx][3];
    }
  
    bool c = R & (1<<nc);
    
    z &= CS[x][4] == c;
    
    C0[x][4] = c;    
  }

  if( z )
  {
    initCA();
  }
}

void updateCA()
{
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
