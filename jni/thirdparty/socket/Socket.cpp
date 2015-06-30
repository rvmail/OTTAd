///////////////////////////////////////////////////////////////////////
// socket.cpp:  socket class realization
// date:        2004/01/01
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
//Used by time and timeval
#include <time.h>
#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <signal.h>
void Sleep(int msec)
{
	usleep(msec* 1000);
}
#endif

#include "Socket.h"


#ifdef WIN32
#pragma comment(lib,"Ws2_32.lib")
bool g_bInitialized = false;
#endif

#ifdef _test_connection
	FILE  *gfpSocketTest;
#endif

void CSocket::InitializeSock()
{
#ifdef WIN32
	if (!g_bInitialized) {
		WORD ver; WSADATA wsa;
		ver = MAKEWORD( 2, 2 );
		int ret = WSAStartup(ver, &wsa);
		g_bInitialized = true;
	}
#endif
}

//Initialization.
CSocket::CSocket()
{
	m_bConnected = false;
	m_bSelected  = false;
	m_hSocket = 0;
	m_szLastErr[0] = '\0';
    m_nConnectFlag = 0;
	memset(&m_sin,0x00,sizeof(SOCKADDR_IN));

    InitializeSock();
}
CSocket::CSocket(SOCKET con_id)
{
    m_bConnected = true;
    m_bSelected  = false;
    m_hSocket = con_id;
    m_szLastErr[0] = '\0';
    m_nConnectFlag = 0;
}


//Deconstruction.
CSocket::~CSocket()
{
	//Close socket before destroying it
	if (IsConnected()) Close();
}

void CSocket::initaddr(int nSockPort, const char* pszSockAddr)
{
	m_sin.sin_family = AF_INET;
	if (nSockPort != 0)
		m_sin.sin_port = htons((unsigned short)nSockPort);
	if (pszSockAddr != NULL)
	{
		if(pszSockAddr[0]>='0' && pszSockAddr[0]<='9')
		{
			m_sin.sin_addr.s_addr = inet_addr(pszSockAddr);
		}
		else
		{
			//如果是使用名字的
			struct hostent *hp;
			hp = gethostbyname(pszSockAddr);
			if (hp == 0) {
				printf( "%s: unknow host!\n", pszSockAddr );
				return;
			}
			memcpy((char*)&m_sin.sin_addr, (char*)hp->h_addr, hp->h_length);
		}
	}
}

//Create a socket.
bool CSocket::Create(int nSockPort, const char* pszSockAddr, int nType)
{
	initaddr(nSockPort, pszSockAddr);
	if (nType == SOCKET_TYPE_TCP)
		m_nSocketType = SOCK_STREAM;
	else
		m_nSocketType = SOCK_DGRAM;

	m_hSocket = socket(AF_INET, m_nSocketType, 0);

#ifndef WIN32
    signal(SIGPIPE,SIG_IGN);    //忽略SIGPIPE信号
#endif

	if (-1 == m_hSocket)
	{
		sprintf(m_szLastErr, "Failed to create a socket");
		return false;
	}

	int on=0,status;
	status = setsockopt(m_hSocket,SOL_SOCKET,SO_REUSEADDR,(const char *) &on, sizeof(on));
	if (-1 == status)
	{
		sprintf(m_szLastErr, "%s:%d 警告:设置重用属性失败!(Port=%s:%d)\n", 
			__FILE__, __LINE__, pszSockAddr, nSockPort);
		Close();
		return false;
	} 

	return true;
}

//Close the socket.
void CSocket::Close()
{
	if(m_bConnected) closesocket(m_hSocket);
	//m_hSocket = 0;
    //这里不能设置为0，否则回头会无法取到该数值,后续还要用GetSock()
	m_bConnected = false;
}

bool CSocket::ShutDown()
{
	if(m_hSocket) 
	{
		int nRet = shutdown(m_hSocket, 2);

		if (nRet != 0)
			return false;
	}
	return true;	
}

