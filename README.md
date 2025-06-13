
One of the most relevant ones is the so-called "virtual File Subsystem" /proc part of the Linux OS (see https://man7.org/linux/man-pages/man5/proc.5.htmlLinks to an external site. and https://tldp.org/LDP/Linux-Filesystem-Hierarchy/html/proc.htmlLinks to an external site.).
Under /proc we can find (as we already discovered in Assignment #1) a lot of information and resources description from the actual system.

 

The program, will generate:

process FD table
System-wide FD Table
Vnodes FD table, and
a composed view of the previous table
a table summarizing number of FDs open per user processes
 

The program will accept multiple commands line arguments:

flagged arguments
--per-process, indicates that only the process FD table will be displayed
--systemWide, indicates that only the system-wide FD table will be displayed
--Vnodes, indicates that the Vnodes FD table will be displayed
--composite, indicates that only the composed table will be displayed
--summary, indicates that a table summarizing number of FDs open per process will be displayed
--threshold=X, where X denotes an integer, indicating that processes which have a number of FD assigned larger than X should be flagged in the output.
For this it will list the PID and number of assigned FDs, e.g. PID (FD)
and combination of these, i.e. the program can receive multiple of these flags combined.
Default behaviour: if no argument is passed to the program, then you must implement one of the following approaches -- please state this design decision clearly in your documentation:
the program will display the composite table, i.e. same effect as having used the --composite flag
the program will display all the tables, i.e. same effect as having used all the flagged arguments  --per-process --systemWide --Vnode --composite 
positional argument:
only one positional argument indicating a particular process id number (PID), if not specified the program will attempt to process all the currently running processes for the user executing the program
