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
#include "node.h"
#include "simulator.h"
#include "link.h"
#include "addr.h"
#include "packet.h"


Node::Node()
{
	lastID++;
	ID = lastID;
}

void
Node::attachSimulator(Simulator *sim)
{
	this->sim = sim;
}

uint32_t
Node::getID()
{
	return ID;
}

void
Node::addLink(Link *link)
{
	this->links.push_back(link);
}

void
Node::setIP(std::string ip_str)
{
	this->IP = new IPAddr(ip_str);	
}

IPAddr*
Node::getIP()
{
	return this->IP;
}

void
Node::setNetMask(std::string netmask_str)
{
	this->netmask = new NetMask(netmask_str);
}

NetMask*
Node::getNetMask()
{
	return this->netmask;
}

std::vector<Node *>
Node::getNeighbours()
{
	std::vector<Node *> neighbours;	
	for (int i=0 ; i<links.size() ; i++) {
		if (!IPAddr::compare(links[i]->getEndPoint1()->getIP(), getIP())) {
			neighbours.push_back(links[i]->getEndPoint2());
		}
		if (!IPAddr::compare(links[i]->getEndPoint2()->getIP(), getIP())) {
			neighbours.push_back(links[i]->getEndPoint1());
		}
	}
	return neighbours;
}

std::vector<std::pair<Link *, Node *> >
Node::getConnections()
{
	std::vector<std::pair<Link *, Node *> > connections;	
	for (int i=0 ; i<links.size() ; i++) {
		if (!IPAddr::compare(links[i]->getEndPoint1()->getIP(), getIP())) {
			connections.push_back(std::make_pair(links[i], links[i]->getEndPoint2()));
		}
		if (!IPAddr::compare(links[i]->getEndPoint2()->getIP(), getIP())) {
			connections.push_back(std::make_pair(links[i], links[i]->getEndPoint1()));
		}
	}
	return connections;
}


void
Node::printNeighbours()
{
	std::vector<Node *> neighbours = getNeighbours();
	for (int i=0 ; i<neighbours.size() ; i++) {
		std::cout << neighbours[i]->getIP()->toStr() << std::endl;
	}
}

void
Node::sendPacket(double time, Packet *pkt)
{
	std::vector<std::pair<Link *, Node *> > cnx = getConnections();
	if (cnx.size() < 1)
		return; 

	// by defaults forward all the traffic to the default gateway!
	Link *gw_link = cnx[0].first;
	Node *gw_node = cnx[0].second;

	gw_link->sendPacket(time, this, pkt);
}
/*
void
Node::packetReceived(Packet *pkt)
{

}
*/
