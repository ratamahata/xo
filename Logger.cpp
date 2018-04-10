//---------------------------------------------------------------------------


#pragma hdrstop

#include "Logger.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Logger::Logger() {
        hitCount = 0;
        missAgeCount = 0;
        missHashCount = 0;
        missIndexCount = 0;
        lastError = NULL;
}

void Logger::hit() {
        ++hitCount;
}

void Logger::missAge() {
        ++missAgeCount;
}

void Logger::missHash() {
        ++missHashCount;
}

void Logger::missIndex() {
        ++missIndexCount;
}

void Logger::error(const char* message) {
        this->lastError = message;
}

void Logger::printLastError(char *buffer) {
        sprintf(buffer,
                lastError != NULL
                        ? lastError : missAgeCount > 0
                        ? "Hash collisions %d"
                        : "\0", missAgeCount);
}
