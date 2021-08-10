#include <string>

#include "format.h"

using std::string;

// Ideas from https://knowledge.udacity.com/questions/155686
const int SEC_IN_MIN = 60;
const int SEC_IN_HR = 3600;

string Format::ElapsedTime(long s) {
    // INPUT: Long int measuring seconds
    string elapsed_time = "";
    int hours = s / SEC_IN_HR;
    int minutes = s / SEC_IN_MIN - hours * SEC_IN_MIN;
    int seconds = s - minutes * SEC_IN_MIN - hours * SEC_IN_HR;

    // OUTPUT: HH:MM:SS
    string hh = (hours > 9 ? string() : "0") + std::to_string(hours);
    string mm = string{":"} + (minutes > 9 ? "" : "0") + std::to_string(minutes);
    string ss = string{":"} + (seconds > 9 ? "" : "0") + std::to_string(seconds);
    elapsed_time = hh + mm + ss;
    return elapsed_time;
}