#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


struct telegram{

 
  char start;
  char lge;
  char adr;
  char data[96];
  char BCC[1];
  
}  t ;

int x;

char  str[100] , response[10];

char calc_BCC(char * s , int n)
{
    int i; 
    char temp ,bcc = 0x00 ;
    
    printf("\n\n----------BCC_CALC-----------\n\n");
    
    for(i=0;i<n;i++)
    {
      printf("\nBCC : %x",bcc);
      temp =  s[i] ;
      bcc  = (temp & ~bcc) | (~temp & bcc) ;
	  
      
    }
  
  printf("\n\n-------------------------------------\n\n");
  return bcc;
  
}
  
  
void state_machine(char * c)
{
    int state=0,test=1;
    int length = (int) c[2] ;
    char bcc = calc_BCC(&c[3],strlen(c) - 4);
    
    
    while(test)
    {
	switch(state)
	{
	  case 0 : 
	    if(c[0] == 0x02)
	      state=1;
	    else
	      test=0;
	    break;
	    
	   
	  case 1:
	    if(c[1] == 0x43)
	      state=2;
	    else
	      test=0;
	  break;
	  
	  case 2:
	 
	    if(length == strlen(c) - 4)
	      state=3;
	    else
	      test=0;
	    break;
	    
	  case 3:
	    if(bcc == c[strlen(c)-1])
	      state=4;
	    else
	      test=0;
	   break;
	}
	
    }
    
    if(state!=4)
    {
      puts("Invalid Packet recieved");
     
    }
	    
    else 
    {
      printf("\n\n------------REC_PACKET-------------\n\n");
      for(test=0;test<strlen(c);test++)
	printf("%x ",c[test]);
   
    }
	    
	    
	    
	    
    }
  
  
  
void initialise()
{
  int i=0;

  t.start = 0x02;
// t.lge = 0x42;
  t.adr = 0x43;
  
  puts("Enter the data that is to be sent (type '0x00' when done): ");
  
  while(1)
  {
    scanf("%x",&t.data[i]);
    if(t.data[i]==0x00)
    {
      break;
    }
    i++;
  }
  
  t.lge=i--;
  
  printf("\n%d\n\n",t.lge);
  
  for(i;i>=0;i--)
  {
    printf(" %X ",t.data[i]);
  }
  
  

  
  
  
  t.BCC[0] = calc_BCC(t.data , t.lge) ;
  
  t.lge+=0x02;
  
  printf("\n\nBCC: %X",t.BCC[0]);
  
}



void encode()
{
  
  strcpy(str,"");
  str[0]=t.start;
  str[1]=t.lge;
  str[2]=t.adr;
  strcat(str,t.data);
  strcat(str,t.BCC);
 
}

  int sock1 ,sock2;
  struct addrinfo aux;         
  struct addrinfo * servinfo , *p;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  
  
  
  
void send1()
{
  
  aux.ai_family = AF_UNSPEC;
  aux.ai_socktype = SOCK_STREAM;
  aux.ai_flags = AI_PASSIVE;

  getaddrinfo( NULL , "23" , &aux , &servinfo);
  
  for(p = servinfo ; p!=NULL ; p=p->ai_next)
  {
    if ((sock1 = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
    {
     perror("server: socket");
     continue;
    }

    if (bind(sock1, p->ai_addr, p->ai_addrlen) == -1) 
    {
    close(sock1);
    perror("server: bind");
    continue;
    }
    
    break;
  }
  
  
  if(p==NULL)
  {
    puts("ERROR");
   
  }
  
  printf("\nLISTENING.........\n\n");
  
  listen(sock1,31);
  
 
  while(1)
  {

    sin_size = sizeof(struct sockaddr_storage);
    sock2 = accept(sock1,(struct sockaddr *)&their_addr, &sin_size);
    printf("Socket values : %d , %d\n\n",sock1,sock2);
    
    printf("Length : %c , %d\n\n",t.lge,t.lge);
    
   
    
    if (!fork()) { 
	close(sock1);
	
	
	
	while (x=send(sock2,str,sizeof(str), 0) != -1)
	{
	  
	
	if(recv(sock2, response , 10 , 0)==-1)
	  perror("recv");

else{
	  
	
	state_machine(response);
	
	}
	}
	
	if(x==-1)
	  perror("send");
	
	close(sock2);
        exit(0);
}
  }
  
}



  

    
    
    
    
  
  
 
