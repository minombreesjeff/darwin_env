//   NOTE: this is a machine generated file--editing not recommended
//
// sm_x509cmn.cpp - class member functions for ASN.1 module CommonX509Definitions
//
//   This file was generated by snacc on Wed Jun 27 16:40:55 2001
//   UBC snacc written by Mike Sample
//   A couple of enhancements made by IBM European Networking Center


#include "asn-incl.h"
#include "sm_vdatypes.h"
#include "sm_x501ud.h"
#include "sm_x411ub.h"
#include "sm_x411mtsas.h"
#include "sm_x501if.h"
#include "sm_x520sa.h"
#include "sm_x509cmn.h"
#include "sm_x509af.h"
#include "sm_x509ce.h"
#include "pkcs1oids.h"
#include "pkcs9oids.h"
#include "sm_cms.h"
#include "sm_ess.h"
#include "pkcs7.h"
#include "pkcs8.h"
#include "appleoids.h"

//------------------------------------------------------------------------------
// value defs


//------------------------------------------------------------------------------
// class member definitions:

EDIPartyName::EDIPartyName()
{
  nameAssigner = NULL;
#if TCL
  partyName = new DirectoryString;
#else
  partyName = NULL; // incomplete initialization of mandatory element!
#endif // TCL
}

EDIPartyName::EDIPartyName (const EDIPartyName &)
{
  Asn1Error << "use of incompletely defined EDIPartyName::EDIPartyName (const EDIPartyName &)" << endl;
  abort();
}

EDIPartyName::~EDIPartyName()
{
  delete nameAssigner;
  delete partyName;
}

AsnType *EDIPartyName::Clone() const
{
  return new EDIPartyName;
}

AsnType *EDIPartyName::Copy() const
{
  return new EDIPartyName (*this);
}

#if SNACC_DEEP_COPY
EDIPartyName &EDIPartyName::operator = (const EDIPartyName &that)
#else // SNACC_DEEP_COPY
EDIPartyName &EDIPartyName::operator = (const EDIPartyName &)
#endif // SNACC_DEEP_COPY
{
#if SNACC_DEEP_COPY
  if (this != &that)
  {
    if (that.nameAssigner)
    {
      if (!nameAssigner)
        nameAssigner = new DirectoryString;
      *nameAssigner = *that.nameAssigner;
    }
    else
    {
      delete nameAssigner;
      nameAssigner = NULL;
    }
    if (that.partyName)
    {
      if (!partyName)
        partyName = new DirectoryString;
      *partyName = *that.partyName;
    }
    else
    {
      delete partyName;
      partyName = NULL;
    }
  }

  return *this;
#else // SNACC_DEEP_COPY
  Asn1Error << "use of incompletely defined EDIPartyName &EDIPartyName::operator = (const EDIPartyName &)" << endl;
  abort();
  // if your compiler complains here, check the -novolat option
#endif // SNACC_DEEP_COPY
}

AsnLen
EDIPartyName::BEncContent (BUF_TYPE b)
{
  AsnLen totalLen = 0;
  AsnLen l;

      BEncEocIfNec (b);
    l = partyName->BEncContent (b);
    l += BEncConsLen (b, l);

    l += BEncTag1 (b, CNTX, CONS, 1);
    totalLen += l;

  if (NOT_NULL (nameAssigner))
  {
      BEncEocIfNec (b);
    l = nameAssigner->BEncContent (b);
    l += BEncConsLen (b, l);

    l += BEncTag1 (b, CNTX, CONS, 0);
    totalLen += l;
  }

  return totalLen;
} // EDIPartyName::BEncContent


