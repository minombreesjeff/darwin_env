/*
 * Copyright (c) 2000, Boris Popov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Boris Popov.
 * 4. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: nbns_rq.c,v 1.9 2005/02/24 02:04:38 lindak Exp $
 */
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <ctype.h>
#include <netdb.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define NB_NEEDRESOLVER
#include <netsmb/netbios.h>
#include <netsmb/smb_lib.h>
#include <netsmb/nb_lib.h>


static int  nbns_rq_create(int opcode, struct nb_ctx *ctx, struct nbns_rq **rqpp);
static void nbns_rq_done(struct nbns_rq *rqp);
static int  nbns_rq_getrr(struct nbns_rq *rqp, struct nbns_rr *rrp);
static int  nbns_rq_prepare(struct nbns_rq *rqp);
static int  nbns_rq(struct nbns_rq *rqp);

static struct nb_ifdesc *nb_iflist;

int
nbns_resolvename(const char *name, struct nb_ctx *ctx, struct sockaddr **adpp)
{
	struct nbns_rq *rqp;
	struct nb_name nn;
	struct nbns_rr rr;
	struct sockaddr_in *dest;
	int error, rdrcount, len;

	if (strlen(name) > NB_NAMELEN)
		return NBERROR(NBERR_NAMETOOLONG);
	error = nbns_rq_create(NBNS_OPCODE_QUERY, ctx, &rqp);
	if (error)
		return error;
	bzero(&nn, sizeof(nn));
	strcpy(nn.nn_name, name);
	nn.nn_scope = ctx->nb_scope;
	nn.nn_type = NBT_SERVER;
	rqp->nr_nmflags = NBNS_NMFLAG_RD;
	rqp->nr_qdname = &nn;
	rqp->nr_qdtype = NBNS_QUESTION_TYPE_NB;
	rqp->nr_qdclass = NBNS_QUESTION_CLASS_IN;
	rqp->nr_qdcount = 1;
	dest = &rqp->nr_dest;
	*dest = ctx->nb_ns;
	dest->sin_family = AF_INET;
	dest->sin_len = sizeof(*dest);
	if (dest->sin_port == 0)
		dest->sin_port = htons(137);
	if (dest->sin_addr.s_addr == INADDR_ANY)
		dest->sin_addr.s_addr = htonl(INADDR_BROADCAST);
	if (dest->sin_addr.s_addr == INADDR_BROADCAST)
		rqp->nr_flags |= NBRQF_BROADCAST;
	error = nbns_rq_prepare(rqp);
	if (error) {
		nbns_rq_done(rqp);
		return error;
	}
	rdrcount = NBNS_MAXREDIRECTS;
	for (;;) {
		error = nbns_rq(rqp);
		if (error)
			break;
		if ((rqp->nr_rpnmflags & NBNS_NMFLAG_AA) == 0) {
			if (rdrcount-- == 0) {
				error = NBERROR(NBERR_TOOMANYREDIRECTS);
				break;
			}
			error = nbns_rq_getrr(rqp, &rr);
			if (error)
				break;
			error = nbns_rq_getrr(rqp, &rr);
			if (error)
				break;
			bcopy(rr.rr_data, &dest->sin_addr, 4);
			rqp->nr_flags &= ~NBRQF_BROADCAST;
			continue;
		}
		if (rqp->nr_rpancount == 0) {
			error = NBERROR(NBERR_HOSTNOTFOUND);
			break;
		}
		error = nbns_rq_getrr(rqp, &rr);
		if (error)
			break;
		len = sizeof(struct sockaddr_in);
		dest = malloc(len);
		if (dest == NULL)
			return ENOMEM;
		bzero(dest, len);
		dest->sin_len = len;
		dest->sin_family = AF_INET;
		bcopy(rr.rr_data + 2, &dest->sin_addr.s_addr, 4);
		dest->sin_port = htons(SMB_TCP_PORT);
		*adpp = (struct sockaddr*)dest;
		ctx->nb_lastns = rqp->nr_sender;
		break;
	}
	nbns_rq_done(rqp);
	return error;
}

static char *
smb_optstrncpy(char *d, char *s, unsigned maxlen)
{
	if (d && s) {
		strncpy(d, s, maxlen);
		d[maxlen] = (char)0;
	}
	return (d);
}


