/*
 * "$Id: dirsvc.c,v 1.17 2004/06/05 03:49:46 jlovell Exp $"
 *
 *   Directory services routines for the Common UNIX Printing System (CUPS).
 *
 *   Copyright 1997-2004 by Easy Software Products, all rights reserved.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Easy Software Products and are protected by Federal
 *   copyright law.  Distribution and use rights are outlined in the file
 *   "LICENSE.txt" which should have been included with this file.  If this
 *   file is missing or damaged please contact Easy Software Products
 *   at:
 *
 *       Attn: CUPS Licensing Information
 *       Easy Software Products
 *       44141 Airport View Drive, Suite 204
 *       Hollywood, Maryland 20636-3111 USA
 *
 *       Voice: (301) 373-9603
 *       EMail: cups-info@cups.org
 *         WWW: http://www.cups.org
 *
 * Contents:
 *
 *   ProcessBrowseData() - Process new browse data.
 *   SendBrowseList()    - Send new browsing information as necessary.
 *   SendCUPSBrowse()    - Send new browsing information using the CUPS protocol.
 *   StartBrowsing()     - Start sending and receiving broadcast information.
 *   StartPolling()      - Start polling servers as needed.
 *   StopBrowsing()      - Stop sending and receiving broadcast information.
 *   StopPolling()       - Stop polling servers as needed.
 *   UpdateCUPSBrowse()  - Update the browse lists using the CUPS protocol.
 *   UpdatePolling()     - Read status messages from the poll daemons.
 *   RegReportCallback() - Empty SLPRegReport.
 *   SendSLPBrowse()     - Register the specified printer with SLP.
 *   SLPDeregPrinter()   - SLPDereg() the specified printer
 *   GetSlpAttrVal()     - Get an attribute from an SLP registration.
 *   AttrCallback()      - SLP attribute callback 
 *   SrvUrlCallback()    - SLP service url callback
 *   UpdateSLPBrowse()   - Get browsing information via SLP.
 */

/*
 * Include necessary headers...
 */

#include "cupsd.h"
#include <grp.h>


#ifdef HAVE_MDNS
#  include <dns_sd.h>
#  include <nameser.h>

#  ifdef HAVE_NOTIFY_H
#    include <notify.h>
#  endif /* HAVE_NOTIFY_H */
#endif /* HAVE_MDNS */


/*
 * Local functions...
 */

static printer_t* ProcessBrowseData(const char *uri, cups_ptype_t type,
		                  ipp_pstate_t state, const char *location,
				  const char *info, const char *make_model,
				  int protocol);
static void	SendCUPSBrowse(printer_t *p);

#ifdef HAVE_LIBSLP
static void	SendSLPBrowse(printer_t *p);
#endif /* HAVE_LIBSLP */

#ifdef HAVE_MDNS

/*
 * Due to a bug (rdar://3588761 I believe) we can't yet use the 
 * "_cups._ipp._tcp" subtype we prefer.
 */
static char mDNSIPPRegType[] = "_cups-ipp._tcp";
static char mDNSLPDRegType[] = "_printer._tcp";
static int  mDNSBrowsing = 0;

static void mDNSRegisterPrinter(printer_t *p);
static void mDNSDeregisterPrinter(printer_t *p);

static void mDNSRegisterCallback(DNSServiceRef sdRef, DNSServiceFlags flags, 
				 DNSServiceErrorType errorCode, const char *name,
				 const char *regtype, const char *domain, void *context);
static void mDNSBrowseCallback(DNSServiceRef sdRef, DNSServiceFlags flags, 
				uint32_t interfaceIndex, DNSServiceErrorType errorCode, 
				const char *service_name, const char *regtype, 
				const char *replyDomain, void *context);
static void mDNSResolveCallback(DNSServiceRef sdRef, DNSServiceFlags flags,
				uint32_t interfaceIndex, DNSServiceErrorType errorCode,
				const char *fullname, const char *host_target, uint16_t port,
				uint16_t txt_len, const char *txtRecord, void *context);
static void mDNSQueryRecordCallback(DNSServiceRef sdRef, DNSServiceFlags flags, 
				uint32_t interfaceIndex, DNSServiceErrorType errorCode,
				const char *fullname, uint16_t rrtype, uint16_t rrclass, 
				uint16_t rdlen, const void *rdata, uint32_t ttl, void *context);

static char *mDNSBuildTxtRecord(int *txt_len, printer_t *p);
static char *mDNSPackTxtRecord(int *txt_len, char *keyvalue[][2], int count);
static int  mDNSFindAttr(const unsigned char *txtRecord, int txt_len, const char *key, char **value);

#endif /* HAVE_MDNS */


/*
 * 'ProcessBrowseData()' - Process new browse data.
 */

