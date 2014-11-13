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

#include <sstream>
#include "event.h"
#include "node.h"
#include "addr.h"
#include "packet.h"

void
Event::setTime(double time)
{
	this->time = time;
}

double
Event::getTime()
{
	return time;
}

char
Event::getType()
{
	return type;
}

std::string
Event::toStr()
{
	std::stringstream ss;
	ss << "event: time=" << time << ", type=" << type;
	return ss.str();
}


SendEvent::SendEvent()
{
	this->type = 's';
}

void
SendEvent::setSource(Node &n)
{
	from = &n; 
}

Node *
SendEvent::getSource()
{
	return from;
}

void
SendEvent::setDestination(Node &n)
{
	to = &n;
}

Node *
SendEvent::getDestination()
{
	return to;
}

void
SendEvent::setPacket(Packet *pkt)
{
	this->pkt = pkt;
}

Packet *
SendEvent::getPacket()
{
	return pkt;
}

std::string
SendEvent::toStr()
{
	std::stringstream ss;
	ss << "event: time=" << time << ", type=" << type << ", src=" << from->getIP()->toStr() << ", dst=" << to->getIP()->toStr();
	return ss.str();
}


ReceiveEvent::ReceiveEvent()
{
	this->type = 'r';
}

void
ReceiveEvent::setSource(Node &n)
{
	from = &n; 
}

Node *
ReceiveEvent::getSource()
{
	return from;
}

void
ReceiveEvent::setDestination(Node &n)
{
	to = &n;
}

Node *
ReceiveEvent::getDestination()
{
	return to;
}

void
ReceiveEvent::setPacket(Packet *pkt)
{
	this->pkt = pkt;
}

Packet *
ReceiveEvent::getPacket()
{
	return pkt;
}

std::string
ReceiveEvent::toStr()
{
	std::stringstream ss;
	ss << "event: time=" << time << ", type=" << type << ", src=" << from->getIP()->toStr() << ", dst=" << to->getIP()->toStr();
	return ss.str();
}


bool
EventCompare::operator() (Event &a, Event &b)
{
	return a.getTime () > b.getTime() ? true : false;
	//return operator()(&a, &b);
}


bool
EventCompare::operator() (Event *a, Event *b)
{
	return a->getTime () > b->getTime() ? true : false;
}

DropEvent::DropEvent()
{
	this->type = 'd';
}

void
DropEvent::setSource(Node &n)
{
	from = &n;
}

Node *
DropEvent::getSource()
{
	return from;
}

void
DropEvent::setDestination(Node &n)
{
	to = &n;
}

Node *
DropEvent::getDestination()
{
	return to;
}

void
DropEvent::setPacket(Packet *pkt)
{
	this->pkt = pkt;
}

Packet *
DropEvent::getPacket()
{
	return pkt;
}

std::string
DropEvent::toStr()
{
	std::stringstream ss;
	ss << "event: time=" << time << ", type=" << type << ", src=" << from->getIP()->toStr() << ", dst=" << to->getIP()->toStr();
	return ss.str();
}

