# OSKernelsCSCE351

Build Environment:
- Windows 7 Virtual Machine in VMWare Workstation
- Cygwin64 
- Visual Studio 2015 
- VisualGDB (30-day trial) Extension set to use GCC 4.9.3-1 with Clang Intellisense 
  --> Use a VM and create a Snapshot before installing VisualGDB. 

HW2: GCC command line build commands.  Execute from the HW2FinalCode directory.
  Problem 1:
    gcc prob1/pro_con1.c SharedResources.h SharedResources.c -O0 -o prob1/prob1
  Problem 2:
    gcc prob2/pro_con2.c prob2/monitor2.h prob2/monitor2.c SharedResources.h SharedResources.c -O0 -o prob2/prob2
  Problem 3:
    gcc prob3/pro_con3.c prob3/monitor3.h prob2/monitor3.c SharedResources.h SharedResources.c -O0 -o prob3/prob3
