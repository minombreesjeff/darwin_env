/*
 * chvalid.c:	this module implements the character range
 *		validation APIs
 *
 * This file is automatically generated from the cvs source
 * definition files using the genChRanges.py Python script
 *
 * Generation date: Tue Nov 18 08:14:21 2003
 * Sources: chvalid.def
 * William Brack <wbrack@mmm.com.hk>
 */

#define IN_LIBXML
#include "libxml.h"
#include <libxml/chvalid.h>

/*
 * The initial tables ({func_name}_tab) are used to validate whether a
 * single-byte character is within the specified group.  Each table
 * contains 256 bytes, with each byte representing one of the 256
 * possible characters.  If the table byte is set, the character is
 * allowed.
 *
 */
unsigned char xmlIsPubidChar_tab[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01,
    0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00 };

static xmlChSRange xmlIsBaseChar_srng[] = { {0x100, 0x131}, {0x134, 0x13e}, 
    {0x141, 0x148}, {0x14a, 0x17e}, {0x180, 0x1c3}, {0x1cd, 0x1f0}, 
    {0x1f4, 0x1f5}, {0x1fa, 0x217}, {0x250, 0x2a8}, {0x2bb, 0x2c1}, 
    {0x386, 0x386}, {0x388, 0x38a}, {0x38c, 0x38c}, {0x38e, 0x3a1}, 
    {0x3a3, 0x3ce}, {0x3d0, 0x3d6}, {0x3da, 0x3da}, {0x3dc, 0x3dc}, 
    {0x3de, 0x3de}, {0x3e0, 0x3e0}, {0x3e2, 0x3f3}, {0x401, 0x40c}, 
    {0x40e, 0x44f}, {0x451, 0x45c}, {0x45e, 0x481}, {0x490, 0x4c4}, 
    {0x4c7, 0x4c8}, {0x4cb, 0x4cc}, {0x4d0, 0x4eb}, {0x4ee, 0x4f5}, 
    {0x4f8, 0x4f9}, {0x531, 0x556}, {0x559, 0x559}, {0x561, 0x586}, 
    {0x5d0, 0x5ea}, {0x5f0, 0x5f2}, {0x621, 0x63a}, {0x641, 0x64a}, 
    {0x671, 0x6b7}, {0x6ba, 0x6be}, {0x6c0, 0x6ce}, {0x6d0, 0x6d3}, 
    {0x6d5, 0x6d5}, {0x6e5, 0x6e6}, {0x905, 0x939}, {0x93d, 0x93d}, 
    {0x958, 0x961}, {0x985, 0x98c}, {0x98f, 0x990}, {0x993, 0x9a8}, 
    {0x9aa, 0x9b0}, {0x9b2, 0x9b2}, {0x9b6, 0x9b9}, {0x9dc, 0x9dd}, 
    {0x9df, 0x9e1}, {0x9f0, 0x9f1}, {0xa05, 0xa0a}, {0xa0f, 0xa10}, 
    {0xa13, 0xa28}, {0xa2a, 0xa30}, {0xa32, 0xa33}, {0xa35, 0xa36}, 
    {0xa38, 0xa39}, {0xa59, 0xa5c}, {0xa5e, 0xa5e}, {0xa72, 0xa74}, 
    {0xa85, 0xa8b}, {0xa8d, 0xa8d}, {0xa8f, 0xa91}, {0xa93, 0xaa8}, 
    {0xaaa, 0xab0}, {0xab2, 0xab3}, {0xab5, 0xab9}, {0xabd, 0xabd}, 
    {0xae0, 0xae0}, {0xb05, 0xb0c}, {0xb0f, 0xb10}, {0xb13, 0xb28}, 
    {0xb2a, 0xb30}, {0xb32, 0xb33}, {0xb36, 0xb39}, {0xb3d, 0xb3d}, 
    {0xb5c, 0xb5d}, {0xb5f, 0xb61}, {0xb85, 0xb8a}, {0xb8e, 0xb90}, 
    {0xb92, 0xb95}, {0xb99, 0xb9a}, {0xb9c, 0xb9c}, {0xb9e, 0xb9f}, 
    {0xba3, 0xba4}, {0xba8, 0xbaa}, {0xbae, 0xbb5}, {0xbb7, 0xbb9}, 
    {0xc05, 0xc0c}, {0xc0e, 0xc10}, {0xc12, 0xc28}, {0xc2a, 0xc33}, 
    {0xc35, 0xc39}, {0xc60, 0xc61}, {0xc85, 0xc8c}, {0xc8e, 0xc90}, 
    {0xc92, 0xca8}, {0xcaa, 0xcb3}, {0xcb5, 0xcb9}, {0xcde, 0xcde}, 
    {0xce0, 0xce1}, {0xd05, 0xd0c}, {0xd0e, 0xd10}, {0xd12, 0xd28}, 
    {0xd2a, 0xd39}, {0xd60, 0xd61}, {0xe01, 0xe2e}, {0xe30, 0xe30}, 
    {0xe32, 0xe33}, {0xe40, 0xe45}, {0xe81, 0xe82}, {0xe84, 0xe84}, 
    {0xe87, 0xe88}, {0xe8a, 0xe8a}, {0xe8d, 0xe8d}, {0xe94, 0xe97}, 
    {0xe99, 0xe9f}, {0xea1, 0xea3}, {0xea5, 0xea5}, {0xea7, 0xea7}, 
    {0xeaa, 0xeab}, {0xead, 0xeae}, {0xeb0, 0xeb0}, {0xeb2, 0xeb3}, 
    {0xebd, 0xebd}, {0xec0, 0xec4}, {0xf40, 0xf47}, {0xf49, 0xf69}, 
    {0x10a0, 0x10c5}, {0x10d0, 0x10f6}, {0x1100, 0x1100}, {0x1102, 0x1103}, 
    {0x1105, 0x1107}, {0x1109, 0x1109}, {0x110b, 0x110c}, {0x110e, 0x1112}, 
    {0x113c, 0x113c}, {0x113e, 0x113e}, {0x1140, 0x1140}, {0x114c, 0x114c}, 
    {0x114e, 0x114e}, {0x1150, 0x1150}, {0x1154, 0x1155}, {0x1159, 0x1159}, 
    {0x115f, 0x1161}, {0x1163, 0x1163}, {0x1165, 0x1165}, {0x1167, 0x1167}, 
    {0x1169, 0x1169}, {0x116d, 0x116e}, {0x1172, 0x1173}, {0x1175, 0x1175}, 
    {0x119e, 0x119e}, {0x11a8, 0x11a8}, {0x11ab, 0x11ab}, {0x11ae, 0x11af}, 
    {0x11b7, 0x11b8}, {0x11ba, 0x11ba}, {0x11bc, 0x11c2}, {0x11eb, 0x11eb}, 
    {0x11f0, 0x11f0}, {0x11f9, 0x11f9}, {0x1e00, 0x1e9b}, {0x1ea0, 0x1ef9}, 
    {0x1f00, 0x1f15}, {0x1f18, 0x1f1d}, {0x1f20, 0x1f45}, {0x1f48, 0x1f4d}, 
    {0x1f50, 0x1f57}, {0x1f59, 0x1f59}, {0x1f5b, 0x1f5b}, {0x1f5d, 0x1f5d}, 
    {0x1f5f, 0x1f7d}, {0x1f80, 0x1fb4}, {0x1fb6, 0x1fbc}, {0x1fbe, 0x1fbe}, 
    {0x1fc2, 0x1fc4}, {0x1fc6, 0x1fcc}, {0x1fd0, 0x1fd3}, {0x1fd6, 0x1fdb}, 
    {0x1fe0, 0x1fec}, {0x1ff2, 0x1ff4}, {0x1ff6, 0x1ffc}, {0x2126, 0x2126}, 
    {0x212a, 0x212b}, {0x212e, 0x212e}, {0x2180, 0x2182}, {0x3041, 0x3094}, 
    {0x30a1, 0x30fa}, {0x3105, 0x312c}, {0xac00, 0xd7a3}};
