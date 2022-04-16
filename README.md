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



<h4> 6. Crc32 hash function </h4> 

//pict here

<h3> Analysis </h4>


Second part. Optimizing performance
-----------------------------------

