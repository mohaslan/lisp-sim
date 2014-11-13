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
#include "etr.h"
#include "packet.h"
#include "addr.h"
#include "link.h"
#include "simulator.h"


void
ETR::setMapDB(std::string mapdbIP_str)
{
	this->mapdbIP = new IPAddr(mapdbIP_str);
}

void
ETR::aPacketReceived(Packet *pkt)
{
	if (!IPAddr::compare(pkt->getDstIP(), getIP())) {
		packetReceived(pkt);
		return;
	}
	if (pkt->isLISP()) {
#ifdef DEBUG
		std::cout << "GOT: " << pkt->getSrcIP()->toStr() << " -> " << pkt->getDstIP()->toStr() << std::endl;
#endif
		pkt->decap();
#ifdef DEBUG
		std::cout << "NOW: " << pkt->getSrcIP()->toStr() << " -> " << pkt->getDstIP()->toStr() << std::endl;
#endif
		sendPacket(sim->now(), pkt);
	}
	// FIXME: else
}

void
ETR::packetReceived(Packet *pkt)
{
	// FIXME: drop
}

void
ETR::sendMap(double time, std::string eid_str, std::string rloc_str)
{
	Packet *pkt = new Packet();
	pkt->setSrcIP(getIP());
	pkt->setDstIP(mapdbIP);

	IPAddr *eid = new IPAddr(eid_str);
	IPAddr *rloc = new IPAddr(rloc_str);

	uint8_t b[4], c[4];
	b[0] = eid->toInt() & 0xff;
	b[1] = (eid->toInt() >> 8) & 0xff;
	b[2] = (eid->toInt() >> 16) & 0xff;
	b[3] = (eid->toInt() >> 24) & 0xff;
	c[0] = rloc->toInt() & 0xff;
	c[1] = (rloc->toInt() >> 8) & 0xff;
	c[2] = (rloc->toInt() >> 16) & 0xff;
	c[3] = (rloc->toInt() >> 24) & 0xff;
	pkt->payload()->push_back(0x2);
	for (int i=0 ; i<4 ; i++)
		pkt->payload()->push_back(b[i]);
	for (int i=0 ; i<4 ; i++)
		pkt->payload()->push_back(c[i]);
	sendPacket(time, pkt);
}