void EDIPartyName::BDecContent (BUF_TYPE b, AsnTag /*tag0*/, AsnLen elmtLen0, AsnLen &bytesDecoded, ENV_TYPE env)
{
  AsnTag tag1;
  AsnLen seqBytesDecoded = 0;
  AsnLen elmtLen1;
  AsnLen elmtLen2;
  tag1 = BDecTag (b, seqBytesDecoded, env);

  if ((tag1 == MAKE_TAG_ID (CNTX, CONS, 0)))
  {
    elmtLen1 = BDecLen (b, seqBytesDecoded, env);
    tag1 = BDecTag (b, seqBytesDecoded, env);
    elmtLen2 = BDecLen (b, seqBytesDecoded, env);
    nameAssigner = new DirectoryString;
    nameAssigner->BDecContent (b, tag1, elmtLen2, seqBytesDecoded, env);
    if (elmtLen1 == INDEFINITE_LEN)
      BDecEoc (b, seqBytesDecoded, env);

    tag1 = BDecTag (b, seqBytesDecoded, env);
  }

  if ((tag1 == MAKE_TAG_ID (CNTX, CONS, 1)))
  {
    elmtLen1 = BDecLen (b, seqBytesDecoded, env);
    tag1 = BDecTag (b, seqBytesDecoded, env);
    elmtLen2 = BDecLen (b, seqBytesDecoded, env);
    partyName = new DirectoryString;
    partyName->BDecContent (b, tag1, elmtLen2, seqBytesDecoded, env);
    if (elmtLen1 == INDEFINITE_LEN)
      BDecEoc (b, seqBytesDecoded, env);

  }
  else
  {
    Asn1Error << "ERROR - SEQUENCE is missing non-optional elmt." << endl;
    longjmp (env, -100);
  }

  bytesDecoded += seqBytesDecoded;
  if (elmtLen0 == INDEFINITE_LEN)
  {
    BDecEoc (b, bytesDecoded, env);
    return;
  }
  else if (seqBytesDecoded != elmtLen0)
  {
    Asn1Error << "ERROR - Length discrepancy on sequence." << endl;
    longjmp (env, -101);
  }
  else
    return;
} // EDIPartyName::BDecContent

AsnLen EDIPartyName::BEnc (BUF_TYPE b)
{
  AsnLen l;
  l = BEncContent (b);
  l += BEncConsLen (b, l);
  l += BEncTag1 (b, UNIV, CONS, SEQ_TAG_CODE);
  return l;
}

void EDIPartyName::BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env)
{
  AsnTag tag;
  AsnLen elmtLen1;

  if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, CONS, SEQ_TAG_CODE))
  {
    Asn1Error << "EDIPartyName::BDec: ERROR - wrong tag" << endl;
    longjmp (env, -102);
  }
  elmtLen1 = BDecLen (b, bytesDecoded, env);
  BDecContent (b, tag, elmtLen1, bytesDecoded, env);
}

int EDIPartyName::BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded)
{
    bytesEncoded = BEnc (b);
    return !b.WriteError();
}

int EDIPartyName::BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded)
{
    ENV_TYPE env;
    int val;

    bytesDecoded = 0;
    if ((val = setjmp (env)) == 0)
    {
         BDec (b, bytesDecoded, env);
         return !b.ReadError();
    }
    else
        return false;
}

void EDIPartyName::Print (ostream &os) const
{
#ifndef NDEBUG
  os << "{ -- SEQUENCE --" << endl;
  indentG += stdIndentG;

  if (NOT_NULL (nameAssigner))
  {
    Indent (os, indentG);
    os << "nameAssigner ";
    os << *nameAssigner;
  }
  else
  {
    Indent (os, indentG);
    os << "nameAssigner ";
    os << "-- void --";
    os << "," << endl;
  }

  if (NOT_NULL (partyName))
  {
    Indent (os, indentG);
    os << "partyName ";
    os << *partyName;
  }
  else
  {
    Indent (os, indentG);
    os << "partyName ";
    os << "-- void --";
    os << endl;
  }

  os << endl;
  indentG -= stdIndentG;
  Indent (os, indentG);
  os << "}";
#endif /* NDEBUG */
} // EDIPartyName::Print


GeneralName::GeneralName()
{
  choiceId = otherNameCid;
#if TCL
  otherName = new OTHER_NAME;
#else
  otherName = NULL; // incomplete initialization of mandatory element!
#endif // TCL
}

GeneralName::GeneralName (const GeneralName &)
{
  Asn1Error << "use of incompletely defined GeneralName::GeneralName (const GeneralName &)" << endl;
  abort();
}

GeneralName::~GeneralName()
{
  switch (choiceId)
  {
    case otherNameCid:
      delete otherName;
      break;
    case rfc822NameCid:
      delete rfc822Name;
      break;
    case dNSNameCid:
      delete dNSName;
      break;
    case x400AddressCid:
      delete x400Address;
      break;
    case directoryNameCid:
      delete directoryName;
      break;
    case ediPartyNameCid:
      delete ediPartyName;
      break;
    case uniformResourceIdentifierCid:
      delete uniformResourceIdentifier;
      break;
    case iPAddressCid:
      delete iPAddress;
      break;
    case registeredIDCid:
      delete registeredID;
      break;
  } // end of switch
} // end of destructor

AsnType *GeneralName::Clone() const
{
  return new GeneralName;
}

AsnType *GeneralName::Copy() const
{
  return new GeneralName (*this);
}

#if SNACC_DEEP_COPY
GeneralName &GeneralName::operator = (const GeneralName &that)
#else // SNACC_DEEP_COPY
GeneralName &GeneralName::operator = (const GeneralName &)
#endif // SNACC_DEEP_COPY
{
#if SNACC_DEEP_COPY
  if (this != &that)
  {
    switch (choiceId)
    {
      case otherNameCid:
        delete otherName;
        break;
      case rfc822NameCid:
        delete rfc822Name;
        break;
      case dNSNameCid:
        delete dNSName;
        break;
      case x400AddressCid:
        delete x400Address;
        break;
      case directoryNameCid:
        delete directoryName;
        break;
      case ediPartyNameCid:
        delete ediPartyName;
        break;
      case uniformResourceIdentifierCid:
        delete uniformResourceIdentifier;
        break;
      case iPAddressCid:
        delete iPAddress;
        break;
      case registeredIDCid:
        delete registeredID;
        break;
    }
    switch (choiceId = that.choiceId)
    {
      case otherNameCid:
        otherName = new OTHER_NAME;
        *otherName = *that.otherName;
        break;
      case rfc822NameCid:
        rfc822Name = new IA5String;
        *rfc822Name = *that.rfc822Name;
        break;
      case dNSNameCid:
        dNSName = new IA5String;
        *dNSName = *that.dNSName;
        break;
      case x400AddressCid:
        x400Address = new ORAddress;
        *x400Address = *that.x400Address;
        break;
      case directoryNameCid:
        directoryName = new Name;
        *directoryName = *that.directoryName;
        break;
      case ediPartyNameCid:
        ediPartyName = new EDIPartyName;
        *ediPartyName = *that.ediPartyName;
        break;
      case uniformResourceIdentifierCid:
        uniformResourceIdentifier = new IA5String;
        *uniformResourceIdentifier = *that.uniformResourceIdentifier;
        break;
      case iPAddressCid:
        iPAddress = new AsnOcts;
        *iPAddress = *that.iPAddress;
        break;
      case registeredIDCid:
        registeredID = new AsnOid;
        *registeredID = *that.registeredID;
        break;
    }
  }

  return *this;
#else // SNACC_DEEP_COPY
  Asn1Error << "use of incompletely defined GeneralName &GeneralName::operator = (const GeneralName &)" << endl;
  abort();
  // if your compiler complains here, check the -novolat option
#endif // SNACC_DEEP_COPY
}

AsnLen
GeneralName::BEncContent (BUF_TYPE b)
{
  AsnLen l;
  switch (choiceId)
  {
    case otherNameCid:
      l = otherName->BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, CNTX, PRIM, 0);
      break;

    case rfc822NameCid:
      l = rfc822Name->BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, CNTX, PRIM, 1);
      break;

    case dNSNameCid:
      l = dNSName->BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, CNTX, PRIM, 2);
      break;

    case x400AddressCid:
      BEncEocIfNec (b);
      l = x400Address->BEncContent (b);
    l += BEncConsLen (b, l);

    l += BEncTag1 (b, CNTX, CONS, 3);
      break;

    case directoryNameCid:
      BEncEocIfNec (b);
      l = directoryName->BEncContent (b);
    l += BEncConsLen (b, l);

    l += BEncTag1 (b, CNTX, CONS, 4);
      break;

    case ediPartyNameCid:
      BEncEocIfNec (b);
      l = ediPartyName->BEncContent (b);
    l += BEncConsLen (b, l);

    l += BEncTag1 (b, CNTX, CONS, 5);
      break;

    case uniformResourceIdentifierCid:
      l = uniformResourceIdentifier->BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, CNTX, PRIM, 6);
      break;

    case iPAddressCid:
      l = iPAddress->BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, CNTX, PRIM, 7);
      break;

    case registeredIDCid:
      l = registeredID->BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, CNTX, PRIM, 8);
      break;

  } // end switch
  return l;
} // GeneralName::BEncContent


void GeneralName::BDecContent (BUF_TYPE b, AsnTag tag, AsnLen elmtLen0, AsnLen &bytesDecoded, ENV_TYPE env)
{
  AsnLen elmtLen1;
  switch (tag)
  {
    case MAKE_TAG_ID (CNTX, PRIM, 0):
      choiceId = otherNameCid;
      otherName = new OTHER_NAME;
        otherName->BDecContent (b, tag, elmtLen0, bytesDecoded, env);
      break;

    case MAKE_TAG_ID (CNTX, PRIM, 1):
    case MAKE_TAG_ID (CNTX, CONS, 1):
      choiceId = rfc822NameCid;
      rfc822Name = new IA5String;
        rfc822Name->BDecContent (b, tag, elmtLen0, bytesDecoded, env);
      break;

    case MAKE_TAG_ID (CNTX, PRIM, 2):
    case MAKE_TAG_ID (CNTX, CONS, 2):
      choiceId = dNSNameCid;
      dNSName = new IA5String;
        dNSName->BDecContent (b, tag, elmtLen0, bytesDecoded, env);
      break;

    case MAKE_TAG_ID (CNTX, CONS, 3):
      choiceId = x400AddressCid;
      x400Address = new ORAddress;
        x400Address->BDecContent (b, tag, elmtLen0, bytesDecoded, env);
      break;

    case MAKE_TAG_ID (CNTX, CONS, 4):
      tag = BDecTag (b, bytesDecoded, env);
      elmtLen1 = BDecLen (b, bytesDecoded, env);
      choiceId = directoryNameCid;
      directoryName = new Name;
        directoryName->BDecContent (b, tag, elmtLen1, bytesDecoded, env);
      if (elmtLen0 == INDEFINITE_LEN)
        BDecEoc (b, bytesDecoded, env);
      break;

    case MAKE_TAG_ID (CNTX, CONS, 5):
      choiceId = ediPartyNameCid;
      ediPartyName = new EDIPartyName;
        ediPartyName->BDecContent (b, tag, elmtLen0, bytesDecoded, env);
      break;

    case MAKE_TAG_ID (CNTX, PRIM, 6):
    case MAKE_TAG_ID (CNTX, CONS, 6):
      choiceId = uniformResourceIdentifierCid;
      uniformResourceIdentifier = new IA5String;
        uniformResourceIdentifier->BDecContent (b, tag, elmtLen0, bytesDecoded, env);
      break;

    case MAKE_TAG_ID (CNTX, PRIM, 7):
    case MAKE_TAG_ID (CNTX, CONS, 7):
      choiceId = iPAddressCid;
      iPAddress = new AsnOcts;
        iPAddress->BDecContent (b, tag, elmtLen0, bytesDecoded, env);
      break;

    case MAKE_TAG_ID (CNTX, PRIM, 8):
      choiceId = registeredIDCid;
      registeredID = new AsnOid;
        registeredID->BDecContent (b, tag, elmtLen0, bytesDecoded, env);
      break;

    default:
      Asn1Error << "ERROR - unexpected tag in CHOICE" << endl;
      longjmp (env, -103);
      break;
  } // end switch
} // GeneralName::BDecContent


AsnLen GeneralName::BEnc (BUF_TYPE b)
{
    AsnLen l;
    l = BEncContent (b);
    return l;
}

void GeneralName::BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env)
{
    AsnLen elmtLen;
    AsnTag tag;

    /*  CHOICEs are a special case - grab identifying tag */
    /*  this allows easier handling of nested CHOICEs */
    tag = BDecTag (b, bytesDecoded, env);
    elmtLen = BDecLen (b, bytesDecoded, env);
    BDecContent (b, tag, elmtLen, bytesDecoded, env);
}

int GeneralName::BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded)
{
    bytesEncoded = BEnc (b);
    return !b.WriteError();
}

int GeneralName::BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded)
{
    ENV_TYPE env;
    int val;

    bytesDecoded = 0;
    if ((val = setjmp (env)) == 0)
    {
         BDec (b, bytesDecoded, env);
         return !b.ReadError();
    }
    else
        return false;
}

void GeneralName::Print (ostream &os) const
{
#ifndef NDEBUG
  switch (choiceId)
  {
    case otherNameCid:
      os << "otherName ";
      if (otherName)
        os << *otherName;
      else
        os << "-- void3 --\n";
      break;

    case rfc822NameCid:
      os << "rfc822Name ";
      if (rfc822Name)
        os << *rfc822Name;
      else
        os << "-- void3 --\n";
      break;

    case dNSNameCid:
      os << "dNSName ";
      if (dNSName)
        os << *dNSName;
      else
        os << "-- void3 --\n";
      break;

    case x400AddressCid:
      os << "x400Address ";
      if (x400Address)
        os << *x400Address;
      else
        os << "-- void3 --\n";
      break;

    case directoryNameCid:
      os << "directoryName ";
      if (directoryName)
        os << *directoryName;
      else
        os << "-- void3 --\n";
      break;

    case ediPartyNameCid:
      os << "ediPartyName ";
      if (ediPartyName)
        os << *ediPartyName;
      else
        os << "-- void3 --\n";
      break;

    case uniformResourceIdentifierCid:
      os << "uniformResourceIdentifier ";
      if (uniformResourceIdentifier)
        os << *uniformResourceIdentifier;
      else
        os << "-- void3 --\n";
      break;

    case iPAddressCid:
      os << "iPAddress ";
      if (iPAddress)
        os << *iPAddress;
      else
        os << "-- void3 --\n";
      break;

    case registeredIDCid:
      os << "registeredID ";
      if (registeredID)
        os << *registeredID;
      else
        os << "-- void3 --\n";
      break;

  } // end of switch
#endif /* NDEBUG */
} // GeneralName::Print

AsnType *GeneralNames::Clone() const
{
  return new GeneralNames;
}

AsnType *GeneralNames::Copy() const
{
  return new GeneralNames (*this);
}

AsnLen GeneralNames::BEnc (BUF_TYPE b)
{
  AsnLen l;
  l = BEncContent (b);
  l += BEncConsLen (b, l);
  l += BEncTag1 (b, UNIV, CONS, SEQ_TAG_CODE);
  return l;
}

void GeneralNames::BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env)
{
  AsnTag tag;
  AsnLen elmtLen1;

  if ((tag = BDecTag (b, bytesDecoded, env)) != MAKE_TAG_ID (UNIV, CONS, SEQ_TAG_CODE))
  {
    Asn1Error << "GeneralNames::BDec: ERROR - wrong tag" << endl;
    longjmp (env, -104);
  }
  elmtLen1 = BDecLen (b, bytesDecoded, env);
  BDecContent (b, tag, elmtLen1, bytesDecoded, env);
}

