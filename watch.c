#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>

/**
 * Simple cmdline watcher
 * 
 * Useful to watch all cmdline (argv 0) for all running/new processes.
 * 
 * Notes:	
 * 	- Won't check very quick running processes
 * 	- The is pretty bad
 * 	- Prints results to stdout
**/

#define MAX_PROC_ID 1000000
#define USLEEP_TIME 5000

int main(){
	char current_file_name[BUFSIZ];
	char already_logged [MAX_PROC_ID] = {0};
	char buf[BUFSIZ];

	DIR* proc_dir;
	struct dirent *dir_e;
	int curr_e_fp;

	while(1){
		proc_dir = opendir("/proc");
		if(!proc_dir)
			abort();

		usleep(USLEEP_TIME);
		while((dir_e = readdir(proc_dir)) != NULL){
			char* d_name = dir_e->d_name;
			
			// If not a digit (not a process folder)
			if(!isdigit(*d_name))
				continue;

			int num = atoi(d_name);

			if(num>MAX_PROC_ID){
				puts("PID larger than MAX_PROC_ID");
				abort();
			}

			if(already_logged[num])
				continue;
			already_logged[num] = 1;
			
			snprintf(current_file_name, sizeof(current_file_name), "%s%s%s", "/proc/", d_name, "/cmdline");

			curr_e_fp = open(current_file_name, O_RDONLY);
			
			int ra = read(curr_e_fp, buf, BUFSIZ-1);

			close(curr_e_fp);
			
			for(int i = 0; i<ra-1; i++)
				if(buf[i] == '\0') buf[i] = ' ';

			// guaranteed to be in-bounds
			buf[ra-1] = '\n';

			write(1, buf, ra);
		}
		
		closedir(proc_dir);
	}

}
