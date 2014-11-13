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
#include "router.h"
#include "packet.h"
#include "addr.h"
#include "simulator.h"
#include "link.h"


RIB::RIB(IPAddr *subnet, NetMask *netmask, Link *link)
{
	this->subnet = subnet;
	this->netmask = netmask;
	this->link = link;
}

void
RIB::setSubNet(IPAddr *subnet)
{
	this->subnet = subnet;
}

IPAddr *
RIB::getSubNet()
{
	return subnet;
}

void
RIB::setNetMask(NetMask *netmask)
{
	this->netmask = netmask;
}

NetMask *
RIB::getNetMask()
{
	return netmask;
}

void
RIB::setLink(Link *link)
{
	this->link = link;
}

Link *
RIB::getLink()
{
	return link;
}

void
Router::insertRoute(std::string subnet, std::string netmask, Link &link)
{
	rib.push_back(RIB(new IPAddr(subnet), new NetMask(netmask), &link));
}

Link *
Router::findRoute(IPAddr *ip)
{
#ifdef DEBUG
	std::cout << "Routing..." << std::endl;
#endif
	for (int i=0 ; i<rib.size() ; i++) {
		std::pair<IPAddr, IPAddr> *range = SubNetting::getRange(rib[i].getSubNet(), rib[i].getNetMask());
#ifdef DEBUG
		std::cout << "B: " << range->first.toStr() << ", E: " << range->second.toStr() << std::endl;
#endif
		if (IPAddr::inRange(ip, &(range->first), &(range->second))) {
			Link *link = rib[i].getLink();
#ifdef DEBUG
			std::cout << "FOUND" << std::endl;
#endif
			return link;
		} 
		delete range;
	}
	return NULL;
}

void
Router::sendPacket(double time, Packet *pkt)
{
#ifdef DEBUG
	std::cout << "ROUTER IS SENDING (" << time << ")" << std::endl;
#endif
	Link *link = findRoute(pkt->getDstIP());
	if (link == NULL) { // FIXME: drop
#ifdef DEBUG
		std::cout << "NO ROUTE " << pkt->getSrcIP()->toStr() << " -> " << pkt->getDstIP()->toStr() << std::endl;
#endif
		return;
	}
	link->sendPacket(time, this, pkt);
}

void
Router::aPacketReceived(Packet *pkt)
{
#ifdef DEBUG
	std::cout << "ROUTER" << std::endl;
#endif
	if (!IPAddr::compare(pkt->getDstIP(), getIP())) {
		packetReceived(pkt);
	}
	else {
#ifdef DEBUG
		std::cout << "NOT ME " << pkt->getSrcIP()->toStr() << " -> " << pkt->getDstIP()->toStr() << std::endl;
#endif
		Link *link = findRoute(pkt->getDstIP());
		if (link == NULL) { // FIXME: drop
#ifdef DEBUG
			std::cout << "NOT ROUTE " << pkt->getSrcIP()->toStr() << " -> " << pkt->getDstIP()->toStr() << std::endl;
#endif
			return;
		}
		link->sendPacket(sim->now(), this, pkt);
	}
}

void
Router::packetReceived(Packet *pkt)
{

	std::cout << "At " << sim->now()
		<< " RECV "
		<< pkt->getSrcIP()->toStr()
		<< " -> "
		<< pkt->getDstIP()->toStr()
		<< std::endl;
}

