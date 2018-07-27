/* testmain.c
 *
 *	Simple test routines for the DNR and socket interfaces that are
 *	linked from Sequin.  A friendlier user interface would be nice...
 *
 *	02/27/00	pjc	
 *		Added utility printing function and tests for 6 more functions
 *
 *	02/26/00	pjc
 *		gethostbyname and gethostbyaddr: first working
 *
 *	Created 1/15/00	by pchurchill
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <s_socket.h>
#include <OpenTptInternet.h>	// for TCP/IP
#include <netdb.h>
#include <neti_in.h>
#include <a_inet.h>

/* printIP
 *
 *	Utility function for printing out IP addresses during
 *	debugging
 */
int printIP( char** theAddr)
{
	int i = 0;
	unsigned char *b;
	unsigned long **x = (unsigned long **) theAddr;

	while( (theAddr[i] != 0) && (i < kMaxHostAddrs)){
		b = (unsigned char *) theAddr[i];
		printf( "IP address for 0x%lx \t", *x[i]);
		printf( "is %d.%d.%d.%d\n",b[0],b[1],b[2],b[3]);
		i++;
	}
	printf("\n");
	return 0;
}

/* printHNames
 *
 *	Utility function to print out the fully qualified hostname
 *	along with any aliases that were identified
 */
void printHNames( struct hostent* info)
{
	int i = 0;

	printf("Fully qualified hostname:  %s\n", info->h_name);
	printf("Also reported the aliases: ");
	while( info->h_aliases[i] != NULL){
		printf("%s, ", info->h_aliases[i]);
		i++;
	}
	printf("\n");
	return;
}


void testGetHostByName( const char* hostname)
{
	struct hostent *ans = NULL;

	printf("Calling gethostbyname() for '%s'\n", hostname);
	ans = gethostbyname(hostname);
	if( ans != NULL){
		printIP( &ans->h_addr_list[0]);
		printHNames( ans);
	}
	else{
		printf("gethostbyname returned 'NULL'.\n\terrno reads as %d\n",ncbi_GetErrno());
	}
	printf("\n");
	return;
}

void testGetHostByAddr( InetHost theAddr)
{
	struct hostent *ans = NULL;

	printf("Calling gethostbyaddr() for '%8lx'\n", theAddr);
	ans = gethostbyaddr( &theAddr, 4, AF_INET);
	if( ans != NULL){
		printIP( &ans->h_addr_list[0]);
		printHNames( ans);
	}
	else{
		printf("Hmm, gethostbyaddr returned 'NULL'.  Need to implement errno\n");
	}
	printf("\n");
	return;
}

void testGetHostID( void)
{
	unsigned char *b;
	InetHost	hostid;

	printf("About to call gethostid()\n");
	hostid = gethostid();
	if( hostid == 0){
		printf("gethostid returned 0.  Need to implement errno\n");
	}
	else{
		b = (unsigned char *) &hostid;
		printf("gethostid() returned 0x%8x, %d.%d.%d.%d\n\n", hostid,
				b[0],b[1],b[2],b[3]);
	}
	return;
}
	
void testGetHostName( void)
{
	char myname[255];
	int	ans = 0;
	
	printf("About to call gethostname()\n");
	ans = gethostname( myname, 255);
	if( ans != 0){
		printf("gethostname returned 0.  Need to implement errno\n");
	}
	else{
		printf("gethostname() returned %s\n\n", myname);
	}
	return;
}

/* testInet_ntoa
 *
 *	test the address utility that converts IP addresses from network
 *	to host byte ordering (this is trivial on a Mac, they are both
 *	in the same order)
 */
void testInet_ntoa( InetHost x)
{
	char*	ans;
	struct in_addr addr;
	
	addr.s_addr = x;
	printf("About to call inet_ntoa for 0x%8x\n", x);
	ans = inet_ntoa( addr);
	if( ans != NULL){
		printf("inet_ntoa returned: %s\n\n", ans);
	}
	else{
		printf("inet_ntoa returned NULL, Need to implement errno\n");
	}
	return;
}

