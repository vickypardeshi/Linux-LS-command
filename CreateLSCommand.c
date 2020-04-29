#include<stdio.h>
#include<stdbool.h>    
#include<getopt.h>     
#include<dirent.h>     
#include<string.h>
#include<sys/stat.h>   
#include<sysexits.h>   
#include<stdlib.h>     
#include<grp.h>        
#include<pwd.h>        
#include<time.h>
#include<unistd.h>     


static char * globalDir = ".";

struct Flags
{
	bool flag_a;
  	bool flag_d;
  	bool flag_h;
  	bool flag_i;
  	bool flag_l;
  	bool flag_m;
  	bool flag_n;
  	bool flag_o;
  	bool flag_p;
  	bool flag_Q;
  	bool flag_r;
  	bool flag_R;
  	bool flag_s;
  	bool flag_S;
  	bool flag_t;
  	bool flag_U;
};

void initFlags(struct Flags *flags)
{
  	flags->flag_a = false;
  	flags->flag_d = false;
  	flags->flag_h = false;
  	flags->flag_i = false;
  	flags->flag_l = false;
  	flags->flag_m = false;
  	flags->flag_n = false;
  	flags->flag_o = false;
  	flags->flag_p = false;
  	flags->flag_Q = false;
	flags->flag_r = false;
 	flags->flag_R = false;
  	flags->flag_s = false;
  	flags->flag_S = false;
  	flags->flag_t = false;
  	flags->flag_U = false;
}


struct Flags getFlags(int argc, char * argv[])
{
  	struct Flags flags;
  	initFlags(&flags);
  	char flag;

  	while((flag = getopt(argc, argv, "adhilmnopQrRsStU")) != -1)
    	{
      		switch(flag)
		{	  
			case 'a' : flags.flag_a = true; break;
			case 'd' : flags.flag_d = true; break;
			case 'h' : flags.flag_h = true; break;
			case 'i' : flags.flag_i = true; break;
			case 'l' : flags.flag_l = true; break;
			case 'm' : flags.flag_m = true; break;
			case 'n' : flags.flag_n = true; break;
			case 'o' : flags.flag_o = true; break;
			case 'p' : flags.flag_p = true; break;
			case 'Q' : flags.flag_Q = true; break;
			case 'r' : flags.flag_r = true; break;
			case 'R' : flags.flag_R = true; break;
			case 's' : flags.flag_s = true; break;
			case 'S' : flags.flag_S = true; break;
			case 't' : flags.flag_t = true; break;
			case 'U' : flags.flag_U = true; break;  
		}
    	}
  
  	return flags; 
}


bool isInDirectory(char *mainDir, char *subDir)
{
  	DIR *dr = opendir(mainDir);
  	if(!dr)
    	{
      		printf("./a.out: cannot access \'%s\': No such file or directory\n", mainDir);
      		return false;
    	}

  	struct dirent *de = readdir(dr);

  	while(de)
   	{
      		if(strcmp(subDir, de->d_name) == 0)
		{
	  		closedir(dr);
	  		return true;
		}
      
      		de = readdir(dr);
    	}

  	printf("./a.out: cannot access \'%s\': No such file or directory\n", subDir);
 	closedir(dr);
  
  	return false;
}


struct stat getStats(const char *subDir)
{
  	char path[100];
  	sprintf(path, "%s/%s", globalDir, subDir);
  	struct stat st;

  	if (lstat(path, &st) == -1 )
    	{
      		printf("./a.out: cannot access \'%s\': No such file or directory\n",path);
      		exit(EXIT_FAILURE);
    	}

  	return st;      
}



static void printFileType(mode_t mode)
{
    	switch (mode & S_IFMT)
    	{
        	case S_IFREG: putchar('-'); break;
        	case S_IFDIR: putchar('d'); break;
    	}
}


static void printPermissions(mode_t mode)
{
    	putchar((mode & S_IRUSR) ? 'r' : '-');
    	putchar((mode & S_IWUSR) ? 'w' : '-');
    	putchar((mode & S_IXUSR) ? 'x' : '-');
    	putchar((mode & S_IRGRP) ? 'r' : '-');
    	putchar((mode & S_IWGRP) ? 'w' : '-');
    	putchar((mode & S_IXGRP) ? 'x' : '-');
    	putchar((mode & S_IROTH) ? 'r' : '-');
    	putchar((mode & S_IWOTH) ? 'w' : '-');
    	putchar((mode & S_IXOTH) ? 'x' : '-');
}


void humanReadableFormat(double size, char* buf)
{
  	const char* units[] = { "", "K", "M", "G", "T" , "P", "E", "Z"};
  	int i = 0;

  	while (size > 1024)
    	{
      		size /= 1024;
      		++i;
    	}
  
  	sprintf(buf, "%.*f%s", i, size, units[i]); 
}


