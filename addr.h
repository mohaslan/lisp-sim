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

#ifndef ADDR_H
#define ADDR_H

#include <stdint.h>
#include <string>
#include <utility>

class IPAddr {
private:
	uint32_t addr;
	static uint32_t string_to_uint32(std::string);
	static std::string uint32_to_string(uint32_t);
public:
	IPAddr();
	IPAddr(std::string);
	IPAddr(uint32_t);
	std::string toStr();
	uint32_t toInt();
	static bool compare(IPAddr *, IPAddr *);
	static bool inRange(IPAddr *, IPAddr *, IPAddr *);
	bool isValid();
};

class NetMask : public IPAddr {
public:
	NetMask(std::string);
};

class SubNetting {
private:
	static uint8_t bitCount(uint32_t);
public:
	static std::pair<IPAddr, IPAddr> *getRange(IPAddr *, NetMask *);
};

#endif
