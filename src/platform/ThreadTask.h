#pragma once

#include <Windows.h>


// This transfers tasks between two threads.
// You can see it as a circular buffer of size 1.
// This task requires no more than 1 task transfered at a time. Provided there'll be more - change this to real circular buffer.

template<typename T>
class ThreadTask
{
    public:
        bool Push(T task, float timeout)
        {
            DWORD waitResult = WaitForSingleObject(push_semaphore, (DWORD)(timeout * 1000));
            if (waitResult == WAIT_OBJECT_0)
            {
                task = task;
                ReleaseSemaphore(pop_semaphore, 1, NULL);
                return true;
            }
            else
            {
                return false;
            }
        }

        bool Pop(float timeout, T &result)
        {
            DWORD waitResult = WaitForSingleObject(pop_semaphore, (DWORD)(timeout * 1000));
            if (waitResult == WAIT_OBJECT_0)
            {
                result = task;
                ReleaseSemaphore(push_semaphore, 1, NULL);
                return true;
            }
            return false;
        }

        ThreadTask()
        {
            push_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
            pop_semaphore = CreateSemaphore(NULL, 0, 1, NULL);
        }
        ~ThreadTask()
        {
            CloseHandle(push_semaphore);
            CloseHandle(pop_semaphore);
        }
    private:
        T task;
        HANDLE push_semaphore;
        HANDLE pop_semaphore;
};