xmlChRangeGroup xmlIsBaseCharGroup =
	{197, 0, xmlIsBaseChar_srng, (xmlChLRangePtr)0};

static xmlChSRange xmlIsChar_srng[] = { {0x100, 0xd7ff}, {0xe000, 0xfffd}};
static xmlChLRange xmlIsChar_lrng[] = { {0x10000, 0x10ffff}};
xmlChRangeGroup xmlIsCharGroup =
	{2, 1, xmlIsChar_srng, xmlIsChar_lrng};

static xmlChSRange xmlIsCombining_srng[] = { {0x300, 0x345}, 
    {0x360, 0x361}, {0x483, 0x486}, {0x591, 0x5a1}, {0x5a3, 0x5b9}, 
    {0x5bb, 0x5bd}, {0x5bf, 0x5bf}, {0x5c1, 0x5c2}, {0x5c4, 0x5c4}, 
    {0x64b, 0x652}, {0x670, 0x670}, {0x6d6, 0x6dc}, {0x6dd, 0x6df}, 
    {0x6e0, 0x6e4}, {0x6e7, 0x6e8}, {0x6ea, 0x6ed}, {0x901, 0x903}, 
    {0x93c, 0x93c}, {0x93e, 0x94c}, {0x94d, 0x94d}, {0x951, 0x954}, 
    {0x962, 0x963}, {0x981, 0x983}, {0x9bc, 0x9bc}, {0x9be, 0x9be}, 
    {0x9bf, 0x9bf}, {0x9c0, 0x9c4}, {0x9c7, 0x9c8}, {0x9cb, 0x9cd}, 
    {0x9d7, 0x9d7}, {0x9e2, 0x9e3}, {0xa02, 0xa02}, {0xa3c, 0xa3c}, 
    {0xa3e, 0xa3e}, {0xa3f, 0xa3f}, {0xa40, 0xa42}, {0xa47, 0xa48}, 
    {0xa4b, 0xa4d}, {0xa70, 0xa71}, {0xa81, 0xa83}, {0xabc, 0xabc}, 
    {0xabe, 0xac5}, {0xac7, 0xac9}, {0xacb, 0xacd}, {0xb01, 0xb03}, 
    {0xb3c, 0xb3c}, {0xb3e, 0xb43}, {0xb47, 0xb48}, {0xb4b, 0xb4d}, 
    {0xb56, 0xb57}, {0xb82, 0xb83}, {0xbbe, 0xbc2}, {0xbc6, 0xbc8}, 
    {0xbca, 0xbcd}, {0xbd7, 0xbd7}, {0xc01, 0xc03}, {0xc3e, 0xc44}, 
    {0xc46, 0xc48}, {0xc4a, 0xc4d}, {0xc55, 0xc56}, {0xc82, 0xc83}, 
    {0xcbe, 0xcc4}, {0xcc6, 0xcc8}, {0xcca, 0xccd}, {0xcd5, 0xcd6}, 
    {0xd02, 0xd03}, {0xd3e, 0xd43}, {0xd46, 0xd48}, {0xd4a, 0xd4d}, 
    {0xd57, 0xd57}, {0xe31, 0xe31}, {0xe34, 0xe3a}, {0xe47, 0xe4e}, 
    {0xeb1, 0xeb1}, {0xeb4, 0xeb9}, {0xebb, 0xebc}, {0xec8, 0xecd}, 
    {0xf18, 0xf19}, {0xf35, 0xf35}, {0xf37, 0xf37}, {0xf39, 0xf39}, 
    {0xf3e, 0xf3e}, {0xf3f, 0xf3f}, {0xf71, 0xf84}, {0xf86, 0xf8b}, 
    {0xf90, 0xf95}, {0xf97, 0xf97}, {0xf99, 0xfad}, {0xfb1, 0xfb7}, 
    {0xfb9, 0xfb9}, {0x20d0, 0x20dc}, {0x20e1, 0x20e1}, {0x302a, 0x302f}, 
    {0x3099, 0x3099}, {0x309a, 0x309a}};