static printer_t *				/* O - Printer in list */
ProcessBrowseData(const char   *uri,		/* I - URI of printer/class */
                  cups_ptype_t type,		/* I - Printer type */
		  ipp_pstate_t state,		/* I - Printer state */
                  const char   *location,	/* I - Printer location */
		  const char   *info,		/* I - Printer information */
                  const char   *make_model,	/* I - Printer make and model */
                  int		protocol)	/* I - Browse protocol */
{
  int		i;			/* Looping var */
  int		update;			/* Update printer attributes? */
  char		method[HTTP_MAX_URI],	/* Method portion of URI */
		username[HTTP_MAX_URI],	/* Username portion of URI */
		host[HTTP_MAX_URI],	/* Host portion of URI */
		resource[HTTP_MAX_URI];	/* Resource portion of URI */
  int		port;			/* Port portion of URI */
  char		name[IPP_MAX_NAME],	/* Name of printer */
		*hptr,			/* Pointer into hostname */
		*sptr;			/* Pointer into ServerName */
  char		local_make_model[IPP_MAX_NAME];
					/* Local make and model */
  printer_t	*p,			/* Printer information */
		*pclass,		/* Printer class */
		*first,			/* First printer in class */
		*next;			/* Next printer in list */
  int		offset,			/* Offset of name */
		len;			/* Length of name */
  struct hostent *hostent;		/* Host entry for server address */
  int		same_host;


  p = NULL;
  hostent = NULL;

 /*
  * Pull the URI apart to see if this is a local or remote printer...
  */

  httpSeparate(uri, method, username, host, &port, resource);

 /*
  * Determine if the URI contains any illegal characters in it...
  */

  if (strncmp(uri, "ipp://", 6) != 0 ||
      !host[0] ||
      (strncmp(resource, "/printers/", 10) != 0 &&
       strncmp(resource, "/classes/", 9) != 0))
  {
    LogMessage(L_ERROR, "ProcessBrowseData: Bad printer URI in browse data: %s",
               uri);
    return NULL;
  }

  if (strchr(resource, '?') != NULL ||
      (strncmp(resource, "/printers/", 10) == 0 &&
       strchr(resource + 10, '/') != NULL) ||
      (strncmp(resource, "/classes/", 9) == 0 &&
       strchr(resource + 9, '/') != NULL))
  {
    LogMessage(L_ERROR, "ProcessBrowseData: Bad resource in browse data: %s",
               resource);
    return NULL;
  }
    
 /*
  * OK, this isn't a local printer; see if we already have it listed in
  * the Printers list, and add it if not...
  */

  type   |= CUPS_PRINTER_REMOTE;
  update = 0;
  hptr   = strchr(host, '.');
  sptr   = strchr(ServerName, '.');

  if (sptr != NULL && hptr != NULL)
  {
   /*
    * Strip the common domain name components...
    */

    while (hptr != NULL)
    {
      if (strcasecmp(hptr, sptr) == 0)
      {
        *hptr = '\0';
	break;
      }
      else
        hptr = strchr(hptr + 1, '.');
    }
  }

  if (type & CUPS_PRINTER_CLASS)
  {
   /*
    * Remote destination is a class...
    */

    if (strncmp(resource, "/classes/", 9) == 0)
      snprintf(name, sizeof(name), "%s@%s", resource + 9, host);
    else
      return NULL;

    if ((p = FindClass(name)) == NULL && BrowseShortNames)
    {
      if ((p = FindClass(resource + 9)) != NULL)
      {
        if (p->hostname && strcasecmp(p->hostname, host) != 0)
	{
	 /*
	  * Nope, this isn't the same host; if the hostname isn't the local host,
	  * add it to the other class and then find a class using the full host
	  * name...
	  */

	  if (p->type & CUPS_PRINTER_REMOTE)
	  {
            SetStringf(&p->name, "%s@%s", p->name, p->hostname);
	    SetPrinterAttrs(p);
	    SortPrinters();
	  }

          p = NULL;
	}
	else if (!p->hostname)
	{
          SetString(&p->hostname, host);
	  SetString(&p->uri, uri);
	  SetString(&p->device_uri, uri);
          update = 1;
        }
      }
      else
        strlcpy(name, resource + 9, sizeof(name));
    }
    else if (p != NULL && !p->hostname)
    {
      SetString(&p->hostname, host);
      SetString(&p->uri, uri);
      SetString(&p->device_uri, uri);
      update = 1;
    }

    if (p == NULL)
    {
     /*
      * Class doesn't exist; add it...
      */

      p = AddClass(name);

      LogMessage(L_INFO, "Added remote class \"%s\"...", name);

     /*
      * Force the URI to point to the real server...
      */

      p->type      = type & ~CUPS_PRINTER_REJECTING;
      p->accepting = 1;
      SetString(&p->uri, uri);
      SetString(&p->device_uri, uri);
      SetString(&p->hostname, host);

      update = 1;
    }
  }
  else
  {
   /*
    * Remote destination is a printer...
    */

    if (strncmp(resource, "/printers/", 10) == 0)
      snprintf(name, sizeof(name), "%s@%s", resource + 10, host);
    else
      return NULL;

    if ((p = FindPrinter(name)) != NULL || 
	(p = FindPrinter(resource + 10)) != NULL)
    {
      same_host = (p->hostname && strcasecmp(p->hostname, host) == 0);

#ifdef HAVE_MDNS
      if (!same_host && (hostent = httpGetHostByName(host)) != NULL)
      {
	for (i = 0; hostent->h_addr_list[i] && !same_host; i ++)
	  same_host = (memcmp(&(p->hostaddr.sin_addr), hostent->h_addr_list[i], 4) == 0);
      }
#endif /* HAVE_MDNS */

      if (!same_host)
      {
       /*
	* Nope, this isn't the same host; if the hostname isn't the local host,
	* add it to the other printer and then find a printer using the full host
	* name...
	*/

	if (p->type & CUPS_PRINTER_REMOTE)
	{
	  SetStringf(&p->name, "%s@%s", p->name, p->hostname);
	  SetPrinterAttrs(p);
	  SortPrinters();
	}

        p = NULL;
      }
    }
    else
      strlcpy(name, resource + 10, sizeof(name));

    if (p && !p->hostname)
    {
      SetString(&p->hostname, host);
      SetString(&p->uri, uri);
      SetString(&p->device_uri, uri);
      update = 1;
    }

    if (p == NULL)
    {
     /*
      * Printer doesn't exist; add it...
      */

      p = AddPrinter(name);

      LogMessage(L_INFO, "Added remote printer \"%s\"...", name);

     /*
      * Force the URI to point to the real server...
      */

      p->type      = type & ~CUPS_PRINTER_REJECTING;
      p->accepting = 1;
      SetString(&p->hostname, host);
      SetString(&p->uri, uri);
      SetString(&p->device_uri, uri);

#ifdef HAVE_MDNS
       /*
	* Remember the address for later compares.
	*/

      if (hostent || (hostent = httpGetHostByName(host)))
	memcpy(&(p->hostaddr.sin_addr), hostent->h_addr_list[0], 4);

#endif /* HAVE_MDNS */

      update = 1;
    }
  }

 /*
  * Update the state...
  */

  if (p->state != state)
  {
    update       = 1;
    p->state     = state;
  }

  p->browse_time = time(NULL);
  p->browse_protocol |= protocol;

  if (type & CUPS_PRINTER_REJECTING)
  {
    type &= ~CUPS_PRINTER_REJECTING;

    if (p->accepting)
    {
      update       = 1;
      p->accepting = 0;
    }
  }
  else if (!p->accepting)
  {
    update       = 1;
    p->accepting = 1;
  }

  if (p->type != type)
  {
    p->type = type;
    update  = 1;
  }

  if (location && (!p->location || strcmp(p->location, location)))
  {
    SetString(&p->location, location);
    update = 1;
  }

  if (info && (!p->info || strcmp(p->info, info)))
  {
    SetString(&p->info, info);
    update = 1;
  }

  if (!make_model || !make_model[0])
  {
    if (type & CUPS_PRINTER_CLASS)
      snprintf(local_make_model, sizeof(local_make_model),
               "Remote Class on %s", host);
    else
      snprintf(local_make_model, sizeof(local_make_model),
               "Remote Printer on %s", host);
  }
  else
#ifdef __APPLE__
    /* Changing make_model makes it difficult to localize so we don't do it */
    strlcpy(local_make_model, make_model, sizeof(local_make_model));
#else
    snprintf(local_make_model, sizeof(local_make_model),
             "%s on %s", make_model, host);
#endif        /* __APPLE__ */

  if (!p->make_model || strcmp(p->make_model, local_make_model))
  {
    SetString(&p->make_model, local_make_model);
    update = 1;
  }

  if (update)
  {
    SetPrinterAttrs(p);
    UpdateImplicitClasses();
  }

 /*
  * See if we have a default printer...  If not, make the first printer the
  * default.
  */

  if (DefaultPrinter == NULL && Printers != NULL)
  {
#ifdef __APPLE__
   /*
    * Don't pick a remote printer as the default...
    */

    for (p = Printers; p != NULL; p = p->next)
      if (!(p->type & CUPS_PRINTER_REMOTE))
      {
	DefaultPrinter = p;
	WritePrintcap();
	break;
      }
#else
    DefaultPrinter = Printers;

    WritePrintcap();
#endif        /* __APPLE__ */
  }

 /*
  * Do auto-classing if needed...
  */

  if (ImplicitClasses)
  {
   /*
    * Loop through all available printers and create classes as needed...
    */

    for (p = Printers, len = 0, offset = 0, first = NULL;
         p != NULL;
	 p = next)
    {
     /*
      * Get next printer in list...
      */

      next = p->next;

     /*
      * Skip implicit classes...
      */

      if (p->type & CUPS_PRINTER_IMPLICIT)
      {
        len = 0;
        continue;
      }

     /*
      * If len == 0, get the length of this printer name up to the "@"
      * sign (if any).
      */

      if (len > 0 &&
	  strncasecmp(p->name, name + offset, len) == 0 &&
	  (p->name[len] == '\0' || p->name[len] == '@'))
      {
       /*
	* We have more than one printer with the same name; see if
	* we have a class, and if this printer is a member...
	*/

        if ((pclass = FindDest(name)) == NULL)
	{
	 /*
	  * Need to add the class...
	  */

	  pclass = AddPrinter(name);
	  pclass->type      |= CUPS_PRINTER_IMPLICIT;
	  pclass->accepting = 1;
	  pclass->state     = IPP_PRINTER_IDLE;

          SetString(&pclass->location, p->location);
          SetString(&pclass->info, p->info);

          SetPrinterAttrs(pclass);

          LogMessage(L_INFO, "Added implicit class \"%s\"...", name);
	}

        if (first != NULL)
	{
          for (i = 0; i < pclass->num_printers; i ++)
	    if (pclass->printers[i] == first)
	      break;

          if (i >= pclass->num_printers)
	    AddPrinterToClass(pclass, first);

	  first = NULL;
	}

        for (i = 0; i < pclass->num_printers; i ++)
	  if (pclass->printers[i] == p)
	    break;

        if (i >= pclass->num_printers)
	  AddPrinterToClass(pclass, p);
      }
      else
      {
       /*
        * First time around; just get name length and mark it as first
	* in the list...
	*/

	if ((hptr = strchr(p->name, '@')) != NULL)
	  len = hptr - p->name;
	else
	  len = strlen(p->name);

        strncpy(name, p->name, len);
	name[len] = '\0';
	offset    = 0;

	if ((pclass = FindDest(name)) != NULL &&
	    !(pclass->type & CUPS_PRINTER_IMPLICIT))
	{
	 /*
	  * Can't use same name as a local printer; add "Any" to the
	  * front of the name, unless we have explicitly disabled
	  * the "ImplicitAnyClasses"...
	  */

          if (ImplicitAnyClasses && len < (sizeof(name) - 4))
	  {
	   /*
	    * Add "Any" to the class name...
	    */

            strcpy(name, "Any");
            strncpy(name + 3, p->name, len);
	    name[len + 3] = '\0';
	    offset        = 3;
	  }
	  else
	  {
	   /*
	    * Don't create an implicit class if we have a local printer
	    * with the same name...
	    */

	    len = 0;
	    continue;
	  }
	}

	first = p;
      }
    }
  }
  return p;
}


/*
 * 'SendBrowseList()' - Send new browsing information as necessary.
 */

void
SendBrowseList(void)
{
  int			count;	/* Number of dests to update */
  printer_t		*p,	/* Current printer */
			*np;	/* Next printer */
  time_t		ut,	/* Minimum update time */
			to;	/* Timeout time */


  if (!Browsing || !BrowseProtocols)
    return;

 /*
  * Compute the update and timeout times...
  */

  ut = time(NULL) - BrowseInterval;
  to = time(NULL) - BrowseTimeout;

 /*
  * Figure out how many printers need an update...
  */

  if (BrowseInterval > 0)
  {
    for (count = 0, p = Printers; p != NULL; p = p->next)
      if (!(p->type & (CUPS_PRINTER_REMOTE | CUPS_PRINTER_IMPLICIT)) &&
          p->browse_time < ut && p->shared)
        count ++;

   /*
    * Throttle the number of printers we'll be updating this time
    * around...
    */

    count = 2 * count / BrowseInterval + 1;
  }
  else
    count = 0;

 /*
  * Loop through all of the printers and send local updates as needed...
  */

  for (p = Printers; p != NULL; p = np)
  {
    np = p->next;

    if (p->type & CUPS_PRINTER_REMOTE)
    {
     /*
      * See if this printer needs to be timed out...
      */

      if (p->browse_time < to && p->browse_protocol == BROWSE_CUPS)
      {
        LogMessage(L_INFO, "Remote destination \"%s\" has timed out; deleting it...",
	           p->name);
        DeletePrinter(p, 1);
      }
    }
    else if (p->browse_time < ut && count > 0 &&
             !(p->type & CUPS_PRINTER_IMPLICIT) &&
             p->shared)
    {
     /*
      * Need to send an update...
      */

      count --;

      p->browse_time = time(NULL);

      if (BrowseProtocols & BROWSE_CUPS)
        SendCUPSBrowse(p);

#ifdef HAVE_LIBSLP
      if (BrowseProtocols & BROWSE_SLP)
        SendSLPBrowse(p);
#endif /* HAVE_LIBSLP */
    }
  }
}


/*
 * 'SendCUPSBrowse()' - Send new browsing information using the CUPS protocol.
 */

