ydp11 & wa147

Our memory allocation system uses a 4096 byte array to represent memory, 
with each allocated block containing a 5 byte header for metadata including an allocation status flag. 
A separate byte tracks initialization. Malloc checks for sufficient free space then scans to find the 
first open block fitting the requested size, allocating it by updating metadata and splitting any excess 
into a new free block, returning a pointer to the user data start. Free releases a block by setting its 
header to free and invoking coalesce to combine adjacent freed blocks. Coalesce scans the full array, 
merging adjacent freed blocks by updating their headers to reduce fragmentation. Any errors in malloc 
or free are reported by calling error with a description, which prints the message and terminates the 
program, providing visibility into anomalous operations.

In order to see how much time each test takes and if it operates correctly, 
we incorporated the time.h library which allows us to see specifically how long each test takes. 
Also, we took the average of all 50 runs of each test to get a precise time for each test. 
We were able to use the gettimeofday function in the time.h library to retrieve the total time it took to run.

For test case 4 this test case was created by us and in short it randomly allocates memory of different sizes 
from range 16, 32, 64, and 128. After it allocates, the array will be shuffled which means that the data is 
in different order. After this, this shuffled data will be freed by our free function. On the other hand, 
for test case 5, we also allocated memory at different sizes. However, for this function/test, we free 
everything in the function. The reason that this is different because it can have byte sizes that are 
potentially too big and can throw errors for us way more.
