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
#include "itr.h"
#include "cache.h"
#include "packet.h"
#include "addr.h"
#include "link.h"
#include "simulator.h"


void
ITR::setCacheTTL(double ttl)
{
	default_ttl = ttl;	
}

void
ITR::setMapDB(std::string mapdbIP_str)
{
	this->mapdbIP = new IPAddr(mapdbIP_str);
}

void
ITR::aPacketReceived(Packet *pkt)
{
	if (!IPAddr::compare(pkt->getDstIP(), getIP())) {
		packetReceived(pkt);
		return;
	}
	IPAddr *src_rloc = eid_to_rloc(pkt->getSrcIP());
	IPAddr *dst_rloc = eid_to_rloc(pkt->getDstIP());
	if (src_rloc != NULL && dst_rloc != NULL) {
		pkt->encap();
		pkt->setSrcIP(src_rloc);
		pkt->setDstIP(dst_rloc);
		sendPacket(sim->now(), pkt);
	}
	else {
#ifdef DEBUG
		std::cout << "QUEUEING" << std::endl;
#endif
		if (src_rloc == NULL)
			askMapDB(pkt->getSrcIP());
		if (dst_rloc == NULL)
			askMapDB(pkt->getDstIP());
		dqueue.push_back(pkt);
	}
}

void
ITR::packetReceived(Packet *pkt)
{
#ifdef DEBUG
	std::cout << "GOT UR MSG" << std::endl;
#endif
	if (!IPAddr::compare(pkt->getSrcIP(), mapdbIP))
		mapReceived(pkt);
}

IPAddr *
ITR::eid_to_rloc(IPAddr *eid)
{
	// get rid of stalled cache items
	for (std::vector<CacheEntry>::iterator itr = cache.begin() ; itr != cache.end() ;) {
		if ((*itr).ttl < sim->now())
			cache.erase(itr);
		else
			itr++;
	}
	IPAddr *rloc = NULL;
	for (int i=0 ; i<cache.size() ; i++)
		if (!IPAddr::compare(eid, cache[i].eid))
			return cache[i].rloc;
	return NULL;
}

void
ITR::askMapDB(IPAddr *eid)
{
	Packet *pkt = new Packet();
	pkt->setSrcIP(getIP());
	pkt->setDstIP(mapdbIP);
	uint8_t b[4];
	b[0] = eid->toInt() & 0xff;
	b[1] = (eid->toInt() >> 8) & 0xff;
	b[2] = (eid->toInt() >> 16) & 0xff;
	b[3] = (eid->toInt() >> 24) & 0xff;
	pkt->payload()->push_back(0x1);
	for (int i=0 ; i<4 ; i++)
		pkt->payload()->push_back(b[i]);
	sendPacket(sim->now(), pkt);
}

void
ITR::mapReceived(Packet *pkt)
{
	uint8_t b[4], c[4];
	for (int i=0 ; i<4 ; i++) {
		b[i] = pkt->payload()->at(i);
		c[i] = pkt->payload()->at(i + 4);
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
	std::cout << "[" << sim->now() << "] MAP RECV'D " << eid->toStr() << " is " << rloc->toStr() << std::endl;
#endif

	if (!(rloc->isValid())) {
		for (std::vector<Packet *>::iterator itr = dqueue.begin() ; itr != dqueue.end() ;) {
			if (!IPAddr::compare(eid, (*itr)->getSrcIP()) || !IPAddr::compare(eid, (*itr)->getDstIP())) {
				DropEvent *dev = new DropEvent();
				dev->setTime(sim->now());
				dev->setSource(*this);
				dev->setDestination(*((*itr)->getHop()));
				dev->setPacket(pkt);
				sim->addEvent(*dev);
				dqueue.erase(itr);
#ifdef DEBUG
				std::cout << "INV DROPPING" << std::endl;
#endif
			}
			else
				itr++;
		}
	}
	else {
#ifdef DEBUG
		std::cout << "MAP (" << sim->now() << "): " << eid->toStr() << " -> " << rloc->toStr() << std::endl;
#endif
		// insert in cache
		cache.push_back(CacheEntry(eid, rloc, sim->now() + default_ttl));
		// check if any packet was delayed, waiting for this map
		for (std::vector<Packet *>::iterator itr = dqueue.begin() ; itr != dqueue.end() ;) {
			IPAddr *src_rloc = eid_to_rloc((*itr)->getSrcIP());
			IPAddr *dst_rloc = eid_to_rloc((*itr)->getDstIP());
			if (src_rloc != NULL && dst_rloc != NULL) {
				(*itr)->encap();
				(*itr)->setSrcIP(src_rloc);
				(*itr)->setDstIP(dst_rloc);
				sendPacket(sim->now(), *itr);
				dqueue.erase(itr);
			}
			else
				itr++;
		}
	}
}

