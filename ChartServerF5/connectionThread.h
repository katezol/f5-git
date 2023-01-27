#ifndef CONNECTIONTHREAD_H_
#define CONNECTIONTHREAD_H_

//#define chartConnectionCfgPath	"/home/furke5/chartConnection.cfg"
#define killFSCIFS system("slay -f fs-nfs3")

struct connection{
	int _chanal;
	char mnt[2][256];
};

extern struct connection connection;
char chartConnectionCfgPath[50];


int read_ConnectionCfg(void);
void* connectionThread(void* data);

#endif /*CONNECTIONTHREAD_H_*/
