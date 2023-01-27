#include "ping.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>


#include <hw/inout.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <hw/pci.h>
#include <hw/inout.h>

#define SMB_VID 	0x8086
#define SMB_DID		0x24C3
#define SMB_DFUNC	0x3
#define DEV_LEN		0x20
#define NANO		sleep(1)

#define BUFSIZE 1500

#define osnKksPid 321
#define resKksPid 123

unsigned char data[7];
int logic, i;

char connectKKSbuf[2]={0,0};

char ipKKSosn[100] = "17.0.1.107"; // katetemp
char ipKKSres[100] = "16.0.1.106";

time_t osnTime;
time_t resTime;

int sd;						// Дискриптор сокета
struct sockaddr_in srvKksOsn; 	// Основной канал ККС
struct sockaddr_in srvKksRes;	// Резервный канал ККС
struct sockaddr_in from;     /* структура sockaddr() для получения пакета */

/* прототипы функций */
void pinger(int, struct sockaddr_in*);
void output(char *, int, struct timeval *);
unsigned short in_cksum(unsigned short *, int);
void sendKKSstate(void* data2);

int i2c_connect(uintptr_t *_logical)
{
	void *pci_id;
	struct pci_dev_info dev_info;
	int pci_hndl, ret;
	int io_addr;
	uint64_t addr2; 
	pci_hndl = pci_attach(0); // присоеденение к серверу PCI
	if (pci_hndl == -1)
	{
		return EXIT_FAILURE;
	}
	
	memset(&dev_info, 0, sizeof(dev_info));
	dev_info.VendorId = SMB_VID;
	dev_info.DeviceId = SMB_DID;
	dev_info.DevFunc = SMB_DFUNC;
	
	pci_id = pci_attach_device (NULL, PCI_INIT_ALL|PCI_INIT_ROM , 0, &dev_info);
	if (pci_id == NULL)
	{
		pci_detach(pci_hndl);
		return EXIT_FAILURE;
	}
	pci_read_config32(dev_info.BusNumber, dev_info.DevFunc, 0x20, 1, &io_addr);
	addr2 = io_addr & 0xfffffffe; 
	ret = mmap_device_io(DEV_LEN, addr2);
	*_logical = ret;
	return 0;
}

int ask83(int logic, int adr)
{
	int ans;
	out8(logic + 0, 0xFE);

	out8(logic + 4, 0x32);
	out8(logic + 3, adr);

	out8(logic + 2, 0x44);
	while (!(in8(logic + 0) & 0x02)) i++;
	
	out8(logic + 0, 0xFE);
	out8(logic + 4, 0x33);
	out8(logic + 2, 0x44);
	while (!(in8(logic + 0) & 0x02)) i++;
	
	ans = in8(logic + 5);//*/
	return (ans);
//	return 0;
	
}

int ask75_2(int logic)
{
	out8(logic + 0, 0xFE);
	
	out8(logic + 4, 0x9D);
	out8(logic + 3, 0x00);
	out8(logic + 2, 0x44);
	while (in8(logic + 0) & 0x01);

	return (in8(logic + 5));
}	

int ask75_1(int logic)
{
	int ans, d;
	out8(logic + 5, 0);
	
	out8(logic + 0, 0xFE);
	
	out8(logic + 4, 0x9F);
	out8(logic + 3, 0x00);
	out8(logic + 2, 0x44);
	while (!(in8(logic + 0) & 0x02)) i++;
	ans = in8(logic + 5);
//	while (in8(logic + 0) & 0x01);
//	NANO;
	return (in8(logic + 5));
}	

