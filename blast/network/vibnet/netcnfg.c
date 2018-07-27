/*   netcnfg.c
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
 * RCS $Id: netcnfg.c,v 6.14 1999/08/31 22:25:23 kans Exp $
 *
 * Author:  Kans, Epstein
 *
 * Version Creation Date:   9/10/96
 *
 * File Description:
 *       Network Entrez configuration
 *
 * Modifications:
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 */

#include <vibrant.h>
#include <netcnfg.h>

typedef struct netconfigdata {
  FORM_MESSAGE_BLOCK

  GrouP           srvConnMode;
  GrouP           netGroup;

  TexT            proxyHost;
  TexT            proxyPort;
  TexT            firewallProxy;

  Char            proxyValue [256];
  Char            portValue [16];
  Char            firewallValue [256];

  ButtoN          dnsAvailable;
  PopuP           timeOut;

  VoidProc        accepted;
  VoidProc        cancelled;
  VoidProc        turnedOff;

  ButtoN          accept;
} NetConfigData, PNTR NetConfigPtr;

static void ConfigMessageProc (ForM f, Int2 mssg)

{
  VoidProc        cancelled;
  NetConfigPtr    ncp;

  ncp = (NetConfigPtr) GetObjectExtra (f);
  if (ncp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE:
        cancelled = ncp->cancelled;
        Remove (f);
        if (cancelled != NULL) {
          cancelled ();
        }
        break;
      case VIB_MSG_CUT:
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY:
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE:
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE:
        StdDeleteTextProc (NULL);
        break;
      default:
        if (ncp->appmessage != NULL) {
          ncp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void ConfigFormActivate (WindoW w)

{
  NetConfigPtr    ncp;

  ncp = (NetConfigPtr) GetObjectExtra (w);
  if (ncp != NULL) {
    if (ncp->activate != NULL) {
      ncp->activate (w);
    }
  }
}

static Boolean NoEntryExists (CharPtr type)

{
  Char  str [256];

  if (GetAppParam ("NCBI", "NET_SERV", type, NULL, str, sizeof (str))) {
    if (! StringHasNoText (str)) return FALSE;
  }
  return TRUE;
}

static void AcceptNetConfigForm (ButtoN b)

{
  VoidProc      accepted;
  NetConfigPtr  ncp;
  Char          str [256];
  VoidProc      turnedOff;
  Int2          val;

  ncp = (NetConfigPtr) GetObjectExtra (b);
  if (ncp == NULL) return;

  val = GetValue (ncp->srvConnMode);
  if (val == 1) {
    turnedOff = ncp->turnedOff;
    Remove (ncp->form);
    if (turnedOff != NULL) {
      turnedOff ();
    }
    Update ();
    return;
  } else if (val == 3) {
    SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_MODE", "FIREWALL");
    GetTitle (ncp->proxyHost, str, sizeof (str));
    if (! StringHasNoText (str)) {
      SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_HOST", str);
      GetTitle (ncp->proxyPort, str, sizeof (str));
      if (StringICmp (str, "80") == 0) {
        str [0] = '\0';
      }
      if (! StringHasNoText (str)) {
        SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", str);
      } else {
        SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", NULL);
      }
    } else {
      SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_HOST", NULL);
      SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", NULL);
    }
    GetTitle (ncp->firewallProxy, str, sizeof (str));
    if (! StringHasNoText (str)) {
      SetAppParam ("NCBI", "NET_SERV", "SRV_PROXY_HOST", str);
    } else {
      SetAppParam ("NCBI", "NET_SERV", "SRV_PROXY_HOST", NULL);
    }
  } else {
    SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_MODE", NULL);
    SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_HOST", NULL);
    SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", NULL);
    SetAppParam ("NCBI", "NET_SERV", "SRV_PROXY_HOST", NULL);
  }

  if (GetStatus (ncp->dnsAvailable)) {
    SetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_HOST", NULL);
  } else {
    SetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_HOST", "130.14.22.106");
  }

  switch (GetValue (ncp->timeOut)) {
    case 1 :
      SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", "10");
      break;
    case 3 :
      SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", "60");
      break;
    case 4 :
      SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", "300");
      break;
    default :
      SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", NULL);
      break;
  }

  if (GetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_PORT", NULL, str, sizeof (str))) {
    if (StringICmp (str, "80") == 0) {
      SetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_PORT", NULL);
    }
  }

  if (GetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_URL", NULL, str, sizeof (str))) {
    if (StringICmp (str, "/Service/nph-dispd.cgi") == 0) {
      SetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_URL", NULL);
    }
  }

  if (NoEntryExists ("SRV_CONN_MODE") &&
      NoEntryExists ("SRV_CONN_TIMEOUT") &&
      NoEntryExists ("SRV_ENGINE_HOST") &&
      NoEntryExists ("SRV_ENGINE_PORT") &&
      NoEntryExists ("SRV_ENGINE_URL") &&
      NoEntryExists ("SRV_HTTP_PROXY_HOST") &&
      NoEntryExists ("SRV_HTTP_PROXY_PORT") &&
      NoEntryExists ("SRV_PROXY_HOST")) {
    SetAppParam ("NCBI", "NET_SERV", NULL, NULL);
  }

  accepted = ncp->accepted;
  Remove (ncp->form);
  if (accepted != NULL) {
    accepted ();
  }
  Update ();
}

static void ChangeConfigControls (NetConfigPtr ncp)

{
  Boolean  hasProxy;
  Int2     val;

  if (ncp == NULL) return;
  val = GetValue (ncp->srvConnMode);
  if (val == 1) {
    SafeHide (ncp->netGroup);
  }
  if (val == 3) {
    if (! Enabled (ncp->proxyHost)) {
      SafeSetTitle (ncp->proxyHost, ncp->proxyValue);
    }
    Enable (ncp->proxyHost);
    if (! Enabled (ncp->firewallProxy)) {
      SafeSetTitle (ncp->firewallProxy, ncp->firewallValue);
    }
    Enable (ncp->firewallProxy);
  } else {
    if (Enabled (ncp->proxyHost)) {
      GetTitle (ncp->proxyHost, ncp->proxyValue, sizeof (ncp->proxyValue));
      SafeSetTitle (ncp->proxyHost, NULL);
    }
    Disable (ncp->proxyHost);
    if (Enabled (ncp->firewallProxy)) {
      GetTitle (ncp->firewallProxy, ncp->firewallValue, sizeof (ncp->firewallValue));
      SafeSetTitle (ncp->firewallProxy, NULL);
    }
    Disable (ncp->firewallProxy);
  }
  hasProxy = (Boolean) (! TextHasNoText (ncp->proxyHost));
  if (hasProxy) {
    if (! Enabled (ncp->proxyHost)) {
      SafeSetTitle (ncp->proxyHost, ncp->proxyValue);
    }
    Enable (ncp->proxyHost);
    if (! Enabled (ncp->proxyPort)) {
      SafeSetTitle (ncp->proxyPort, ncp->portValue);
    }
    Enable (ncp->proxyPort);
  } else {
    if (Enabled (ncp->proxyPort)) {
      GetTitle (ncp->proxyPort, ncp->portValue, sizeof (ncp->portValue));
      SafeSetTitle (ncp->proxyPort, NULL);
    }
    Disable (ncp->proxyPort);
  }
  if (val != 1) {
    SafeShow (ncp->netGroup);
  }
}

static void ChangeConnection (GrouP g)

{
  NetConfigPtr  ncp;

  ncp = (NetConfigPtr) GetObjectExtra (g);
  ChangeConfigControls (ncp);
}

static void ChangeProxy (TexT t)

{
  NetConfigPtr  ncp;

  ncp = (NetConfigPtr) GetObjectExtra (t);
  ChangeConfigControls (ncp);
}

extern void ShowNetConfigForm (WndActnProc activate, FormMessageFunc messages,
                               VoidProc accepted, VoidProc cancelled,
                               VoidProc turnedOff, Boolean netCurrentlyOn)
{
  ButtoN        b;
  GrouP         c;
  GrouP         g;
  GrouP         h;
  GrouP         j;
  Int2          len;
  NetConfigPtr  ncp;
  PrompT        ppt0, ppt1;
  ButtoN        rb;
  Char          str [256];
  WindoW        w;
  GrouP         x;
  GrouP         z;

  w = NULL;
  ncp = (NetConfigPtr) MemNew (sizeof (NetConfigData));
  if (ncp != NULL) {

    w = FixedWindow (-50, -33, -10, -10, "Network Configuration",
                     StdSendCloseWindowMessageProc);
    SetObjectExtra (w, ncp, StdCleanupFormProc);
    ncp->form = (ForM) w;
    ncp->formmessage = ConfigMessageProc;

    ncp->appmessage = messages;
    ncp->activate = activate;
    SetActivate (w, ConfigFormActivate);

    ncp->accepted = accepted;
    ncp->cancelled = cancelled;
    ncp->turnedOff = turnedOff;

    h = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h, 5, 10);

    j = HiddenGroup (h, 2, 0, NULL);
    ppt0 = StaticPrompt (j, "Connection", 0, 0, programFont, 'l');
    ncp->srvConnMode = HiddenGroup (j, 4, 0, ChangeConnection);
    SetObjectExtra (ncp->srvConnMode, ncp, NULL);
    rb = RadioButton (ncp->srvConnMode, "None");
    RadioButton (ncp->srvConnMode, "Normal");
    RadioButton (ncp->srvConnMode, "Firewall");
    /* RadioButton (ncp->srvConnMode, "Stateless"); */
    SetValue (ncp->srvConnMode, 2);
    if (turnedOff == NULL) {
      Disable (rb);
    }
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt0, (HANDLE) ncp->srvConnMode, NULL);

    ncp->netGroup = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (ncp->netGroup, 5, 10);
  
    SelectFont (programFont);
    len = StringWidth ("HTTP Proxy Server ") + 2;
    SelectFont (systemFont);

    z = HiddenGroup (ncp->netGroup, -2, 0, NULL);
    StaticPrompt (z, "HTTP Proxy Server", len, dialogTextHeight, programFont, 'l');
    ncp->proxyHost = DialogText (z, "", 12, ChangeProxy);
    SetObjectExtra (ncp->proxyHost, ncp, NULL);
    /*
    StaticPrompt (z, "", 0, 0, programFont, 'l');
    StaticPrompt (z, "", 0, 0, programFont, 'l');
    */
    ppt0 = StaticPrompt (z, "HTTP Proxy Port ", len, dialogTextHeight, programFont, 'l');
    ncp->proxyPort = DialogText (z, "", 3, NULL);
    x = MultiLinePrompt (z, "Non-transparent Proxy Server", len, programFont);
    ncp->firewallProxy = DialogText (z, "", 12, NULL);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) x, (HANDLE) ncp->firewallProxy, NULL);
    /*
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt0, (HANDLE) ncp->proxyPort,
                  (HANDLE) ncp->firewallProxy, NULL);
    */
    AlignObjects (ALIGN_RIGHT, (HANDLE) ncp->proxyHost, (HANDLE) ncp->firewallProxy, NULL);
    AlignObjects (ALIGN_LEFT, (HANDLE) ncp->proxyHost, (HANDLE) ncp->firewallProxy, NULL);

    g = HiddenGroup (ncp->netGroup, 5, 0, NULL);
    /*
    ppt0 = StaticPrompt (g, "Domain name server", 0, 0, programFont, 'l');
    ncp->dnsAvailable = CheckBox (g, "Available", NULL);
    */
    ncp->dnsAvailable = CheckBox (g, "Domain Name Server", NULL);
    SetStatus (ncp->dnsAvailable, TRUE);
    /* StaticPrompt (g, " ", 0, 0, programFont, 'l'); */
    ppt1 = StaticPrompt (g, "Timeout", 0, popupMenuHeight, programFont, 'l');
    ncp->timeOut = PopupList (g, TRUE, NULL);
    PopupItem (ncp->timeOut, "10 seconds");
    PopupItem (ncp->timeOut, "30 seconds");
    PopupItem (ncp->timeOut, "60 seconds");
    PopupItem (ncp->timeOut, " 5 minutes");
    SetValue (ncp->timeOut, 2);
    AlignObjects (ALIGN_MIDDLE, /* (HANDLE) ppt0, */ (HANDLE) ncp->dnsAvailable,
                  (HANDLE) ppt1, (HANDLE) ncp->timeOut, NULL);

    c = HiddenGroup (w, 4, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    ncp->accept = PushButton (c, "Accept", AcceptNetConfigForm);
    SetObjectExtra (ncp->accept, ncp, NULL);
    b = PushButton (c, "Cancel", StdSendCancelButtonMessageProc);
    SetObjectExtra (b, ncp, NULL);

    /*
    AlignObjects (ALIGN_CENTER, (HANDLE) h, (HANDLE) c, NULL);
    */
    AlignObjects (ALIGN_CENTER, (HANDLE) j, (HANDLE) z, (HANDLE) g, (HANDLE) c, NULL);

    RealizeWindow (w);

    if (! netCurrentlyOn) {
      SafeSetValue (ncp->srvConnMode, 1);
      Hide (ncp->netGroup);
    } else if (GetAppParam ("NCBI", "NET_SERV", "SRV_CONN_MODE", "WWW", str, sizeof (str))) {
      if (StringICmp (str, "FIREWALL") == 0) {
        SafeSetValue (ncp->srvConnMode, 3);
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_HOST", NULL, str, sizeof (str))) {
      if (! StringHasNoText (str)) {
        SafeSetTitle (ncp->proxyHost, str);
        if (GetValue (ncp->srvConnMode) == 2) {
          SafeSetValue (ncp->srvConnMode, 3);
        }
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", NULL, str, sizeof (str))) {
      if (! StringHasNoText (str)) {
        SafeSetTitle (ncp->proxyPort, str);
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_PROXY_HOST", NULL, str, sizeof (str))) {
      if (! StringHasNoText (str)) {
        SafeSetTitle (ncp->firewallProxy, str);
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_HOST", NULL, str, sizeof (str))) {
      if (StringICmp (str, "130.14.22.106") == 0 || StringICmp (str, "130.14.22.107") == 0 ) {
        SafeSetStatus (ncp->dnsAvailable, FALSE);
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", "30", str, sizeof (str))) {
      if (StringICmp (str, "10") == 0) {
        SafeSetValue (ncp->timeOut, 1);
      } else if (StringICmp (str, "60") == 0) {
        SafeSetValue (ncp->timeOut, 3);
      } else if (StringICmp (str, "300") == 0) {
        SafeSetValue (ncp->timeOut, 4);
      } else {
        SafeSetValue (ncp->timeOut, 2);
      }
    }

    ChangeConfigControls (ncp);

    Show (w);
    Select (w);
    Select (ncp->proxyHost);
  }
}