int
nbns_getnodestatus(struct sockaddr *targethost,
		   struct nb_ctx *ctx, 
		   char *system,
		   char *workgroup)
{
	struct nbns_rq *rqp;
	struct nbns_rr rr;
	struct nb_name nn;
	struct nbns_nr *nrp;
	char nrtype;
	char *cp, *retname = NULL;
	struct sockaddr_in *dest;
	unsigned char nrcount;
	int error, rdrcount, i, foundserver = 0, foundgroup = 0;

	if (targethost->sa_family != AF_INET) return EINVAL;
	error = nbns_rq_create(NBNS_OPCODE_QUERY, ctx, &rqp);
	if (error)
		return error;
	bzero(&nn, sizeof(nn));
	strcpy(nn.nn_name, "*");
	nn.nn_scope = ctx->nb_scope;
	nn.nn_type = NBT_WKSTA;
	rqp->nr_nmflags = 0;
	rqp->nr_qdname = &nn;
	rqp->nr_qdtype = NBNS_QUESTION_TYPE_NBSTAT;
	rqp->nr_qdclass = NBNS_QUESTION_CLASS_IN;
	rqp->nr_qdcount = 1;
	dest = &rqp->nr_dest;
	*dest = *(struct sockaddr_in *)targethost;
	dest->sin_family = AF_INET;		/* XXX isn't this set in the copy? */
	dest->sin_len = sizeof(*dest);		/* XXX isn't this set in the copy? */
	dest->sin_port = htons(137);
	if (dest->sin_addr.s_addr == INADDR_ANY)
		dest->sin_addr.s_addr = htonl(INADDR_BROADCAST);
	if (dest->sin_addr.s_addr == INADDR_BROADCAST)
		rqp->nr_flags |= NBRQF_BROADCAST;
	error = nbns_rq_prepare(rqp);
	if (error) {
		nbns_rq_done(rqp);
		return error;
	}
	rdrcount = NBNS_MAXREDIRECTS;
	for (;;) {
		error = nbns_rq(rqp);
		if (error)
			break;
		if (rqp->nr_rpancount == 0) {
			error = NBERROR(NBERR_HOSTNOTFOUND);
			break;
		}
		error = nbns_rq_getrr(rqp, &rr);
		if (error)
			break;
		nrcount = *((unsigned char *)rr.rr_data)++;
		for (i = 1, nrp = (struct nbns_nr *)rr.rr_data;
		     i <= nrcount; ++i, ++nrp) {
			nrtype = nrp->nr_name[NB_NAMELEN-1];
			/* Terminate the string: */
			nrp->nr_name[NB_NAMELEN-1] = (char)0;
			/* Strip off trailing spaces */
			for (cp = &nrp->nr_name[NB_NAMELEN-2];
			     cp >= nrp->nr_name; --cp) {
				if (*cp != (char)0x20)
					break;
				*cp = (char)0;
			}
			if (nrp->nr_flags & NBNS_GROUPFLG) {
				if (!foundgroup ||
				    (foundgroup != NBT_WKSTA+1 &&
				     nrtype == NBT_WKSTA)) {
					smb_optstrncpy(workgroup, nrp->nr_name,
						       SMB_MAXUSERNAMELEN);
					foundgroup = nrtype+1;
				}
			} else {
				/* Track at least ONE name, in case
				   no server name is found */
				retname = nrp->nr_name;
			}
			if (nrtype == NBT_SERVER) {
				smb_optstrncpy(system, nrp->nr_name,
					       SMB_MAXSRVNAMELEN);
				foundserver = 1;
			}
		}
		if (!foundserver)
			smb_optstrncpy(system, retname, SMB_MAXSRVNAMELEN);
		ctx->nb_lastns = rqp->nr_sender;
		break;
	}
	nbns_rq_done(rqp);
	return error;
}

int
nbns_rq_create(int opcode, struct nb_ctx *ctx, struct nbns_rq **rqpp)
{
	struct nbns_rq *rqp;
	static u_int16_t trnid;
	int error;

	rqp = malloc(sizeof(*rqp));
	if (rqp == NULL)
		return ENOMEM;
	bzero(rqp, sizeof(*rqp));
	error = mb_init(&rqp->nr_rq, NBDG_MAXSIZE);
	if (error) {
		free(rqp);
		return error;
	}
	rqp->nr_opcode = opcode;
	rqp->nr_nbd = ctx;
	rqp->nr_trnid = trnid++;
	*rqpp = rqp;
	return 0;
}

