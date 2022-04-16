HashTable
=========
Testing and optimizing different verions of hash functions

Description
-----------

Studying task, which includes two parts: 
1. Comparing different hash functions 
2. Using different optimizing methods to increase performance of work with hash table

In this task list structure used, that is made by me previously. [List structure here](https://github.com/RustamSubkhankulov/list "List Structure")

First part. Comparing hash functions
------------------------------------

Main propeties of good hash function:
1. determinism
2. high counting speed
3. all values are equiprobable
4. one-wayness
5. by changing one bit in the key, the hash differs significantly ( hash(key) is not continuous function)

To compare different hash functions I used hash table with List structures as elements of it. If collisions occures, element ads to list, 
so by examing size of each list we can assume how often hash collisions are happening during experiment

For compare I used [hamlet](https://github.com/RustamSubkhankulov/hamlet "Hamlet lib") implementation to split input text file - Oxford Dictionary into words.
In the cycle firstly added words to hash table, then I recorded sizes of lists in hash table and used simple python script to make histograms for each hash function I tested

Size of hash table were fixed during tests and it was equal 4096

Data, used for making plots are here: [/text_files/res.txt](https://github.com/RustamSubkhankulov/HashTable/blob/master/text_files/res.txt "data")

<h3> List of compared functions: </h3>

1. Hash function, that always returns 1 
2. Hash function, that returns ASCII code of first symbols of string
3. Hash function, that returns size of string
4. Hash function, that returns sum of ASCII codes of string elements
5. Rol hash function ( code of function will be shown later)
6. Crc32 hash function

<h3> Plots </h3>

Firstly we will look at plots and data, that I got from tests. Secondly we will process results.

<h4> 1. Hash function, that always returns 1 </h4> 

  ![plot #0](/pictures/graph_0.png "one hash")
  
Situation with colisions with this hash function is so bad, that we can not see one big column in histogram.
Here is a screenshot of data, that were used for building plot:

  ![data #0](/pictures/screenshots/one_ascii_data.png "one hash")

As we can see, this hash function is pretty bad. Collisions in 100% of cases. This function does not have properties of good hash function

<h4> 2. Hash function, that returns ASCII code of first symbols of string </h4>

  ![plot #1](/pictures/graph_1.png "first ascii hash")

This function is better, but still can not be used for proper use of hash table, collisions are happening too frequently. 
Values of hash function are not equiprobable. This is not a good hash function too.

Here are data for plot: 

  ![data #1](/pictures/screenshots/first_ascii_data.png "first ascii data")

<h4> 3. Hash function, that returns size of string </h4> 

  ![plot #2](/pictures/graph_2.png "sizeof hash")
 
As we can see, there are some troubles with making plot. Let's look at data:

  ![data #2](/pictures/screenshots/sizeof_data.png "sizeof data")
  
This hash function has similar problems like previous one.

<h4> 4. Hash function, that returns sum of ASCII codes of string elements </h4> 

  ![plot #3](/pictures/graph_3.png "ascii sum hash")
  
Dispersion of values of this hash function is better than of previous one, but still this function can not be called good hash function

<h4> 5. Rol hash function </h4> 

Here is code of this functions:

  ![code](/pictures/screenshots/rol_hash.png "rol hash")
  
And here is plot:

  ![plot #4](/pictures/graph_4.png "rol hash")
  
We can see, that this hash function is much better. All values in range are used, although there are some lists filles more than other. It meas that collisions are still happening pretty frequently with some key values

<h4> 6. Crc32 hash function </h4> 

  ![plot #5](/pictures/graph_5.png "crc32 hash")
  
This hash functions is even better. Less high values of list sizes means that values in range of hash function are equiprobable, as it has to be with ideal hash function.

<h3> Analysis </h4>

Dispersion values for different hash functions:

| Hash Func   | Dispersion |
|-------------|------------|
| one         | 1202909    |
| first ascii | 35906      |
| sizeof      | 131952     |
| ascii sum   | 5077       |
| rol         | 87         |
| crc32       | 17         |

We can see, that the best results are shown by crc32 hash function.

Second part. Optimizing performance
-----------------------------------

<h3> Introduction </h3>

Second part of task - optimizing performance of some tests for hash table

I made test, that taked as input array of structures, containing char* pointer tp word from input text and unsigned int lenght of this string.
Then it calculates hash value of tokens and ads them to hash table if there were no such words before. Then it 128 times executes hash table search, that includes hash value calculating and list search. Then for each word from array of tokens it searches it once again and deleted from hash table. Aso I added timer, that measures time of this stress test especially. 
During executing I don't have to calculate lenght of word again, cause it is already known after parsing text into tokens.

For measuring time of executing whole program, I used 'time' linux command. Before any optimizations, I measured time of perfoming programm with different optimization flags of gcc compiler. Here there are:

| Number \ Flags  | No flags | No flags | -O1   | -O1  | -O2   | -O2  | -O3   | -O3  |
|-----------------|----------|----------|-------|------|-------|------|-------|------|
| Period          | Total    | Test     | Total | Test | Total | Test | Total | Test |
| 1               | 5.52     | 5.43     | 2.99  | 2.93 | 2.95  | 2.90 | 2.90  | 2.85 |
| 2               | 5.47     | 5.39     | 3.00  | 2.95 | 2.97  | 2.91 | 2.89  | 2.84 |
| 3               | 5.52     | 5.43     | 3.01  | 2.95 | 2.94  | 2.89 | 2.93  | 2.84 |
| 4               | 5.52     | 5.44     | 2.99  | 2.94 | 2.94  | 2.89 | 2.92  | 2.83 |
| 5               | 5.60     | 5.51     | 3.00  | 2.94 | 2.95  | 2.89 | 2.90  | 2.84 |
| Average         | 5.53     | 5.44     | 3.00  | 2.94 | 2.95  | 2.89 | 2.91  | 2.84 |

Input used for test - Oxford Dictionary, [here it is](https://github.com/RustamSubkhankulov/HashTable/blob/master/text_files/oxford.txt)

Also important thing to note that I tried to optimize only 'stress test function', initial processes like parsing input text and etc was not examined and changed with making optimizations.

<h3> Optimizations </h3>

<h4> Step 1. Profiler </h4>
