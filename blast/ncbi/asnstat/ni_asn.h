/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "ni_asn.h60";
static AsnType atx[149] = {
  {401, "SVC-Entry" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[1],0,&atx[20]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "minvers" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "maxvers" ,128,2,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[6]} ,
  {0, "id" ,128,3,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[7]} ,
  {0, "priority" ,128,4,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[8]} ,
  {0, "group" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[9]} ,
  {0, "description" ,128,6,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[10]} ,
  {0, "types" ,128,7,0,1,0,0,0,0,NULL,&atx[12],&atx[11],0,&atx[13]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "priority-timeout" ,128,8,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[14]} ,
  {0, "priority-penalty" ,128,9,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[15]} ,
  {0, "encryption-supported" ,128,10,0,1,0,0,0,0,NULL,&atx[16],NULL,0,&atx[17]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "tracking-period" ,128,11,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[18]} ,
  {0, "tracking-count" ,128,12,0,1,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "RES-Entry" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[21],0,&atx[28]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[22]} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[23]} ,
  {0, "minvers" ,128,2,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[24]} ,
  {0, "maxvers" ,128,3,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[25]} ,
  {0, "id" ,128,4,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[26]} ,
  {0, "group" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[27]} ,
  {0, "description" ,128,6,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {403, "Toolset" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[29],0,&atx[40]} ,
  {0, "host" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[30]} ,
  {0, "motd" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[31]} ,
  {0, "services" ,128,2,0,1,0,0,0,0,NULL,&atx[12],&atx[32],0,&atx[33]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {0, "resources" ,128,3,0,1,0,0,0,0,NULL,&atx[12],&atx[34],0,&atx[35]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "regions" ,128,4,0,1,0,0,0,0,NULL,&atx[12],&atx[36],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {416, "Region-Descr" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[38],0,&atx[61]} ,
  {0, "region-name" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[39]} ,
  {0, "priority-delta" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {404, "Identity" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[41],0,&atx[44]} ,
  {0, "username" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[42]} ,
  {0, "group" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[43]} ,
  {0, "domain" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {405, "Request" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[45],0,&atx[50]} ,
  {0, "address" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[46]} ,
  {0, "port" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[47]} ,
  {0, "svcentry" ,128,2,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[48]} ,
  {0, "resentry" ,128,3,0,1,0,0,0,0,NULL,&atx[12],&atx[49],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {406, "MSG-ACK" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[51],0,&atx[68]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[52]} ,
  {0, "disp-info" ,128,1,0,1,0,0,0,0,NULL,&atx[53],NULL,0,&atx[66]} ,
  {418, "Dispatcher-Info" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[54],0,&atx[100]} ,
  {0, "serial-no" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[55]} ,
  {0, "is-alternate-list" ,128,1,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[56]} ,
  {0, "num-dispatchers" ,128,2,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[57]} ,
  {0, "disp-list" ,128,3,0,0,0,0,0,0,NULL,&atx[59],&atx[58],0,&atx[60]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "pub-key" ,128,4,0,1,0,0,0,0,NULL,&atx[61],NULL,0,NULL} ,
  {417, "RSA-Pubkey" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[62],0,&atx[53]} ,
  {0, "bits" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[63]} ,
  {0, "modulus" ,128,1,0,0,0,0,0,0,NULL,&atx[64],NULL,0,&atx[65]} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "exponent" ,128,2,0,0,0,0,0,0,NULL,&atx[64],NULL,0,NULL} ,
  {0, "admin-info" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[67]} ,
  {0, "motd" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {407, "MSG-NACK" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[69],0,&atx[73]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[70]} ,
  {0, "code" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[71]} ,
  {0, "reason" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[72]} ,
  {0, "disp-info" ,128,3,0,1,0,0,0,0,NULL,&atx[53],NULL,0,NULL} ,
  {408, "MSG-Login" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[74],0,&atx[83]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[75]} ,
  {0, "uid" ,128,1,0,0,0,0,0,0,NULL,&atx[40],NULL,0,&atx[76]} ,
  {0, "password" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[77]} ,
  {0, "disp-serial-no" ,128,3,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[78]} ,
  {0, "encryption-desired" ,128,4,0,1,0,0,0,0,NULL,&atx[16],NULL,0,&atx[79]} ,
  {0, "pub-key" ,128,5,0,1,0,0,0,0,NULL,&atx[61],NULL,0,&atx[80]} ,
  {0, "des-key" ,128,6,0,1,0,0,0,0,NULL,&atx[64],NULL,0,&atx[81]} ,
  {0, "connect-delay" ,128,7,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[82]} ,
  {0, "server-port" ,128,8,0,1,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {409, "MSG-SVC-List" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[84],0,&atx[87]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[85]} ,
  {0, "toollist" ,128,1,0,0,0,0,0,0,NULL,&atx[28],NULL,0,&atx[86]} ,
  {0, "knows-tracking" ,128,2,0,1,0,0,0,0,NULL,&atx[16],NULL,0,NULL} ,
  {410, "MSG-SVC-Request" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[88],0,&atx[109]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[89]} ,
  {0, "conid" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[90]} ,
  {0, "uid" ,128,2,0,0,0,0,0,0,NULL,&atx[40],NULL,0,&atx[91]} ,
  {0, "request" ,128,3,0,0,0,0,0,0,NULL,&atx[44],NULL,0,&atx[92]} ,
  {0, "platform" ,128,4,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[93]} ,
  {0, "appl-id" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[94]} ,
  {0, "des-key" ,128,6,0,1,0,0,0,0,NULL,&atx[64],NULL,0,&atx[95]} ,
  {0, "want-pre-response" ,128,7,0,1,0,0,0,0,NULL,&atx[16],NULL,0,&atx[96]} ,
  {0, "server-ip" ,128,8,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[97]} ,
  {0, "server-port" ,128,9,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[98]} ,
  {0, "want-ticket" ,128,10,0,1,0,0,0,0,NULL,&atx[16],NULL,0,&atx[99]} ,
  {0, "ticket" ,128,11,0,1,0,0,0,0,NULL,&atx[100],NULL,0,NULL} ,
  {419, "Ticket" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[101],0,&atx[137]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[102]} ,
  {0, "confounding-rand-num" ,128,1,0,0,0,0,0,0,NULL,&atx[64],NULL,0,&atx[103]} ,
  {0, "client-ip-1" ,128,2,0,0,0,0,0,0,NULL,&atx[64],NULL,0,&atx[104]} ,
  {0, "client-ip-2" ,128,3,0,1,0,0,0,0,NULL,&atx[64],NULL,0,&atx[105]} ,
  {0, "server-ip" ,128,4,0,0,0,0,0,0,NULL,&atx[64],NULL,0,&atx[106]} ,
  {0, "client-des-key" ,128,5,0,1,0,0,0,0,NULL,&atx[64],NULL,0,&atx[107]} ,
  {0, "ticket-expiration" ,128,6,0,0,0,0,0,0,NULL,&atx[64],NULL,0,&atx[108]} ,
  {0, "checksum" ,128,7,0,0,0,0,0,0,NULL,&atx[64],NULL,0,NULL} ,
  {411, "MSG-SVC-Response" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[110],0,&atx[112]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[111]} ,
  {0, "request" ,128,1,0,0,0,0,0,0,NULL,&atx[44],NULL,0,NULL} ,
  {412, "MSG-Cmd" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[113],0,&atx[115]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[114]} ,
  {0, "command" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {413, "MSG-Acct" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[116],0,&atx[121]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[117]} ,
  {0, "conid" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[118]} ,
  {0, "jobname" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[119]} ,
  {0, "usertime" ,128,3,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[120]} ,
  {0, "systemtime" ,128,4,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {414, "MSG-Catalog" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[122],0,&atx[126]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[123]} ,
  {0, "motd" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[124]} ,
  {0, "toollists" ,128,2,0,1,0,0,0,0,NULL,&atx[12],&atx[125],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[28],NULL,0,NULL} ,
  {415, "Message" ,1,0,0,0,0,1,0,0,NULL,&atx[148],&atx[127],0,&atx[37]} ,
  {0, "ack" ,128,0,0,0,0,0,0,0,NULL,&atx[50],NULL,0,&atx[128]} ,
  {0, "nack" ,128,1,0,0,0,0,0,0,NULL,&atx[68],NULL,0,&atx[129]} ,
  {0, "login" ,128,2,0,0,0,0,0,0,NULL,&atx[73],NULL,0,&atx[130]} ,
  {0, "svc-list" ,128,3,0,0,0,0,0,0,NULL,&atx[83],NULL,0,&atx[131]} ,
  {0, "svc-request" ,128,4,0,0,0,0,0,0,NULL,&atx[87],NULL,0,&atx[132]} ,
  {0, "svc-response" ,128,5,0,0,0,0,0,0,NULL,&atx[109],NULL,0,&atx[133]} ,
  {0, "command" ,128,6,0,0,0,0,0,0,NULL,&atx[112],NULL,0,&atx[134]} ,
  {0, "acct" ,128,7,0,0,0,0,0,0,NULL,&atx[115],NULL,0,&atx[135]} ,
  {0, "catalog" ,128,8,0,0,0,0,0,0,NULL,&atx[121],NULL,0,&atx[136]} ,
  {0, "svc-pre-response" ,128,9,0,0,0,0,0,0,NULL,&atx[137],NULL,0,&atx[140]} ,
  {420, "MSG-SVC-Pre-Response" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[138],0,&atx[141]} ,
  {0, "seqno" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[139]} ,
  {0, "server-ip" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "load-status" ,128,10,0,0,0,0,0,0,NULL,&atx[141],NULL,0,NULL} ,
  {421, "MSG-Load-Status" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[142],0,NULL} ,
  {0, "load" ,128,0,0,0,0,0,0,0,NULL,&atx[143],NULL,0,&atx[144]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "power" ,128,1,0,0,0,0,0,0,NULL,&atx[143],NULL,0,&atx[145]} ,
  {0, "light-thresh" ,128,2,0,0,0,0,0,0,NULL,&atx[143],NULL,0,&atx[146]} ,
  {0, "heavy-thresh" ,128,3,0,0,0,0,0,0,NULL,&atx[143],NULL,0,&atx[147]} ,
  {0, "job-penalty" ,128,4,0,0,0,0,0,0,NULL,&atx[143],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-MESSAGE" , "ni_asn.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-MESSAGE
*
**************************************************/

#define SVC_ENTRY &at[0]
#define SVC_ENTRY_name &at[1]
#define SVC_ENTRY_minvers &at[3]
#define SVC_ENTRY_maxvers &at[5]
#define SVC_ENTRY_id &at[6]
#define SVC_ENTRY_priority &at[7]
#define SVC_ENTRY_group &at[8]
#define SVC_ENTRY_description &at[9]
#define SVC_ENTRY_types &at[10]
#define SVC_ENTRY_types_E &at[11]
#define SVC_ENTRY_priority_timeout &at[13]
#define SVC_ENTRY_priority_penalty &at[14]
#define SVC_ENTRY_encryption_supported &at[15]
#define SVC_ENTRY_tracking_period &at[17]
#define SVC_ENTRY_tracking_count &at[18]

#define RES_ENTRY &at[20]
#define RES_ENTRY_name &at[21]
#define RES_ENTRY_type &at[22]
#define RES_ENTRY_minvers &at[23]
#define RES_ENTRY_maxvers &at[24]
#define RES_ENTRY_id &at[25]
#define RES_ENTRY_group &at[26]
#define RES_ENTRY_description &at[27]

#define TOOLSET &at[28]
#define TOOLSET_host &at[29]
#define TOOLSET_motd &at[30]
#define TOOLSET_services &at[31]
#define TOOLSET_services_E &at[32]
#define TOOLSET_resources &at[33]
#define TOOLSET_resources_E &at[34]
#define TOOLSET_regions &at[35]
#define TOOLSET_regions_E &at[36]

#define IDENTITY &at[40]
#define IDENTITY_username &at[41]
#define IDENTITY_group &at[42]
#define IDENTITY_domain &at[43]

#define REQUEST &at[44]
#define REQUEST_address &at[45]
#define REQUEST_port &at[46]
#define REQUEST_svcentry &at[47]
#define REQUEST_resentry &at[48]
#define REQUEST_resentry_E &at[49]

#define MSG_ACK &at[50]
#define MSG_ACK_seqno &at[51]
#define MSG_ACK_disp_info &at[52]
#define MSG_ACK_admin_info &at[66]
#define MSG_ACK_motd &at[67]

#define MSG_NACK &at[68]
#define MSG_NACK_seqno &at[69]
#define MSG_NACK_code &at[70]
#define MSG_NACK_reason &at[71]
#define MSG_NACK_disp_info &at[72]

#define MSG_LOGIN &at[73]
#define MSG_LOGIN_seqno &at[74]
#define MSG_LOGIN_uid &at[75]
#define MSG_LOGIN_password &at[76]
#define MSG_LOGIN_disp_serial_no &at[77]
#define MSG_LOGIN_encryption_desired &at[78]
#define MSG_LOGIN_pub_key &at[79]
#define MSG_LOGIN_des_key &at[80]
#define MSG_LOGIN_connect_delay &at[81]
#define MSG_LOGIN_server_port &at[82]

#define MSG_SVC_LIST &at[83]
#define MSG_SVC_LIST_seqno &at[84]
#define MSG_SVC_LIST_toollist &at[85]
#define MSG_SVC_LIST_knows_tracking &at[86]

#define MSG_SVC_REQUEST &at[87]
#define MSG_SVC_REQUEST_seqno &at[88]
#define MSG_SVC_REQUEST_conid &at[89]
#define MSG_SVC_REQUEST_uid &at[90]
#define MSG_SVC_REQUEST_request &at[91]
#define MSG_SVC_REQUEST_platform &at[92]
#define MSG_SVC_REQUEST_appl_id &at[93]
#define MSG_SVC_REQUEST_des_key &at[94]
#define SVC_REQUEST_want_pre_response &at[95]
#define MSG_SVC_REQUEST_server_ip &at[96]
#define MSG_SVC_REQUEST_server_port &at[97]
#define MSG_SVC_REQUEST_want_ticket &at[98]
#define MSG_SVC_REQUEST_ticket &at[99]

#define MSG_SVC_RESPONSE &at[109]
#define MSG_SVC_RESPONSE_seqno &at[110]
#define MSG_SVC_RESPONSE_request &at[111]

#define MSG_CMD &at[112]
#define MSG_CMD_seqno &at[113]
#define MSG_CMD_command &at[114]

#define MSG_ACCT &at[115]
#define MSG_ACCT_seqno &at[116]
#define MSG_ACCT_conid &at[117]
#define MSG_ACCT_jobname &at[118]
#define MSG_ACCT_usertime &at[119]
#define MSG_ACCT_systemtime &at[120]

#define MSG_CATALOG &at[121]
#define MSG_CATALOG_seqno &at[122]
#define MSG_CATALOG_motd &at[123]
#define MSG_CATALOG_toollists &at[124]
#define MSG_CATALOG_toollists_E &at[125]

#define MESSAGE &at[126]
#define MESSAGE_ack &at[127]
#define MESSAGE_nack &at[128]
#define MESSAGE_login &at[129]
#define MESSAGE_svc_list &at[130]
#define MESSAGE_svc_request &at[131]
#define MESSAGE_svc_response &at[132]
#define MESSAGE_command &at[133]
#define MESSAGE_acct &at[134]
#define MESSAGE_catalog &at[135]
#define MESSAGE_svc_pre_response &at[136]
#define MESSAGE_load_status &at[140]

#define REGION_DESCR &at[37]
#define REGION_DESCR_region_name &at[38]
#define REGION_DESCR_priority_delta &at[39]

#define RSA_PUBKEY &at[61]
#define RSA_PUBKEY_bits &at[62]
#define RSA_PUBKEY_modulus &at[63]
#define RSA_PUBKEY_exponent &at[65]

#define DISPATCHER_INFO &at[53]
#define DISPATCHER_INFO_serial_no &at[54]
#define INFO_is_alternate_list &at[55]
#define DISPATCHER_INFO_num_dispatchers &at[56]
#define DISPATCHER_INFO_disp_list &at[57]
#define DISPATCHER_INFO_disp_list_E &at[58]
#define DISPATCHER_INFO_pub_key &at[60]

#define TICKET &at[100]
#define TICKET_seqno &at[101]
#define TICKET_confounding_rand_num &at[102]
#define TICKET_client_ip_1 &at[103]
#define TICKET_client_ip_2 &at[104]
#define TICKET_server_ip &at[105]
#define TICKET_client_des_key &at[106]
#define TICKET_ticket_expiration &at[107]
#define TICKET_checksum &at[108]

#define MSG_SVC_PRE_RESPONSE &at[137]
#define MSG_SVC_PRE_RESPONSE_seqno &at[138]
#define MSG_SVC_PRE_RESPONSE_server_ip &at[139]

#define MSG_LOAD_STATUS &at[141]
#define MSG_LOAD_STATUS_load &at[142]
#define MSG_LOAD_STATUS_power &at[144]
#define MSG_LOAD_STATUS_light_thresh &at[145]
#define MSG_LOAD_STATUS_heavy_thresh &at[146]
#define MSG_LOAD_STATUS_job_penalty &at[147]