bool CSocket::Bind(int nSockPort, const char* pszSockAddr)
{
	initaddr(nSockPort, pszSockAddr);
	int ret = bind(m_hSocket, (SOCKADDR *)(void *)&m_sin, \
		(socklen_t)sizeof(SOCKADDR_IN));
	if (ret != 0)
	{
		sprintf(m_szLastErr, "Cann't bind on %s:%d", pszSockAddr, nSockPort);
		return false;
	}
    m_bConnected = true;
	//////////////////////////////////////////////////////////////////////////
	//设置发送和接收缓冲
	if (m_nSocketType == SOCKET_TYPE_UDP)
	{
		int rcvbuf = 0; 
		int rcvbufsize=sizeof(int); 

		if(getsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*) 
			&rcvbuf,(socklen_t *)&rcvbufsize)!=-1) 
		{ 
			if(rcvbuf<65536) 
				rcvbuf=128 * 1024; //128K
			setsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*) 
				&rcvbuf,(socklen_t )rcvbufsize); 
		} 

		if(getsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*) 
			&rcvbuf,(socklen_t *)&rcvbufsize)!=-1) 
		{ 
			if(rcvbuf<65536) 
				rcvbuf=128 * 1024; //128K
			setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*) 
				&rcvbuf,(socklen_t )rcvbufsize); 
		}
	}
	//////////////////////////////////////////////////////////////////////////

	return true;
}

//Listen on certaion address and port
bool CSocket::Listen(int nConnectionBacklog)
{
    m_nConnectFlag = 1;
	if(listen(m_hSocket, nConnectionBacklog) == -1)
	{
		sprintf(m_szLastErr, "Listening on %d failed", m_hSocket);
		return false;
	}
	return true;
}

//Connect to a listenning socket
bool CSocket::Connect(int nHostPort, const char* pszHostAddr, int nTimeout)
{
	initaddr(nHostPort, pszHostAddr);
#ifndef WIN32
    signal(SIGPIPE,SIG_IGN);    //忽略SIGPIPE信号
#endif
    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_hSocket == -1)
	{
		printf("Connect socket failed-----\n");
		return (m_bConnected = false);
	}
    int on=0,status;
    unsigned long ul = 1;

    SetBlockMode(true);
    //以非阻塞式连接
	if(connect(m_hSocket, (SOCKADDR *)(void *)&m_sin, (socklen_t)(sizeof(SOCKADDR_IN))) == -1)
	{
        struct timeval timeout ;
        fd_set r;

        FD_ZERO(&r);
        FD_SET(m_hSocket, &r);
        timeout.tv_sec = nTimeout; //连接超时1秒
        timeout.tv_usec =0;
        //ret = select(0, 0, &r, 0, &timeout);
        if ( select(m_hSocket+1, 0, &r, 0, &timeout) <= 0 )
        {
            SetBlockMode(false);
            //ioctlsocket(m_hSocket, FIONBIO, (unsigned long*)&ul);
            ::closesocket(m_hSocket);
            m_bConnected = false;
            sprintf(m_szLastErr, "Connecting to %s:%d failed", \
                pszHostAddr, nHostPort);
            return 0;
        }
        
        m_bConnected = true;
	}
    else
    {
        m_bConnected = true;
    }

    //重新变成阻塞模式
    //ioctlsocket(m_hSocket, FIONBIO, (unsigned long*)&ul);
    SetBlockMode(false);

	int rcvbuf = 0; 
	int rcvbufsize=sizeof(int); 

	if(getsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*) 
		&rcvbuf,(socklen_t *)&rcvbufsize)!=-1) 
	{ 
		if(rcvbuf<65536) 
			rcvbuf=65536; 
		setsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*) 
			&rcvbuf,(socklen_t )rcvbufsize); 
	} 

	if(getsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*) 
		&rcvbuf,(socklen_t *)&rcvbufsize)!=-1) 
	{ 
		if(rcvbuf<65536) 
			rcvbuf=65536; 
		setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*) 
			&rcvbuf,(socklen_t )rcvbufsize); 
	}


	return m_bConnected;
}

