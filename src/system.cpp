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
using std::sort;

// Return the system's CPU Processor object
// NOTE: this Processor object is created during instantiation and updated when its Utilization() method is called
Processor& System::Cpu() { return _cpu; }

vector<Process>& System::Processes() { 
    // Use LinuxParser method to grab all current process IDs
    vector<int> pids = LinuxParser::Pids();

    // Empty the vector of processes since some may have started or ended since last loop
    // NOTE: this method doesn't seem efficient to me, and I'm sure there is a cleaner way. 
    // Could maybe use Union/Intersection type methods to only add/subtract difference between new and last pid list?
    _processes = {};

    // Iterate through all existing process IDs and add a corresponding Process object to the vector of processes  
    for (unsigned int i=0; i<pids.size(); i++) {
        _processes.push_back(Process(pids[i]));
    }

    // Sort the processes vector based on cpu utilization
    sort(_processes.begin(), _processes.end());

    return _processes; 
}

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