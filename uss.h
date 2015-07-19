


void initialise();
void  encode();
void  send1();
  
typedef struct {

 
  char start;
  char lge;
  char adr;
  char data[96];
  char BCC[1];
  
}  telegram ;


extern telegram t;

extern char  str[100] ;
extern response[10];

extern  int sock1 ,sock2;
extern  struct addrinfo aux;         
extern  struct addrinfo * servinfo , *p;
extern  struct sockaddr_storage their_addr;
extern  socklen_t sin_size;
extern int x;