static void
SendCUPSBrowse(printer_t *p)		/* I - Printer to send */
{
  int			i;		/* Looping var */
  cups_ptype_t		type;		/* Printer type */
  dirsvc_addr_t		*b;		/* Browse address */
  int			bytes;		/* Length of packet */
  char			packet[1453];	/* Browse data packet */
  cups_netif_t		*iface;		/* Network interface */


 /*
  * Figure out the printer type value...
  */

  type = p->type | CUPS_PRINTER_REMOTE;

  if (!p->accepting)
    type |= CUPS_PRINTER_REJECTING;

 /*
  * Send a packet to each browse address...
  */

  for (i = NumBrowsers, b = Browsers; i > 0; i --, b ++)
    if (b->iface[0])
    {
     /*
      * Send the browse packet to one or more interfaces...
      */

      if (strcmp(b->iface, "*") == 0)
      {
       /*
        * Send to all local interfaces...
	*/

        NetIFUpdate();

        for (iface = NetIFList; iface != NULL; iface = iface->next)
	{
	 /*
	  * Only send to local interfaces...
	  */

	  if (!iface->is_local)
	    continue;

	  snprintf(packet, sizeof(packet), "%x %x ipp://%s/%s/%s \"%s\" \"%s\" \"%s\"\n",
        	   type, p->state, iface->hostname,
		   (p->type & CUPS_PRINTER_CLASS) ? "classes" : "printers",
		   p->name, p->location ? p->location : "",
		   p->info ? p->info : "",
		   p->make_model ? p->make_model : "Unknown");

	  bytes = strlen(packet);

	  LogMessage(L_DEBUG2, "SendBrowseList: (%d bytes to \"%s\") %s", bytes,
        	     iface->name, packet);

          iface->broadcast.sin_port = htons(BrowsePort);

	  sendto(BrowseSocket, packet, bytes, 0,
		 (struct sockaddr *)&(iface->broadcast),
		 sizeof(struct sockaddr_in));
        }
      }
      else if ((iface = NetIFFind(b->iface)) != NULL)
      {
       /*
        * Send to the named interface...
	*/

	snprintf(packet, sizeof(packet), "%x %x ipp://%s/%s/%s \"%s\" \"%s\" \"%s\"\n",
        	 type, p->state, iface->hostname,
		 (p->type & CUPS_PRINTER_CLASS) ? "classes" : "printers",
		 p->name, p->location ? p->location : "",
		 p->info ? p->info : "",
		 p->make_model ? p->make_model : "Unknown");

	bytes = strlen(packet);

	LogMessage(L_DEBUG2, "SendBrowseList: (%d bytes to \"%s\") %s", bytes,
        	   iface->name, packet);

        iface->broadcast.sin_port = htons(BrowsePort);

	sendto(BrowseSocket, packet, bytes, 0,
	       (struct sockaddr *)&(iface->broadcast),
	       sizeof(struct sockaddr_in));
      }
    }
    else
    {
     /*
      * Send the browse packet to the indicated address using
      * the default server name...
      */

      snprintf(packet, sizeof(packet), "%x %x %s \"%s\" \"%s\" \"%s\"\n",
       	       type, p->state, p->uri,
	       p->location ? p->location : "",
	       p->info ? p->info : "",
	       p->make_model ? p->make_model : "Unknown");

      bytes = strlen(packet);
      LogMessage(L_DEBUG2, "SendBrowseList: (%d bytes to %x) %s", bytes,
        	 (unsigned)ntohl(b->to.sin_addr.s_addr), packet);

      if (sendto(BrowseSocket, packet, bytes, 0,
		 (struct sockaddr *)&(b->to), sizeof(struct sockaddr_in)) <= 0)
      {
       /*
        * Unable to send browse packet, so remove this address from the
	* list...
	*/

	LogMessage(L_ERROR, "SendBrowseList: sendto failed for browser %d - %s.",
	           b - Browsers + 1, strerror(errno));

        if (i > 1)
	  memcpy(b, b + 1, (i - 1) * sizeof(dirsvc_addr_t));

	b --;
	NumBrowsers --;
      }
    }
}


/*
 * 'StartBrowsing()' - Start sending and receiving broadcast information.
 */

void
StartBrowsing(void)
{
  int			val;	/* Socket option value */
  struct sockaddr_in	addr;	/* Broadcast address */
  printer_t		*p;	/* Pointer to current printer/class */


  if (!Browsing || !BrowseProtocols)
    return;

  if (BrowseProtocols & BROWSE_CUPS)
  {
   /*
    * Create the broadcast socket...
    */

    if ((BrowseSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      LogMessage(L_ERROR, "StartBrowsing: Unable to create broadcast socket - %s.",
        	 strerror(errno));
      BrowseProtocols &= ~BROWSE_CUPS;
      return;
    }

   /*
    * Set the "broadcast" flag...
    */

    val = 1;
    if (setsockopt(BrowseSocket, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val)))
    {
      LogMessage(L_ERROR, "StartBrowsing: Unable to set broadcast mode - %s.",
        	 strerror(errno));

#ifdef WIN32
      closesocket(BrowseSocket);
#else
      close(BrowseSocket);
#endif /* WIN32 */

      BrowseSocket    = -1;
      BrowseProtocols &= ~BROWSE_CUPS;
      return;
    }

   /*
    * Bind the socket to browse port...
    */

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(BrowsePort);

    if (bind(BrowseSocket, (struct sockaddr *)&addr, sizeof(addr)))
    {
      LogMessage(L_ERROR, "StartBrowsing: Unable to bind broadcast socket - %s.",
        	 strerror(errno));

#ifdef WIN32
      closesocket(BrowseSocket);
#else
      close(BrowseSocket);
#endif /* WIN32 */

      BrowseSocket    = -1;
      BrowseProtocols &= ~BROWSE_CUPS;
      return;
    }

   /*
    * Finally, add the socket to the input selection set...
    */

    LogMessage(L_DEBUG2, "StartBrowsing: Adding fd %d to InputSet...",
               BrowseSocket);

    FD_SET(BrowseSocket, InputSet);
  }
  else
    BrowseSocket = -1;

#ifdef HAVE_LIBSLP
  if (BrowseProtocols & BROWSE_SLP)
  {
   /* 
    * Open SLP handle...
    */

    if (SLPOpen("en", SLP_FALSE, &BrowseSLPHandle) != SLP_OK)
    {
      LogMessage(L_ERROR, "Unable to open an SLP handle; disabling SLP browsing!");
      BrowseProtocols &= ~BROWSE_SLP;
    }

    BrowseSLPRefresh = 0;
  }
#endif /* HAVE_LIBSLP */

#ifdef HAVE_MDNS
  if (BrowseProtocols & BROWSE_MDNS)
  {
    DNSServiceErrorType	se;	/* mDNS errors */

    mDNSBrowsing = 1;

   /*
    * Start browsing for services
    */

    if ((se = DNSServiceBrowse(&BrowseMDNSRef, 0, 0, mDNSIPPRegType,
				NULL, mDNSBrowseCallback, NULL)) == 0)
    {
      BrowseMDNSfd = DNSServiceRefSockFD(BrowseMDNSRef);

      LogMessage(L_DEBUG2, "StartBrowsing: Adding fd %d to InputSet...",
               BrowseMDNSfd);

      FD_SET(BrowseMDNSfd, InputSet);
    }
  }
#endif /* HAVE_MDNS */

 /*
  * Register the individual printers
  */

  for (p = Printers; p != NULL; p = p->next)
    BrowseRegisterPrinter(p);
}


/*
 * 'StartPolling()' - Start polling servers as needed.
 */

void
StartPolling(void)
{
  int		i;		/* Looping var */
  dirsvc_poll_t	*poll;		/* Current polling server */
  int		pid;		/* New process ID */
  char		sport[10];	/* Server port */
  char		bport[10];	/* Browser port */
  char		interval[10];	/* Poll interval */
  int		statusfds[2];	/* Status pipe */
  int		fd;		/* Current file descriptor */
#if defined(HAVE_SIGACTION) && !defined(HAVE_SIGSET)
  struct sigaction action;	/* POSIX signal handler */
#endif /* HAVE_SIGACTION && !HAVE_SIGSET */


 /*
  * Don't do anything if we aren't polling...
  */

  if (NumPolled == 0)
  {
    PollPipe = -1;
    return;
  }

 /*
  * Setup string arguments for port and interval options.
  */

  sprintf(bport, "%d", BrowsePort);

  if (BrowseInterval)
    sprintf(interval, "%d", BrowseInterval);
  else
    strcpy(interval, "30");

 /*
  * Create a pipe that receives the status messages from each
  * polling daemon...
  */

  if (pipe(statusfds))
  {
    LogMessage(L_ERROR, "Unable to create polling status pipes - %s.",
	       strerror(errno));
    PollPipe = -1;
    return;
  }

  PollPipe = statusfds[0];

 /*
  * Run each polling daemon, redirecting stderr to the polling pipe...
  */

  for (i = 0, poll = Polled; i < NumPolled; i ++, poll ++)
  {
    sprintf(sport, "%d", poll->port);

   /*
    * Block signals before forking...
    */

    HoldSignals();

    if ((pid = fork()) == 0)
    {
     /*
      * Child...
      */

      if (getuid() == 0)
      {
       /*
	* Running as root, so change to non-priviledged user...
	*/

	if (setgid(Group))
          exit(errno);

	if (setgroups(1, &Group))
          exit(errno);

	if (setuid(User))
          exit(errno);
      }
      else
      {
       /*
	* Reset group membership to just the main one we belong to.
	*/

	setgroups(1, &Group);
      }

     /*
      * Redirect stdin and stdout to /dev/null, and stderr to the
      * status pipe.  Close all other files.
      */

      close(0);
      open("/dev/null", O_RDONLY);

      close(1);
      open("/dev/null", O_WRONLY);

      close(2);
      dup(statusfds[1]);

      for (fd = 3; fd < MaxFDs; fd ++)
	close(fd);

     /*
      * Unblock signals before doing the exec...
      */

#ifdef HAVE_SIGSET
      sigset(SIGTERM, SIG_DFL);
      sigset(SIGCHLD, SIG_DFL);
#elif defined(HAVE_SIGACTION)
      memset(&action, 0, sizeof(action));

      sigemptyset(&action.sa_mask);
      action.sa_handler = SIG_DFL;

      sigaction(SIGTERM, &action, NULL);
      sigaction(SIGCHLD, &action, NULL);
#else
      signal(SIGTERM, SIG_DFL);
      signal(SIGCHLD, SIG_DFL);
#endif /* HAVE_SIGSET */

      ReleaseSignals();

     /*
      * Execute the polling daemon...
      */

      execl(CUPS_SERVERBIN "/daemon/cups-polld", "cups-polld", poll->hostname,
            sport, interval, bport, NULL);
      exit(errno);
    }
    else if (pid < 0)
    {
      LogMessage(L_ERROR, "StartPolling: Unable to fork polling daemon - %s",
                 strerror(errno));
      poll->pid = 0;
      break;
    }
    else
    {
      poll->pid = pid;
      LogMessage(L_DEBUG, "StartPolling: Started polling daemon for %s:%d, pid = %d",
                 poll->hostname, poll->port, pid);
    }

    ReleaseSignals();
  }

  close(statusfds[1]);

 /*
  * Finally, add the pipe to the input selection set...
  */

  LogMessage(L_DEBUG2, "StartPolling: Adding fd %d to InputSet...",
             PollPipe);

  FD_SET(PollPipe, InputSet);
}


/*
 * 'StopBrowsing()' - Stop sending and receiving broadcast information.
 */