void printTime(time_t mod_time)
{
  	time_t curr_time;
  	time(&curr_time);
  	struct tm* t = localtime(&curr_time);
  	const int curr_mon = t->tm_mon;
  	const int curr_yr = 1970 + t->tm_year;

  	t = localtime(&mod_time);
  	const int mod_mon = t->tm_mon;
  	const int mod_yr = 1970 + t->tm_year;

  	const char* format = ((mod_yr == curr_yr) && (mod_mon >= (curr_mon - 6))) ? "%b %e %H:%M" : "%b %e  %Y";

  	char time_buf[128];
  	strftime(time_buf, sizeof(time_buf), format, t);
  	printf("\t%s", time_buf);
}


bool isDirectory(char *subDir)
{
  	struct stat st = getStats(subDir);

  	if (lstat(subDir, &st) == -1)
    	{
      		printf("./a.out: cannot access \'%s\': No such file or directory\n",subDir);
      		return false;
    	}

 	return (st.st_mode && S_IFDIR) ? true : false ;
}


void printFileNameOrLinkFName(char* subDir, struct Flags flags, mode_t mode)
{
  	if (mode && S_IFLNK)
    	{
      		char symbolicLinkPath[100];
      		int totalBytes;
      		totalBytes = readlink(subDir, symbolicLinkPath, sizeof(symbolicLinkPath));

      		if (totalBytes != -1)    
		{	
	  		symbolicLinkPath[totalBytes] = '\0';

	  		if (flags.flag_Q)
	    		{
	      			printf("\t\"%s\" -> \"%s\" \n", subDir, symbolicLinkPath);
	    		}
	  		else
	    		{
	      			printf("\t%s -> %s \n", subDir, symbolicLinkPath);
	    		}

	  		return ;
		}    
    	}

  	if (flags.flag_Q)
    	{
      		printf("\t\"%s\"", subDir);
    	}	
  	else
    	{
      		printf("\t%s", subDir);
    	}

  	if (flags.flag_p && isDirectory(subDir))
    	{
      		putchar('/');
    	}

  	putchar('\n');
}


void displayStatus(char *mainDir, char *subDir, struct Flags flags)
{
  	if (!isInDirectory(mainDir, subDir))   
    	{
      		return ;
    	}

  	if (!flags.flag_d)
    		globalDir = mainDir;

  	struct stat st = getStats(subDir);

  	if (flags.flag_i)   
    	{
      		printf("%ld\t\t", (long)st.st_ino);
    	}

  	if (flags.flag_s)  
    	{
      		printf("%ld\t", (long)st.st_blocks/2);  
    	}

  	if (flags.flag_Q && !flags.flag_l)   
    	{
      		if(flags.flag_m)    
			printf("\"%s\",\n", subDir);
      		else
			printf("\"%s\"\n", subDir);
      
      		return ;
    	}
  
  	if (flags.flag_m && !flags.flag_l && !flags.flag_d)  
    	{
      		printf("%s,\n", subDir);
      		return ; 
    	}

  	if(flags.flag_l || flags.flag_n || flags.flag_o)   
    	{
      		printFileType(st.st_mode);
      		printPermissions(st.st_mode);
      		printf("\t%ld", st.st_nlink);

      		if (flags.flag_n)   
		{
	  		printf("\t%d", st.st_uid);
	  		printf("\t%d", st.st_gid);
		}
      		else
		{
	  		printf("\t%s", getpwuid(st.st_uid)->pw_name);
	  
	  		if(!flags.flag_o)  
	    		printf("\t%s", getgrgid(st.st_gid)->gr_name);
		}

      		if(flags.flag_h)
		{
	  		char buff[10];
	  		humanReadableFormat(st.st_size, buff);
	  		printf("\t%s", buff);
		}
      		else
		{
	  		printf("\t%ld", (long)st.st_size);
		}

      		printTime(st.st_mtime);
      		printFileNameOrLinkFName(subDir, flags, st.st_mode);
      	}	

  	if (!flags.flag_l && !flags.flag_n && !flags.flag_o)   
    	{
      		printf("%s\n", subDir);
      		return ;
    	}
}


static int compareTime(const void *f1, const void *f2)
{
  	const char *str1 = *(const char **)f1;
  	const char *str2 = *(const char **)f2;

  	time_t t1 = getStats(str1).st_mtime;
  	time_t t2 = getStats(str2).st_mtime;

  	return t1 < t2;
}


static int compareSize(const void *f1, const void *f2)
{
  	const char *str1 = *(const char **)f1;
  	const char *str2 = *(const char **)f2;

  	long int s1 = getStats(str1).st_size;
  	long int s2 = getStats(str2).st_size;

  	return s1 < s2;
}


