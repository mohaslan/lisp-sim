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

#include <iostream>
#include "mapdb.h"
#include "packet.h"
#include "simulator.h"
#include "addr.h"


void
MapDB::packetReceived(Packet *pkt)
{
#ifdef DEBUG
	std::cout << "MAP" << std::endl;
#endif

	if (!(pkt->payload()->size() == 5 || pkt->payload()->size() == 9))
		return;	// FIXME: drop

	if (pkt->payload()->at(0) == 0x1)
		mapRequest(pkt);
	else if (pkt->payload()->at(0) == 0x2)
		mapUpdate(pkt);
	
}

void
MapDB::insertMap(std::string eid, std::string rloc)
{
	db.push_back(std::make_pair(new IPAddr(eid), new IPAddr(rloc)));
}

IPAddr *
MapDB::eid_to_rloc(IPAddr *eid)
{
	IPAddr *rloc = NULL;
	for (int i=0 ; i<db.size() ; i++)
		if (!IPAddr::compare(eid, db[i].first))
			return db[i].second;
	return NULL;
}

void
MapDB::mapRequest(Packet *pkt)
{
	uint8_t b[4];
	for (int i=0 ; i<4 ; i++)
		b[i] = pkt->payload()->at(i + 1);
	IPAddr *eid = new IPAddr(
		(uint32_t(b[3]) << 24) |
		(uint32_t(b[2]) << 16) |
		(uint32_t(b[1]) << 8) |
		(uint32_t(b[0])));
#ifdef DEBUG
	std::cout << "MAPPING: " << eid->toStr() << std::endl;
#endif

	IPAddr *rloc = eid_to_rloc(eid);
	if (rloc == NULL) {
		rloc = new IPAddr("0.0.0.0");	// INVALID IP
	}

	uint8_t c[4];
	c[0] = rloc->toInt() & 0xff;
	c[1] = (rloc->toInt() >> 8) & 0xff;
	c[2] = (rloc->toInt() >> 16) & 0xff;
	c[3] = (rloc->toInt() >> 24) & 0xff;

	// reply
	Packet *reply = new Packet();
	reply->setSrcIP(getIP());
	reply->setDstIP(pkt->getSrcIP());
	for (int i=0 ; i<4 ; i++)
		reply->payload()->push_back(b[i]);
	for (int i=0 ; i<4 ; i++)
		reply->payload()->push_back(c[i]);
	sendPacket(sim->now(), reply);
}

void
MapDB::mapUpdate(Packet *pkt)
{
	uint8_t b[4], c[4];
	for (int i=0 ; i<4 ; i++) {
		b[i] = pkt->payload()->at(i + 1);
		c[i] = pkt->payload()->at(i + 1 + 4);
	}
	IPAddr *eid = new IPAddr(
		(uint32_t(b[3]) << 24) |
		(uint32_t(b[2]) << 16) |
		(uint32_t(b[1]) << 8) |
		(uint32_t(b[0])));
	IPAddr *rloc = new IPAddr(
		(uint32_t(c[3]) << 24) |
		(uint32_t(c[2]) << 16) |
		(uint32_t(c[1]) << 8) |
		(uint32_t(c[0])));
#ifdef DEBUG
	std::cout << "UPDATE MAP: " << eid->toStr() << std::endl;
#endif
	db.push_back(std::make_pair(eid, rloc));
}