GeneralNames::GeneralNames (const GeneralNames &)
{
  Asn1Error << "use of incompletely defined GeneralNames::GeneralNames (const GeneralNames &)" << endl;
  abort();
}

GeneralNames::~GeneralNames()
{
  SetCurrToFirst();
  for (; Curr() != NULL; RemoveCurrFromList())
    ;
} // end of destructor

#if SNACC_DEEP_COPY
GeneralNames &GeneralNames::operator = (const GeneralNames &that)
#else // SNACC_DEEP_COPY
GeneralNames &GeneralNames::operator = (const GeneralNames &)
#endif // SNACC_DEEP_COPY
{
#if SNACC_DEEP_COPY
  if (this != &that)
  {
    SetCurrToFirst();
    for (; Curr(); RemoveCurrFromList())
      ;

    //that.SetCurrToFirst();
    //for (; that.Curr(); that.GoNext())
    //  AppendCopy (*that.Curr());
    for (const AsnListElmt *run=that.first; run; run=run->next)
      AppendCopy (*run->elmt);
  }

  return *this;
#else // SNACC_DEEP_COPY
  Asn1Error << "use of incompletely defined GeneralNames &GeneralNames::operator = (const GeneralNames &)" << endl;
  abort();
  // if your compiler complains here, check the -novolat option
#endif // SNACC_DEEP_COPY
}

void GeneralNames::Print (ostream &os) const
{
#ifndef NDEBUG
    os << "{ -- SEQUENCE/SET OF -- " << endl;
    indentG += stdIndentG;
    //SetCurrToFirst();
    //for (; Curr() != NULL; GoNext())
    for (const AsnListElmt *run=first; run; run=run->next)
    {
        Indent (os, indentG);
        //os << *Curr();
        os << *run->elmt;
        //if (Curr() != Last())
        if (run != last)
            os << ",";
        os << endl;
    }
    indentG -= stdIndentG;
    Indent (os, indentG);
    os << "}\n";
#endif /* NDEBUG */


} // Print


void  GeneralNames::SetCurrElmt (unsigned long int index)
{
  unsigned long int i;
  curr = first;
  if (count)
    for (i = 0; (i < (count-1)) && (i < index); i++)
      curr = curr->next;
} // GeneralNames::SetCurrElmt


unsigned long int  GeneralNames::GetCurrElmtIndex()
{
    unsigned long int i;
    AsnListElmt *tmp;
    if (curr != NULL)
    {
        for (i = 0, tmp = first; tmp != NULL; i++)
        {
            if (tmp == curr)
                return i;
            else
                tmp = tmp->next;
        }
    }
    return count;
} // GeneralNames::GetCurrElmtIndex


// alloc new list elmt, put at end of list
//  and return the component type
GeneralName *GeneralNames::Append()
{
    AsnListElmt *newElmt;
    newElmt  = new AsnListElmt;
    newElmt->elmt  = new GeneralName;
    newElmt->next = NULL;
    if (last == NULL)
    {
        newElmt->prev = NULL;
        first = last  = newElmt;
    }
    else
    {
        newElmt->prev = last;
        last->next    = newElmt;
        last          = newElmt;
    }
    count++;
    return (curr = newElmt)->elmt;
} // GeneralNames::Append


// alloc new list elmt, put at begining of list
//  and return the component type
GeneralName  *GeneralNames::Prepend()
{
    AsnListElmt *newElmt;
    newElmt  = new AsnListElmt;
    newElmt->elmt = new GeneralName;
    newElmt->prev = NULL;
    if (first == NULL)
    {
        newElmt->next = NULL;
        first = last  = newElmt;
    }
    else
    {
        newElmt->next = first;
        first->prev   = newElmt;
        first         = newElmt;
    }
    count++;
    return (curr = newElmt)->elmt;
} // GeneralNames::Prepend


