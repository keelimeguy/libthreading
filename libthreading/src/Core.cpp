/*=============================================================================
    Copyright (c) 2019 Keelin Becker-Wheeler
    Core.cpp
    Distributed under the GNU GENERAL PUBLIC LICENSE
    See https://github.com/keelimeguy/libthreading
==============================================================================*/
#include "Core.h"

// Allocate static class variables
unsigned int Core::m_numProc = 0;
std::vector<int> Core::m_avail;
