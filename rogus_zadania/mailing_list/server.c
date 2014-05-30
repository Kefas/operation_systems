#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>




char** topic_list;
int topic_size=0;
int** message_list;
int* message_size=0;
struct message* users = NULL;
int users_size=0;


struct message{
  int topics[100];
  int size;
  pid_t pid;
};

struct message2{
  int is_topic;
  int topic;
  int subtopic[100];
  int size;
  int topic_size;
};

int addUser(struct message msg){
  int i,j;
  for(i=0;i<users_size;i++)
    if(users[i].pid == msg.pid && msg.size == 0){
      printf("uzytkownik jest w bazie danych\n");
      return 1;
    }else if(users[i].pid == msg.pid){
      printf("Uzytkownik jest w bazie danych ale zmienia subskrypcje\n");
      users[i].size = 0;
      for(j=0;j<msg.size;j++){
	users[i].topics[j] = msg.topics[j];
	users[i].size++;
      }
      return 3;
    }
  printf("nowy uzytkownik\n");
 
  
  users = (struct message*) realloc(users, (users_size+1) * sizeof(struct message));
  users[users_size].pid = msg.pid;
  for(i=0;i<msg.size;i++){
    printf("co to\n");
    users[users_size].topics[i] = msg.topics[i];
    users[users_size].size++;
  }
  users_size++;
  return 2;
}

void makeTopics(int n){
  int i,j;
  char buf[5];
  int random;
  char* result;
  char* strFrom;
  topic_list = (char**) malloc ( n * sizeof(char*));
  message_list  = (int **) malloc (n * sizeof(int*));
  message_size = (int*) malloc (n* sizeof(int));
  topic_size = n;
  for(i=0;i<n;i++){
    sprintf(buf, "%d",n);
    strFrom = (char*) malloc(7*sizeof(char));
    strcpy(strFrom, "Topic ");
    strcat(strFrom, buf);
    
    topic_list[i] = (char*) malloc ( sizeof(strFrom) * sizeof(char));
    strcpy(topic_list[i], strFrom);
    random = rand()%10+5;
    message_size[i] = random;
    message_list[i] = (int*) malloc( random * sizeof(int));
    for(j=0;j<random;j++)
      message_list[i][j] = j;  
    free(strFrom);
  }
}

int main(void){

  int n;
  int fdsrv, fdsrv_w,flags,bread,i;
  int fdcnt, bwrite, j;
  int n_user;
  struct message msg;
  struct message2 msg2;
  char str[5];
  srand(time(NULL));
  printf("ile tematow?\n");
  scanf("%d", &n);

  makeTopics(n);
  printf("Works\n");

  if((mkfifo("fifo1", S_IFIFO|0755)) == -1 ){
    perror("Tworzenie fifo1");
    exit(EXIT_FAILURE);
  }

  if((fdsrv = open("fifo1", O_RDONLY)) == -1){
    perror("open fail");
    exit(EXIT_FAILURE);
  }

    
  while(1){
    if((bread = read(fdsrv, &msg, sizeof(msg))) == -1){
      perror("czytanie z kolejki");
      /*break;*/
    }
    if(bread > 0){
      printf("Message received %d\n", bread);
    
      printf("From %d:\n", msg.pid);
      
      printf("Msg size %d\n",msg.size);
      for(i=0;i<msg.size;i++){
	printf("Topic %d\n", msg.topics[i] );
      }
    
      sprintf(str, "%d", msg.pid);
      
      if((mkfifo(str, S_IFIFO|0755)) == -1){
	perror("Tworzenie fifo dla klienta");
	/*exit(EXIT_FAILURE);*/
      }

      if((fdcnt = open(str, O_WRONLY)) == -1){
	perror("fdcnt open fail");
	exit(EXIT_FAILURE);
      }
      for(i=0;i<100;i++)
	msg2.subtopic[i] = 0;
      msg2.size = 0;

      if(addUser(msg) != 0){
	printf("users_size %d\n", users_size);
	for(i=0;i<users_size;i++){
	  if(users[i].pid == msg.pid){
	    printf("\n");
	    n_user= i;
	    i = users_size;
	  }
	}
	
	for(i=0;i<users[n_user].size;i++){
	  printf("w forze\n");
	  if( users[n_user].topics[i] < topic_size){
	    msg2.is_topic = 1;
	    msg2.topic = users[n_user].topics[i];
	    msg2.size =0;
	    msg2.topic_size = users[n_user].size;
	    printf("Message size[i] %d\n",message_size[i]);
	    for(j=0;j<message_size[i];j++){
	      msg2.subtopic[j] = message_list[i][j];
	      msg2.size++;
	    }
	    printf("send\n");
	    if((bwrite = write(fdcnt, &msg2, sizeof(msg2))) == -1){
	      perror("write");
	      exit(EXIT_FAILURE);
	    }
	    printf("sended: %d", bwrite);
	  }
	  else{
	    msg2.is_topic = 0;
	    msg2.topic = users[n_user].topics[i];
	    bwrite = write(fdcnt, &msg2, sizeof(msg2));
	  }
	}
      }
            
      
      
    }
    
    /*write(fdcnt,0,0);*/
    unlink(fdcnt);
   
    
    
  }

  close(fdsrv);
  unlink("fifo1");

  return 0;

}