//Accept a connection
bool CSocket::Accept(CSocket* rConnectedSock, SOCKADDR* pAddr, int* pAddrLen)
{
	//In HP 
#ifdef _XOPEN_SOURCE_EXTENDED
    	socklen_t uLen = sizeof(SOCKADDR_IN);
#else
    	int uLen = sizeof(SOCKADDR_IN);
#endif
	rConnectedSock->m_hSocket = accept(m_hSocket, \
		(SOCKADDR *)(void *)&(rConnectedSock->m_sin), &uLen);
	if (rConnectedSock->m_hSocket<=0)
	{
		sprintf(m_szLastErr, "Failed to accept a connection on %d and errno=%d error=%s ", m_hSocket,errno,strerror(errno));
		return false;
	}
	int rcvbuf = 0; 
	int rcvbufsize=sizeof(int); 

	if(getsockopt(rConnectedSock->m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*) 
		&rcvbuf,(socklen_t *)&rcvbufsize)!=-1) 
	{ 
		if(rcvbuf<65536) 
			rcvbuf=65536; 
		setsockopt(rConnectedSock->m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*) 
			&rcvbuf,(socklen_t )rcvbufsize); 
	} 

	if(getsockopt(rConnectedSock->m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*) 
		&rcvbuf,(socklen_t *)&rcvbufsize)!=-1) 
	{ 
		if(rcvbuf<65536) 
			rcvbuf=65536; 
		setsockopt(rConnectedSock->m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*) 
			&rcvbuf,(socklen_t )rcvbufsize); 
	}

	rConnectedSock->m_bConnected = true;
	return true;
}

//It hasn't been implemented!
bool CSocket::Accept(CSocket& rConnectedSock, SOCKADDR* pAddr, int* pAddrLen)
{
	return Accept(&rConnectedSock, pAddr, pAddrLen);
}

bool CSocket::GetSockOpt(int nOptName, void* lpOptValue, int* lpOptLen, int nLevel)
{
	//TODO:
	return true;
}

bool CSocket::SetSockOpt(int nOptName, const void* lpOptValue, int lpOptLen, int nLevel)
{
	//TODO:
	return true;
}

//Read data
int CSocket::Read(void* lpBuf, int nBufLen, int nFlags)
{
	return Recv(lpBuf, nBufLen, nFlags);
}

//Recv data
int CSocket::Recv(void* lpBuf, int nBufLen, int nFlags)
{
	int len = 1, tot = 0;
#ifdef _test_connection
	char szMsg[100];
	strcpy(szMsg,"CSocket:going to recv\n");
	fwrite(szMsg,1,strlen(szMsg),gfpSocketTest);
	fflush(gfpSocketTest);
#endif
	if (nFlags & MSG_EXPECTED)
	{
		while (tot < nBufLen && len > 0)
		{
		#ifdef WIN32
			len = recv(m_hSocket, ((char *)lpBuf) + tot, nBufLen - tot, 0);
		#else
			len = recv(m_hSocket, (void *)(((char *)lpBuf) + tot), nBufLen - tot, 0);
		#endif
			if (len > 0) tot += len;
		}

		//Win32测试结果，当对方非正常关闭，返回值可能为-1或0
		if (len <= 0 )
		{
			//printf( "CSocket:recievied with len < 0\n" );
			Close();
			return tot;
		}

		if (tot > 0 && tot < nBufLen)
		{
			sprintf(m_szLastErr, "Expected %d bytes, while received %d bytes", \
				nBufLen, tot);
			printf( "%s\n", m_szLastErr );
#ifdef _test_connection	
			 
			fwrite(m_szLastErr,1,strlen(m_szLastErr),gfpSocketTest);
#endif
			return -1;
		}
#ifdef _test_connection	
		strcpy(szMsg,"CSocket:recv message is lpBuf=begin<<\n");
		fwrite(szMsg,1,strlen(szMsg),gfpSocketTest);
		fwrite(lpBuf,1,nBufLen+10,gfpSocketTest);
		sprintf(szMsg,"\n>>lpBuf=end,  len(lpBuf)=%d finish recv\n",tot);
		fwrite(szMsg,1,strlen(szMsg),gfpSocketTest);
		fflush(gfpSocketTest);
#endif	 
		return tot;
	}
	else
	{
	#ifdef WIN32
		len = recv(m_hSocket, (char *)lpBuf, nBufLen, 0);
	#else
		len = recv(m_hSocket, lpBuf, nBufLen, 0);
	#endif
 
		if (len == -1)
			sprintf(m_szLastErr, "Recving data failed");
		if (len == 0) Close();
		if(len <= 0)
		{
			printf( "Recving data failed!\n" );
		}

		return len;
	}
}

