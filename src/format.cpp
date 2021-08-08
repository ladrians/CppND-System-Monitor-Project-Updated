#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long s) {
    // INPUT: Long int measuring seconds
    string elapsed_time = "";
    int hours = s / 3600;
    int minutes = s / 60;
    int seconds = s % 60;

    // OUTPUT: HH:MM:SS
    string hh = (hours > 9 ? string() : "0") + std::to_string(hours);
    string mm = string{":"} + (minutes > 9 ? "" : "0") + std::to_string(minutes);
    string ss = string{":"} + (seconds > 9 ? "" : "0") + std::to_string(seconds);
    elapsed_time = hh + mm + ss;
    return elapsed_time;
}