void
nbns_rq_done(struct nbns_rq *rqp)
{
	if (rqp == NULL)
		return;
	if (rqp->nr_fd >= 0)
		close(rqp->nr_fd);
	mb_done(&rqp->nr_rq);
	mb_done(&rqp->nr_rp);
	free(rqp);
}

/*
 * Extract resource record from the packet. Assume that there is only
 * one mbuf.
 */
int
nbns_rq_getrr(struct nbns_rq *rqp, struct nbns_rr *rrp)
{
	struct mbdata *mbp = &rqp->nr_rp;
	u_char *cp;
	int error, len;

	bzero(rrp, sizeof(*rrp));
	cp = mbp->mb_pos;
	len = nb_encname_len(cp);
	if (len < 1)
		return NBERROR(NBERR_INVALIDRESPONSE);
	rrp->rr_name = cp;
	error = mb_get_mem(mbp, NULL, len);
	if (error)
		return error;
	mb_get_uint16be(mbp, &rrp->rr_type);
	mb_get_uint16be(mbp, &rrp->rr_class);
	mb_get_uint32be(mbp, &rrp->rr_ttl);
	mb_get_uint16be(mbp, &rrp->rr_rdlength);
	rrp->rr_data = mbp->mb_pos;
	error = mb_get_mem(mbp, NULL, rrp->rr_rdlength);
	return error;
}

int
nbns_rq_prepare(struct nbns_rq *rqp)
{
	struct nb_ctx *ctx = rqp->nr_nbd;
	struct mbdata *mbp = &rqp->nr_rq;
	u_int8_t nmflags;
	u_char *cp;
	int len, error;

	error = mb_init(&rqp->nr_rp, NBDG_MAXSIZE);
	if (error)
		return error;
	if (rqp->nr_dest.sin_addr.s_addr == INADDR_BROADCAST) {
		rqp->nr_nmflags |= NBNS_NMFLAG_BCAST;
		if (nb_iflist == NULL) {
			error = nb_enum_if(&nb_iflist, 100);
			if (error)
				return error;
		}
	} else
		rqp->nr_nmflags &= ~NBNS_NMFLAG_BCAST;
	mb_put_uint16be(mbp, rqp->nr_trnid);
	nmflags = ((rqp->nr_opcode & 0x1F) << 3) | ((rqp->nr_nmflags & 0x70) >> 4);
	mb_put_uint8(mbp, nmflags);
	mb_put_uint8(mbp, (rqp->nr_nmflags & 0x0f) << 4 /* rcode */);
	mb_put_uint16be(mbp, rqp->nr_qdcount);
	mb_put_uint16be(mbp, rqp->nr_ancount);
	mb_put_uint16be(mbp, rqp->nr_nscount);
	mb_put_uint16be(mbp, rqp->nr_arcount);
	if (rqp->nr_qdcount) {
		if (rqp->nr_qdcount > 1)
			return EINVAL;
		len = nb_name_len(rqp->nr_qdname);
		error = mb_fit(mbp, len, (char**)&cp);
		if (error)
			return error;
		/* 
		 * tell nb_name encode NOT to uppercase 
		 * the name. We know that calls from
		 * mount_smbfs have uppercased the
		 * name if appropriate (some codepages
		 * should not be uppercased). I tested 
		 * smbutil lookup and it still works
		 * OK.  
		 */
		nb_name_encode(rqp->nr_qdname, cp,0);
		mb_put_uint16be(mbp, rqp->nr_qdtype);
		mb_put_uint16be(mbp, rqp->nr_qdclass);
	}
	m_lineup(mbp->mb_top, &mbp->mb_top);
	if (ctx->nb_timo == 0)
		ctx->nb_timo = 1;	/* by default 1 second */
	return 0;
}

