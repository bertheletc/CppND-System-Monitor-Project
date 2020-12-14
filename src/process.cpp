#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;
using std::to_string;
using namespace LinuxParser;

// Return this process's ID
int Process::Pid() { return _pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
    // Declare variables to store other time values needed for calculation
    string utime, stime, cutime, cstime, starttime;
    
    // Code to extract relevant data from stat file
    string line, value;
    std::ifstream stream(kProcDirectory + to_string(_pid) + kStatFilename);
    if (stream.is_open()) {
        // Only need to get the single line in stat and parse through that
        std::getline(stream, line);
        std::istringstream linestream(line);
        // Pop off values from the linestream and save the important ones
        for (int i=1; i<=22; i++) {
            switch (i) {
                case 14:
                    linestream >> utime;
                    break;
                case 15:
                    linestream >> stime;
                    break;
                case 16:
                    linestream >> cutime;
                    break;
                case 17:
                    linestream >> cstime;
                    break;
                case 22:
                    linestream >> starttime;
                    break;
                default:
                    linestream >> value;
                    break;
            }
        }
    }

    // Code to calculate cpu utilization using values found above
    //
    // Get the system up time using existing LinuxParser method
    long sysUpTime = LinuxParser::UpTime();

    unsigned long long int total_time = stoi(utime) + stoi(stime) + stoi(cutime) + stoi(cstime);
    unsigned long long int seconds = sysUpTime - (stoi(starttime) / sysconf(_SC_CLK_TCK));

    // Calculate final cpu utilization and set to private variable
    _cpu = 100 * ((float)(total_time / sysconf(_SC_CLK_TCK)) / (float) seconds);

    return _cpu;
    
    // unsigned long long int total_time_diff = abs(total_time - _prev_total_time);
    // unsigned long long int seconds_diff = abs(seconds - _prev_seconds);

    // // Set the previous values to the current ones for next time
    // _prev_total_time = total_time;
    // _prev_seconds = seconds;

    // return 100 * ((float)(total_time_diff / sysconf(_SC_CLK_TCK)) / (float) seconds_diff);

    // Algoritm taken from StackOverflow post below
    // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    //
    // total_time = utime + stime
    // total_time = total_time + cutime + cstime
    // seconds = uptime - (starttime / Hertz)
    // cpu_usage = 100 * ((total_time / Hertz) / seconds)
}

// Simple feedthru using LinuxParser namespace Command() method
string Process::Command() { return LinuxParser::Command(_pid); }

// Convert the output from LinuxParser from kB to MB and return
string Process::Ram() { 
    int ram = stof(LinuxParser::Ram(_pid));
    // Conversion from kB to MB
    ram /= 1000;
    return to_string(ram);
}

// Simple feedthru using LinuxParser namespace User() method
string Process::User() { return LinuxParser::User(_pid); }

// Simple feedthru using LinuxParser namespace UpTime() method
long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

// Overload the "less than" comparison operator for Process objects to compare cpu utilization
bool Process::operator<(const Process& a) const {
    return (this->_cpu < a._cpu);
}
// StackOverflow for sorting vector of custom objects
// https://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects