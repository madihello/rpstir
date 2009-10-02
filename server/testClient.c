/* ***** BEGIN LICENSE BLOCK *****
 *
 * BBN Address and AS Number PKI Database/repository software
 * Version 1.0
 *
 * US government users are permitted unrestricted rights as
 * defined in the FAR.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT
 * WARRANTY OF ANY KIND, either express or implied.
 *
 * Copyright (C) BBN Technologies 2007.  All Rights Reserved.
 *
 * Contributor(s):  David Montana
 *
 * ***** END LICENSE BLOCK ***** */

/*
  $Id: query.c 857 2009-09-30 15:27:40Z dmontana $
*/

/************************
 * Sample client for the purposes of testing and demonstrating
 *   use of the library
 ***********************/

#include "pdu.h"
#include "socket.h"
#include <stdio.h>

static int getBits(uint val, uint start, uint len) {
	return (val << start) >> (32 - len);
}

int main(int argc, char **argv) {
	int sock, i;
	PDU request, *response;
	IPPrefixData *prefixData;

	if ((sock = getClientSocket("localhost")) == -1) {
		printf ("Error opening socket\n");
		return -1;
	}
	fillInPDUHeader(&request, PDU_RESET_QUERY, 1);
	if (writePDU(&request, sock) == -1) {
		printf ("Error writing reset query\n");
		return -1;
	}
	if (! (response = readPDU(sock))) {
		printf ("Error reading cache response\n");
		return -1;
	}
	if (response->pduType != PDU_CACHE_RESPONSE) {
		printf ("Was expecting cache response, got %d\n", response->pduType);
		return -1;
	}
	for (response = readPDU(sock);
		 response && (response->pduType != PDU_END_OF_DATA);
		 response = readPDU(sock)) {
		prefixData = (IPPrefixData *) response->typeSpecificData;
		if (response->pduType == PDU_IPV4_PREFIX) {
			printf("Received pdu of type IPv4 prefix\naddr = ");
			for (i = 0; i < 4; i++)
				printf("%d%s", getBits(prefixData->ipAddress[0], 8*i, 8),
					   (i == 3) ? "\n" : ".");
		} else if (response->pduType == PDU_IPV6_PREFIX) {
			printf("Received pdu of type IPv6 prefix\naddr = ");
			for (i = 0; i < 8; i++)
				printf("%d%s",
					   getBits(prefixData->ipAddress[i/2], (i%2)*16, 16),
					   (i == 7) ? "\n" : ":");
		} else {
			printf("Received unexpected pdu type %d\n", response->pduType);
			return -1;
		}
		printf ("as# = %d len = %d max = %d\n", prefixData->asNumber,
				prefixData->prefixLength, prefixData->maxLength);
	}
	if (! response) {
		printf ("Missing end-of-data pdu\n");
		return -1;
	}
	printf("Completed successfully\n");
	return 1;
}