void
StopBrowsing(void)
{
  printer_t		*p;	/* Pointer to current printer/class */

  if (!Browsing || !BrowseProtocols)
    return;

  if (BrowseProtocols & BROWSE_CUPS)
  {
   /*
    * Close the socket and remove it from the input selection set.
    */

    if (BrowseSocket >= 0)
    {
#ifdef WIN32
      closesocket(BrowseSocket);
#else
      close(BrowseSocket);
#endif /* WIN32 */

      LogMessage(L_DEBUG2, "StopBrowsing: Removing fd %d from InputSet...",
        	 BrowseSocket);

      FD_CLR(BrowseSocket, InputSet);
      BrowseSocket = -1;
    }
  }

#ifdef HAVE_LIBSLP
  if (BrowseProtocols & BROWSE_SLP)
  {
   /* 
    * Close SLP handle...
    */

    SLPClose(BrowseSLPHandle);
  }
#endif /* HAVE_LIBSLP */

#ifdef HAVE_MDNS
  if (BrowseProtocols & BROWSE_MDNS)
  {
    mDNSBrowsing = 0;

   /*
    * Close the socket to stop browsing
    */

    if (BrowseMDNSRef)
    {
      LogMessage(L_DEBUG2, "StopBrowsing: Removing fd %d from InputSet...",
        	 BrowseMDNSfd);

      FD_CLR(BrowseMDNSfd, InputSet);
      DNSServiceRefDeallocate(BrowseMDNSRef);
      BrowseMDNSRef = NULL;
      BrowseMDNSfd = -1;
    }
  }

 /*
  * De-register the individual printers
  */

  for (p = Printers; p != NULL; p = p->next)
    BrowseDeregisterPrinter(p);
#endif /* HAVE_MDNS */
}


/*
 * 'StopPolling()' - Stop polling servers as needed.
 */

void
StopPolling(void)
{
  int		i;		/* Looping var */
  dirsvc_poll_t	*poll;		/* Current polling server */


  if (PollPipe >= 0)
  {
    close(PollPipe);

    LogMessage(L_DEBUG2, "StopPolling: removing fd %d from InputSet.",
               PollPipe);
    FD_CLR(PollPipe, InputSet);

    PollPipe = -1;
  }

  for (i = 0, poll = Polled; i < NumPolled; i ++, poll ++)
    if (poll->pid)
      kill(poll->pid, SIGTERM);
}


/*
 * 'UpdateCUPSBrowse()' - Update the browse lists using the CUPS protocol.
 */

void
UpdateCUPSBrowse(void)
{
  int		i;			/* Looping var */
  int		auth;			/* Authorization status */
  int		len;			/* Length of name string */
  int		bytes;			/* Number of bytes left */
  char		packet[1540],		/* Broadcast packet */
		*pptr;			/* Pointer into packet */
  struct sockaddr_in srcaddr;		/* Source address */
  char		srcname[1024];		/* Source hostname */
  unsigned	address;		/* Source address (host order) */
  struct hostent *srchost;		/* Host entry for source address */
  unsigned	type;			/* Printer type */
  unsigned	state;			/* Printer state */
  char		uri[HTTP_MAX_URI],	/* Printer URI */
		method[HTTP_MAX_URI],	/* Method portion of URI */
		username[HTTP_MAX_URI],	/* Username portion of URI */
		host[HTTP_MAX_URI],	/* Host portion of URI */
		resource[HTTP_MAX_URI],	/* Resource portion of URI */
		info[IPP_MAX_NAME],	/* Information string */
		location[IPP_MAX_NAME],	/* Location string */
		make_model[IPP_MAX_NAME];/* Make and model string */
  int		port;			/* Port portion of URI */
  cups_netif_t	*iface;			/* Network interface */


 /*
  * Read a packet from the browse socket...
  */

  len = sizeof(srcaddr);
  if ((bytes = recvfrom(BrowseSocket, packet, sizeof(packet), 0, 
                        (struct sockaddr *)&srcaddr, &len)) <= 0)
  {
   /*
    * "Connection refused" is returned under Linux if the destination port
    * or address is unreachable from a previous sendto(); check for the
    * error here and ignore it for now...
    */

    if (errno != ECONNREFUSED)
    {
      LogMessage(L_ERROR, "Browse recv failed - %s.", strerror(errno));
      LogMessage(L_ERROR, "Browsing turned off.");

      StopBrowsing();
      Browsing = 0;
    }

    return;
  }

  packet[bytes] = '\0';


#ifdef __APPLE__
 /*
  * If we're about to sleep ignore incoming browse packets.
  */

  if (Sleeping)
    return;
#endif	/* __APPLE__ */


 /*
  * Figure out where it came from...
  */

  address = ntohl(srcaddr.sin_addr.s_addr);

  if (HostNameLookups)
#ifndef __sgi
    srchost = gethostbyaddr((char *)&(srcaddr.sin_addr), sizeof(struct in_addr),
                            AF_INET);
#else
    srchost = gethostbyaddr(&(srcaddr.sin_addr), sizeof(struct in_addr),
                            AF_INET);
#endif /* !__sgi */
  else
    srchost = NULL;

  if (srchost == NULL)
    sprintf(srcname, "%d.%d.%d.%d", address >> 24, (address >> 16) & 255,
            (address >> 8) & 255, address & 255);
  else
    strlcpy(srcname, srchost->h_name, sizeof(srcname));

  len = strlen(srcname);

 /*
  * Do ACL stuff...
  */

  if (BrowseACL && (BrowseACL->num_allow || BrowseACL->num_deny))
  {
    if (address == 0x7f000001 || strcasecmp(srcname, "localhost") == 0)
    {
     /*
      * Access from localhost (127.0.0.1) is always allowed...
      */

      auth = AUTH_ALLOW;
    }
    else
    {
     /*
      * Do authorization checks on the domain/address...
      */

      switch (BrowseACL->order_type)
      {
        default :
	    auth = AUTH_DENY;	/* anti-compiler-warning-code */
	    break;

	case AUTH_ALLOW : /* Order Deny,Allow */
            auth = AUTH_ALLOW;

            if (CheckAuth(address, srcname, len,
	        	  BrowseACL->num_deny, BrowseACL->deny))
	      auth = AUTH_DENY;

            if (CheckAuth(address, srcname, len,
	        	  BrowseACL->num_allow, BrowseACL->allow))
	      auth = AUTH_ALLOW;
	    break;

	case AUTH_DENY : /* Order Allow,Deny */
            auth = AUTH_DENY;

            if (CheckAuth(address, srcname, len,
	        	  BrowseACL->num_allow, BrowseACL->allow))
	      auth = AUTH_ALLOW;

            if (CheckAuth(address, srcname, len,
	        	  BrowseACL->num_deny, BrowseACL->deny))
	      auth = AUTH_DENY;
	    break;
      }
    }
  }
  else
    auth = AUTH_ALLOW;

  if (auth == AUTH_DENY)
  {
    LogMessage(L_DEBUG, "UpdateCUPSBrowse: Refused %d bytes from %s", bytes,
               srcname);
    return;
  }

  LogMessage(L_DEBUG2, "UpdateCUPSBrowse: (%d bytes from %s) %s", bytes, srcname,
             packet);

 /*
  * Parse packet...
  */

  if (sscanf(packet, "%x%x%1023s", &type, &state, uri) < 3)
  {
    LogMessage(L_WARN, "UpdateCUPSBrowse: Garbled browse packet - %s",
               packet);
    return;
  }

  strcpy(location, "Location Unknown");
  strcpy(info, "No Information Available");
  make_model[0] = '\0';

  if ((pptr = strchr(packet, '\"')) != NULL)
  {
   /*
    * Have extended information; can't use sscanf for it because not all
    * sscanf's allow empty strings with %[^\"]...
    */

    for (i = 0, pptr ++;
         i < (sizeof(location) - 1) && *pptr && *pptr != '\"';
         i ++, pptr ++)
      location[i] = *pptr;

    if (i)
      location[i] = '\0';

    if (*pptr == '\"')
      pptr ++;

    while (*pptr && isspace(*pptr & 255))
      pptr ++;

    if (*pptr == '\"')
    {
      for (i = 0, pptr ++;
           i < (sizeof(info) - 1) && *pptr && *pptr != '\"';
           i ++, pptr ++)
	info[i] = *pptr;

      if (i)
	info[i] = '\0';

      if (*pptr == '\"')
	pptr ++;

      while (*pptr && isspace(*pptr & 255))
	pptr ++;

      if (*pptr == '\"')
      {
	for (i = 0, pptr ++;
             i < (sizeof(make_model) - 1) && *pptr && *pptr != '\"';
             i ++, pptr ++)
	  make_model[i] = *pptr;

	if (i)
	  make_model[i] = '\0';
      }
    }
  }

  DEBUG_puts(packet);
  DEBUG_printf(("type=%x, state=%x, uri=\"%s\"\n"
                "location=\"%s\", info=\"%s\", make_model=\"%s\"\n",
	        type, state, uri, location, info, make_model));

 /*
  * Pull the URI apart to see if this is a local or remote printer...
  */

  httpSeparate(uri, method, username, host, &port, resource);

  DEBUG_printf(("host=\"%s\", ServerName=\"%s\"\n", host, ServerName));

 /*
  * Check for packets from the local server...
  */

  if (strcasecmp(host, ServerName) == 0)
    return;

  NetIFUpdate();

  for (iface = NetIFList; iface != NULL; iface = iface->next)
    if (strcasecmp(host, iface->hostname) == 0)
      return;

 /*
  * Do relaying...
  */

  for (i = 0; i < NumRelays; i ++)
    if (CheckAuth(address, srcname, len, 1, &(Relays[i].from)))
      if (sendto(BrowseSocket, packet, bytes, 0,
                 (struct sockaddr *)&(Relays[i].to),
		 sizeof(struct sockaddr_in)) <= 0)
      {
	LogMessage(L_ERROR, "UpdateCUPSBrowse: sendto failed for relay %d - %s.",
	           i + 1, strerror(errno));
	return;
      }

 /*
  * Process the browse data...
  */

  ProcessBrowseData(uri, type, state, location, info, make_model, BROWSE_CUPS);
}


/*
 * 'UpdatePolling()' - Read status messages from the poll daemons.
 */

