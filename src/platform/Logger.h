#pragma once

#include <sstream>

// It is not thread-safe, there's no real sense in making it thread safe;

class LogMessage;

class Logger
{
    public:
        bool Init(const char * filename, bool isLoggingToOutput);
        bool Write(const LogMessage &stream);

        static bool InitDefaultLogger(const char * filename, bool isLoggingToOutput);
    private:
        std::string Filename;
        bool IsLoggingToOutput;
};

class LogMessage
{
    friend Logger;
    public:
        enum LogLevel { LOGINFO, LOGWARNING, LOGERROR };

        LogMessage(LogLevel logLevel, Logger * logger);

        template<typename T>
        LogMessage&  operator<<(T data)
        {
            Stream << data;
            return *this;
        };

        void flush();

    private:
        std::stringstream Stream;
        LogLevel logLevel;
        Logger * logger;
};

extern Logger UBIDefaultLogger;

#define LOG_ERROR {LogMessage UBIlogMessage(LogMessage::LOGERROR, &UBIDefaultLogger); UBIlogMessage <<
#define LOG_WARNING {LogMessage UBIlogMessage(LogMessage::LOGWARNING, &UBIDefaultLogger); UBIlogMessage <<
#define LOG_INFO {LogMessage UBIlogMessage(LogMessage::LOGINFO, &UBIDefaultLogger); UBIlogMessage <<

#define LOG_END ; UBIlogMessage.flush(); }

#define LOG_E(msg)  {LOG_ERROR msg LOG_END}
#define LOG_I(msg)  {LOG_INFO msg LOG_END}