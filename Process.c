#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//Global variables
int nSon;
int nSonAsked;
pid_t pid;
int Rnumber;
int pipefd[2]; //parent writing and child reading
int pipefd1[2];//child writing and parent reading
char instring[50];
int processid;


// create a structure
struct process_info{
int processID;
int data;
};

//To create structure array pointer 
struct process_info *info = NULL;

// Random number generator function
int GenerateRandom(){
srand(getpid());
int random_number = rand() % 100 + 1;
return random_number;
}

void sendbid(){
  Rnumber = GenerateRandom(); 
  //close read end of pipefd1 to write to the parent
  //close(pipefd1[0]);
  processid = getpid();
  write(pipefd1[1],&processid,sizeof(processid));
  write(pipefd1[1],&Rnumber,sizeof(Rnumber));
}

// To send auction message from parent process to the child
void StartAuction(){
// close read end of pipefd to write to the child
//close(pipefd[0]); 
write(pipefd[1], "Start Auction!",14);
}

//Function to compare random no.s recived to the parent process
int compareData()
{
	int i;
	int location;
	int greatest = info[0].data;
	for(i=0;i<nSonAsked;i++){
		if(info[i].data > greatest){
		if(info[i].data == greatest){
			return -1;
		}
		greatest = info[i].data;
		location = i;
		}
	}
	return location;

}
int main(void)
{	
	//To create unidirectional pipes
	if (pipe(pipefd) == -1) {
	perror("pipe");
	exit(EXIT_FAILURE);
	}	
	//to make pipes bidirectional using two unidirectional pipes
	if (pipe(pipefd1) == -1) {
	perror("pipe");
	exit(EXIT_FAILURE);
	}
	int counter = 0;
	printf("PID of manager process is %d\n",getpid());
	printf("Please input No.of processes to create\n");
	scanf("%d",&nSonAsked);
	info = malloc(sizeof(struct process_info) * nSonAsked);
	if(nSonAsked<3){
	printf("Please enter atleast 3 processes\n");
	exit(0);
	}
	for (nSon=0;nSon<nSonAsked;nSon++){
	pid= fork();
	if(pid==0){
	//close write end of pipefd to read from the parent
	//close(pipefd[1]);
	read(pipefd[0],instring,14);

	printf("My PID is %d\n",getpid());
 	//printf("%s\n",instring);
		if(strcmp(instring,"Start Auction!")==0){
			sendbid();
		}
		
	exit(0);
	}
	// To check parent process and start auction
	else if(pid > 0){
	StartAuction();
	read(pipefd1[0],&processid,sizeof(processid));
	read(pipefd1[0],&Rnumber,sizeof(Rnumber));
	counter++;
	info[nSon].processID= processid;
  	info[nSon].data= Rnumber;
	wait(NULL);
		if(counter==nSonAsked){
		int index = compareData();
		int j;
		int k;
		int winner = info[index].processID ;
		// To display bids received from child process
		for (j=0;j<nSonAsked;j++){
		printf("%d th bidder is %d\n",j+1,info[j].data);
		}
		// To display winner process
		printf("%d is Winner \n", info[index].processID);
			// To kill looser processes
			for (k=0;k<nSonAsked;k++){
				if(info[k].processID!=info[winner].processID){
				kill(info[k].processID,SIGKILL);
			     }
			}
		// To kill winner process
                kill(winner,SIGKILL);
		}
		
	}
	else if(pid == -1){
	 perror ("fork");
	 exit(0);
	}
     }      
	return 0;
}