xmlChRangeGroup xmlIsCombiningGroup =
	{95, 0, xmlIsCombining_srng, (xmlChLRangePtr)0};

static xmlChSRange xmlIsDigit_srng[] = { {0x660, 0x669}, {0x6f0, 0x6f9}, 
    {0x966, 0x96f}, {0x9e6, 0x9ef}, {0xa66, 0xa6f}, {0xae6, 0xaef}, 
    {0xb66, 0xb6f}, {0xbe7, 0xbef}, {0xc66, 0xc6f}, {0xce6, 0xcef}, 
    {0xd66, 0xd6f}, {0xe50, 0xe59}, {0xed0, 0xed9}, {0xf20, 0xf29}};
xmlChRangeGroup xmlIsDigitGroup =
	{14, 0, xmlIsDigit_srng, (xmlChLRangePtr)0};

static xmlChSRange xmlIsExtender_srng[] = { {0x2d0, 0x2d0}, {0x2d1, 0x2d1}, 
    {0x387, 0x387}, {0x640, 0x640}, {0xe46, 0xe46}, {0xec6, 0xec6}, 
    {0x3005, 0x3005}, {0x3031, 0x3035}, {0x309d, 0x309e}, {0x30fc, 0x30fe}};
xmlChRangeGroup xmlIsExtenderGroup =
	{10, 0, xmlIsExtender_srng, (xmlChLRangePtr)0};

