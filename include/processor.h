#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float prevUser = 0;
  float prevNice = 0;
  float prevSystem = 0;
  float prevIdle = 0;
  float prevIowait = 0;
  float prevIrq = 0;
  float prevSoftirq = 0;
  float prevSteal = 0;
  // float prevGuest = 0;
  // float prevGuest_nice;
  float prevTotal = 0;
  float prevNonIdle = 0;
};

#endif