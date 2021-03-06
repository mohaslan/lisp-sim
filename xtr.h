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

#ifndef XTR_H
#define XTR_H

#include <vector>
#include "router.h"

class IPAddr;
class Packet;
class CacheEntry;

class XTR : public Router {
private:
	double default_ttl;
	IPAddr *mapdbIP;
	std::vector<Packet *> dqueue;
	std::vector<CacheEntry> cache;
	void packetReceived(Packet *);
	IPAddr *eid_to_rloc(IPAddr *);
	void askMapDB(IPAddr *);
	void mapReceived(Packet *);
public:
	void setCacheTTL(double);
	void setMapDB(std::string);
	void sendMap(double, std::string, std::string);
	void aPacketReceived(Packet *);
};

#endif
