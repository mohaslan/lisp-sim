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
#include <string>
#include "logger.h"
#include "simulator.h"
#include "event.h"
#include "node.h"
#include "link.h"
#include "addr.h"
#include "packet.h"


Logger::Logger(std::string log_name)
{
	this->log_name = log_name;
	std::string nam = log_name + ".nam";
	namfile.open(nam.c_str());
	std::cout << "logging to nam trace file: " << nam << std::endl;
}

void
Logger::setSimulator(Simulator *sim)
{
	this->sim = sim;
}

void
Logger::start()
{
	std::vector<Node *> nodes = sim->getNodes();
	for (int i=0 ; i<nodes.size() ; i++) {
		namfile << "n -t *"
			//<< " -a " << nodes[i]->getIP()->toInt()
			<< " -a " << nodes[i]->getID()
			//<< " -s " << nodes[i]->getIP()->toInt()
			<< " -s " << nodes[i]->getID()
			<< " -S " << "UP"
			<< " -v circle -c black -i black"
			<< std::endl;
	}
	std::vector<Link *> links = sim->getLinks();
	for (int i=0 ; i<links.size() ; i++) {
		namfile << "l -t *"
			//<< " -s " << links[i]->getEndPoint1()->getIP()->toInt()
			<< " -s " << links[i]->getEndPoint1()->getID()
			//<< " -d " << links[i]->getEndPoint2()->getIP()->toInt()
			<< " -d " << links[i]->getEndPoint2()->getID()
			<< " -S " << "UP"
			<< " -r 1000000 -D 0.01 -c black"
			<< std::endl;
	}
}

void
Logger::stop()
{
	namfile.close();
}

void
Logger::log(Event *event)
{
	switch (event->getType()) {
	case 's': {
		SendEvent *ev = (SendEvent *)event;
		namfile << "h"
			<< " -t " << ev->getTime()
			//<< " -s " << ev->getSource()->getIP()->toInt()
			<< " -s " << ev->getSource()->getID()
			//<< " -d " << ev->getDestination()->getIP()->toInt()
			<< " -d " << ev->getDestination()->getID()
			<< " -p " << "udp"
			<< " -e " << "40"
			<< " -c " << "0"
			<< " -i " << "0"
			//<< " -a " << ev->getSource()->getIP()->toInt()
			<< " -a " << ev->getSource()->getID()
			<< std::endl;
		}
		break;
	case 'r': {
		ReceiveEvent *ev = (ReceiveEvent *)event;
		// for nam, only log 'r' at the final destination
		if (!IPAddr::compare(ev->getDestination()->getIP(), ev->getPacket()->getDstIP())) {
			namfile << "r"
				<< " -t " << ev->getTime()
				//<< " -s " << ev->getSource()->getIP()->toInt()
				<< " -s " << ev->getSource()->getID()
				//<< " -d " << ev->getDestination()->getIP()->toInt()
				<< " -d " << ev->getDestination()->getID()
				<< " -p " << "udp"
				<< " -e " << "40"
				<< " -c " << "0"
				<< " -i " << "0"
				//<< " -a " << ev->getSource()->getIP()->toInt()
				<< " -a " << ev->getSource()->getID()
				<< std::endl;
		}
		}
		break;
	case 'd': {
		DropEvent *ev = (DropEvent *)event;
		namfile << "d"
			<< " -t " << ev->getTime()
			//<< " -s " << ev->getSource()->getIP()->toInt()
			<< " -s " << ev->getSource()->getID()
			//<< " -d " << ev->getDestination()->getIP()->toInt()
			<< " -d " << ev->getDestination()->getID()
			<< " -p " << "udp"
			<< " -e " << "40"
			<< " -c " << "0"
			<< " -i " << "0"
			//<< " -a " << ev->getSource()->getIP()->toInt()
			<< " -a " << ev->getSource()->getID()
			<< std::endl;
		}
		break;
	default:
		//namfile << event->toStr() << std::endl;
		break;
	}
}