//Send data
int CSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	int len;
#ifdef _test_connection	
	char szMsg[200];
	strcpy(szMsg,"CSocket:going to send message is lpBuf=begin<<\n");
	fwrite(szMsg,1,strlen(szMsg),gfpSocketTest);
	fwrite(lpBuf,1,nBufLen+10,gfpSocketTest);
	sprintf(szMsg,"\n>>lpBuf=end,  len(lpBuf)=%d\n",nBufLen);
	fwrite(szMsg,1,strlen(szMsg),gfpSocketTest);
	fflush(gfpSocketTest);
#endif	
#ifdef WIN32
	len = send(m_hSocket, (const char*)lpBuf, nBufLen, nFlags);
#else
	len = send(m_hSocket, lpBuf, nBufLen, nFlags);
#endif
	//Win32测试结果，当对方非正常关闭，返回值可能为-1或0
	//if (len == -1)
	if (len <=0 )
	{
        Close();
		sprintf(m_szLastErr, "Sending data failed");
		return -1;
	}
#ifdef _test_connection	
	strcpy(szMsg,"CSocket:send message finish <<\n");
	fwrite(szMsg,1,strlen(szMsg),gfpSocketTest);
	fflush(gfpSocketTest);
#endif	 
	return len;
}
//根据连接号直接发数据的静态函数
int CSocket::SendData(SOCKET con_id,const void* lpBuf, int nBufLen, int nFlags )
{
    int len;
#ifdef WIN32
    len = send(con_id, (const char*)lpBuf, nBufLen, nFlags);
#else
    len = send(con_id, lpBuf, nBufLen, nFlags);
#endif
    //Win32测试结果，当对方非正常关闭，返回值可能为-1或0
    //if (len == -1)
    if (len <=0 )
    {
        //Close();
        //sprintf(m_szLastErr, "Sending data failed");
        return -1;
    }
    return len;
}

//Return whether a blocking call is in progress.
bool CSocket::IsBlocking()
{
	struct timeval timeout = {0, 0};
	fd_set fdsck;
	
	memset(&fdsck, '\0', sizeof(fdsck));
	FD_SET((unsigned long)m_hSocket, &fdsck);

	select(m_hSocket+1, NULL, &fdsck, NULL, &timeout);
	if(!FD_ISSET((unsigned long)m_hSocket, &fdsck))
		return false;
	return true;
}

//Sets blocking mode  设置阻塞模式
bool CSocket::SetBlockMode(bool blocking)
{
    unsigned long ul = 1;
    if (blocking)
        ul = 1;
    else
        ul = 0;
#ifdef WIN32
    ioctlsocket(m_hSocket, FIONBIO, (unsigned long*)&ul);
#else
    ioctl(m_hSocket,FIONBIO,(unsigned long*)&ul); 
#endif
	return true;
/*
#else
	int iBlockFlag;
	//Blocking or not
	if((iBlockFlag = fcntl(m_hSocket, F_GETFL)) == -1)
	{
		sprintf(m_szLastErr, "Failed to test the socket mode");
		return false;
	}
	if(blocking)
	{
		//Synchronize
		iBlockFlag = iBlockFlag&(~O_NDELAY);
	}
	else
	{
		//Asynchronize
		iBlockFlag = iBlockFlag | O_NDELAY;
	}
	if(fcntl(m_hSocket, F_SETFL, iBlockFlag) == -1)
	{
		sprintf(m_szLastErr, "Failed to set the socket mode");
		return false;
	}
	return true;
#endif
    */
}

//Return whether there is data in the incomming buffer
bool CSocket::DataArrival(long waitMilliseconds)
{
	fd_set fdsck;

    FD_ZERO(&fdsck);
	
	//memset(&fdsck, '\0', sizeof(fdsck));
	FD_SET((unsigned long)m_hSocket, &fdsck);
	
	if(waitMilliseconds >= 0)
	{
		struct timeval timeout = { \
			waitMilliseconds / 1000, (waitMilliseconds % 1000) * 1000};
		select(m_hSocket+1, &fdsck, NULL, NULL, &timeout);
	}
	else
	{
		select(m_hSocket+1, &fdsck, NULL, NULL, NULL);
	}
	if(FD_ISSET((unsigned long)m_hSocket, &fdsck))
		return true;
	else
		return false;
}

