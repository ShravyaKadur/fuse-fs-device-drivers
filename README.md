# 577 Project Description
- Due Date:  April 24th, 11:59pm (4+ weeks)
- Group Assignment (2)

## 1. Goal
The Goal of this assignment is to teach students the needed techniques to implement device drivers, and file systems in user space (FUSE). FUSE is a concept applied in operating system where the filesystem is part of the kernel, this is why the FUSE part will be done in **Linux** not **Minix**. In this project the students will learn:

-   How to create a simple Character Device Driver?
-   What is FUSE and How to write a fuse file system in Linux.

The final project consists of two parts:

## 2. Part I: Device Drivers

Minix3 is a micro-kernel OS where minix drivers lies outside the kernel. As mentioned in lecture devices can be block devices or character devices, although both are very important this lab only focuses on character device drivers. The description of the lab and the task is available at [link](./Samples/Drivers).

## 3. Part II: FUSE
A filesystem is a method for storing and organizing computer files and directories and the data they contain, making it easy to find and access them. In most operating systems, filesystem is considered to be a part of the kernel, where adding or modifying the file system was an arduous process. In recent years an important concept have appeared File System in User Space or FUSE. More details about fuse the the task is available at [link](/Samples/FUSE). 

## 4. Submission and Grading
### 4.1 Submission
The repository will look as follows:

    /project-id
    |-- README.md # Lab Descriptions
    |-- Sample/ # Sample Code
    |-- submit/ # submission folder 
        |-- driver/ # driver related deliverables
            |-- README.md # Driver related Documentation
        |-- /fuse/ # fuse related 
            |-- README.md # Fuse Related Documentation

Please be sure to copy all these files into the code folder where you cloned your repo to be able to able to push it.

### 4.2.	Grading:
The assignment will be graded on a 20 mins meeting session, time slots will be announced on piazza before the deadline.

This assignment is graded per part as follows:
-   Working Assignment End to End (50%)
-   Answering Questions on meeting (25%)
-   Code Upload and Documentation (25%)

Also students are encouraged to report tweaks they had to follow to complete this assignment. Finally, if you have any problem don't hesitate to drop by in the office hours, post a question on piazza, or send us an e-mail.