/* testInet_aton
 *
 *	tests utility that converts from dotted decimal notation (string)
 *	to internet host address (hex).  Both input and output are through
 *	parameters passed to the function.
 */
void testInet_aton( const char* string)
{
	int			error;
	struct in_addr addr;
	
	printf("About to call inet_aton for %s\n", string);
	error = inet_aton( string, &addr);
	if( error == INET_SUCCESS){
		printf("inet_aton returned: 0x%8x\n\n", addr.s_addr);
	}
	else{
		printf("inet_ntoa returned %d\n\n", error);
	}
	return;
}


/* testInet_addr
 *
 *	tests utility that converts from dotted decimal notation (string)
 *	to internet host address (hex)
 */
void testInet_addr( const char* string)
{
	InetHost	addr;
	
	printf("About to call inet_addr for %s\n", string);
	addr = inet_addr( string);
	printf("inet_addr returned: 0x%8x\n\n", addr);
	return;
}

/* testGetServByName
 *
 */
void testGetServByName( void)
{
	struct servent *ans;
	char *proto[] = {"udp","tcp"};
	char *serv[] = {"ftp","http","gopher","nfs","\0"};
	int i;

	for( i = 0; serv[i] != "\0"; i++){
		printf("\nCalling getservbyname for %s, %s\n", serv[i], proto[1]);
		ans = getservbyname( serv[i], proto[1]);
		if( ans != NULL){
			printf("    getservbyname: returned %s, %s, %d\n",
					ans->s_name, ans->s_proto, ans->s_port);
		} else{
			printf("    getservbyname: Returned NULL, maybe it doesn't exist?\n");
		}
	}
	return;
}


/* testGetServByPort
 *
 */
void testGetServByPort( void)
{
	struct servent *udpAns, *tcpAns;
	char	ans1[32], ans2[32];
	int	i;
	
	printf("Testing getservbyport, by scanning the first 255 ports\n\n");
	printf("Port #:  udp service   tcp service\n");
	for( i=0; i<=255; i++){
		udpAns = getservbyport( i, "udp");
		if( udpAns){
			strncpy( ans1, udpAns->s_name, 30);
		}
		tcpAns = getservbyport( i, "tcp");
		if( tcpAns){
			strncpy( ans2, tcpAns->s_name, 30);
		}
		if( (udpAns == NULL) && (tcpAns == NULL)){
			continue;
		}
		printf(" %3d: ", i);
		if( udpAns){
			printf("%12s", ans1);
		}		
		if( tcpAns){
			printf("%14s", ans2);
		}		
		printf("\n");
	}
	return;
}


/* main
 *
 */

int main( void)
{
	// for now, just call the functions.  don't do anything real ;-)

	printf("Hello, this is Phil's OT dnr tester\n\n");	

	// first, test functions that don't require network
	testInet_ntoa( 0x80082680);
	testInet_aton( "128.8.38.128");
	testInet_addr( "38.128.8.128");
	testGetServByName();
	// This test takes quite a while to run...
	//	testGetServByPort();

	// next get info about our local machine
	testGetHostID();
	testGetHostName();

	// then lookup a few others to see if they work...
	testGetHostByAddr( 0x80082680);
	testGetHostByName( "www.apple.com");
	testGetHostByName( "www.churchillandassociates.com");
	testGetHostByName( "mail");
	// one that should fail...
	testGetHostByName( "foo.dev.com");

//	s_socket();		// create the socket
//	s_close();		// close it (free the resources)
//	s_connect();	// connect using udp or tcp
//	s_send();		// just send # bytes
//	s_write();		// similar, but datagram oriented
//	s_recv();		// just get the next n bytes
//	s_read();		// similar, but datagram oriented
//	s_setsockopt();
//	s_accept();
//	s_listen();
//	s_bind();
//	s_fcntl();
/*
	bzero();			// unchanged from ncsalib
 */
	return 0;
}

