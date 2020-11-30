#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h" 

using std::stoi;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    // Use LinuxParser method to get all 10 CPU values in vector of strings
    std::vector<std::string> cpu_values = LinuxParser::CpuUtilization();

    unsigned long long int idle = stoi(cpu_values[3]) + stoi(cpu_values[4]);
    unsigned long long int total = idle + stoi(cpu_values[0]) + stoi(cpu_values[1]) + stoi(cpu_values[2]) + stoi(cpu_values[5]) + stoi(cpu_values[6]) + stoi(cpu_values[7]);

    unsigned long long int totaled = abs(total - _prev_total);
    unsigned long long int idled = abs(idle - _prev_idle);

    // Set the previous values to the current ones for next time
    _prev_idle = idle;
    _prev_total = total;

    return (float)(totaled - idled) / (float) totaled;
}

// ALGORITHM PSEUDOCODE from suggested StackOverflow post
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
//
// VALUE KEY: 0) user, 1) nice, 2) system, 3) idle, 4) iowait, 5) irq, 6) softirq, 7) steal, 8) guest, 9) guest_nice
//
// PrevIdle = previdle + previowait
// Idle = idle + iowait

// PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
// NonIdle = user + nice + system + irq + softirq + steal

// PrevTotal = PrevIdle + PrevNonIdle
// Total = Idle + NonIdle

// # differentiate: actual value minus the previous one
// totald = Total - PrevTotal
// idled = Idle - PrevIdle

// CPU_Percentage = (totald - idled)/totald