static int compareAlpha(const void *f1, const void *f2)
{
  	const char *str1 = *(const char **)f1;
  	const char *str2 = *(const char **)f2;

  	return strcasecmp(str1, str2);
}


bool canRecursiveDirectory(char *mainDir, char *subDir)
{
  	if (!strcmp(".", subDir) || !strcmp("..", subDir))
    	{
      		return false;
    	}

  	char nextDir[100];
  	sprintf(nextDir, "%s/%s", mainDir, subDir);
  	struct stat st;

  	if (lstat(nextDir, &st) == -1)
    	{
      		printf("./a.out: cannot access \'%s\': No such file or directory\n",nextDir);
      		exit(EXIT_FAILURE);
    	}

  	return S_ISDIR(st.st_mode);  
}


void displaySubDirectoryRecursively(char *dir, struct Flags flags);

void displayDirectory(char *dir, struct Flags flags)
{
  	DIR *dr = opendir(dir);
  	struct dirent *de = readdir(dr);
  	long int currAllocationAmount = 500;
  	char **dirArray = (char **)malloc(currAllocationAmount * sizeof(char *));

  	if (!dirArray)
    	{
      		printf("Error: Memory error\n");
      		exit(0);
    	}

  	long int count = 0 , i;

  	while(de)
    	{
      		const bool ignoreHiddenFile = (!flags.flag_a && (de->d_name[0] == '.'));

      		if (!ignoreHiddenFile)
		{
	  		if (count >= currAllocationAmount)
	    		{
	      			currAllocationAmount *= 2;
	      			dirArray = realloc(dirArray, currAllocationAmount * sizeof(char *));
	      			if(!dirArray)
				{
		  			printf("Error: Memory error\n");
		  			exit(0);
				}
	    		}

	  		dirArray[count] = de->d_name;
	  		count++;
		}

      		de = readdir(dr);     
    	}

  //globalDir = dir;  

  	if (!flags.flag_U && flags.flag_t)   
    	{
      		qsort(dirArray, count, sizeof(char *), compareTime);
    	}

  	else if (!flags.flag_U && flags.flag_S)   
    	{
      		qsort(dirArray, count, sizeof(char *), compareSize);
    	}

  	else if (!flags.flag_U)    
    	{
      		qsort(dirArray, count, sizeof(char *), compareAlpha);
    	}


  	if (flags.flag_r)
    	{
      		for (i=count-1; i>=0; i--)
		{
	  		displayStatus(dir, dirArray[i], flags);
		}
    	}
  	else
    	{
      		for (i=0; i<count; i++)
		{
	  		displayStatus(dir, dirArray[i], flags);
		}
    	}

  	if (flags.flag_R && !flags.flag_d)   
    	{
      		for(i=0; i<count; i++)
		{
	  		if ( canRecursiveDirectory(dir, dirArray[i]) )
	    		{
	      			char nextDir[100];
	      			sprintf(nextDir, "%s/%s", dir, dirArray[i]);
	      			displaySubDirectoryRecursively(nextDir, flags);
	    		}
		}
    	}
  
  	closedir(dr);
  	free(dirArray);
}



void displaySubDirectoryRecursively(char *dir, struct Flags flags)
{
  	printf("\n%s:\n", dir);
  	displayDirectory(dir, flags);
}


void scanDirectory(int argc, char * argv[], struct Flags flags)
{ 
  	if (flags.flag_d)  
    	{
      		const bool noOtherDirectoryGiven = ((argc - optind) == 0);
      
      		if (noOtherDirectoryGiven)   
		{
	  		displayStatus(".", ".", flags);   
		}

      		else
		{
	  		for (int i=optind; i<argc; i++)  
	    		{
	      			displayStatus(".", argv[i], flags);  
	    		}
		}
      
      		return;
    	}
  
  	if (!flags.flag_R && (optind == argc))   
    	{
      		displayDirectory(".", flags);   
    	}

  	if (flags.flag_R && (optind == argc))
    	{
      		displaySubDirectoryRecursively(".", flags);
      		return ;
    	}
  
	else if (flags.flag_R)
    	{
      		int i;
      		for(i=optind; i<argc; i++)
			displaySubDirectoryRecursively(argv[i], flags);
      		return ; 
    	}

  
  	const bool multipleDirectory = (argc - optind) >= 2;

  	for(int i=optind; i<argc; i++)
    	{
      		if (!isDirectory(argv[i]))    
		{
	  		displayStatus(".", argv[i], flags);
	  		continue ;
		}
      
      		if (multipleDirectory)  
		{
	  		printf("\n%s:\n", argv[i]);
		}

      		if (!isInDirectory(".", argv[i]))    
		{
	  		continue ; 
		}

      		displayDirectory(argv[i], flags);
      		printf("\n");
    	}
}
    
  
void main(int argc, char * argv[])
{
 	scanDirectory(argc, argv, getFlags(argc, argv));
}
