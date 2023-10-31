# Quine-McCluskey
## Environment
GCC: 4.9.2  
## Introduction
Two-level logic optimizer based on the Quine-McCluskey method.  
The main objective is to find the minimum cover of the given function.  
It is accomplished in two steps:
1. Find all prime implicants of the function.  
2. Find the sum-of-products expression of the function with minimum number of prime implicant.
## Method
### Step1:
The Quine-McCluskey method can be found in [here](https://www.geeksforgeeks.org/quine-mccluskey-method/ "link").  
### Step2:
Create a table based on the prime implicants obtained from step1.  
- For example  

|  | 0 | 1 | 2 | 9 | 12 | 20 | 21 | 22 | 23 | 24 | 25 | 29 | 30 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 0000- | x | x |   |   |   |   |   |   |   |   |   |   |   |
| 000-0 | x |   | x |   |   |   |   |   |   |   |   |   |   |
| -1100 |   |   |   |   | x |   |   |   |   |   |   |   |   |
| --001 |   | x |   | x |   |   |   |   |   |   | x |   |   |
| 11-0- |   |   |   |   |   |   |   |   |   | x | x | x |   |
| 1---1 |   |   |   |   |   |   | x |   | x |   | x | x |   |
| 1-1-- |   |   |   |   |   | x | x | x | x |   |   | x | x |  

Find the combination of rows (prime implicants) that can mark 'x' on all columns (on-set elements) with the minimum number of rows.  
- Algorithm  
Generate a general tree and increase the number of rows as depth increase.
## Result
This project implements the function by forward mapping: source->destination.  
Which causes the black strips in the destination.  
There are two ways to eliminate the black strips.  
1. Do it by inverse mapping: scan the destination and copy the source pixel value.  
2. Use the neighbor pixel to interpolate pixels in the black strips.

|  | execution time (second) | speedup |
| :---: | :---: |  :---: |
| CPU | 0.027 |  |
| GPU | 0.0072 | 3.7 |
## How to use
### Windows
CPU:  
  ```
  nvcc -o pt_cpu perspective_transformation_CPU.cu -arch=sm_75  
  pt_cpu.exe
  ```
GPU:  
  ```
  nvcc -o pt_gpu perspective_transformation_GPU_device1.cu -arch=sm_75  
  pt_gpu.exe
  ```
### Linux

CPU:  
  ```
  ./compile_cpu.sh  
  ```
GPU: 
  ```
  ./compile_gpu_1.sh  
  ```
