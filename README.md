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

To compare different hash functions I used hash table with List structures as elements of it. If collisions occures, element ads to list, 
so by examing size of each list we can assume how often hash collisions are happening during experiment

For compare I used [hamlet](https://github.com/RustamSubkhankulov/hamlet "Hamlet lib") implementation to split input text file - Oxford Dictionary into words.
In the cycle firstly added words to hash table, then I recorded sizes of lists in hash table and used simple python script to make histograms for each hash function I tested

<h3> List of compared functions: </h3>

1. Hash function, that always returns 1 
2. Hash function, that returns ASCII code of first symbols of string
3. Hash function, that returns size of string
4. Hash function, that returns sum of ASCII codes of string elements
5. Rol hash function ( code of function will be shown later)
6. Crc32 hash function

<h3> Plots </h3>

<h4> 1. Hash function, that always returns 1 </h4> 

  // plot here
  
Situation with colisions with this hash function is so bad, that we can not see one big column in histogram.
Here is a screenshot of data, that were used for building plot:

  // pict here

As we can see, this hash function is pretty bad. Collisions in 100% of cases

<h4> 2. Hash function, that returns ASCII code of first symbols of string </h4>

  //plot here

This function is better, but still can not be used for proper use of hash table, collisions are happening too frequently

<h4> 3. Hash function, that returns size of string </h4> 

  //plot here
  

<h4> 4. Hash function, that returns sum of ASCII codes of string elements </h4> 
<h4> 5. Rol hash function ( code of function will be shown later) </h4> 
<h4> 6. Crc32 hash function </h4> 

//pict here

<h3> Analysis </h4>


Second part. Optimizing performance
-----------------------------------

