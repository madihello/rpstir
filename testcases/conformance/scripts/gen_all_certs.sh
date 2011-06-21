#!/bin/bash -x
#
# ***** BEGIN LICENSE BLOCK *****
#
#  BBN Address and AS Number PKI Database/repository software
#  Version 4.0
#
#  US government users are permitted unrestricted rights as
#  defined in the FAR.
#
#  This software is distributed on an "AS IS" basis, WITHOUT
#  WARRANTY OF ANY KIND, either express or implied.
#
#  Copyright (C) Raytheon BBN Technologies 2011.  All Rights Reserved.
#
#  Contributor(s): Charlie Gardiner, Andrew Chi
#
#  ***** END LICENSE BLOCK ***** */

# gen_all_certs.sh - create all certificates for RPKI syntax
#                    conformance test

# Set up RPKI environment variables if not already done.
THIS_SCRIPT_DIR=$(dirname $0)
. $THIS_SCRIPT_DIR/../../../envir.setup

# Safe bash shell scripting practices
set -o errexit			# exit if anything fails
set -o errtrace			# shell functions inherit 'ERR' trap
trap "echo Error encountered during execution of $0 1>&2" ERR

# Usage
usage ( ) {
    usagestr="
Usage: $0 [options]

Options:
  -P        \tApply patches instead of prompting user to edit (default = false)
  -h        \tDisplay this help file

This script creates a large number of certificates, and for each one
prompts the user multiple times to interactively edit (e.g., in order
to introduce errors), and captures those edits in '.patch' files
(output of diff -u).  Later, running $0 with the -P option can replay
the creation process by automatically applying those patch files
instead of prompting for user intervention.

This tool assumes the repository structure in the diagram below.  It
creates a ton of certificates in the position of the certificate
labeled 'Child'.  In the Child's SIA, the accessMethod
id-ad-rpkiManifest will have an accessLocation of
rsync://rpki.bbn.com/conformance/root/empty/doesNotExist.mft, and that
manifest will be intentionally omitted from the directory named
'empty'.  This allows us to reuse the same empty directory as the SIA
for the large number of certificates that we will generate using this
script.


               +-----------------------------------+
               | rsync://rpki.bbn.com/conformance/ |
               |     +--------+                    |
         +---------->|  Root  |                    |
         |     |     |  cert  |                    |
         |  +---------- SIA   |                    |
         |  |  |     +--------+                    |
         |  |  +-----------------------------------+
         |  |
         |  |
         |  |  +----------------------------------------+
         |  |  | rsync://rpki.bbn.com/conformance/root/ |
         |  +->|   +--------+     +------------+        |
         |     |   | *Child |     | CRL issued |        |
         |     |   | CRLDP------->| by Root    |        |
         +----------- AIA   |     | root.crl   |        |
               |   |  SIA------+  +------------+        |
               |   +--------+  |  +-----------------+   |
               |               |  | Manifest issued |   |
               |               |  | by Root         |   |
               | Root's Repo   |  | root.mft        |   |
               | Directory     |  +-----------------+   |
               +---------------|------------------------+
                               |
                               V
               +----------------------------------------------+
               | rsync://rpki.bbn.com/conformance/root/empty/ |
               |                                              |
               | Empty Directory (MFT intentionally omitted)  |
               +----------------------------------------------+

Inputs:
  -P - (optional) use patch mode for automatic insertion of errors

Outputs:
  child CA certificates - AS/IP is hardcoded in goodCert.raw template
  patch files - manual edits are saved as diff output in
                'badCert<filestem>.stageN.patch' (N=0..1) in the patch
                directory
    "
    printf "${usagestr}\n"
    exit 1
}

# NOTES

# 1. Variable naming convention -- preset constants and command line
# arguments are in ALL_CAPS.  Derived/computed values are in
# lower_case.

# 2. Assumes write-access to current directory even though the output
# directory will be different.

# Set up paths to ASN.1 tools.
CGTOOLS=$RPKI_ROOT/cg/tools	# Charlie Gardiner's tools

# Options and defaults
OUTPUT_DIR="$RPKI_ROOT/testcases/conformance/raw/root"
USE_EXISTING_PATCHES=

# Process command line arguments.
while getopts Ph opt
do
  case $opt in
      P)
	  USE_EXISTING_PATCHES=1
	  ;;
      h)
	  usage
	  ;;
  esac
