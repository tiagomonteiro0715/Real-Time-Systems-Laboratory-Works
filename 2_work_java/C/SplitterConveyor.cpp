// SplitterConveyor.cpp : This file contains the 'main' function.Program execution begins and ends there.
//

#include <stdio.h>
#include <conio.h>
#include <stdio.h>

extern "C" {
    // observe your project contents. We are mixing C files with cpp ones.
    // Therefore, inside cpp files, we need to tell which functions are written in C.
    // That is why we use extern "C" directive
#include <interface.h>
#include <SplitterConveyor.h>
}

int main()
{
    printf("Welcome to the Splitter Conveyor Application\n");
    int t = -1;

    printf("\nPress: q, a, z, i or 0 ");
    while (t != '0') {
        t = _getch();
        switch (t) {
        case 'q': cylinder1_moveForward(); break;
        case 'a': cylinder1_moveBackward(); break;
        case 'z': cylinder1_stop(); break;
        case 'i': inicializeHardwarePorts(); break;

        }
    }

    writeDigitalU8(2, 0x00);
    closeChannels();
    return 0;
}
