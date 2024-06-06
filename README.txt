DESCTIPTION

For solving the task of quickly searching for text in files, I came up with the following design:
There will be several threads.
One thread will search for files in the specified directory.  Why one?
Here, it could have tried to speed up by dividing into several threads for subdirectories, but in this case,
The "bad place" is the speed of reading HDD\SSD, it would not give a significant advantage in time, but take it away from other threads.
Several other threads (the number depends on the hardware) will search for text in files.
And another thread needed for printing results.
Printing results directly during the search would lead to a significant slowdown and problems with the sequence of output on the screen.
In the end, we have
1 - a thread for iterating over the file system
1 - for printing on the screen
N - threads for searching the text.

After one thread found a suitable file, he puts it in the processing queue, from which several other threads take it.
In the case of finding the text, it is highlighted in color and put in the printing queue.

The queue is the most important component. He must be very fast.
In our case, there are two variants:
1. one thread writes many read.
2. many write one read.

I considered the Lock-free variants and chose the implementation with std::atomic and CAS.
Because the size of the queue is unknown, I chose the implementation on linked lists.
In this case, we are losing performance on creating list nodes, but it can create a random queue.

For optimization of search, I made the determination of file type, so that we don't search in binary data.

I make unittest for lock-free queue, as the main component, with gtests.
Also, I conducted a load test on a large number of different file systems and ensured that all cores are utilized (see cpu_load.png).
I tested it on Windows 11 and Ubuntu 20.04.

For user convenience:
The found text is output with the line number and is highlighted.
You can set search parameters:
- ignore_binaries: A boolean flag indicating whether to ignore binary files.
- verbose_mode: A boolean flag indicating whether to enable verbose mode.
- file_mask: A string representing the file mask for filtering files.

Complexity:
files iteratinon  O(n), n - numbers of files
Search in file O(m*с), m - numbers of lines, с - numbers of symbols in lime
Output  O(s), s - number of founded lines
Queue complexity:
push back O(1)
pop front O(1)

BUILDING AND USING

Building
cmake -S . -B build
cmake --build build

Test running
cd build/grep/tests && ctest

Using
./grep --help

Example:
./grep /home/user "OFFER"
./grep /home/user "DATA" -v
./grep /home/user "Get|GetValue" .json
./grep /home/user "([a-z]+)\\.([a-z]+)"