void* pingLoop(void* data){
	time_t timeNow;
	int size;
	int fromlen;
	int n;
	struct timeval tval;
	char recvbuf[BUFSIZE];
	struct hostent *hpOsn;
	struct hostent *hpRes;

	const int on = 1;
	
	ThreadCtl(_NTO_TCTL_IO, 0);
	i2c_connect(&logic);

	if ( (hpOsn = gethostbyname(ipKKSosn)) == NULL) {
		herror("gethostbyname() failed");
		exit(-1);
	}
	
	bzero(&srvKksOsn, sizeof(srvKksOsn));
	
	srvKksOsn.sin_family = AF_INET;
	srvKksOsn.sin_addr= *((struct in_addr *) hpOsn->h_addr);
	if ( (hpRes = gethostbyname(ipKKSres)) == NULL) {
		printf("%s\n",ipKKSres);
		herror("gethostbyname() failed");
		exit(-1);
	}
	bzero(&srvKksRes, sizeof(srvKksRes));
	srvKksRes.sin_family = AF_INET;
	srvKksRes.sin_addr= *((struct in_addr *) hpRes->h_addr);
	
	if ( (sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		perror("socket() failed");
		exit(-1);
	}
	/* восстановление исходных прав */
	//setuid(getuid());
	/* даем возможность посылать широковещательные сообщения */
	setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	/* увеличиваем размер приемного буфера */
	size = 60 *1024;
	setsockopt(sd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	


	fromlen = sizeof(from);
 
	osnTime = 0;
	resTime = 0;
  
	while (1) {
		timeNow = time(NULL);
		pinger(osnKksPid, &srvKksOsn);
		pinger(resKksPid, &srvKksRes);
		delay(600);
		if(timeNow-osnTime>2)
			connectKKSbuf[0] = 0;
		else 
			connectKKSbuf[0] = 1;
		if(timeNow-resTime>2)
			connectKKSbuf[1] = 0;
		else 
			connectKKSbuf[1] = 1;
		//MSG_DONTWAIT
		bzero(&from, sizeof(struct sockaddr));
		n = recvfrom(sd, recvbuf, sizeof(recvbuf), MSG_DONTWAIT, 
				(struct sockaddr *)&from, &fromlen);
		output(recvbuf, n, &tval);
		bzero(&from, sizeof(struct sockaddr));
		n = recvfrom(sd, recvbuf, sizeof(recvbuf), MSG_DONTWAIT , 
					(struct sockaddr *)&from, &fromlen);
		output(recvbuf, n, &tval);
		// sendKKSstate(); на отправка у нас поток
		
	}
	return 0;
}

/*-------------------------------------------------*/
/* Формирование и отсылка ICMP ECHO REQUEST пакета */
/*-------------------------------------------------*/
void pinger(int id, struct sockaddr_in* sockAddr )
{
  int icmplen;
  struct icmp *icmp;
  char sendbuf[BUFSIZE];

  icmp = (struct icmp *) sendbuf;
  /* заполняем все поля ICMP-сообщения */
  icmp->icmp_type = ICMP_ECHO;
  icmp->icmp_code = 0;
  icmp->icmp_id = id;
  icmp->icmp_seq = 1;
  gettimeofday((struct timeval *) icmp->icmp_data, NULL);
  /* длина ICMP-заголовка составляет 8 байт и 56 байт данных */
  icmplen = 8 + 56;
  /* контрольная сумма ICMP-заголовка и данных */
  icmp->icmp_cksum = 0;
  icmp->icmp_cksum = in_cksum((unsigned short *) icmp, icmplen);

  sendto(sd, sendbuf, icmplen, 0, 
    (struct sockaddr *)sockAddr, sizeof(srvKksOsn));
}

void output(char *ptr, int len, struct timeval *tvrecv)
{
  int iplen;
  struct ip *ip;
  struct icmp *icmp;
  ip = (struct ip *) ptr; /* начало IP-заголовка */
  iplen = ip->ip_hl << 2; /* длина IP-заголовка */

  //printf("ping addr: %s\n",inet_ntoa(from.sin_addr));
  icmp = (struct icmp *) (ptr + iplen); /* начало ICMP-заголовка */
  if (icmp->icmp_type == ICMP_ECHOREPLY) {
	  if(!strcmp( ipKKSosn, inet_ntoa(from.sin_addr))){		 // Пинг от основоного канала ККС
		//  printf("PING osn kanal\n");
		  osnTime = time(NULL);
	  }else  if(!strcmp( ipKKSres, inet_ntoa(from.sin_addr))){ // Пинг от резервного канала ККС
		//  printf("PING res kanal\n");
		  resTime = time(NULL);
	  }
  }
}

unsigned short in_cksum(unsigned short *addr, int len){
  unsigned short result;
  unsigned int sum = 0;
  while (len > 1) {
    sum += *addr++;
    len -= 2;
  }
   /* если остался лишний байт, прибавляем его к сумме */
  if (len == 1)
    sum += *(unsigned char*) addr;
  sum = (sum >> 16) + (sum & 0xFFFF); /* добавляем перенос */
  sum += (sum >> 16);                 /* еще раз */
  result = ~sum;                      /* инвертируем результат */
  return result;
}

// Еще температура процессора =)
void sendKKSstate(void* data2){
	int fd =-1;
	int n;
	char buff[60];
	while (1)
	{	
		delay (2000);
		//data[0] = ask83(logic,0x30);	// нам не надо мерить темпервтуру этого проца 
		if (fd == -1)
		{	
			fd = open(port_KKSstate, O_WRONLY );
			continue;
		}	
		buff[0] = connectKKSbuf[0];
		buff[1] = connectKKSbuf[1];
		buff[2] = 0; 
		//printf("%d %d %d\n", buff[0], buff[1], buff[2]);
		n = write(fd, &buff[0], 3);
		if (n!=3) {delay (500); continue;}
		printf("Send KKSState %d %d %d\n",
			buff[0],buff[1],buff[2]);
	}	
	close(fd);
	return NULL;
}
