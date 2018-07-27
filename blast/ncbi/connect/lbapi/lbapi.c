/* $Id: lbapi.c,v 1.11 1999/02/26 21:17:36 shavirin Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  $RCSfile: lbapi.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 03/24/1997
*
* $Revision: 1.11 $
*
* File Description:
*        Utilities for the load balancing client library
*
* $Log: lbapi.c,v $
* Revision 1.11  1999/02/26 21:17:36  shavirin
* Moved al ntohl function to the lbdaemon area.
*
* Revision 1.10  1999/02/02 21:10:28  shavirin
* Changed formula to calculate status:
* Added adjustment of load and used 4th lb-parameter.
*
* Revision 1.9  1998/12/07 19:01:50  shavirin
* Added complience with little-big endian platforms.
*
* Revision 1.8  1998/09/22 17:22:57  yaschenk
* LBCalculateStatus: changed additive constant 1 which is very high to 0.01
*
* Revision 1.7  1998/05/08 15:26:57  vakatov
* [LB_DIRECT]  now can skip IP addresses(for the dispatcher hosts) which
* were already offered by the load-balancing daemon but failed by some reason
*
* Revision 1.6  1998/04/23 14:26:18  shavirin
* Added include #include <sys/ipc.h>
*
* Revision 1.5  1998/04/07 14:17:27  vakatov
* Skip all code(#ifdef'd) if LB_DIRECT is not defined or if compiled on
* any platform but Solaris and IRIX
*
* Revision 1.4  1998/04/03 21:50:08  shavirin
* Added include file for TCP/IP address define
*
* Revision 1.3  1998/04/03 21:16:47  vakatov
* Cleaned up the code and prepared "lbapi.[ch]" to be moved to "netcli" lib
*
* Revision 1.2  1998/04/02 17:36:04  yaschenk
* Adding dispd-ncbid shortcut when they are on the same server, adding preference to same server in stateless mode
*
* Revision 1.1  1998/03/06 18:54:25  shavirin
* Initial revision
* ==========================================================================
*/

#include <lbapi.h>

#ifdef LB_DIRECT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_FLAGS ((SHM_R >> 6) | (SHM_R >> 3) | SHM_R)
#define MAX_RAND_NUMBER 0x7FFF 


typedef struct LBUserInfo {
  LB_MessagePtr lbtable;
  int shmemid;
} LBUserInfo, *LBUserInfoPtr;

typedef struct LBStatus {
  int   index;
  float status;
  int   skip;
} LBStatus, *LBStatusPtr;


static LBUserInfo theinfo;

static float LBCalculateStatus(LB_MessagePtr msgp)
{
    float status;
    int cpuload;

    /* Penalize machine if load[0]/256 > load[1] */

    cpuload = msgp->load[0];

    if(msgp->load[0] > (msgp->load[1]<<8))
	cpuload *= exp(msgp->load[0] / (256.0*msgp->load[1]) - 1.0);
    
    status = 0.01 + 256000*msgp->load[1] / (float)(cpuload*msgp->load[2] + 1);
    
    status = status * msgp->load[3];
    
    return status;
}

static int LBClientInit(void)
{
  /* setting it completely random */
  srand(time(NULL));

  if ((theinfo.shmemid = shmget(LB_SHARED_KEY, 
                                sizeof(LB_Message) * MAX_NUM_HOSTS, 
                                SHM_FLAGS)) < 0)
    return -1;
    
  if ((theinfo.lbtable = (LB_MessagePtr)
      shmat(theinfo.shmemid, (void *)0, SHM_RDONLY)) == (LB_MessagePtr)-1)
    return -1;
    
  return 0;
}


extern int LBPrintTable(void)
{
    int i, j;
    struct sockaddr_in sin;
    struct hostent    *hp;
    float status;
    LB_MessagePtr lbtable;
    
    if((lbtable = theinfo.lbtable) == NULL) {
        if(LBClientInit() < 0)
            return -1;
        lbtable = theinfo.lbtable;
    }
    
  for(j = 0; j < MAX_NUM_HOSTS; j++) {
      if(lbtable[j].address != 0) {
          sin.sin_addr.s_addr = lbtable[j].address;
                    
          hp = gethostbyaddr((char *)&sin.sin_addr, 
                             sizeof (sin.sin_addr), AF_INET);
          printf("%-15s: ", 
                 hp == NULL? inet_ntoa(sin.sin_addr) : hp->h_name);
          
          status = LBCalculateStatus(&lbtable[j]);
          printf("%-10.2f ", status);
          
          for(i = 0; i < NUM_LOAD_PARAMETERS; i++)
              printf("%5d ", lbtable[j].load[i]);
          
          for(i = 0; i < MAX_NUM_SERVICES; i++) {
              if(*lbtable[j].service[i] != '\0')
                  printf("%s ", lbtable[j].service[i]);
          }
          printf("\n");
          fflush(stdout);
      }
  }
  printf("----\n");
  
  return 0;
}


extern unsigned LBGetIPAddress(const char *service, unsigned int pref_ip,
                               unsigned *skip_ip, size_t n_skip)
{
  int i, j, k;
  LBStatus trace[MAX_NUM_HOSTS];
  int count;
  float status, all_status, point;
  LB_MessagePtr lbtable;

  if (!theinfo.lbtable  &&  LBClientInit() < 0)
    return 0;

  lbtable = theinfo.lbtable;

  memset(trace, '\0', sizeof(trace));

  /* the hosts to be skipped */
  for (j = 0;  j < MAX_NUM_HOSTS;  j++) {
    size_t m;
    for (m = 0;  m < n_skip;  m++)
      if (lbtable[j].address == skip_ip[m]) {
        trace[j].skip = 1;
        break;
      }
  }

  /* first try to find the preferred address */
  if ( pref_ip ) {
    for (j = 0, i = 0;  j < MAX_NUM_HOSTS;  j++) {
      if (pref_ip == lbtable[j].address) {
        if ( trace[j].skip )
          break;
        for (k = 0;  k < MAX_NUM_SERVICES;  k++) {
          if ( !strcmp(lbtable[j].service[k], service) )
            return pref_ip;
        }
        break;
      }
    }
  }
    
  all_status = 0;
  for (j = 0, i = 0;  j < MAX_NUM_HOSTS;  j++) {
    if ( trace[j].skip )
      continue;

    for (k = 0;  k < MAX_NUM_SERVICES;  k++) {
      if(!strcmp(lbtable[j].service[k], service)) {
        status = LBCalculateStatus(&lbtable[j]);
        all_status += status;
        trace[i].status = all_status;
        trace[i].index = j;
        i++;
        break;
      }
    }
  }
  if ( !i )
    return 0;

  count = i;
  point = (float)(trace[count-1].status * rand()) / (float)MAX_RAND_NUMBER;
    
  for (i = 0;  i < count;  i++) {
    if (point < trace[i].status)
      break;
  }

  return lbtable[trace[i].index].address;
}

#endif /* LB_DIRECT */
