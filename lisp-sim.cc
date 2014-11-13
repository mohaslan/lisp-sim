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

#include "simulator.h"
#include "addr.h"
#include "link.h"
#include "packet.h"
#include "host.h"
#include "router.h"
#include "itr.h"
#include "mapdb.h"
#include "logger.h"


using namespace std;

uint32_t Node::lastID = 0;

/*
int
main(int argc, char **argv)
{
	Simulator s;

	Logger log("mytopo");
	s.attachLogger(log);

	Host h1, h2;
	h1.setIP(new IPAddr("10.0.0.1"));
	h1.setNetMask(new NetMask("255.255.255.0"));
	h2.setIP(new IPAddr("10.0.0.2"));
	h2.setNetMask(new NetMask("255.255.255.0"));
	s.addNode(h1);
	s.addNode(h2);

	Router r1, r2;
	r1.setIP(new IPAddr("10.0.0.5"));
	r1.setNetMask(new NetMask("255.255.255.0"));
	r2.setIP(new IPAddr("10.0.0.6"));
	r2.setNetMask(new NetMask("255.255.255.0"));
	s.addNode(r1);
	s.addNode(r2);

	ITR itr;
	itr.setIP(new IPAddr("10.0.0.7"));
	itr.setNetMask(new NetMask("255.255.255.0"));
	itr.setMapDB(new IPAddr("192.168.0.1"));
	itr.setCacheTTL(0.9);
	s.addNode(itr);
	Router etr;
	etr.setIP(new IPAddr("10.0.0.8"));
	etr.setNetMask(new NetMask("255.255.255.0"));
	s.addNode(etr);

	Link l1, l2, l3, l4, l5;
	l1.link(&h1, &r1);
	l2.link(&r1, &itr);
	l3.link(&itr, &etr);
	l4.link(&etr, &r2);
	l5.link(&r2, &h2);
	l3.setDelay(0.2);
	s.addLink(l1);
	s.addLink(l2);
	s.addLink(l3);
	s.addLink(l4);
	s.addLink(l5);

	r1.insertRoute(new IPAddr("10.0.0.2"), new NetMask("255.255.255.255"), &l2);
	itr.insertRoute(new IPAddr("10.0.0.2"), new NetMask("255.255.255.255"), &l3);
	etr.insertRoute(new IPAddr("10.0.0.2"), new NetMask("255.255.255.255"), &l4);
	r2.insertRoute(new IPAddr("10.0.0.2"), new NetMask("255.255.255.255"), &l5);


	cout << "Node: " << h1.getIP()->toStr() << endl;
	cout << "Neighbours:" << endl;
	h1.printNeighbours();
	cout << endl;
	cout << "Node: " << h2.getIP()->toStr() << endl;
	cout << "Neighbours:" << endl;
	h2.printNeighbours();
	cout << endl;



	MapDB db;
	db.setIP(new IPAddr("192.168.0.1"));
	db.setNetMask(new NetMask("255.255.255.0"));
	s.addNode(db);

	Link l6;
	l6.link(&itr, &db);
	s.addLink(l6);

	itr.insertRoute(new IPAddr("192.168.0.1"), new NetMask("255.255.255.255"), &l6);
	db.insertRoute(new IPAddr("10.0.0.7"), new NetMask("255.255.255.255"), &l6);

	db.insertMap(new IPAddr("10.0.0.1"), new IPAddr("172.0.0.1"));
	db.insertMap(new IPAddr("10.0.0.2"), new IPAddr("172.0.0.2"));


	Packet pkt;
	pkt.setSrcIP(h1.getIP());
	pkt.setDstIP(h2.getIP());
	//pkt.setDstIP(new IPAddr("10.0.0.5"));
	h1.sendPacket(1, &pkt);
	h1.sendPacket(1.6, &pkt);
	h1.sendPacket(3, &pkt);


	s.start();

	return 0;
}
*/
