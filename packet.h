/*
 * Copyright (c) 2014 Mohamed Aslan <maslan@sce.carleton.ca>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <stdint.h>

class IPAddr;
class Node;

class Packet {
private:
	IPAddr *src_ip;
	IPAddr *dst_ip;
	bool is_lisp;
	IPAddr *encap_src_ip;
	IPAddr *encap_dst_ip;
	uint8_t protocol;
	std::vector<uint8_t> buf;
	Node *hop;	// the last hop
public:
	Packet();
	void setSrcIP(IPAddr *);
	IPAddr *getSrcIP();
	void setDstIP(IPAddr *);
	IPAddr *getDstIP();
	/*
	void setEncapSrcIP(IPAddr *);
	IPAddr *getEncapDstIP();
	void setEncapDstIP(IPAddr *);
	IPAddr *getEncapDstIP();
	*/
	void encap();
	void decap();
	bool isLISP();
	std::vector<uint8_t> *payload();
	void setHop(Node &);
	Node *getHop();
};

#endif
