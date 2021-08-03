#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "system.h"
#include "process.h"
#include "linux_parser.h"

const int TO_KB = 1024;

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

string Process::Command() {
    return LinuxParser::Command(pid_);
}

string Process::Ram() {
    if(pid_ > 0)
        return std::to_string(stoi(LinuxParser::Ram(pid_)) / TO_KB);
    return "";
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    if (user_ == "") {
        string uid = LinuxParser::Uid(pid_);
        if (std::all_of(uid.begin(), uid.end(), isdigit)) {
            user_ = LinuxParser::User(stoi(uid));
        }
    }
    return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
    //return System::UpTime() - LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
}

bool Process::operator<(Process const& a) const {
    return pid_ < a.pid_;
}