// alloc new list elmt, insert it before the
// current element and return the component type
// if the current element is null, the new element
// is placed at the beginning of the list.
GeneralName  *GeneralNames::InsertBefore()
{
    AsnListElmt *newElmt;
    newElmt  = new AsnListElmt;
    newElmt->elmt = new GeneralName;
    if (curr == NULL)
    {
        newElmt->next = first;
        newElmt->prev = NULL;
        first = newElmt;
        if (last == NULL)
            last = newElmt;
    }
    else
    {
        newElmt->next = curr;
        newElmt->prev = curr->prev;
        curr->prev = newElmt;
        if (curr == first)
            first = newElmt;
        else
            newElmt->prev->next = newElmt;
    }
    count++;
    return (curr = newElmt)->elmt;
} // GeneralNames::InsertBefore


// alloc new list elmt, insert it after the
// current element and return the component type
// if the current element is null, the new element
// is placed at the end of the list.
GeneralName *GeneralNames::InsertAfter()
{
    AsnListElmt *newElmt;
    newElmt  = new AsnListElmt;
    newElmt->elmt = new GeneralName;
    if (curr == NULL)
    {
        newElmt->prev = last;
        newElmt->next = NULL;
        last = newElmt;
        if (first == NULL)
            first = newElmt;
    }
    else
    {
        newElmt->prev = curr;
        newElmt->next = curr->next;
        curr->next = newElmt;
        if (curr == last)
            last = newElmt;
        else
            newElmt->next->prev = newElmt;
    }
    count++;
    return (curr = newElmt)->elmt;
} // GeneralNames::InsertAfter


GeneralNames  &GeneralNames::AppendCopy (GeneralName &elmt)
{
    AsnListElmt *newElmt;
    newElmt  = new AsnListElmt;
    newElmt->elmt = new GeneralName;
    *newElmt->elmt = elmt;
    newElmt->next = NULL;
    if (last == NULL)
    {
        newElmt->prev = NULL;
        first = last  = newElmt;
    }
    else
    {
        newElmt->prev = last;
        last->next    = newElmt;
        last          = newElmt;
    }
    count++;
    return *this;
} // AppendCopy


GeneralNames  &GeneralNames::PrependCopy (GeneralName &elmt)
{
    AsnListElmt *newElmt;
    newElmt  = new AsnListElmt;
    newElmt->elmt = new GeneralName;
    *newElmt->elmt = elmt;
    newElmt->prev = NULL;
    if (first == NULL)
    {
        newElmt->next = NULL;
        first = last  = newElmt;
    }
    else
    {
        newElmt->next = first;
        first->prev   = newElmt;
        first         = newElmt;
    }
    count++;
    return *this;
} // GeneralNames::PrependCopy


// alloc new list elmt, insert it before the
// current element, copy the given elmt into the new elmt
// and return the component type.
// if the current element is null, the new element
// is placed at the beginning of the list.
GeneralNames &GeneralNames::InsertBeforeAndCopy (GeneralName &elmt)
{
    AsnListElmt *newElmt;

    newElmt  = new AsnListElmt;
    newElmt->elmt = new GeneralName;
    *newElmt->elmt = elmt;

    if (curr == NULL)
    {
        newElmt->next = first;
        newElmt->prev = NULL;
        first = newElmt;
        if (last == NULL)
            last = newElmt;
    }
    else
    {
        newElmt->next = curr;
        newElmt->prev = curr->prev;
        curr->prev = newElmt;
        if (curr == first)
            first = newElmt;
        else
            newElmt->prev->next = newElmt;
    }
    count++;
    return *this;
} // GeneralNames::InsertBeforeAndCopy