static xmlChSRange xmlIsIdeographic_srng[] = { {0x3007, 0x3007}, 
    {0x3021, 0x3029}, {0x4e00, 0x9fa5}};
xmlChRangeGroup xmlIsIdeographicGroup =
	{3, 0, xmlIsIdeographic_srng, (xmlChLRangePtr)0};


/**
 * xmlCharInRange:
 * @val: character to be validated
 * @rptr: pointer to range to be used to validate
 *
 * Does a binary search of the range table to determine if char
 * is valid
 *
 * Returns: true if character valid, false otherwise
 */
int
xmlCharInRange (unsigned int val, const xmlChRangeGroupPtr rptr) {
    int low, high, mid;
    xmlChSRangePtr sptr;
    xmlChLRangePtr lptr;
    if (val < 0x10000) {	/* is val in 'short' or 'long'  array? */
	if (rptr->nbShortRange == 0)
	    return 0;
	low = 0;
	high = rptr->nbShortRange - 1;
	sptr = rptr->shortRange;
	while (low <= high) {
	    mid = (low + high) / 2;
	    if ((unsigned short) val < sptr[mid].low) {
		high = mid - 1;
	    } else {
	        if ((unsigned short) val > sptr[mid].high) {
		    low = mid + 1;
		} else {
		    return 1;
		}
	    }
	}
    } else {
	if (rptr->nbLongRange == 0) {
	    return 0;
	}
	low = 0;
	high = rptr->nbLongRange - 1;
	lptr = rptr->longRange;
	while (low <= high) {
	    mid = (low + high) / 2;
	    if (val < lptr[mid].low) {
		high = mid - 1;
	    } else {
	        if (val > lptr[mid].high) {
		    low = mid + 1;
		} else {
		    return 1;
		}
	    }
	}
    }
    return 0;
}


/**
 * xmlIsBaseChar:
 * @ch:  character to validate
 *
 * This function is DEPRECATED.
 * Use xmlIsBaseChar_ch or xmlIsBaseCharQ instead
 *
 * Returns true if argument valid, false otherwise
 */
int
xmlIsBaseChar(unsigned int ch) {
    return(xmlIsBaseCharQ(ch));
}


/**
 * xmlIsBlank:
 * @ch:  character to validate
 *
 * This function is DEPRECATED.
 * Use xmlIsBlank_ch or xmlIsBlankQ instead
 *
 * Returns true if argument valid, false otherwise
 */
int
xmlIsBlank(unsigned int ch) {
    return(xmlIsBlankQ(ch));
}


/**
 * xmlIsChar:
 * @ch:  character to validate
 *
 * This function is DEPRECATED.
 * Use xmlIsChar_ch or xmlIsCharQ instead
 *
 * Returns true if argument valid, false otherwise
 */
int
xmlIsChar(unsigned int ch) {
    return(xmlIsCharQ(ch));
}


/**
 * xmlIsCombining:
 * @ch:  character to validate
 *
 * This function is DEPRECATED.
 * Use xmlIsCombiningQ instead
 *
 * Returns true if argument valid, false otherwise
 */
int
xmlIsCombining(unsigned int ch) {
    return(xmlIsCombiningQ(ch));
}


/**
 * xmlIsDigit:
 * @ch:  character to validate
 *
 * This function is DEPRECATED.
 * Use xmlIsDigit_ch or xmlIsDigitQ instead
 *
 * Returns true if argument valid, false otherwise
 */
int
xmlIsDigit(unsigned int ch) {
    return(xmlIsDigitQ(ch));
}


/**
 * xmlIsExtender:
 * @ch:  character to validate
 *
 * This function is DEPRECATED.
 * Use xmlIsExtender_ch or xmlIsExtenderQ instead
 *
 * Returns true if argument valid, false otherwise
 */
int
xmlIsExtender(unsigned int ch) {
    return(xmlIsExtenderQ(ch));
}


/**
 * xmlIsIdeographic:
 * @ch:  character to validate
 *
 * This function is DEPRECATED.
 * Use xmlIsIdeographicQ instead
 *
 * Returns true if argument valid, false otherwise
 */
int
xmlIsIdeographic(unsigned int ch) {
    return(xmlIsIdeographicQ(ch));
}


/**
 * xmlIsPubidChar:
 * @ch:  character to validate
 *
 * This function is DEPRECATED.
 * Use xmlIsPubidChar_ch or xmlIsPubidCharQ instead
 *
 * Returns true if argument valid, false otherwise
 */
int
xmlIsPubidChar(unsigned int ch) {
    return(xmlIsPubidCharQ(ch));
}

