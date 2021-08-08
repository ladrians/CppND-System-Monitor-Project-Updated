#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization(std::string str) {
  string line, key, unit;
  float val = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::remove(line.begin(), line.end(), ':');
      std::istringstream linestream(line);
      while (linestream >> key >> val >> unit) {
        if (key == str) {
          return val;
        }
      }
    }
  }
  return val;
}

long LinuxParser::UpTime() {
  long uptime{0};
  string line, uptime_string;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime_string;
    uptime = stof(uptime_string);
  }
  return uptime;
}

long LinuxParser::Jiffies() {
  long active = ActiveJiffies();
  long idle = IdleJiffies();
  return active + idle;
}

long LinuxParser::ActiveJiffies(int pid) {
  string item, line;
  long active = 0;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i=1; i<=17; i++)
    {
      linestream >> item;
      if(i >=14 && i<=17) // get #14_utime + #15_stime + #16_cutime + #17_cstime
      {
        active += std::stol(item);
      }
    }
  }
  return active;
}

long LinuxParser::ActiveJiffies() {
  long active = 0;
  vector<CPUStates> states = {
    kUser_,
    kNice_,
    kSystem_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
  };

  vector<float> cpu_utilization = CpuUtilization();
  for (auto state: states) {
    active += cpu_utilization[state];
  }
  return active;
}

long LinuxParser::IdleJiffies() {
  long idle = 0;
  vector<CPUStates> idle_states = {
    kIdle_,
    kIOwait_,
  };
  vector<float> usage = CpuUtilization();
  for (auto state: idle_states) {
    idle += usage[state];
  }
  return idle;
}

vector<float> LinuxParser::CpuUtilization() {
    string key;
    vector<float> result = {};
    float user, nice, system, idle;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        if (linestream >> key >> user >> nice >> system >> idle) {
          if (key == "cpu") {
              return { user, nice, system, idle};
          }
        }
      }
    }
    return result;
}

int LinuxParser::TotalProcesses() {
  return GetProcessKey("processes");
}

int LinuxParser::RunningProcesses() {
  return GetProcessKey("procs_running");
}

int LinuxParser::GetProcessKey(std::string str) {
  string key = "";
  int value = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == str) {
        return value;
      }
    }
  }
  return value;
}

string LinuxParser::Command(int pid) {
  string line = "";
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(filestream.is_open())
  {
    std::getline(filestream, line);
  }
  return line;
}

string LinuxParser::Ram(int pid) {
  string ret = GetProcessKeyById(pid, "VmSize");
  if (std::all_of(ret.begin(), ret.end(), isdigit))
    return ret;
  else
    return "";
}

string LinuxParser::Uid(int pid) {
  return GetProcessKeyById(pid, "Uid");
}

string LinuxParser::GetProcessKeyById(int pid, string str) {
    string line, key, value;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::remove(line.begin(), line.end(), ':');
        std::istringstream linestream(line);
        if (linestream >> key >> value) {
          if (key == str) {
            return value;
          }
        }
      }
    }
    return "";
}

string LinuxParser::User(int pid) {
    string line;
    int key;
    string X, user;
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ' ', '_');
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        if (linestream >> user >> X >> key) {
          if (key == pid) {
            std::replace(user.begin(), user.end(), '_', ' ');
            return user;
          }
        }
      }
    }
    return "";
}

long LinuxParser::UpTime(int pid) {
    long uptime = 0;
    string line = "";
    string current = "";
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open())
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      for (int i=1; i <= 22; i++)
      {
        linestream >> current;
        if (i == 22) // relates to start time
        {
          uptime = stol(current);
        }
      }
    }
    return uptime;
}
