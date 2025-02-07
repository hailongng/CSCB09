1) How I solve the problem

  
2) Functions explanation

int min(int a, int b):    (Auxiliary function) Find the minimum of 2 values.
int max(int a, int b):    (Auxiliary function) Find the maximum of 2 values.
int find_index(double value):    (Auxiliary function) Find the index of a value on a graph with height 10, based on its percentage ratio to the upper bound being value.
void grab_memory_info(double *freeram):    Find the amount of free memory in RAM at a given instance.
void memory_display(int sample_count, int tdelay):    Display the memory usage (based on information gathered from grab_memory_info(freeram)) in sample_count instances, with a tdelay interval (in microseconds)
void grab_cpu_info(int* cpudata):    Gather the information of CPU usage at a given instance
void cpu_display(int sample_count, int tdelay):    Display the CPU usage (based on information gathered from grab_cpu_info(cpudata)) in sample_count instances, with a tdelay interval (in microseconds)
void duo_display(int sample_count, int tdelay):    Display the CPU usage (based on information gathered from grab_cpu_info(cpudata)) in sample_count instances, with a tdelay interval (in microseconds)

3) How to run the program:
To compile: gcc myMonitoringTool.c -Wall -Werror -std=c99 -lm -o Monitor
To run: execute the command
    ./Monitor [samples [tdelay]] [--cores] [--memory] [--cpu] [--samples=N] [--tdelay=T]
Explanation:
--cores: Declare this flag to display the number of cores and maximum processing frequency
--memory: Declare this flag to display memory usage
--cpu: Declare this flag to display CPU usage
--samples=N: Indicating N samples will be collected and displayed
  Note: this value must be a whole positive number. The maximum value will be 80. If exceeded, the program will plot the graph with default max value to be 80.
  If not declared, the program will run with the default value being 20.
--tdelay=T: Declaring the interval of measurement being T (measured in microseconds)
  Note: this value must be a whole positive number. The minimum value will be 100000 microseconds (0.1 seconds). The maximum value will be 10,000,000 microseconds (10 seconds).
  If the value declared is above the upper bound, the program will run with tdelay being the upper bound.
  If the value declared is below the lower bound, the program will run with tdelay being the lower bound.
  If not declared, the program will run with the default value being 500000 microseconds (0.5 seconds).
In the case that the CLAs is declared to have only 1 additional arguments, that value by default is the number of samples being fetched.
In the case that the CLAs is declared to have only 2 additional arguments, the first one is the number of samples being fetched and the second one is the time delay (measured in microseconds).

If there are any duplicate flags, or if the declaration is syntactically incorrect (mistyped flag, inappropriate numerical value type), the program will terminate.
