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

#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <stdint.h>

class Node;
class Packet;

class Event {
protected:
	double time;
	char type;
public:
	void setTime(double);
	double getTime();
	char getType();
	virtual std::string toStr();
};

class EventCompare {
public:
	bool operator() (Event &, Event &);
	bool operator() (Event *, Event *);
};

class SendEvent : public Event {
private:
	Node *from;
	Node *to;
	Packet *pkt;
public:
	SendEvent();
	void setSource(Node &);
	Node *getSource();
	void setDestination(Node &);
	Node *getDestination();
	void setPacket(Packet *pkt);
	Packet *getPacket();
	std::string toStr();
};

class ReceiveEvent : public Event {
private:
	Node *from;
	Node *to;
	Packet *pkt;
public:
	ReceiveEvent();
	void setSource(Node &);
	Node *getSource();
	void setDestination(Node &);
	Node *getDestination();
	void setPacket(Packet *pkt);
	Packet *getPacket();
	std::string toStr();
};

class DropEvent : public Event {
private:
	Node *from;
	Node *to;
	Packet *pkt;
public:
	DropEvent();
	void setSource(Node &);
	Node *getSource();
	void setDestination(Node &);
	Node *getDestination();
	void setPacket(Packet *pkt);
	Packet *getPacket();
	std::string toStr();
};

#endif