void
UpdatePolling(void)
{
  int		bytes;		/* Number of bytes read */
  char		*lineptr;	/* Pointer to end of line in buffer */
  static int	bufused = 0;	/* Number of bytes used in buffer */
  static char	buffer[1024];	/* Status buffer */


  if ((bytes = read(PollPipe, buffer + bufused,
                    sizeof(buffer) - bufused - 1)) > 0)
  {
    bufused += bytes;
    buffer[bufused] = '\0';
    lineptr = strchr(buffer, '\n');
  }
  else if (bytes < 0 && errno == EINTR)
    return;
  else
  {
    lineptr    = buffer + bufused;
    lineptr[1] = 0;
  }

  if (bytes == 0 && bufused == 0)
    lineptr = NULL;

  while (lineptr != NULL)
  {
   /*
    * Terminate each line and process it...
    */

    *lineptr++ = '\0';

    if (!strncmp(buffer, "ERROR: ", 7))
      LogMessage(L_ERROR, "%s", buffer + 7);
    else if (!strncmp(buffer, "DEBUG: ", 7))
      LogMessage(L_DEBUG, "%s", buffer + 7);
    else if (!strncmp(buffer, "DEBUG2: ", 8))
      LogMessage(L_DEBUG2, "%s", buffer + 8);
    else
      LogMessage(L_DEBUG, "%s", buffer);

   /*
    * Copy over the buffer data we've used up...
    */

    cups_strcpy(buffer, lineptr);
    bufused -= lineptr - buffer;

    if (bufused < 0)
      bufused = 0;

    lineptr = strchr(buffer, '\n');
  }

  if (bytes <= 0)
  {
   /*
    * All polling processes have died; stop polling...
    */

    LogMessage(L_ERROR, "UpdatePolling: all polling processes have exited!");
    StopPolling();
  }
}


/***********************************************************************
 **** SLP Support Code *************************************************
 ***********************************************************************/

#ifdef HAVE_LIBSLP 
/*
 * SLP service name for CUPS...
 */

#  define SLP_CUPS_SRVTYPE	"service:printer"
#  define SLP_CUPS_SRVLEN	15


/* 
 * Printer service URL structure
 */

typedef struct _slpsrvurl
{
  struct _slpsrvurl	*next;
  char			url[HTTP_MAX_URI];
} slpsrvurl_t;


/*
 * 'RegReportCallback()' - Empty SLPRegReport.
 */

void
RegReportCallback(SLPHandle hslp,
                  SLPError  errcode,
		  void      *cookie)
{
  (void)hslp;
  (void)errcode;
  (void)cookie;

  return;
}


/*
 * 'SendSLPBrowse()' - Register the specified printer with SLP.
 */

static void 
SendSLPBrowse(printer_t *p)		/* I - Printer to register */
{
  char		srvurl[HTTP_MAX_URI],	/* Printer service URI */
		attrs[8192],		/* Printer attributes */
		finishings[1024],	/* Finishings to support */
		make_model[IPP_MAX_NAME * 2],
					/* Make and model, quoted */
		location[IPP_MAX_NAME * 2],
					/* Location, quoted */
		info[IPP_MAX_NAME * 2],
					/* Info, quoted */
		*src,			/* Pointer to original string */
		*dst;			/* Pointer to destination string */
  ipp_attribute_t *authentication;	/* uri-authentication-supported value */
  SLPError	error;			/* SLP error, if any */


  LogMessage(L_DEBUG, "SendSLPBrowse(%p = \"%s\")", p, p->name);

 /*
  * Make the SLP service URL that conforms to the IANA 
  * 'printer:' template.
  */

  snprintf(srvurl, sizeof(srvurl), SLP_CUPS_SRVTYPE ":%s", p->uri);

  LogMessage(L_DEBUG2, "Service URL = \"%s\"", srvurl);

 /*
  * Figure out the finishings string...
  */

  if (p->type & CUPS_PRINTER_STAPLE)
    strcpy(finishings, "staple");
  else
    finishings[0] = '\0';

  if (p->type & CUPS_PRINTER_BIND)
  {
    if (finishings[0])
      strlcat(finishings, ",bind", sizeof(finishings));
    else
      strcpy(finishings, "bind");
  }

  if (p->type & CUPS_PRINTER_PUNCH)
  {
    if (finishings[0])
      strlcat(finishings, ",punch", sizeof(finishings));
    else
      strcpy(finishings, "punch");
  }

  if (p->type & CUPS_PRINTER_COVER)
  {
    if (finishings[0])
      strlcat(finishings, ",cover", sizeof(finishings));
    else
      strcpy(finishings, "cover");
  }

  if (p->type & CUPS_PRINTER_SORT)
  {
    if (finishings[0])
      strlcat(finishings, ",sort", sizeof(finishings));
    else
      strcpy(finishings, "sort");
  }

  if (!finishings[0])
    strcpy(finishings, "none");

 /*
  * Quote any commas in the make and model, location, and info strings...
  */

  for (src = p->make_model, dst = make_model;
       src && *src && dst < (make_model + sizeof(make_model) - 2);)
  {
    if (*src == ',' || *src == '\\' || *src == ')')
      *dst++ = '\\';

    *dst++ = *src++;
  }

  *dst = '\0';

  if (!make_model[0])
    strcpy(make_model, "Unknown");

  for (src = p->location, dst = location;
       src && *src && dst < (location + sizeof(location) - 2);)
  {
    if (*src == ',' || *src == '\\' || *src == ')')
      *dst++ = '\\';

    *dst++ = *src++;
  }

  *dst = '\0';

  if (!location[0])
    strcpy(location, "Unknown");

  for (src = p->info, dst = info;
       src && *src && dst < (info + sizeof(info) - 2);)
  {
    if (*src == ',' || *src == '\\' || *src == ')')
      *dst++ = '\\';

    *dst++ = *src++;
  }

  *dst = '\0';

  if (!info[0])
    strcpy(info, "Unknown");

 /*
  * Get the authentication value...
  */

  authentication = ippFindAttribute(p->attrs, "uri-authentication-supported",
                                    IPP_TAG_KEYWORD);

 /*
  * Make the SLP attribute string list that conforms to
  * the IANA 'printer:' template.
  */

  snprintf(attrs, sizeof(attrs),
           "(printer-uri-supported=%s),"
           "(uri-authentication-supported=%s>),"
#ifdef HAVE_SSL
           "(uri-security-supported=tls>),"
#else
           "(uri-security-supported=none>),"
#endif /* HAVE_SSL */
           "(printer-name=%s),"
           "(printer-location=%s),"
           "(printer-info=%s),"
           "(printer-more-info=%s),"
           "(printer-make-and-model=%s),"
	   "(charset-supported=utf-8),"
	   "(natural-language-configured=%s),"
	   "(natural-language-supported=de,en,es,fr,it),"
           "(color-supported=%s),"
           "(finishings-supported=%s),"
           "(sides-supported=one-sided%s),"
	   "(multiple-document-jobs-supported=true)"
	   "(ipp-versions-supported=1.0,1.1)",
	   p->uri, authentication->values[0].string.text, p->name, location,
	   info, p->uri, make_model, DefaultLanguage,
           p->type & CUPS_PRINTER_COLOR ? "true" : "false",
           finishings,
           p->type & CUPS_PRINTER_DUPLEX ?
	       ",two-sided-long-edge,two-sided-short-edge" : "");

  LogMessage(L_DEBUG2, "Attributes = \"%s\"", attrs);

 /*
  * Register the printer with the SLP server...
  */

  error = SLPReg(BrowseSLPHandle, srvurl, BrowseTimeout,
	         SLP_CUPS_SRVTYPE, attrs, SLP_TRUE, RegReportCallback, 0);

  if (error != SLP_OK)
    LogMessage(L_ERROR, "SLPReg of \"%s\" failed with status %d!", p->name,
               error);
}


/*
 * 'SLPDeregPrinter()' - SLPDereg() the specified printer
 */

void 
SLPDeregPrinter(printer_t *p)
{
  char	srvurl[HTTP_MAX_URI];	/* Printer service URI */


  if((p->type & CUPS_PRINTER_REMOTE) == 0)
  {
   /*
    * Make the SLP service URL that conforms to the IANA 
    * 'printer:' template.
    */

    snprintf(srvurl, sizeof(srvurl), SLP_CUPS_SRVTYPE ":%s", p->uri);

   /*
    * Deregister the printer...
    */

    SLPDereg(BrowseSLPHandle, srvurl, RegReportCallback, 0);
  }
}


/*
 * 'GetSlpAttrVal()' - Get an attribute from an SLP registration.
 */

int 					/* O - 0 on success */
GetSlpAttrVal(const char *attrlist,	/* I - Attribute list string */
              const char *tag,		/* I - Name of attribute */
              char       **valbuf)	/* O - Value */
{
  char	*ptr1,				/* Pointer into string */
	*ptr2;				/* ... */


  ClearString(valbuf);

  if ((ptr1 = strstr(attrlist, tag)) != NULL)
  {
    ptr1 += strlen(tag);

    if ((ptr2 = strchr(ptr1,')')) != NULL)
    {
     /*
      * Copy the value...
      */

      *valbuf = calloc(ptr2 - ptr1 + 1, 1);
      strncpy(*valbuf, ptr1, ptr2 - ptr1);

     /*
      * Dequote the value...
      */

      for (ptr1 = *valbuf; *ptr1; ptr1 ++)
	if (*ptr1 == '\\' && ptr1[1])
	  cups_strcpy(ptr1, ptr1 + 1);

      return (0);
    }
  }

  return (-1);
}


/*
 * 'AttrCallback()' - SLP attribute callback 
 */

