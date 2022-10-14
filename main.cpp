/*
    Connector is an opensource software to connect the keyboards and mouses of two separate computers together and use them as one.
    It is written in C++ and runs on Windows, Linux and Mac OS X.
    for now it is only a proof of concept and not a finished product.
    we are simply sending the mouse and keyboard events from one computer to the other and vice versa, writing the events to a text file.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#define READ_LIMIT 0.5

#ifdef _WIN32
#include <windows.h>

#elif __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#elif __APPLE__
#include <ApplicationServices/ApplicationServices.h>

#endif

using namespace std;

int main()
{
    // open the file
    ofstream myfile;
    myfile.open ("events.txt");

    // get time of the first event
    auto start = chrono::system_clock::now();
    // get the events
    while (true)
    {
        // get the time of the event
        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = end-start;
        if (elapsed_seconds.count() > READ_LIMIT)
            {
            #ifdef _WIN32
            // get the mouse position
            POINT p;
            GetCursorPos(&p);
            //myfile << "mouse " << p.x << " " << p.y << endl;

            // get the keyboard events
            for (int i = 0; i < 256; i++)
            {
                if (GetAsyncKeyState(i) == -32767)
                {
                    myfile << "key " << i << endl;
                }
            }

            #elif __linux__
            // get the mouse position
            Display *display = XOpenDisplay(NULL);
            Window root = DefaultRootWindow(display);
            int x, y;
            Window child;
            unsigned int mask;
            XQueryPointer(display, root, &root, &child, &x, &y, &x, &y, &mask);
            myfile << "mouse " << x << " " << y << endl;

            // get the keyboard events
            for (int i = 0; i < 256; i++)
            {
                if (XQueryKeymap(display)[i/8] & (1<<(i%8)))
                {
                    myfile << "key " << i << endl;
                }
            }

            #elif __APPLE__
            // get the mouse position
            CGEventRef event = CGEventCreate(NULL);
            CGPoint point = CGEventGetLocation(event);
            myfile << "mouse " << point.x << " " << point.y << endl;

            // get the keyboard events
            for (int i = 0; i < 256; i++)
            {
                if (CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, i))
                {
                    myfile << "key " << i << endl;
                }
            }

            #endif
            }
    }

    // close the file
    myfile.close();

    return 0;
}

