#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>




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

void newTopic(int n, struct message* msg){
  int i;
  for(i=0;i<msg->size;i++)
    if( msg->topics[i] == n ){
      printf("Sorry ale juz subskrybujesz ten temat\n");
      return ;
    }
  printf("Dodaje subskrypcje\n"); 
  msg->topics[msg->size] = n;
  msg->size++;

}

void deleteTopic(int n, struct message* msg){
  int i,j;
  
  for(i=0;i<msg->size;i++)
    if( msg->topics[i] == n ){
      printf("Usuwam subskrypcje\n");
      for(j=i;j<msg->size-1;j++)
	msg->topics[j] = msg->topics[j+1];
      msg->size--;
      return ;
    }
  printf("Nie ma takiego tematu\n");

}

void sendData(int fdsrv, struct message msg){
  int bwrite;
  if((bwrite = write(fdsrv, &msg, sizeof(msg))) == -1 ){
    perror("write fail");
    exit(EXIT_FAILURE);
  }


}

void receiveData(char* fifo_name, struct message* msg){
  int fdcnt;
  int bread;
  int i,j;
  int start;
  struct message2 msg2;
  
  if((mkfifo(fifo_name, S_IFIFO|0755)) == -1){
    perror("Tworzenie fifo dla klienta");
    exit(EXIT_FAILURE);
  }

  if((fdcnt = open(fifo_name, O_RDONLY)) == -1){
    perror("receiveData open fail");
    exit(EXIT_FAILURE);
  }
  
  if(msg->size == 0)
    start = 1;
  else
    start = msg->size;
  
  
  
  for(j=0;j<start;j++){
    bread = read(fdcnt, &msg2, sizeof(msg2));
    if(msg->size == 0)
      start = msg2.topic_size;
    msg->topics[j] = msg2.topic;
    if(msg2.is_topic == 1){ 
      printf("Temat nr: %d\n", msg2.topic);
      
      for(i=0;i<msg2.size;i++)
	printf("  Wiadomosc: %d\n", msg2.subtopic[i]);
    }
    else
      printf("Topic %d is not valid topic\n", msg2.topic);
  }

  msg->size = start;
  
  unlink(fifo_name);

}


int main(int argc, char* argv[]){
  struct message msg;
  int random, i, fdsrv;
  char c;
  int choice;

  if(argc != 2){
    printf("Usage: ./client client_number\n");
    return 1;
  }


  srand(time(NULL));
  
  msg.size = 0;
  msg.pid = atoi(argv[1]);
 

  /* open server fifo for writing*/
  
  if((fdsrv = open("fifo1", O_WRONLY)) == -1){
    perror("Otwieranie fifo1");
    exit(EXIT_FAILURE);
  }
  printf("fifo otwarta\n");
  
  while(c != 'q'){
   
    printf("d - chce nowy temat\nu - chce sie usunac\np - pobierz wiadomosci z tematow\n");
    c = getchar();
    switch(c){
    case 'd':
      printf("Podaj numer tematu ktory chcesz subskrybowac\n");
      scanf("%d",&choice);
      newTopic(choice, &msg);
      break; 
    case 'u':
      printf("Podaj numer tematu ktory chcesz przestac subskrybowac\n");
      scanf("%d",&choice);
      deleteTopic(choice, &msg);
      break;
    case 'p':
      printf("Size = %d\n", msg.size);
      sendData(fdsrv, msg);
      receiveData(argv[1],&msg);
      break;
    }
    
    if(c!='q'){
      c=getchar();
      printf("nacisnij klawisz aby kontynowac\n");
      c = getchar();
      system("clear");
    }
  }
  close(fdsrv);
  unlink(argv[2]);
  
  
  return 0;
}
