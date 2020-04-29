Compile the program :-
	gcc CreateLSCommand.c

Run the program :-
	1) ./a.out  
	2) ./a.out flags_name    //(e.g. flags_name are  -l, -R, -a, -ltr  etc.)
	3) ./a.out directory_name    
	4) ./a.out flags_name directory_name 


Process of Making LS Command with flags :-

1)First we define a structure of flags (Total flags are-16  a,d,h,i,l,m,n,o,p,Q,r,R,s,S,t,U).
2)And initialized all flags as "false". 
3)I am write a function ( getFlags() ) , its checks which flags are given in command line argument.
4)In command line argument, we check after "./a.out" if "-" character present or not.
5)If "-" present then we check single single character which are present in "(adhilmnopQrRsStU)" string. If a character is not present in "(adhilmnopQrRsStU)" string then we gives an error.
6)If characters are present in "(adhilmnopQrRsStU)" string then after that, we initialize presents flags as "true".
7)If "-" character is not present then we consider as directory(folder) and check that directory present in our current directory or not. If this directory is not present in our current directory then we gives as an error.
8)We also check multiple directories are given in command line argument. 
9)After that we scan a directory and print all files/directory(s) according to flags.


10)If flag 'a' is present then print all files or directory starting with '.' .
11)If flag 'd' is present then print directories themselves, not their contents.
12)If flag 'h' is present then print all files or directory with human readable sizes.
13)If flag 'i' is present then print the index number of each file.
14)If flag 'l' is present then print files with long listing format.
15)If flag 'm' is present then print files with a comma separated list of entries.
16)If flag 'n' is present then print files like 'l', but list numeric user and group IDs.
17)If flag 'o' is present then print files like 'l', but do not list group information.
18)If flag 'p' is present then print files with append / indicator to directories.
19)If flag 'Q' is present then enclose entry names in double quotes.
20)If flag 'r' is present then print files reverse order while sorting.
21)If flag 'R' is present then list subdirectories recursively.
22)If flag 's' is present then print the allocated size of each file, in blocks.
23)If flag 'S' is present then sort by file size, largest first.
24)If flag 't' is present then sort by modification time, newest first.
25)If flag 'U' is present then do not sort; list entries in directory order.
