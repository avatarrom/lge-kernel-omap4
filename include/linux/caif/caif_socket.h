/* linux/caif_socket.h
 * CAIF Definitions for CAIF socket and network layer
 * Copyright (C) ST-Ericsson AB 2010
 * Author:	 Sjur Brendeland/ sjur.brandeland@stericsson.com
 * License terms: GNU General Public License (GPL) version 2
 */

#ifndef _LINUX_CAIF_SOCKET_H
#define _LINUX_CAIF_SOCKET_H

#include <linux/types.h>

#ifdef __KERNEL__
#include <linux/socket.h>
#else
#include <sys/socket.h>
#endif

/**
 * enum caif_link_selector -    Physical Link Selection.
 * @CAIF_LINK_HIGH_BANDW:	Physical interface for high-bandwidth
 *				traffic.
 * @CAIF_LINK_LOW_LATENCY:	Physical interface for low-latency
 *				traffic.
 *
 * CAIF Link Layers can register their link properties.
 * This enum is used for choosing between CAIF Link Layers when
 * setting up CAIF Channels when multiple CAIF Link Layers exists.
 */
enum caif_link_selector {
	CAIF_LINK_HIGH_BANDW,
	CAIF_LINK_LOW_LATENCY
};

/**
 * enum caif_channel_priority - CAIF channel priorities.
 *
 * @CAIF_PRIO_MIN:	Min priority for a channel.
 * @CAIF_PRIO_LOW:	Low-priority channel.
 * @CAIF_PRIO_NORMAL:	Normal/default priority level.
 * @CAIF_PRIO_HIGH:	High priority level
 * @CAIF_PRIO_MAX:	Max priority for channel
 *
 * Priority can be set on CAIF Channels in order to
 * prioritize between traffic on different CAIF Channels.
 * These priority levels are recommended, but the priority value
 * is not restricted to the values defined in this enum, any value
 * between CAIF_PRIO_MIN and CAIF_PRIO_MAX could be used.
 */
enum caif_channel_priority {
	CAIF_PRIO_MIN	 = 0x01,
	CAIF_PRIO_LOW	 = 0x04,
	CAIF_PRIO_NORMAL = 0x0f,
	CAIF_PRIO_HIGH	 = 0x14,
	CAIF_PRIO_MAX	 = 0x1F
};

/**
 * enum caif_protocol_type  -	CAIF Channel type.
 * @CAIFPROTO_AT:		Classic AT channel.
 * @CAIFPROTO_DATAGRAM:	Datagram channel.
 * @CAIFPROTO_DATAGRAM_LOOP:	Datagram loopback channel, used for testing.
 * @CAIFPROTO_UTIL:		Utility (Psock) channel.
 * @CAIFPROTO_RFM:		Remote File Manager
 *
 * This enum defines the CAIF Channel type to be used. This defines
 * the service to connect to on the modem.
 */
enum caif_protocol_type {
	CAIFPROTO_AT,
	CAIFPROTO_DATAGRAM,
	CAIFPROTO_DATAGRAM_LOOP,
	CAIFPROTO_UTIL,
	CAIFPROTO_RFM,
	_CAIFPROTO_MAX
};
#define	CAIFPROTO_MAX _CAIFPROTO_MAX

/**
 * enum caif_at_type - AT Service Endpoint
 * @CAIF_ATTYPE_PLAIN:	     Connects to a plain vanilla AT channel.
 */
enum caif_at_type {
	CAIF_ATTYPE_PLAIN = 2
};

/**
 * struct sockaddr_caif - the sockaddr structure for CAIF sockets.
 * @family:		     Address family number, must be AF_CAIF.
 * @u:			     Union of address data 'switched' by family.
 * :
 * @u.at:                    Applies when family = CAIFPROTO_AT.
 *
 * @u.at.type:               Type of AT link to set up (enum caif_at_type).
 *
 * @u.util:                  Applies when family = CAIFPROTO_UTIL
 *
 * @u.util.service:          Utility service name.
 *
 * @u.dgm:                   Applies when family = CAIFPROTO_DATAGRAM
 *
 * @u.dgm.connection_id:     Datagram connection id.
 *
 * @u.dgm.nsapi:             NSAPI of the PDP-Context.
 *
 * @u.rfm:                   Applies when family = CAIFPROTO_RFM
 *
 * @u.rfm.connection_id:     Connection ID for RFM.
 *
 * @u.rfm.volume:            Volume to mount.
 *
 * Description:
 * This structure holds the connect parameters used for setting up a
 * CAIF Channel. It defines the service to connect to on the modem.
 */
struct sockaddr_caif {
	sa_family_t  family;
	union {
		struct {
			__u8  type;		/* type: enum caif_at_type */
		} at;				/* CAIFPROTO_AT */
		struct {
			char	  service[16];
		} util;				/* CAIFPROTO_UTIL */
		union {
			__u32 connection_id;
			__u8  nsapi;
		} dgm;				/* CAIFPROTO_DATAGRAM(_LOOP)*/
		struct {
			__u32 connection_id;
			char	  volume[16];
		} rfm;				/* CAIFPROTO_RFM */
	} u;
};

/**
 * enum caif_socket_opts - CAIF option values for getsockopt and setsockopt.
 *
 * @CAIFSO_LINK_SELECT:		Selector used if multiple CAIF Link layers are
 *				available. Either a high bandwidth
 *				link can be selected (CAIF_LINK_HIGH_BANDW) or
 *				or a low latency link (CAIF_LINK_LOW_LATENCY).
 *                              This option is of type __u32.
 *				Alternatively SO_BINDTODEVICE can be used.
 *
 * @CAIFSO_REQ_PARAM:		Used to set the request parameters for a
 *				utility channel. (maximum 256 bytes). This
 *				option must be set before connecting.
 *
 * @CAIFSO_RSP_PARAM:		Gets the response parameters for a utility
 *				channel. (maximum 256 bytes). This option
 *				is valid after a successful connect.
 *
 *
 * This enum defines the CAIF Socket options to be used on a socket
 * of type PF_CAIF.
 *
 */
enum caif_socket_opts {
	CAIFSO_LINK_SELECT	= 127,
	CAIFSO_REQ_PARAM	= 128,
	CAIFSO_RSP_PARAM	= 129,
};

#endif /* _LINUX_CAIF_SOCKET_H */