// alloc new list elmt, insert it after the
// current element, copy given elmt in to new elmt
//  and return the component type
// if the current element is null, the new element
// is placed at the end of the list.
GeneralNames  &GeneralNames::InsertAfterAndCopy (GeneralName &elmt)
{
    AsnListElmt *newElmt;

    newElmt  = new AsnListElmt;
    newElmt->elmt = new GeneralName;
    *newElmt->elmt = elmt;
    if (curr == NULL)
    {
        newElmt->prev = last;
        newElmt->next = NULL;
        last = newElmt;
        if (first == NULL)
            first = newElmt;
    }
    else
    {
        newElmt->prev = curr;
        newElmt->next = curr->next;
        curr->next = newElmt;
        if (curr == last)
            last = newElmt;
        else
            newElmt->next->prev = newElmt;
    }
    count++;
    return *this;
} // GeneralNames::InsertAfterAndCopy


// remove current element from list if current element is not NULL 
// The new current element will be the next element.
// If the current element is the last element in the list
// the second but last element will become the new current element.
void GeneralNames::RemoveCurrFromList()
{
    AsnListElmt *del_elmt;

    if (curr != NULL)
    {
        del_elmt = curr;
        count--;

        if (count == 0)
            first = last = curr = NULL;
        else if (curr == first)
        {
            curr = first= first->next;
            first->prev = NULL;
        }
        else if (curr == last)
        {
            curr = last = last->prev;
            last->next = NULL;
        }
        else
        {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
        }

        delete del_elmt->elmt;
        delete del_elmt;
    }
}


AsnLen GeneralNames::BEncContent (BUF_TYPE b)
{
    AsnListElmt *currElmt;
    AsnLen elmtLen;
    AsnLen totalLen = 0;
    for (currElmt = last; currElmt != NULL; currElmt = currElmt->prev)
    {
        elmtLen = currElmt->elmt->BEncContent (b);
        totalLen += elmtLen;
    }
    return totalLen;
} // GeneralNames::BEncContent


void  GeneralNames::BDecContent (BUF_TYPE b, AsnTag /*tag0*/, AsnLen elmtLen0,
                                  AsnLen &bytesDecoded, ENV_TYPE env)
{
    GeneralName *listElmt;
    AsnTag tag1;
    AsnLen listBytesDecoded = 0;
    AsnLen elmtLen1;

    while ((listBytesDecoded < elmtLen0) || (elmtLen0 == INDEFINITE_LEN))
    {
        tag1 = BDecTag (b, listBytesDecoded, env);
        if ((tag1 == EOC_TAG_ID) && (elmtLen0 == INDEFINITE_LEN))
        {
            BDEC_2ND_EOC_OCTET (b, listBytesDecoded, env);
            break;
        }
        if (!((tag1 == MAKE_TAG_ID (CNTX, PRIM, 0))
         || (tag1 == MAKE_TAG_ID (CNTX, PRIM, 1))
            || (tag1 == MAKE_TAG_ID (CNTX, CONS, 1))

         || (tag1 == MAKE_TAG_ID (CNTX, PRIM, 2))
            || (tag1 == MAKE_TAG_ID (CNTX, CONS, 2))

         || (tag1 == MAKE_TAG_ID (CNTX, CONS, 3))
         || (tag1 == MAKE_TAG_ID (CNTX, CONS, 4))
         || (tag1 == MAKE_TAG_ID (CNTX, CONS, 5))
         || (tag1 == MAKE_TAG_ID (CNTX, PRIM, 6))
            || (tag1 == MAKE_TAG_ID (CNTX, CONS, 6))

         || (tag1 == MAKE_TAG_ID (CNTX, PRIM, 7))
            || (tag1 == MAKE_TAG_ID (CNTX, CONS, 7))

         || (tag1 == MAKE_TAG_ID (CNTX, PRIM, 8))))
        {
            Asn1Error << "Unexpected Tag" << endl;
            longjmp (env, -105);
        }

        elmtLen1 = BDecLen (b, listBytesDecoded, env);
        listElmt = Append();
        listElmt->BDecContent (b, tag1, elmtLen1, listBytesDecoded, env);
    }

    bytesDecoded += listBytesDecoded;
} // GeneralNames::BDecContent


