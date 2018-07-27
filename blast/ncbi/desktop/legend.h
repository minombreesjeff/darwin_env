#ifndef _LEGEND_H_
#define _LEGEND_H_

#define NULL_SYMBOL -1
#define NULL_COLOR  -1
#define NULL_STYLE  -1

#define SOLID_STYLE 1
#define OPEN_STYLE  2

extern void          init_data PROTO((CharPtr filename));
extern UserObjectPtr legend    PROTO((CharPtr map_type));

#endif /* _LEGEND_H_ */
