#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
    float Utilization();
  // Declare private variables for previous idle and total CPU values
  // These will allow for dynamic CPU utilizations calculations
 private:
    // Use large datatype for potentially high numbers over time
    unsigned long long int _prev_idle{0};
    unsigned long long int _prev_total{0};
};

#endif