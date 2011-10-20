/* ***** BEGIN LICENSE BLOCK *****
 * 
 * BBN Rule Editor/Engine for Address and AS Number PKI
 * Verison 1.0
 * 
 * COMMERCIAL COMPUTER SOFTWARE�RESTRICTED RIGHTS (JUNE 1987)
 * US government users are permitted restricted rights as
 * defined in the FAR.  
 *
 * This software is distributed on an "AS IS" basis, WITHOUT
 * WARRANTY OF ANY KIND, either express or implied.
 *
 * Copyright (C) Raytheon BBN Technologies Corp. 2007.  All Rights Reserved.
 *
 * Contributor(s):  Charlie Gardiner
 *
 * ***** END LICENSE BLOCK ***** */
package skaction;
import name.*;
import Algorithms.*;
import certificate.*;
import crlv2.*;
import asn.*;
public class DSS_Parms extends AsnSequence
    {
    public AsnInteger p = new AsnInteger();
    public AsnInteger q = new AsnInteger();
    public AsnInteger g = new AsnInteger();
    public DSS_Parms()
        {
        _tag = AsnStatic.ASN_SEQUENCE;
        _type = (short)AsnStatic.ASN_SEQUENCE;
        _setup((AsnObj)null, p, (short)0, (int)0x0);
        _setup(p, q, (short)0, (int)0x0);
        _setup(q, g, (short)0, (int)0x0);
        }
    public DSS_Parms set(DSS_Parms frobj)
        {
        ((AsnObj)this).set(frobj);
	return this;
	}
    }