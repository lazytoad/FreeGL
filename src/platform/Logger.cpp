#include <Windows.h>
#include <fstream>
#include <iostream>

#include "Logger.h"

Logger UBIDefaultLogger;

bool Logger::Init(const char * filename, bool isLoggingToOutput)
{
    IsLoggingToOutput = isLoggingToOutput;
    Filename = filename;
    std::ofstream logfile;
    logfile.open(filename, std::ios::trunc | std::ios::out);
    if (!logfile.is_open())
    {
        if (isLoggingToOutput)
        {
            std::stringstream out;
            out << "Logger failed to open file " << Filename;
            OutputDebugStringA(out.str().c_str());
        }
        Filename = "";
        return false;
    }
    logfile.close();
    return true;
}

bool Logger::Write(const LogMessage &stream)
{
    if (IsLoggingToOutput)
    {
        OutputDebugStringA(stream.Stream.str().c_str());
    }

    if (!Filename.empty())
    {
        std::ofstream logfile;
        logfile.open(Filename, std::ios::app | std::ios::out);
        if (logfile.is_open())
        {
            logfile << stream.Stream.str();
            logfile.close();
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool Logger::InitDefaultLogger(const char * filename, bool isLoggingToOutput)
{
    return UBIDefaultLogger.Init(filename, isLoggingToOutput);
}

LogMessage::LogMessage(LogLevel level, Logger * logger)
{
    logger = logger;
	logLevel = level;
    switch (logLevel)
    {
        case LogMessage::LOGINFO:
            Stream << "{I} ";
            return;
        case LogMessage::LOGWARNING:
            Stream << "{W} ";
            return;
        case LogMessage::LOGERROR:
            Stream << "{E} ";
            return;
    }
}

void LogMessage::flush()
{
    Stream << std::endl;
    if (logger != NULL)
    {
        logger->Write(*this);
    }
    std::cout << Stream.str();
}