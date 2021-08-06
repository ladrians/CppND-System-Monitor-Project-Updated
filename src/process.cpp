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

float Process::CpuUtilization() {
    if (pid_ <= 0) {
        return 0;
    }

    float utilization = 0;
    long process_active_jiffies{LinuxParser::ActiveJiffies(pid_)};
    float process_active = process_active_jiffies/sysconf(_SC_CLK_TCK);

    long system_uptime_seconds{LinuxParser::UpTime()};

    long process_uptime_jiffies{LinuxParser::UpTime(pid_)};
    long process_uptime_seconds = process_uptime_jiffies/sysconf(_SC_CLK_TCK);

    float time_elapsed = system_uptime_seconds - process_uptime_seconds;

    if (time_elapsed != 0)
    {
        utilization = (process_active/time_elapsed) * 1.0;
    }
	return utilization;
}

string Process::Command() {
    return LinuxParser::Command(pid_);
}

string Process::Ram() {
    if(pid_ > 0)
        return std::to_string(stoi(LinuxParser::Ram(pid_)) / TO_KB);
    return "";
}

string Process::User() {
    if (user_ == "") {
        string uid = LinuxParser::Uid(pid_);
        if (std::all_of(uid.begin(), uid.end(), isdigit)) {
            user_ = LinuxParser::User(stoi(uid));
        }
    }
    return user_;
}

long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
    //return System::UpTime() - LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
}

bool Process::operator<(Process const& a) const {
    return pid_ < a.pid_;
}