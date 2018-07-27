/*
*
*
* RCS Modification History:
* $Log: outmedart.h,v $
* Revision 6.0  1997/08/25 18:36:48  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:40  epstein
* add RCS log revision history
*
*/


#ifndef	MODULE_OUTMEDART_H
#define	MODULE_OUTMEDART_H

int	 ma_OpenOutputStreams();
int	 ma_CloseOutputStreams();
int	 ma_WriteOutput( MedArt *article );

#endif /* MODULE_OUTMEDART_H */