bool CSocket::SendReady(long waitMilliseconds)
{
    fd_set fdsck;
    FD_ZERO(&fdsck);

    //memset(&fdsck, '\0', sizeof(fdsck));
    FD_SET((unsigned long)m_hSocket, &fdsck);

    if(waitMilliseconds >= 0)
    {
        struct timeval timeout = { \
            waitMilliseconds / 1000, (waitMilliseconds % 1000) * 1000};
        select(m_hSocket+1, NULL,&fdsck,  NULL, &timeout);
    }
    else
    {
        select(m_hSocket+1,  NULL,&fdsck, NULL, NULL);
    }
    if(FD_ISSET((unsigned long)m_hSocket, &fdsck))
        return true;
    else
        return false;
}


bool CSocket::Select(CSocket* socks[], int nSocksCount, long waitMilliseconds)
{
	fd_set fdsck;
    FD_ZERO(&fdsck);
	//memset(&fdsck, '\0', sizeof(fdsck));
	
	int i;
	unsigned long maxId = 0;
	CSocket* s;
	for (i = 0; i < nSocksCount; i++)
	{
		s = socks[i];
		if (s != NULL)
		{
			s->m_bSelected = false;
			if (!s->IsClosed() && s->m_hSocket != 0)
			{
				maxId = maxId > s->m_hSocket ? maxId : s->m_hSocket;
				FD_SET((unsigned long)s->m_hSocket, &fdsck);
			}
		}
	}

	//No socket is to be selected
	if (maxId == 0) return false;

	if(waitMilliseconds >= 0)
	{
		struct timeval timeout = { \
			waitMilliseconds / 1000, (waitMilliseconds % 1000) * 1000};
		select(maxId+1, &fdsck, NULL, NULL, &timeout);
	}
	else
	{
		select(maxId+1, &fdsck, NULL, NULL, NULL);
	}
	
	bool hasSelected = false;
	for (i = 0; i < nSocksCount; i++)
	{
		s = socks[i];
		if (s != NULL && !s->IsClosed())
		{
			if(FD_ISSET((unsigned long)s->m_hSocket, &fdsck))
			{
				s->m_bSelected = true;
				hasSelected    = true;
			}
            else
            {
                s->m_bSelected = false;
            }
		}
	}
	return hasSelected;
}
bool CSocket::Select(std::map<int,CSocket *> &mapSck, int nSocksCount, long waitMilliseconds)
{
    fd_set fdsck;
    FD_ZERO(&fdsck);
    //memset(&fdsck, '\0', sizeof(fdsck));

    std::map <int, CSocket*> :: iterator iter;
    int i;
    unsigned long maxId = 0;
    CSocket* s;
    for (iter = mapSck.begin() ; iter != mapSck.end( )  ; ++iter )
    {
        s = iter->second ;
        //s = socks[i];
        if (s != NULL)
        {
            s->m_bSelected = false;
            if (!s->IsClosed() && s->m_hSocket != 0)
            {
                maxId = maxId > s->m_hSocket ? maxId : s->m_hSocket;
                FD_SET((unsigned long)s->m_hSocket, &fdsck);
            }
        }
    }

    //No socket is to be selected
    if (maxId == 0) return false;

    if(waitMilliseconds >= 0)
    {
        struct timeval timeout = { \
            waitMilliseconds / 1000, (waitMilliseconds % 1000) * 1000};
        select(maxId+1, &fdsck, NULL, NULL, &timeout);
    }
    else
    {
        select(maxId+1, &fdsck, NULL, NULL, NULL);
    }

    bool hasSelected = false;
    for (iter = mapSck.begin() ; iter != mapSck.end( )  ; ++iter )
    {
        s = iter->second ;
        if (s != NULL && !s->IsClosed())
        {
            if(FD_ISSET((unsigned long)s->m_hSocket, &fdsck))
            {
                s->m_bSelected = true;
                hasSelected    = true;
            }
            else
            {
                s->m_bSelected = false;
            }
        }
    }
    return hasSelected;
}

