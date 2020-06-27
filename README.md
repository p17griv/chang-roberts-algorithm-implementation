# Chang-Roberts Election Algorithm OMNeT++ Implementation

### Description

This is a student project made for "Distributed Systems" course of department of Informatics - Ionian University. The main purpose of the project is to implement Chang-Roberts Election Algorithm, run it on various networks with ring topology and compare algorithm's complexity-theory (message passes) with the results of the simulations.

### About Chang-Roberts Algorithm

Chang-Roberts algorithm is an improved version of LeLann's algorithm for finding the largest (or smallest) of a set of uniquely numbered processes arranged in a circle/ring. This process is also known as "leader election" in which a node is defined as leader and runs a specific task (e.g. makes a decision). Algorithm's functionality is based on the fact that each node, who wants to become leader, sends his ID and if he recieves it back then he becomes leader. In case that a node, who wants to win the election, recieves an ID larger than his ID, he discards it. Else, if the recieved ID is smaller, he forwards it to the next node, just like a node, who doesn't want to become leader, does.

This algorithm does <b>n(n + 1)/2</b> in worst case, <b>2n – 1</b> in best case and <b>n log n</b> in average case, total forwards of the messages until the end - election of the leader. (n = number of nodes in network)

### How to run - Instructions

1. Download and install [OMNet++](https://omnetpp.org/download/) (tested on 5.6.2 version)
2. Run ```git clone (git url of the repository)``` into a directory in order to download the project.
3. Open OMNeT++ IDE
4. Go to ```File > Open Project from File System...``` and define the project directory as the "Import source"
5. Open the "omnetpp.ini" file and run the simulation.
