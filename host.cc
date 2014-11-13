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
#include "host.h"
#include "addr.h"
#include "packet.h"
#include "simulator.h"

void
Host::aPacketReceived(Packet *pkt)
{
#ifdef DEBUG
	std::cout << "HOST" << std::endl;
#endif
	// If that packet does not belongs to me, ignore it
	if (IPAddr::compare(pkt->getDstIP(), getIP()))
		return;
	packetReceived(pkt);
}

void
Host::packetReceived(Packet *pkt)
{

	std::cout << "At " << sim->now()
		<< " RECV "
		<< pkt->getSrcIP()->toStr()
		<< " -> "
		<< pkt->getDstIP()->toStr()
		<< std::endl;
}