bool CSocket::Select(std::vector<CSocket *> &vSck, int nSocksCount, long waitMilliseconds)
{
    fd_set fdsck;
    FD_ZERO(&fdsck);
    //memset(&fdsck, '\0', sizeof(fdsck));

    std::vector<CSocket *>:: iterator iter;
    int i;
    unsigned long maxId = 0;
    CSocket* s;
    for (iter = vSck.begin() ; iter != vSck.end( )  ; ++iter )
    {
        s = *iter;
        //s = socks[i];
        if (s != NULL)
        {
            s->m_bSelected = false;
            if (!s->IsClosed() && s->m_hSocket != 0)
            {
                maxId = maxId > s->m_hSocket ? maxId : s->m_hSocket;
                FD_SET((unsigned long)s->m_hSocket, &fdsck);
            }
        }
    }

    //No socket is to be selected
    if (maxId == 0) return false;

    if(waitMilliseconds >= 0)
    {
        struct timeval timeout = { \
            waitMilliseconds / 1000, (waitMilliseconds % 1000) * 1000};
        select(maxId+1, &fdsck, NULL, NULL, &timeout);
    }
    else
    {
        select(maxId+1, &fdsck, NULL, NULL, NULL);
    }

    bool hasSelected = false;
    for (iter = vSck.begin() ; iter != vSck.end( )  ; ++iter )
    {
        s = *iter;

        if (s != NULL && !s->IsClosed())
        {
            if(FD_ISSET((unsigned long)s->m_hSocket, &fdsck))
            {
                s->m_bSelected = true;
                hasSelected    = true;
            }
            else
            {
                s->m_bSelected = false;
            }
        }
    }
    return hasSelected;
}

char* CSocket::GetHostIP()
{
	return inet_ntoa(m_sin.sin_addr);
}
int   CSocket::GetPort()
{
    return  m_sin.sin_port;
}


int CSocket::GetHostAddr(char* addr)
{
	char host[256] = "";

	InitializeSock();
	if (gethostname(host, 256) != 0)
		return -1;
	struct hostent * ht = gethostbyname(host);
	if (ht == NULL)
		return -1;
	// Return the first addr
	struct in_addr in;
	memcpy(&in.s_addr, ht->h_addr_list[0], sizeof(in.s_addr));
	strcpy(addr, inet_ntoa(in));
	return 0;
}

int CSocket::GetHostAddr(struct in_addr*& addr, int& count)
{
	// Initialize socket (needed on windows)
	InitializeSock();

	char host[256] = "";
	count = 0;
	if (gethostname(host, 256) != 0)
		return -1;
	struct hostent * ht = gethostbyname(host);
	if (ht == NULL)
		return -1;
	if (ht->h_length == 0) 
		return 0;

	char **p = ht->h_addr_list;
	for (; *p != 0; p++)
		count++;
	addr = new struct in_addr[count];

	count = 0;
	for (p = ht->h_addr_list; *p != 0; p++) {
		memcpy(&addr[count].s_addr, *p, sizeof(addr[count].s_addr));
	}
	return 0;
}
int CSocket::Addratoi(const char* addr)
{
	return inet_addr(addr);
}
void CSocket::Addritoa(int nAddr,char * szAddr)
{
	struct in_addr in;
	in.s_addr = nAddr;
	//in.sin_addr.s_addr = nAddr;
	strcpy(szAddr, inet_ntoa(in));

}


//////////////////////////////////////////////////////////////////////////
//两个UDP数据发送和接收函数
int CSocket::RecvBuf(char *Buf, int BufSize, char* FromIP, unsigned short &FromPort)
{
	if (m_hSocket <= 0)
	{
		return -1;
	}
	int iRecvLen = 0;
	struct sockaddr_in skaddr;
	socklen_t ilen = sizeof(skaddr);
	//Lock();
	iRecvLen = recvfrom(m_hSocket, Buf, BufSize, 0, (sockaddr*)&skaddr, &ilen);
	//UnLock();
	strcpy(FromIP, inet_ntoa(skaddr.sin_addr));
	FromPort = ntohs(skaddr.sin_port);
	return iRecvLen;
}
int CSocket::SendBuf(const char* ToIP, unsigned short ToPort, const char* Buf, int BufSize)
{
	int iLen = -1;
	struct sockaddr_in skaddr;
	skaddr.sin_family = AF_INET;
	skaddr.sin_port = htons(ToPort);
	skaddr.sin_addr.s_addr = inet_addr(ToIP);

	//Lock();
	iLen = sendto(m_hSocket, Buf, BufSize, 0, (sockaddr*)&skaddr, sizeof(skaddr));
	//UnLock();
	return iLen;
}