SLPBoolean				/* O - SLP_TRUE for success */
AttrCallback(SLPHandle  hslp,		/* I - SLP handle */
             const char *attrlist,	/* I - Attribute list */
             SLPError   errcode,	/* I - Parsing status for this attr */
             void       *cookie)	/* I - Current printer */
{
  char         *tmp = 0;
  printer_t    *p = (printer_t*)cookie;


 /*
  * Let the compiler know we won't be using these...
  */

  (void)hslp;

 /*
  * Bail if there was an error
  */

  if (errcode != SLP_OK)
    return (SLP_TRUE);

 /*
  * Parse the attrlist to obtain things needed to build CUPS browse packet
  */

  memset(p, 0, sizeof(printer_t));

  p->type = CUPS_PRINTER_REMOTE;

  if (GetSlpAttrVal(attrlist, "(printer-location=", &(p->location)))
    return (SLP_FALSE);
  if (GetSlpAttrVal(attrlist, "(printer-info=", &(p->info)))
    return (SLP_FALSE);
  if (GetSlpAttrVal(attrlist, "(printer-make-and-model=", &(p->make_model)))
    return (SLP_FALSE);

  if (GetSlpAttrVal(attrlist, "(color-supported=", &tmp))
    return (SLP_FALSE);
  if (strcasecmp(tmp, "true") == 0)
    p->type |= CUPS_PRINTER_COLOR;

  if (GetSlpAttrVal(attrlist, "(finishings-supported=", &tmp))
    return (SLP_FALSE);
  if (strstr(tmp, "staple"))
    p->type |= CUPS_PRINTER_STAPLE;
  if (strstr(tmp, "bind"))
    p->type |= CUPS_PRINTER_BIND;
  if (strstr(tmp, "punch"))
    p->type |= CUPS_PRINTER_PUNCH;

  if (GetSlpAttrVal(attrlist, "(sides-supported=", &tmp))
    return (SLP_FALSE);
  if (strstr(tmp,"two-sided"))
    p->type |= CUPS_PRINTER_DUPLEX;

  ClearString(&tmp);

  return (SLP_TRUE);
}


/*
 * 'SrvUrlCallback()' - SLP service url callback
 */

SLPBoolean				/* O - TRUE = OK, FALSE = error */
SrvUrlCallback(SLPHandle      hslp, 	/* I - SLP handle */
               const char     *srvurl, 	/* I - URL of service */
               unsigned short lifetime,	/* I - Life of service */
               SLPError       errcode, 	/* I - Existing error code */
               void           *cookie)	/* I - Pointer to service list */
{
  slpsrvurl_t	*s,			/* New service entry */
		**head;			/* Pointer to head of entry */


 /*
  * Let the compiler know we won't be using these vars...
  */

  (void)hslp;
  (void)lifetime;

 /*
  * Bail if there was an error
  */

  if (errcode != SLP_OK)
    return (SLP_TRUE);

 /*
  * Grab the head of the list...
  */

  head = (slpsrvurl_t**)cookie;

 /*
  * Allocate a *temporary* slpsrvurl_t to hold this entry.
  */

  if ((s = (slpsrvurl_t *)calloc(1, sizeof(slpsrvurl_t))) == NULL)
    return (SLP_FALSE);

 /*
  * Copy the SLP service URL...
  */

  strlcpy(s->url, srvurl, sizeof(s->url));

 /* 
  * Link the SLP service URL into the head of the list
  */

  if (*head)
    s->next = *head;

  *head = s;

  return (SLP_TRUE);
}


/*
 * 'UpdateSLPBrowse()' - Get browsing information via SLP.
 */

void
UpdateSLPBrowse(void)
{
  slpsrvurl_t	*s,			/* Temporary list of service URLs */
		*next;			/* Next service in list */
  printer_t	p;			/* Printer information */
  const char	*uri;			/* Pointer to printer URI */
  char		method[HTTP_MAX_URI],	/* Method portion of URI */
		username[HTTP_MAX_URI],	/* Username portion of URI */
		host[HTTP_MAX_URI],	/* Host portion of URI */
		resource[HTTP_MAX_URI];	/* Resource portion of URI */
  int		port;			/* Port portion of URI */


  LogMessage(L_DEBUG, "UpdateSLPBrowse() Start...");

 /*
  * Reset the refresh time...
  */

  BrowseSLPRefresh = time(NULL) + BrowseInterval;

 /* 
  * Poll for remote printers using SLP...
  */

  s = NULL;

  SLPFindSrvs(BrowseSLPHandle, SLP_CUPS_SRVTYPE, "", "",
	      SrvUrlCallback, &s);

 /*
  * Loop through the list of available printers...
  */

  for (; s; s = next)
  {
   /*
    * Save the "next" pointer...
    */

    next = s->next;

   /* 
    * Load a printer_t structure with the SLP service attributes...
    */

    SLPFindAttrs(BrowseSLPHandle, s->url, "", "", AttrCallback, &p);

   /*
    * Process this printer entry...
    */

    uri = s->url + SLP_CUPS_SRVLEN + 1;

    if (strncmp(uri, "http://", 7) == 0 ||
        strncmp(uri, "ipp://", 6) == 0)
    {
     /*
      * Pull the URI apart to see if this is a local or remote printer...
      */

      httpSeparate(uri, method, username, host, &port, resource);

      if (strcasecmp(host, ServerName) == 0)
	continue;

     /*
      * OK, at least an IPP printer, see if it is a CUPS printer or
      * class...
      */

      if (strstr(uri, "/printers/") != NULL)
        ProcessBrowseData(uri, p.type, IPP_PRINTER_IDLE, p.location,
	                  p.info, p.make_model, BROWSE_SLP);
      else if (strstr(uri, "/classes/") != NULL)
        ProcessBrowseData(uri, p.type | CUPS_PRINTER_CLASS, IPP_PRINTER_IDLE,
	                  p.location, p.info, p.make_model, BROWSE_SLP);
    }

   /*
    * Free this listing...
    */

    free(s);
  }       

  LogMessage(L_DEBUG, "UpdateSLPBrowse() End...");
}
#endif /* HAVE_LIBSLP */


/*
 * 'BrowseRegisterPrinter()' - Start sending broadcast information for a printer
 *				or update the broadcast contents.
 */

void BrowseRegisterPrinter(printer_t *p)
{
  if (!Browsing || !BrowseProtocols || !BrowseInterval || !Browsers ||
      (p->type & (CUPS_PRINTER_REMOTE | CUPS_PRINTER_IMPLICIT)))
    return;

#ifdef HAVE_MDNS
    if ((BrowseProtocols & BROWSE_MDNS))
      mDNSRegisterPrinter(p);    
#endif /* HAVE_MDNS */
}


/*
 * 'BrowseDeregisterPrinter()' - Stop sending broadcast information for a 
 *				 local printer and remove any pending references
 *				 to remote printers.
 */

void BrowseDeregisterPrinter(printer_t *p)
{
#ifdef HAVE_MDNS
    if ((BrowseProtocols & BROWSE_MDNS))
      mDNSDeregisterPrinter(p);
#endif /* HAVE_MDNS */
}


#ifdef HAVE_MDNS

/* =============================== HAVE_MDNS ============================== */

/*
 * 'mDNSRegisterPrinter()' - Start sending broadcast information for a printer
 *				or update the broadcast contents.
 */

static void 
mDNSRegisterPrinter(printer_t *p)
{
  DNSServiceErrorType	se;		/* mDNS errors */
  char			*txt_record,	/* TXT record buffer */
			*name;		/* Service name */
  int			txt_len;	/* TXT record length */

  if (!mDNSBrowsing)
    return;

  LogMessage(L_DEBUG, "mDNSRegisterPrinter(%s) %s\n", p->name, !p->mdns_lpd_ref ? "new" : "update");

 /*
  * If per-printer sharing is disabled make sure we're not registered before returning
  */

  if (!p->shared)
  {
    if (p->mdns_lpd_ref)
      mDNSDeregisterPrinter(p);
    return;
  }

 /*
  * If an existing printer was renamed just start over
  */

  name = (p->info && strlen(p->info)) ? p->info : p->name;

  if (p->reg_name && strcmp(p->reg_name, name))
    mDNSDeregisterPrinter(p);

  if (p->mdns_lpd_ref == NULL)
  {
   /*
    * Initial _printer (lpd) registration. The port is zero to register this
    * as a "placeholder service."
    */

    SetString(&p->reg_name, name);

    if ((se = DNSServiceRegister(&p->mdns_lpd_ref, 0, 0, name, mDNSLPDRegType,
				NULL, NULL, 0, 0, NULL,
				mDNSRegisterCallback, p)) == 0)
    {
      p->mdns_lpd_fd = DNSServiceRefSockFD(p->mdns_lpd_ref);

      LogMessage(L_DEBUG2, "mDNSRegisterPrinter: Adding fd %d to InputSet...",
               p->mdns_lpd_fd);

      FD_SET(p->mdns_lpd_fd, InputSet);
    }
  }

  if (p->mdns_ipp_ref != NULL)
  {
   /*
    * Update existing _ipp registration
    */

    txt_record = mDNSBuildTxtRecord(&txt_len, p);

    if (txt_len != p->ipp_txt_len || memcmp(txt_record, p->ipp_txt_record, txt_len) != 0)
    {
      free(p->ipp_txt_record);
      p->ipp_txt_record = txt_record;
      p->ipp_txt_len = txt_len;

      /* A TTL of 0 means use record's original value (Radar 3176248) */
      se = DNSServiceUpdateRecord(p->mdns_ipp_ref, NULL, 0,
				    txt_len, txt_record, 0);
    }
    else
    {
      free(txt_record);
    }
  }
}


/*
 * 'mDNSDeregisterPrinter()' - Stop sending broadcast information for a printer
 */

static void mDNSDeregisterPrinter(printer_t *p)
{
  LogMessage(L_DEBUG, "mDNSDeregisterPrinter(%s)", p->name);

 /*
  * Closing the socket deregisters the service
  */

  if (p->mdns_ipp_ref)
  {
    LogMessage(L_DEBUG2, "mDNSDeregisterPrinter: Removing fd %d from InputSet...",
	       p->mdns_ipp_fd);

    FD_CLR(p->mdns_ipp_fd, InputSet);
    DNSServiceRefDeallocate(p->mdns_ipp_ref);
    p->mdns_ipp_ref = NULL;
    p->mdns_ipp_fd = -1;
  }

  if (p->mdns_lpd_ref)
  {
    LogMessage(L_DEBUG2, "mDNSDeregisterPrinter: Removing fd %d from InputSet...",
	       p->mdns_lpd_fd);

    FD_CLR(p->mdns_lpd_fd, InputSet);
    DNSServiceRefDeallocate(p->mdns_lpd_ref);
    p->mdns_lpd_ref = NULL;
    p->mdns_lpd_fd = -1;
  }

  if (p->mdns_query_ref)
  {
    LogMessage(L_DEBUG2, "mDNSDeregisterPrinter: Removing fd %d from InputSet...",
	       p->mdns_query_fd);

    FD_CLR(p->mdns_query_fd, InputSet);
    DNSServiceRefDeallocate(p->mdns_query_ref);
    p->mdns_query_ref = NULL;
    p->mdns_query_fd = -1;
  }

  ClearString(&p->reg_name);
  ClearString(&p->service_name);
  ClearString(&p->host_target);
  ClearString(&p->ipp_txt_record);

  p->browse_protocol &= ~BROWSE_MDNS;
}


