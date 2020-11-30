#include <string>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long time) {
    string hours, minutes, seconds;
    int hr, min, sec;
    hr = time / 3600;
    time %= 3600;
    min = time / 60;
    time %= 60;
    sec = time;

    // Pad with zeros if values are single digit to fullfill HH:MM:SS format
    if (hr < 10) {
        hours += "0";
    }
    hours += to_string(hr);
    if (min < 10) {
        minutes += "0";
    } 
    minutes += to_string(min);
    if (sec < 10) {
        seconds += "0";
    } 
    seconds += to_string(sec);
    
    return hours + ":" + minutes + ":" + seconds; 
}