static int
nbns_rq_recv(struct nbns_rq *rqp)
{
	struct mbdata *mbp = &rqp->nr_rp;
	void *rpdata = mtod(mbp->mb_top, void *);
	fd_set rd, wr, ex;
	struct timeval tv;
	struct sockaddr_in sender;
	int s = rqp->nr_fd;
	int n, len;

	FD_ZERO(&rd);
	FD_ZERO(&wr);
	FD_ZERO(&ex);
	FD_SET(s, &rd);

	tv.tv_sec = rqp->nr_nbd->nb_timo;
	tv.tv_usec = 0;

	n = select(s + 1, &rd, &wr, &ex, &tv);
	if (n == -1)
		return -1;
	if (n == 0)
		return ETIMEDOUT;
	if (FD_ISSET(s, &rd) == 0)
		return ETIMEDOUT;
	len = sizeof(sender);
	n = recvfrom(s, rpdata, mbp->mb_top->m_maxlen, 0,
	    (struct sockaddr*)&sender, &len);
	if (n < 0)
		return errno;
	mbp->mb_top->m_len = mbp->mb_count = n;
	rqp->nr_sender = sender;
	return 0;
}

static int
nbns_rq_opensocket(struct nbns_rq *rqp)
{
	struct sockaddr_in locaddr;
	int opt, s;

	s = rqp->nr_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		return errno;
	if (rqp->nr_flags & NBRQF_BROADCAST) {
		opt = 1;
		if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) < 0)
			return errno;
		if (rqp->nr_if == NULL)
			return NBERROR(NBERR_NOBCASTIFS);
		bzero(&locaddr, sizeof(locaddr));
		locaddr.sin_family = AF_INET;
		locaddr.sin_len = sizeof(locaddr);
		locaddr.sin_addr = rqp->nr_if->id_addr;
		rqp->nr_dest.sin_addr.s_addr = rqp->nr_if->id_addr.s_addr | ~rqp->nr_if->id_mask.s_addr;
		if (bind(s, (struct sockaddr*)&locaddr, sizeof(locaddr)) < 0)
			return errno;
	}
	return 0;
}

static int
nbns_rq_send(struct nbns_rq *rqp)
{
	struct mbdata *mbp = &rqp->nr_rq;
	int s = rqp->nr_fd;

	if (sendto(s, mtod(mbp->mb_top, char *), mbp->mb_count, 0,
	      (struct sockaddr*)&rqp->nr_dest, sizeof(rqp->nr_dest)) < 0)
		return errno;
	return 0;
}

int
nbns_rq(struct nbns_rq *rqp)
{
	struct mbdata *mbp = &rqp->nr_rq;
	u_int16_t rpid;
	u_int8_t nmflags;
	int error, retrycount;

	rqp->nr_if = nb_iflist;
again:
	error = nbns_rq_opensocket(rqp);
	if (error)
		return error;
	retrycount = 1;	/* XXX - configurable or adaptive */
	for (;;) {
		error = nbns_rq_send(rqp);
		if (error)
			return error;
		error = nbns_rq_recv(rqp);
		if (error) {
			if (error != ETIMEDOUT || retrycount == 0) {
				if ((rqp->nr_nmflags & NBNS_NMFLAG_BCAST) &&
				    rqp->nr_if != NULL &&
				    rqp->nr_if->id_next != NULL) {
					rqp->nr_if = rqp->nr_if->id_next;
					close(rqp->nr_fd);
					goto again;
				} else
					return error;
			}
			retrycount--;
			continue;
		}
		mbp = &rqp->nr_rp;
		if (mbp->mb_count < 12)
			return NBERROR(NBERR_INVALIDRESPONSE);
		mb_get_uint16be(mbp, &rpid);
		if (rpid != rqp->nr_trnid)
			return NBERROR(NBERR_INVALIDRESPONSE);
		break;
	}
	mb_get_uint8(mbp, &nmflags);
	rqp->nr_rpnmflags = (nmflags & 7) << 4;
	mb_get_uint8(mbp, &nmflags);
	rqp->nr_rpnmflags |= (nmflags & 0xf0) >> 4;
	rqp->nr_rprcode = nmflags & 0xf;
	if (rqp->nr_rprcode)
		return NBERROR(rqp->nr_rprcode);
	mb_get_uint16be(mbp, &rpid);	/* QDCOUNT */
	mb_get_uint16be(mbp, &rqp->nr_rpancount);
	mb_get_uint16be(mbp, &rqp->nr_rpnscount);
	mb_get_uint16be(mbp, &rqp->nr_rparcount);
	return 0;
}
