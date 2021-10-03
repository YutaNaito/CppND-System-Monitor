#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
// https://maku77.github.io/java/parse-proc-stat.html
float Processor::Utilization() {
  return std::stof(LinuxParser::CpuUtilization()[0]);
  // float user;     // normal processes executing in user mode
  // float nice;     // niced processes executing in user mode
  // float system;   // processes executing in kernel mode
  // float idle;     // twiddling thumbs
  // float iowait;   // In a word, iowait stands for waiting for I/O to
  // complete. float irq;      // servicing interrupts float softirq;  //
  // servicing softirqs float steal;    // involuntary wait

  // vector<string> cpu = LinuxParser::CpuUtilization();
  // user = stof(cpu[1]);
  // nice = stof(cpu[2]);
  // system = stof(cpu[3]);
  // idle = stof(cpu[4]);
  // iowait = stof(cpu[5]);
  // irq = stof(cpu[6]);
  // softirq = stof(cpu[7]);
  // steal = stof(cpu[8]);

  // prevIdle = prevIdle + prevIowait;
  // idle = idle + iowait;

  // prevNonIdle =
  //     prevUser + prevNice + prevSystem + prevIrq + prevSoftirq + prevSteal;
  // float nonidle = user + nice + system + irq + softirq + steal;

  // prevTotal = prevIdle + prevNonIdle;
  // float Total = idle + nonidle;

  // float totald = Total - prevTotal;
  // float idled = idle - prevIdle;

  // prevUser = user;
  // prevNice = nice;
  // prevSystem = system;
  // prevIdle = idle;
  // prevIowait = iowait;
  // prevIrq = irq;
  // prevSoftirq = softirq;
  // prevSteal = steal;
  // prevTotal = Total;

  // return (totald - idled) / totald;
}