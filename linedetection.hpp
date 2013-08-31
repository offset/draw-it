#ifndef LINEDETECTION_H
#define LINEDETECTION_H
#include "linefinder.hpp"

class LineDetection
{
private:
    static LineDetection* singleton;
    LineFinder* finder;
    LineDetection()
    {
        singleton = new LineDetection;
    }
    
public:
    static LineDetection* getInstance()
    {
        if (singleton == 0)
        {
            singleton = new LineDetection;
        }
        return singleton;
    }
    
    static void destroy()
    {
        if (singleton != 0)
        {
            delete singleton;
            singleton = 0;
        }
    }
};

#endif // LINEDETECTION_H
