  
#include <M5Atom.h>
#include <Adafruit_NeoPixel.h>
#include "CA.h"


#define DISPLAYPIN     27
#define DISPLAYCOUNT   5 * 5

#define LEDPIN     26
#define LEDCOUNT   72 + 30

float DeltaSeconds = 0.5f;
float Speed = 0.1337f;

float RSpeed = 1.0f;
float GSpeed = 0.867f;
float BSpeed = 0.371f;

int Brightness = 100;


float Time = 0.0f;
float ROffset = 0.0f;
float GOffset = 0.0f;
float BOffset = 0.0f;

Adafruit_NeoPixel Display = Adafruit_NeoPixel( DISPLAYCOUNT, DISPLAYPIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel LEDs = Adafruit_NeoPixel( LEDCOUNT, LEDPIN, NEO_GRB + NEO_KHZ800 );


void setup()
{
  M5.begin();
  LEDs.begin();
  Display.begin();
  
  randomSeed( analogRead( 0 ) );
  
  ROffset = random( -10, 10 );
  GOffset = random( -10, 10 );
  BOffset = random( -10, 10 );
  
  RSpeed = Speed * (float)random( 500, 1000 ) / 1000.0f;
  GSpeed = Speed * (float)random( 500, 1000 ) / 1000.0f;
  BSpeed = Speed * (float)random( 500, 1000 ) / 1000.0f;
        
  delay( 100 );
}



void loop()
{
  delay( DeltaSeconds * 1000.0f );
  Time += DeltaSeconds;


  M5.update();
  if( M5.Btn.wasPressed() )
  {
    Brightness = (Brightness + 10) % 101;

    // On Button-press, change CA-type
    CAType = (CAType + 1u) % 4u;
    
    if( CAType )
    {
      // Initialize Cells when CA enabled
      initCA();
    }
  }
  
  float R = sin( (Time+ROffset) * RSpeed );
  float G = sin( (Time+GOffset) * GSpeed );
  float B = sin( (Time+BOffset) * BSpeed );
  
  R = (R * 0.5f + 0.5f) * (float)Brightness;
  G = (G * 0.5f + 0.5f) * (float)Brightness;
  B = (B * 0.5f + 0.5f) * (float)Brightness;

  
  for( int i = 0; i < LEDCOUNT; ++i )
  {
    LEDs.setPixelColor( i, LEDs.Color( R, G, B ) );
  }
  LEDs.show();


  if( CAType )
  {
    // When CA is enabled, update and apply Cells
    updateCA();
  
    for( int i = 0; i < DISPLAYCOUNT; ++i )
    {
      int x = i % 5;
      int y = i / 5;

      // Decide on Cell state if applying the LED color or turn it off
      if( CS[x][y] )
      {
        Display.setPixelColor( i, Display.Color( R, G, B ) );
      }
      else
      {
        Display.setPixelColor( i, Display.Color( 0, 0, 0 ) );
      }
    }
  }
  else
  {
    for( int i = 0; i < DISPLAYCOUNT; ++i )
    {
      Display.setPixelColor( i, Display.Color( R, G, B ) );
    }
  }
  Display.show();
}
