#include "QPCTimer.h"
#include "Consts.h"

QPCTimer::QPCTimer()
{
    LARGE_INTEGER largeInt;
    if (!QueryPerformanceFrequency(&largeInt))
    {
        MessageBoxA(NULL, "Timer init failed!\nThe app will now close.", "Critical Error!", MB_OK);
        exit(-1);
    }
    Frequency = double(largeInt.QuadPart);
}