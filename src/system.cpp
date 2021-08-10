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
          if (p.Command() != "")
          {
            currentProcesses.push_back(p);
          }
        }
    }
    processes_.swap(currentProcesses);
    return processes_;
}

std::string System::Kernel() {
    return LinuxParser::Kernel();
}

float System::MemoryUtilization() {
    float total = LinuxParser::MemoryUtilization("MemTotal");
    float free = LinuxParser::MemoryUtilization("MemFree");
    return (total - free) / total;
}

std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}

long int System::UpTime() {
    return LinuxParser::UpTime();
}