/*
 * 'mDNSRegisterCallback()' - DNSServiceRegister callback
 */

static void mDNSRegisterCallback(
    DNSServiceRef	sdRef,		/* I - DNS Service reference	*/
    DNSServiceFlags	flags,		/* I - Reserved for future use	*/
    DNSServiceErrorType	errorCode,	/* I - Error code		*/
    const char		*name,     	/* I - Service name		*/
    const char		*regtype,  	/* I - Service type		*/
    const char		*domain,   	/* I - Domain. ".local" for now */
    void		*context)	/* I - User-defined context	*/
{
  DNSServiceErrorType	se;		/* mDNS errors */
  char			*txt_record;	/* TXT record buffer */
  int			txt_len;	/* TXT record length */
  printer_t		*p;		/* Printer information */

  LogMessage(L_DEBUG, "mDNSRegisterCallback(%s, %s)\n", name, regtype);

  if (errorCode != 0)
  {
    LogMessage(L_ERROR, "DNSServiceRegister failed with error %d", (int)errorCode);
    return;
  }

  p = (printer_t*)context;

  /*
  *  If this is the registration callback for printer (lpd) then proceed to
  *  register ipp
  */

  if (sdRef == p->mdns_lpd_ref)
  {
    SetString(&p->service_name, name);

    txt_record = mDNSBuildTxtRecord(&txt_len, p);

    if ((se = DNSServiceRegister(&p->mdns_ipp_ref, flags, 0, name, mDNSIPPRegType,
			    NULL, NULL, 631, txt_len, txt_record,
			    mDNSRegisterCallback, context)) == 0)
    {
      p->ipp_txt_record = txt_record;
      p->ipp_txt_len = txt_len;
      p->mdns_ipp_fd = DNSServiceRefSockFD(p->mdns_ipp_ref);

      LogMessage(L_DEBUG2, "mDNSRegisterCallback: Adding fd %d to InputSet...",
               p->mdns_ipp_fd);

      FD_SET(p->mdns_ipp_fd, InputSet);
    }
    else
      free(txt_record);
  }
}


/*
 * 'mDNSBrowseCallback()' - DNSServiceBrowse callback
 */

static void mDNSBrowseCallback(
    DNSServiceRef		sdRef,		/* I - DNS Service reference	*/
    DNSServiceFlags		flags,		/* I - Reserved for future use	*/
    uint32_t			interfaceIndex,	/* I - 0 for all interfaces	*/
    DNSServiceErrorType		errorCode,	/* I - Error code		*/
    const char			*service_name,	/* I - Service name		*/
    const char			*regtype,  	/* I - Service type		*/
    const char			*replyDomain,   /* I - Domain. ".local" for now */
    void			*context)	/* I - User-defined context	*/
{
  DNSServiceErrorType	se;			/* mDNS errors		*/
  printer_t		*p;			/* Printer information	*/
  mdns_resolve_t	*mdns_resolve,		/* Resolve request */
    			*prev;			/* Previous resolve request */

  LogMessage(L_DEBUG, "mDNSBrowseCallback(%s) 0x%X %s\n", service_name, (unsigned int)flags,
		(flags & kDNSServiceFlagsAdd) ? "Add" : "Remove" );

  if (errorCode != 0)
  {
    LogMessage(L_ERROR, "DNSServiceBrowse failed with error %d", (int)errorCode);
    return;
  }

  /*
   * Try to match this service name to an existing printer's service name
   */

  for (p = Printers; p != NULL; p = p->next)
    if (p->service_name && strcmp(service_name, p->service_name) == 0)
      break;

  if ( (flags & kDNSServiceFlagsAdd) )
  {
    if (p)
    {
      LogMessage(L_DEBUG, "mDNSBrowseCallback() ignoring existing printer");
      return;
    }

   /*
    * Resolve this service to get the host name
    */

    mdns_resolve = calloc(1, sizeof(mdns_resolve_t));
    if (mdns_resolve != NULL)
    {
      if ((se = DNSServiceResolve(&mdns_resolve->sdRef, flags, interfaceIndex, service_name,
			   regtype, replyDomain, mDNSResolveCallback, NULL)) == 0)
      {
	mdns_resolve->fd = DNSServiceRefSockFD(mdns_resolve->sdRef);

        LogMessage(L_DEBUG2, "mDNSBrowseCallback: Adding fd %d to InputSet...",
		   mdns_resolve->fd);

        FD_SET(mdns_resolve->fd, InputSet);

       /*
        * We'll need the service name in the resolve callback
        */

	SetString(&mdns_resolve->service_name, service_name);

       /*
        * Insert it into the pending resolve list
        */

	mdns_resolve->next = MDNSPendingResolves;
	MDNSPendingResolves = mdns_resolve;
      }
      else
        free(mdns_resolve);
    }
  }
  else
  {
    if (p && (p->type & CUPS_PRINTER_REMOTE))
    {
      LogMessage(L_INFO, "Remote destination \"%s\" unregistered; deleting it...",
			p->name);
      DeletePrinter(p, 1);
    }
    else
    {
     /*
      * Remove any pending resolve requests for this service
      */

      for (prev = NULL, mdns_resolve = MDNSPendingResolves; mdns_resolve; prev = mdns_resolve, mdns_resolve = mdns_resolve->next)
	if (!strcmp(service_name, mdns_resolve->service_name))
	{
	  if (prev == NULL)
	    MDNSPendingResolves = mdns_resolve->next;
	  else
	    prev->next = mdns_resolve->next;

	  LogMessage(L_DEBUG2, "mDNSBrowseCallback: Removing fd %d from InputSet...",
		     mdns_resolve->fd);

	  FD_CLR(mdns_resolve->fd, InputSet);
	  DNSServiceRefDeallocate(mdns_resolve->sdRef);

	  free(mdns_resolve);
	  break;
	}
    }
  }
}


/*
 * 'mDNSResolveCallback()' - DNSServiceResolve callback
 */

static void mDNSResolveCallback(
    DNSServiceRef		sdRef,		/* I - DNS Service reference	*/
    DNSServiceFlags		flags,		/* I - Reserved for future use	*/
    uint32_t			interfaceIndex,	/* I - 0 for all interfaces	*/
    DNSServiceErrorType		errorCode,	/* I - Error code		*/
    const char			*fullname,	/* I - Service name		*/   
    const char			*host_target,	/* I - Hostname			*/
    uint16_t			port,		/* I - Port			*/
    uint16_t			txt_len,		/* I - TXT record length	*/
    const char			*txtRecord,	/* I - TXT reocrd		*/
    void			*context)	/* I - User-defined context	*/
{
  DNSServiceErrorType	se;			/* mDNS errors */
  char			uri[1024];		/* IPP URI */
  char			*txtvers,		/* TXT record version */
  			*rp,			/* Name */
  			*make_model,		/* Make and model */
  			*location,		/* Location */
  			*state_str,		/* State buffer */
			*type_str,		/* Type buffer */
  			*info;			/* Printer information */
  ipp_pstate_t		state;			/* State */
  unsigned int		type;			/* Type */
  printer_t		*p;			/* Printer information */
  mdns_resolve_t	*mdns_resolve,		/* Current resolve request */
    			*prev;			/* Previous resolve request */

  LogMessage(L_DEBUG, "mDNSResolveCallback(%s)\n", fullname);

  /*
   * Find the matching request
   */

  for (prev = NULL, mdns_resolve = MDNSPendingResolves; mdns_resolve; prev = mdns_resolve, mdns_resolve = mdns_resolve->next)
    if (sdRef == mdns_resolve->sdRef)
      break;

  if (!mdns_resolve)
  {
    LogMessage(L_ERROR, "mDNSResolveCallback missing request!");
    return;
  }

 /*
  * Remove it from the pending list
  */

  if (prev == NULL)
    MDNSPendingResolves = mdns_resolve->next;
  else
    prev->next = mdns_resolve->next;

  LogMessage(L_DEBUG2, "mDNSResolveCallback: Removing fd %d from InputSet...",
	     mdns_resolve->fd);

  FD_CLR(mdns_resolve->fd, InputSet);
  DNSServiceRefDeallocate(mdns_resolve->sdRef);

  if (errorCode != 0)
  {
    LogMessage(L_ERROR, "DNSServiceResolve failed with error %d", (int)errorCode);
    ClearString(&mdns_resolve->service_name);
    free(mdns_resolve);
    return;
  }

 /*
  * Search the TXT record for the keys we're interested in
  */

  mDNSFindAttr(txtRecord, txt_len, "txtvers", &txtvers);
  mDNSFindAttr(txtRecord, txt_len, "rp", &rp);
  mDNSFindAttr(txtRecord, txt_len, "ty", &make_model);
  mDNSFindAttr(txtRecord, txt_len, "note", &location);
  mDNSFindAttr(txtRecord, txt_len, "printer-state", &state_str);
  mDNSFindAttr(txtRecord, txt_len, "printer-type", &type_str);

  if (type_str && state_str && rp)
  {
    type = strtod(type_str, NULL);
    state = strtod(state_str, NULL);

    snprintf(uri, sizeof(uri), "ipp://%s/%s/%s", host_target,
	   (type & CUPS_PRINTER_CLASS) ? "classes" : "printers", rp);

   /*
    * If the service name matches the queue name (rp) then set info to NULL,
    * otherwise set it to the service name.
    */

    info = !strcmp(mdns_resolve->service_name, rp) ? NULL : mdns_resolve->service_name;

    p = ProcessBrowseData(uri, type, state, location, info, make_model, BROWSE_MDNS);

    /*
     * If we matched an existing printer or created a new one set it's service name.
     */

    if (p)
    {
      SetString(&p->service_name, mdns_resolve->service_name);
      SetString(&p->host_target, host_target);

      if ((se = DNSServiceQueryRecord(&p->mdns_query_ref, 0, 0, fullname, ns_t_txt, ns_c_in, 
    				mDNSQueryRecordCallback, p)) == 0)
      {
        p->mdns_query_fd = DNSServiceRefSockFD(p->mdns_query_ref);

        LogMessage(L_DEBUG2, "mDNSResolveCallback: Adding fd %d to InputSet...",
		   p->mdns_query_fd);

        FD_SET(p->mdns_query_fd, InputSet);
      }
    }
  }
  else
    LogMessage(L_DEBUG, "mDNSResolveCallback missing TXT record keys");

  ClearString(&txtvers);
  ClearString(&rp);
  ClearString(&make_model);
  ClearString(&location);
  ClearString(&state_str);
  ClearString(&type_str);

  ClearString(&mdns_resolve->service_name);
  free(mdns_resolve);
}


