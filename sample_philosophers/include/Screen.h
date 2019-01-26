#pragma once

#include "Mutex.h"

#include <stdlib.h>
#include <cstdio>
#include <ncurses.h>

// Allows abstraction of print statements for use with console or ncurses
class Screen {
public:
    static Mutex lock; // Defines a global lock to protect print statements

    static void Init(bool verbose) {
        s_verbose = verbose;

        // Initialize ncurses
        if (!s_verbose) {
            initscr();
            curs_set(0);
        }
    }

    static void Terminate() {
        // Terminate ncurses
        if (!s_verbose) {
            endwin();
            system("stty sane");
        }
    }

    template <typename... Args>
    static void Write(Args... args) {
        // Simply print data if not using ncurses
        if (s_verbose) {
            printf(args...);
            return;
        }

        // Else print in table and increment row
        mvprintw(s_row++, s_col, args...);
        refresh();
    }

    template <typename... Args>
    static void WriteAt(int row, int col, Args... args) {
        // Set the row and column before printing
        s_row = row;
        s_col = col;

        Write(args...);

        // Reset the cursor to left side
        s_col = 0;
    }

    static inline bool IsVerbose() { return s_verbose; }

private:
    static int s_row, s_col;
    static bool s_verbose;
};
