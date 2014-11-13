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

#include "simulator.h"
#include "node.h"
#include "link.h"
#include "event.h"
#include "logger.h"

#include <iostream>

Simulator::Simulator()
{
	clock = 0;
}

double
Simulator::now()
{
	return clock;
}

void
Simulator::addNode(Node &n)
{
	n.attachSimulator(this);
	nodes.push_back(&n);
}

void
Simulator::addLink(Link &l)
{
	l.attachSimulator(this);
	links.push_back(&l);
}

std::vector<Node *>
Simulator::getNodes()
{
	return nodes;
}

std::vector<Link *>
Simulator::getLinks()
{
	return links;
}

/*
void
Simulator::at(uint32_t time, Event &event)
{
	event.setTime(time);
	event_queue.push(&event);
}
*/

void
Simulator::attachLogger(Logger &logger)
{
	this->logger = &logger;
	this->logger->setSimulator(this);
}

void
Simulator::addEvent(Event &event)
{
	event_queue.push(&event);
}

void
Simulator::start()
{
	logger->start();
	while (!event_queue.empty()) {
		Event *event = event_queue.top();
		event_queue.pop();
		if (event->getTime() >= clock) {
			clock = event->getTime();
			if (event->getType() == 's') {
				SendEvent *ev = (SendEvent *)event;
				logger->log(ev);
			}
			else if (event->getType() == 'r') {
				ReceiveEvent *ev = (ReceiveEvent *)event;
				logger->log(ev);
				ev->getDestination()->aPacketReceived(ev->getPacket()); 
			}
			else if (event->getType() == 'd') {
				DropEvent *ev = (DropEvent *)event;
				logger->log(ev);
			}
			else {
				logger->log(event);
			}
		}
		delete event;
	}
	logger->stop();
}


