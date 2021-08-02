#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
    std::vector<float> cpu = LinuxParser::CpuUtilization();

    float user = cpu[0];
    float nice = cpu[1];
    float system = cpu[2];
    float idle = cpu[3];

    float total = user + nice + system + idle;
    float busy = total - idle;

    if (total > 0) return busy / total;
    return 0.0;
}