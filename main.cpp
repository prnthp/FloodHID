#include <iostream>
#include "hidapi.h"
#include <stdio.h>
#include <string.h>
#include <chrono>

struct timer
{
    typedef std::chrono::steady_clock clock ;
    typedef std::chrono::milliseconds milli ;

    void reset() { start = clock::now() ; }

    unsigned long long seconds_elapsed() const
    { return std::chrono::duration_cast<milli>( clock::now() - start ).count() ; }

    private: clock::time_point start = clock::now() ;
};


int run_main();

int main(int argc, const char * argv[]) {
    int ret = run_main();
    std::cout << "Exit with code: " + std::to_string(ret) + "\n";
}

int run_main()
{
    hid_device *teensy_handle;
    int r;
    r = hid_init();
    if (r < 0)
    {
        return r;
    }
    
    teensy_handle = hid_open(0x16C0, 0x0486, 0xFFAB);
    if (teensy_handle == NULL)
    {
        std::cout << "Failed to connect to Teensy @ 0x16C0, 0x0486, 0xFFAB\n";
        return -2;
    }
    
    timer t;
    
    unsigned long count = 0;
    
    while(true)
    {
        if (t.seconds_elapsed() < 10)
        {
            continue;
        }
        std::cout << "Sending Raw HID, Count: " + std::to_string(count) + "\n";

        unsigned char write_buf[65];
        memset(write_buf, 0, 65);
        write_buf[1] = 0xAB;
        
        r = hid_write(teensy_handle, write_buf, 65);
        
        if (r < 0)
        {
            std::cout << "Sending Raw HID FAILED, Count: " + std::to_string(count) + "\n";
        }
        
        count++;
        t.reset();
    }
}
