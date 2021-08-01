#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
    std::vector<float> cpu = LinuxParser::CpuUtilization();

    float user = static_cast<float>(cpu[0]);
    float nice = static_cast<float>(cpu[1]);
    float system = static_cast<float>(cpu[2]);
    float idle = static_cast<float>(cpu[3]);

    float total = user + nice + system + idle;
    float busy = total - idle;

    if (total > 0) return busy / total;
    return 0.0;
}