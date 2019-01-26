#include "Screen.h"

// Allocate static class variables
Mutex Screen::lock;
int Screen::s_row = 0;
int Screen::s_col = 0;
bool Screen::s_verbose = false;
