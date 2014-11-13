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
#include <sstream>
#include <cstdio>
#include <cmath>
#include "addr.h"

uint32_t
IPAddr::string_to_uint32(std::string s)
{
	short arr[4];
	sscanf(s.c_str(), "%hu.%hu.%hu.%hu", &arr[0], &arr[1], &arr[2], &arr[3]);
	return (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];
}

std::string
IPAddr::uint32_to_string(uint32_t ip)
{
	short arr[4];
	arr[0] = (ip & (0xff << 24)) >> 24;
	arr[1] = (ip & (0xff << 16)) >> 16;
	arr[2] = (ip & (0xff << 8)) >> 8;
	arr[3] = ip & 0xff;
	std::stringstream ss;
	ss << arr[0] << "." << arr[1] << "." << arr[2] << "." << arr[3];
	return ss.str(); 
}

IPAddr::IPAddr()
{
	this->addr = 0x0;
}

IPAddr::IPAddr(std::string addr)
{
	this->addr = string_to_uint32(addr);
}

IPAddr::IPAddr(uint32_t addr)
{
	this->addr = addr;
}

std::string
IPAddr::toStr()
{
	return uint32_to_string(addr);
}

uint32_t
IPAddr::toInt()
{
	return addr;
}

bool
IPAddr::compare(IPAddr *ip1, IPAddr *ip2)
{
	return ip1->addr == ip2->addr ? false : true;
}

bool
IPAddr::inRange(IPAddr *ipx, IPAddr *ip1, IPAddr *ip2)
{
	if (ipx->addr >= ip1->addr && ipx->addr <= ip2->addr)
		return true;
	return false;
}

bool
IPAddr::isValid()
{
	if (addr == 0x0)
		return false;
	return true;
}

NetMask::NetMask(std::string mask)
	: IPAddr(mask)
{
}


// borrowed from http://blogs.msdn.com/b/jeuge/archive/2005/06/08/hakmem-bit-count.aspx
uint8_t
SubNetting::bitCount(uint32_t dword)
{
	unsigned int count;
	count = dword - ((dword >> 1) & 033333333333) - ((dword >> 2) & 011111111111);
	return (uint8_t)(((count + (count >> 3)) & 030707070707) % 63);
}

std::pair<IPAddr, IPAddr> *
SubNetting::getRange(IPAddr *subnet, NetMask *netmask)
{
	std::pair<IPAddr, IPAddr> *range = new std::pair<IPAddr, IPAddr>();
	range->first = IPAddr(subnet->toInt() & netmask->toInt());
	uint32_t n_bits = (uint32_t) (pow(2, 32 - bitCount(netmask->toInt())));
	range->second = IPAddr(range->first.toInt() + n_bits - 1);
	return range;
}