done
shift $((OPTIND - 1))
if [ $# != "0" ]
then
    usage
fi

###############################################################################
# Computed Variables
###############################################################################

if [ $USE_EXISTING_PATCHES ]
then
    patch_option="-P"
else
    patch_option=
fi

single_cert_script="$RPKI_ROOT/testcases/conformance/scripts/make_test_cert.sh"
single_cert_cmd="${single_cert_script} ${patch_option} -o ${OUTPUT_DIR}"

###############################################################################
# Check for prerequisite tools and files
###############################################################################

ensure_file_exists ( ) {
    if [ ! -e "$1" ]
    then
	echo "Error: file not found - $1" 1>&2
	exit 1
    fi
}

ensure_dir_exists ( ) {
    if [ ! -d "$1" ]
    then
	echo "Error: directory not found - $1" 1>&2
	exit 1
    fi
}

ensure_dir_exists "$OUTPUT_DIR"
ensure_dir_exists "$CGTOOLS"
ensure_file_exists "${single_cert_script}"

###############################################################################
# Generate Child certificates
###############################################################################

${single_cert_cmd} 101 AIA2AccessDesc
${single_cert_cmd} 102 AIABadAccess
${single_cert_cmd} 103 AIAAccessLoc
${single_cert_cmd} 104 AIACrit
${single_cert_cmd} 105 AKIHash
${single_cert_cmd} 106 AKILth
${single_cert_cmd} 107 BadExtension1
${single_cert_cmd} 109 BasicConstrNoCA
${single_cert_cmd} 110 BasicConstrNoCrit
${single_cert_cmd} 111 BasicConstrPathLth
${single_cert_cmd} 112 Cpol2oid
${single_cert_cmd} 113 CpolNoCrit
${single_cert_cmd} 114 CRLDP2DistPt
${single_cert_cmd} 115 CRLDPCrit
${single_cert_cmd} 116 CRLDPCrlIssuer
${single_cert_cmd} 117 CRLDPNoDistPt
${single_cert_cmd} 118 CRLDPReasons
${single_cert_cmd} 119 EKU
${single_cert_cmd} 120 InnerSigAlg
${single_cert_cmd} 121 IssuerOID
${single_cert_cmd} 122 Issuer2Sets
${single_cert_cmd} 123 IssuerUtf
${single_cert_cmd} 124 Issuer2Seq
${single_cert_cmd} 125 Issuer2SerNums
${single_cert_cmd} 126 IssUID
${single_cert_cmd} 127 KUsageExtra
${single_cert_cmd} 128 KUsageNoCertSign
${single_cert_cmd} 129 KUsageNoCrit
${single_cert_cmd} 131 KUsageNoCRLSign
${single_cert_cmd} 134 OuterSigAlg
${single_cert_cmd} 135 PubKeyAlg
${single_cert_cmd} 136 PubKeyExp
${single_cert_cmd} 137 PubKeyLth
${single_cert_cmd} 138 ResourcesASNoCrit
${single_cert_cmd} 139 ResourcesBadAFI
${single_cert_cmd} 140 ResourcesBadASOrder
${single_cert_cmd} 141 ResourcesBadV4Order
${single_cert_cmd} 142 ResourcesBadV6Order
${single_cert_cmd} 143 ResourcesIPNoCrit
${single_cert_cmd} 144 ResourcesNone
${single_cert_cmd} 145 ResourcesSAFI
${single_cert_cmd} 147 SIAAccessLoc
${single_cert_cmd} 148 SIAAccessMethod
${single_cert_cmd} 149 SIAMissing
${single_cert_cmd} 150 SKIHash
${single_cert_cmd} 151 SKILth
${single_cert_cmd} 152 SubjectOID
${single_cert_cmd} 153 Subject2Sets
${single_cert_cmd} 154 SubjectUtf
${single_cert_cmd} 155 Subject2Seq
${single_cert_cmd} 156 Subject2SerNum
${single_cert_cmd} 157 SubjUID
${single_cert_cmd} 158 ValCrossed
${single_cert_cmd} 159 ValFromFuture
${single_cert_cmd} 160 ValFromTyp
${single_cert_cmd} 162 ValToPast
${single_cert_cmd} 163 ValToTyp
${single_cert_cmd} 164 VersionNeg
${single_cert_cmd} 165 Version1
${single_cert_cmd} 166 Version2
${single_cert_cmd} 167 Version4
${single_cert_cmd} 168 SerNum
${single_cert_cmd} 169 AIA2x
${single_cert_cmd} 170 SIA2x
${single_cert_cmd} 171 NoAIA
${single_cert_cmd} 172 NoSIA
${single_cert_cmd} 173 NoBasicConstr
${single_cert_cmd} 174 2BasicConstr
${single_cert_cmd} 175 NoSKI
${single_cert_cmd} 176 2SKI
${single_cert_cmd} 177 NoAKI
${single_cert_cmd} 178 2AKI
${single_cert_cmd} 179 NoKeyUsage
${single_cert_cmd} 180 2KeyUsage
${single_cert_cmd} 181 2CRLDP
${single_cert_cmd} 182 NoCRLDP
${single_cert_cmd} 183 NoCpol
${single_cert_cmd} 184 2Cpol
${single_cert_cmd} 185 2IPAddr
${single_cert_cmd} 186 2ASNum