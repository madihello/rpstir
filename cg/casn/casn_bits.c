/* Jun  8 2004 773U  */
/* Jun  8 2004 GARDINER put test for numm pointer into _clear_error() */
/* Jun  3 2004 769U  */
/* Jun  3 2004 GARDINER more fixes for asn_obj tests */
/* May 11 2004 763U  */
/* May 11 2004 GARDINER fixed warnings */
/* May 11 2004 762U  */
/* May 11 2004 GARDINER more fixes for asn_obj tests */
/* Apr 21 2004 761U  */
/* Apr 21 2004 GARDINER fixed for half of casn_obj testing */
/* Mar 29 2004 746U  */
/* Mar 29 2004 GARDINER corrected warnings */
/* Mar 29 2004 745U  */
/* Mar 29 2004 GARDINER corrected warnings */
/* Mar 25 2004 743U  */
/* Mar 25 2004 GARDINER started */
/* */
/*****************************************************************************
File:     casn_bits.c
Contents: Functions to hanndle ASN.1 BIT STRING objects.
System:   Compact ASN development.
Created:
Author:   Charles W. Gardiner <gardiner@bbn.com>

Remarks:

COPYRIGHT 2004 BBN Systems and Technologies
10 Moulton St.
Cambridge, Ma. 02138
617-873-3000
*****************************************************************************/

char casn_bits_sfcsid[] = "@(#)casn_bits.c 773P";
#include "casn.h"

extern struct casn *_go_up(struct casn *);
extern int _casn_obj_err(struct casn *, int),
	_clear_error(struct casn *);

extern void _fill_upward(struct casn *casnp, int val);
extern void *_free_it(void *);

int _readsize_bits(struct casn *casnp, uchar *to, int *shift, int mode)
    {
    int err;
    ushort box;
    uchar *b, *c, *e;

    if (_clear_error(casnp) < 0) return -1;
    err = 0;
    if (casnp->type != ASN_BITSTRING) err = ASN_TYPE_ERR;
    else if (!casnp->startp) err = ASN_MANDATORY_ERR;
    if (err) return _casn_obj_err(casnp, err);
    c = casnp->startp;
    if ((casnp->flags & ASN_ENUM_FLAG)) *shift = 0;
    else *shift = (int)*c;
    for (e = &c[casnp->lth], c++; --e > c && !*e; );
    if (*e) e++;
    for (err = box = 0, *(b = to) = 0; c < e; c++, err++)
	{
	box = *c << 8;
	box >>= *shift;
	*b |= (box >> 8);
	if (mode) b++;
	*b = box & 0xFF;
	}
    return err;
    }

int read_casn_bits(struct casn *casnp, uchar *to, int *shift)
    {
    return _readsize_bits(casnp, to, shift, 1);
    }

int vsize_casn_bits(struct casn *casnp)
    {
    int shift;
    uchar buf[4];

    return _readsize_bits(casnp, buf, &shift, 0);
    }

int write_casn_bits(struct casn *casnp, uchar *from, int lth, int shift) 
    {
    uchar *c, *e;
    ushort box;

    if (_clear_error(casnp) < 0) return -1;
    if (casnp->type != ASN_BITSTRING) return _casn_obj_err(casnp, ASN_TYPE_ERR);
    _free_it(casnp->startp);
    c = casnp->startp = (uchar *)calloc(1, (casnp->lth = lth + 1));
    for (e = &from[lth]; from < e; from++)
	{
	box = (ushort)*from++;   
	box <<= shift;           // first time top 8 bits are empty
	*c++ |= (box >> 8);    
	*c = box & 0xFF;
	}
    *casnp->startp = (uchar)shift;
    _fill_upward(casnp, ASN_FILLED_FLAG);
    return casnp->lth;
    }
