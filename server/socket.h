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

/*************************
 * The code for setting up socket connections between the server
 *  and the clients
 *************************/

/******
 * Get a server side socket, currently allowing just one client
 * Returns -1 if error
 ******/
int getServerSocket(void);


/******
 * Get a client side socket
 * Argument hostname: host name of the server
 * Returns -1 if error
 ******/
int getClientSocket(char *hostName);
