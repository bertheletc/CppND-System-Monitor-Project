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

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// Simple feedthru using LinuxParser namespace Kernel() method
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Simple feedthru using LinuxParser namespace MemoryUtilization() method
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Simple feedthru using LinuxParser namespace OperatingSystem() method
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Simple feedthru using LinuxParser namespace RunningProcesses() method
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Simple feedthru using LinuxParser namespace TotalProcesses() method
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Simple feedthru using LinuxParser namespace UpTime() method
long int System::UpTime() { return LinuxParser::UpTime(); }