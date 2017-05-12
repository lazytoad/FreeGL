#pragma once

#include <Windows.h>
#include <stdint.h>

// class used for time measurement.

class QPCTimer
{
    public:
        QPCTimer();

        // inlined for higher precision
        inline void Start()
        {
            LARGE_INTEGER largeInt;
            QueryPerformanceCounter(&largeInt);
            start = largeInt.QuadPart;
        }
        inline double Stamp()
        {
            LARGE_INTEGER largeInt;
            QueryPerformanceCounter(&largeInt);
            double difference = double(largeInt.QuadPart - start);
            // ok, we'd really better consider setting thread's affinity, but that too may have unwanted side effects, and this works ok.
            return difference >= 0 ? difference / Frequency : 0;
        }
    private:
        int64_t start;
        double  Frequency;
};