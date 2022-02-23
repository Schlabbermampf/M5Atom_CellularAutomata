1D and 2D Cellular-Automata on M5Atom

four modes can be changed with the Button:
0: CA off - (all Cells alive)	
1: 2D CA with Conway's Game-of-Life rule. With random initialization.	
2: 2D CA with Conway's Game-of-Life rule. With glider initialization.	
3: 1D CA with random rule. Sliding and with random initialization.	

Cells are applied to the 5x5 LED-matrix of the M5Atom. The CA is embedded in a basic color-/brightness-control for the LED-matrix, which uses NeoPixel and also control an LED-strip.

Automatically re-initializes when no cell changed within a time-step.
