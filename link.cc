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

#include "link.h"
#include "simulator.h"
#include "node.h"
#include "addr.h"
#include "packet.h"


void
Link::attachSimulator(Simulator *sim)
{
	this->sim = sim;
}

void
Link::link(Node *a, Node *b)
{
	epoint1 = a;
	epoint2 = b;
	a->addLink(this);
	b->addLink(this);
	delay = 0.1;
}

void
Link::setDelay(double delay)
{
	this->delay = delay;
}

Node *
Link::getEndPoint1()
{
	return epoint1;
}

Node *
Link::getEndPoint2()
{
	return epoint2;
}

void
Link::sendPacket(double time, Node *epoint, Packet *pkt)
{
	Node *sender, *receiver;
	if (!IPAddr::compare(epoint->getIP(), epoint1->getIP())) {
		sender = epoint1;
		receiver = epoint2;
	}
	else if (!IPAddr::compare(epoint->getIP(), epoint2->getIP())) {
		sender = epoint2;
		receiver = epoint1;
	}
	else
		return;

	pkt->setHop(*sender);

	SendEvent *sev = new SendEvent();
	sev->setTime(time + delay);
	sev->setSource(*sender);
	sev->setDestination(*receiver);
	sev->setPacket(pkt);

	// TODO: apply propagation model
	ReceiveEvent *rev = new ReceiveEvent();
	rev->setTime(time + delay);
	rev->setSource(*sender);
	rev->setDestination(*receiver);
	rev->setPacket(pkt);

	sim->addEvent(*sev);
	sim->addEvent(*rev);
}

