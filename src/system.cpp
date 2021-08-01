#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    std::vector<Process> currentProcesses{};
    for (int pid : LinuxParser::Pids()) {
        bool containsPID = false;
        for (Process p_ : processes_) {
          if (p_.Pid() == pid) {
              currentProcesses.push_back(p_);
              containsPID = true;
              break;
          }
        }
        if (containsPID == false) {
          Process p(pid);
          currentProcesses.push_back(p);
        }
    }
    processes_.swap(currentProcesses);
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return string(); }

float System::MemoryUtilization() {
    float total = LinuxParser::MemoryUtilization("MemTotal");
    float free = LinuxParser::MemoryUtilization("MemFree");
    return (total - free) / total;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { return string(); }

int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }