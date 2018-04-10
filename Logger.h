//---------------------------------------------------------------------------

#ifndef LoggerH
#define LoggerH
#include <stdio.h>
//---------------------------------------------------------------------------

class Logger {

  private:
        int hitCount;
        int missAgeCount;
        int missHashCount;
        int missIndexCount;

  public:
        Logger();
        void hit();
        void missAge();
        void missHash();
        void missIndex();

        void printLastError(char *buffer);
};
#endif
