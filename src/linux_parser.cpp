#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
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

// An example of how to read data from the filesystem
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

// Update this to use std::filesystem
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float memtotal;
  float memfree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal") memtotal = std::stof(value);
      if (key == "MemFree") memfree = std::stof(value);
    }
  }
  return (memtotal - memfree) / memtotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string value;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    uptime = std::stol(value);
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string value;
  vector<string> cpuUtilization;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      vector<int> tmp;
      std::istringstream stream(line);
      while (stream >> value) {
        tmp.push_back(stoi(value));
      }
      int idle = tmp[4] + tmp[5];
      int active = tmp[1] + tmp[2] + tmp[3] + tmp[5] + tmp[5] + tmp[8];
      cpuUtilization.push_back(std::to_string(active / (float)(idle + active)));
    }
  }
  return cpuUtilization;
}

// Read and return CPU utilization of pid
float LinuxParser::CpuUtilization(int pid) {
  float cpuUsage = 0.0;
  string line, value;
  vector<string> cpu;
  std::ostringstream oss;
  oss << pid;
  string pid_ = "/" + oss.str();
  std::ifstream filestream(kProcDirectory + pid_ + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    while (stream >> value) {
      cpu.push_back(value);
    }
    int totalTime = stoi(cpu[13]) + stoi(cpu[14]) + stoi(cpu[15]);
    float seconds = UpTime() - (stof(cpu[21]) / sysconf(_SC_CLK_TCK));
    cpuUsage = ((totalTime / sysconf(_SC_CLK_TCK)) / seconds);
  }
  return cpuUsage;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, totalProcesses;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> totalProcesses;
      if (key == "processes") break;
    }
  }
  return stoi(totalProcesses);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, runningProcesses;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> runningProcesses;
      if (key == "procs_running") break;
    }
  }
  return stoi(runningProcesses);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string command;
  std::ostringstream oss;
  oss << pid;
  string pid_ = "/" + oss.str();
  std::ifstream filestream(kProcDirectory + pid_ + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    stream >> command;
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string key, line, ram;
  std::ostringstream oss;
  oss << pid;
  string pid_ = "/" + oss.str();
  std::ifstream filestream(kProcDirectory + pid_ + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> ram;
      if (key == "VmSize") break;
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, uid;
  std::ostringstream oss;
  oss << pid;
  string pid_ = "/" + oss.str();
  std::ifstream filestream(kProcDirectory + pid_ + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stream(line);
      stream >> key >> uid;
      if (key == "Uid:") break;
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, user, x, uid;
  std::ostringstream oss;
  oss << pid;
  string pid_ = "/" + oss.str();
  string uid_ = LinuxParser::Uid(pid);
  std::ifstream filestream(kProcDirectory + pid_ + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      stream >> user >> x >> uid;
      if (uid_ == uid) break;
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, value;
  vector<string> stats;
  std::ostringstream oss;
  oss << pid;
  string pid_ = "/" + oss.str();
  std::ifstream filestream(kProcDirectory + pid_ + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    while (stream >> value) {
      stats.push_back(value);
    }
  }
  long upTime = UpTime() - std::stol(stats[21]) / sysconf(_SC_CLK_TCK);
  return upTime;
}
