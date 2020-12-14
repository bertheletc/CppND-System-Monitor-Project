#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;
using std::stoi;

//
// SYSTEM
//

// Read and return the system OS
string LinuxParser::OperatingSystem() {
  string line, key, value;
  std::ifstream stream(kOSPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
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

// Read and return the system kernel
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string mem_total, mem_free;
  string line, label;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    // Get first line and pop off the value under the MemTotal label
    std::getline(stream,line);
    std::istringstream linestream1(line);
    linestream1 >> label >> mem_total;
    // Get second line and pop off the value under the MemFree label
    std::getline(stream,line);
    std::istringstream linestream2(line);
    linestream2 >> label >> mem_free;
  }
  // Return the memory used based on formula % MemUsed == (MemTotal - MemFree) / MemTotal
  return (stof(mem_total) - stof(mem_free)) / stof(mem_total); 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
}

// Read and return CPU utilization in a vector<strings> with all 10 values
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_values;
  string line, label, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> label;
    while (linestream) {
      linestream >> value;
      cpu_values.push_back(value);
    }
  }
  return cpu_values; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
}

//
// PROCESSES
//

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    // NOTE: No use for an istringstream because we want the entire line
    std::getline(stream, command);
  }
  return command;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  // Grab the UID of the process using local class method 
  string uid = LinuxParser::Uid(pid);
  string line, user, x, id;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      // Replace all colons with spaces to help parsing
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> id) {
        if (id == uid) {
          return user;
        }
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    // Only need to get the single line in stat and parse through that
    std::getline(stream, line);
    std::istringstream linestream(line);
    // Pop off values from the line stream until the 22nd value which is "starttime"
    // Cleaner way to do this???
    for (int i=1; i<=22; i++) {
      linestream >> value;
    }
  }
  // Convert clock ticks to seconds and return the value
  return stol(value) / sysconf(_SC_CLK_TCK);
}

//
//
// The functions below are not implemented because they only apply to older versions of Linux
//
//

// NOTE: Not needed with newer version of Linux, therfore no implementation
long LinuxParser::Jiffies() { return 0; }

// NOTE: Not needed with newer version of Linux, therfore no implementation
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// NOTE: Not needed with newer version of Linux, therfore no implementation
long LinuxParser::ActiveJiffies() { return 0; }

// NOTE: Not needed with newer version of Linux, therfore no implementation
long LinuxParser::IdleJiffies() { return 0; }