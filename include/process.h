#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // Constructor that initializes Process object with a given ID
  Process(int pid) : _pid(pid) {};
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization() const;                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
  // variable to hold processes ID number
  int _pid{0};
  // Declare private variables for previous seconds and totaltime values
  // These will allow for dynamic CPU utilizations calculations
  long long int _prev_total_time{0};
  long long int _prev_seconds{0};
  // Store cpu utilization value
  float _cpu{0};
};

#endif