/*
 * 'mDNSQueryRecordCallback()' - DNSServiceQueryRecord callback
 */

static void mDNSQueryRecordCallback(
    DNSServiceRef		sdRef,		/* I - DNS Service reference */
    DNSServiceFlags		flags,		/* I - Reserved for future use	*/
    uint32_t			interfaceIndex,	/* I - 0 for all interfaces	*/
    DNSServiceErrorType		errorCode,	/* I - Error code		*/
    const char			*fullname,	/* I - Service name		*/   
    uint16_t			rrtype,		/* I - */
    uint16_t			rrclass,	/* I - */
    uint16_t			rdlen,		/* I - */
    const void			*rdata,		/* I - */
    uint32_t			ttl,		/* I - */
    void 			*context)	/* I - */
{
  char		uri[1024];		/* IPP URI */
  char		*txtvers,		/* TXT record version */
  		*rp,			/* Name */
  		*make_model,		/* Make and model */
  		*location,		/* Location */
  		*state_str,		/* State buffer */
		*type_str;		/* Type buffer */
  ipp_pstate_t	state;			/* State */
  unsigned int	type;			/* Type */
  printer_t	*p;			/* Printer information */

  LogMessage(L_DEBUG, "mDNSQueryRecordCallback(%s) %s\n", fullname, (flags & kDNSServiceFlagsAdd) ? "Add" : "Remove");

  if (errorCode != 0)
  {
    LogMessage(L_ERROR, "DNSServiceQueryRecord failed with error %d", (int)errorCode);
    return;
  }


  if ((flags & kDNSServiceFlagsAdd))
  {
    p = (printer_t *)context;

    /*
     * Search the TXT record for the keys we're interested in
     */

    mDNSFindAttr(rdata, rdlen, "txtvers", &txtvers);
    mDNSFindAttr(rdata, rdlen, "rp", &rp);
    mDNSFindAttr(rdata, rdlen, "ty", &make_model);
    mDNSFindAttr(rdata, rdlen, "note", &location);
    mDNSFindAttr(rdata, rdlen, "printer-state", &state_str);
    mDNSFindAttr(rdata, rdlen, "printer-type", &type_str);

    if (type_str && state_str && rp)
    {
      type = strtod(type_str, NULL);
      state = strtod(state_str, NULL);

      snprintf(uri, sizeof(uri), "ipp://%s/%s/%s", p->host_target,
		   (type & CUPS_PRINTER_CLASS) ? "classes" : "printers", rp);

      ProcessBrowseData(uri, type, state, location, p->info, make_model, BROWSE_MDNS);
    }
    else
      LogMessage(L_DEBUG, "mDNSQueryRecordCallback missing TXT record keys");

    ClearString(&txtvers);
    ClearString(&rp);
    ClearString(&make_model);
    ClearString(&location);
    ClearString(&state_str);
    ClearString(&type_str);
  }
}


/*
 * 'mDNSBuildTxtRecord()' - Build a TXT record from printer info
 */

static char *mDNSBuildTxtRecord(int *txt_len,		/* O - TXT record length */
				printer_t *p)		/* I - Printer information */
{
  int		i;			/* Looping var */
  char		type_str[32],		/* Type to string buffer */
		state_str[32],		/* State to string buffer */
		*keyvalue[32][2];	/* Table of key/value pairs */

  i = 0;

  /*
  *  Load up the key value pairs...
  */

  keyvalue[i  ][0] = "txtvers";
  keyvalue[i++][1] = "1";

  keyvalue[i  ][0] = "rp";
  keyvalue[i++][1] = p->name;

  keyvalue[i  ][0] = "ty";
  keyvalue[i++][1] = p->make_model;

  if (p->location && *p->location != '\0')
  {
    keyvalue[i  ][0] = "note";
    keyvalue[i++][1] = p->location;
  }

  keyvalue[i  ][0] = "product";
  keyvalue[i++][1] = p->product ? p->product : "Unknown";

  snprintf(type_str,  sizeof(type_str),  "0x%X", p->type | CUPS_PRINTER_REMOTE);
  snprintf(state_str, sizeof(state_str), "%d", p->state);

  keyvalue[i  ][0] = "printer-state";
  keyvalue[i++][1] = state_str;

  keyvalue[i  ][0] = "printer-type";
  keyvalue[i++][1] = type_str;

  keyvalue[i  ][0] = "Transparent";
  keyvalue[i++][1] = "T";

  keyvalue[i  ][0] = "Binary";
  keyvalue[i++][1] = "T";

  if ((p->type & CUPS_PRINTER_FAX))
  {
    keyvalue[i  ][0] = "Fax";
    keyvalue[i++][1] = "T";
  }

  if ((p->type & CUPS_PRINTER_COLOR))
  {
    keyvalue[i  ][0] = "Color";
    keyvalue[i++][1] = "T";
  }

  if ((p->type & CUPS_PRINTER_DUPLEX))
  {
    keyvalue[i  ][0] = "Duplex";
    keyvalue[i++][1] = "T";
  }

  if ((p->type & CUPS_PRINTER_STAPLE))
  {
    keyvalue[i  ][0] = "Staple";
    keyvalue[i++][1] = "T";
  }

  if ((p->type & CUPS_PRINTER_COPIES))
  {
    keyvalue[i  ][0] = "Copies";
    keyvalue[i++][1] = "T";
  }

  if ((p->type & CUPS_PRINTER_COLLATE))
  {
    keyvalue[i  ][0] = "Collate";
    keyvalue[i++][1] = "T";
  }

  if ((p->type & CUPS_PRINTER_PUNCH))
  {
    keyvalue[i  ][0] = "Punch";
    keyvalue[i++][1] = "T";
  }

  if ((p->type & CUPS_PRINTER_BIND))
  {
    keyvalue[i  ][0] = "Bind";
    keyvalue[i++][1] = "T";
  }

  if ((p->type & CUPS_PRINTER_SORT))
  {
    keyvalue[i  ][0] = "Sort";
    keyvalue[i++][1] = "T";
  }

  keyvalue[i  ][0] = "pdl";
  keyvalue[i++][1] = p->pdl ? p->pdl : "application/postscript";

 /*
  * Then pack them into a proper txt record...
  */

  return mDNSPackTxtRecord(txt_len, keyvalue, i);
}


/*
 * 'mDNSPackTxtRecord()' - Pack an array of key/value pairs into the TXT record format
 */

static char *mDNSPackTxtRecord(int *txt_len,		/* O - TXT record length	*/
			      char *keyvalue[][2],	/* I - Table of key value pairs	*/
			      int count)		/* I - Items in table		*/
{
  int  index;			/* Looping var */
  int  length;			/* Length of TXT record */
  char *txtRecord;		/* TXT record buffer */
  char *cursor;			/* Looping pointer */

  /*
   * Calculate the buffer size
   */

  for (length = index = 0; index < count; index++)
    length += 1 + strlen(keyvalue[index][0]) + 
	      (keyvalue[index][1] ? 1 + strlen(keyvalue[index][1]) : 0);

  /*
   * Allocate and fill it
   */

  txtRecord = malloc(length);
  if (txtRecord)
  {
    *txt_len = length;

    for (cursor = txtRecord, index = 0; index < count; index++)
    {
      /*
       * Drop in the p-string style length byte followed by the data
       */
      *cursor++ = (unsigned char)(strlen(keyvalue[index][0]) + 
				(keyvalue[index][1] ? 1 + strlen(keyvalue[index][1]) : 0));

      length = strlen(keyvalue[index][0]);      
      memcpy(cursor, keyvalue[index][0], length);
      cursor += length;

      if (keyvalue[index][1])
      {
	*cursor++ = '=';
	length = strlen(keyvalue[index][1]);
	memcpy(cursor, keyvalue[index][1], length);
	cursor += length;
      }
    }
  }

  return txtRecord;
}


/*
 * mDNSFindAttr()' - Find a TXT record attribute value
 */

static int mDNSFindAttr(const unsigned char *txtRecord,	/* I - TXT record to search */
			int txt_len, 			/* I - Length of TXT record */
			const char *key,		/* I - Key to match */
			char **value)			/* O - Value string */
{
  int 	result = -1;			/* Return result */
  int 	keyLen;				/* Key length */
  int 	valueLen;			/* Value length */
  const unsigned char *txtRecordEnd;	/* End of TXT record */

  *value = NULL;

  /*
   * Walks the list of p-strings to find the matching key /value
   */

  keyLen = strlen(key);
  txtRecordEnd = txtRecord + txt_len;

  for (txtRecordEnd = txtRecord + txt_len; txtRecord < txtRecordEnd; txtRecord = txtRecord + *txtRecord + 1)
  {
    if (*txtRecord >= keyLen && memcmp(key, txtRecord+1, keyLen) == 0 && (*txtRecord == keyLen || *(txtRecord + keyLen + 1) == '='))
    {
      result = 0;						/* Found the key we're looking for */
      valueLen = *txtRecord - keyLen - 1;

      if (valueLen < 0)
        result = -2;						/* Malformed TXT record */
      else
      {
        if ((*value = malloc(valueLen + 1)) == NULL)
          result = -3;
        else
        {
	  memcpy(*value, txtRecord + keyLen + 2, valueLen);	/* Copy the results as a C-string */
	  (*value)[valueLen] = '\0';
	}
      }
      break;
    }
  }

  return result;
}

#endif /* HAVE_MDNS */

/*
 * End of "$Id: dirsvc.c,v 1.17 2004/06/05 03:49:46 jlovell Exp $".
 */
