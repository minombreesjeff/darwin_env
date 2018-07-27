#ifndef _MBALIGN_H_
#define _MBALIGN_H_

#include <ncbi.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef Uint4 edit_op_t; /* 32 bits */
typedef struct {
    edit_op_t *op;                  /* array of edit operations */
    Uint4 size, num;         /* size of allocation, number in use */
    edit_op_t last;                 /* most recent operation added */
} edit_script_t;

edit_script_t *edit_script_free(edit_script_t *es);
edit_script_t *edit_script_new(void);
edit_script_t *edit_script_append(edit_script_t *es, edit_script_t *et);

enum {
    EDIT_OP_MASK = 0x3,
    EDIT_OP_ERR  = 0x0,
    EDIT_OP_INS  = 0x1,
    EDIT_OP_DEL  = 0x2,
    EDIT_OP_REP  = 0x3
};

enum {         /* half of the (fixed) match score */
    ERROR_FRACTION=2,  /* 1/this */
    MAX_SPACE=1000000,
    sC = 0, sI = 1, sD = 2, LARGE=100000000
};

#define ICEIL(x,y) ((((x)-1)/(y))+1)

/* ----- pool allocator ----- */
typedef struct _three_val_ {
    Int4 I, C, D;
} ThreeVal, PNTR ThreeValPtr;

typedef struct mb_space_struct {
    ThreeValPtr space_array;
    Int4 used, size;
    struct mb_space_struct *next;
} MBSpace, *MBSpacePtr;

#define EDIT_VAL(op) (op >> 2)

#define EDIT_OPC(op) (op & EDIT_OP_MASK)

MBSpacePtr new_mb_space(void);
void refresh_mb_space(MBSpacePtr sp);
void free_mb_space(MBSpacePtr sp);
ThreeValPtr get_mb_space(MBSpacePtr S, Int4 amount);

typedef struct greedy_align_mem {
   Int4Ptr PNTR flast_d;
   Int4Ptr max_row_free;
   ThreeValPtr PNTR flast_d_affine;
   Int4Ptr uplow_free;
   MBSpacePtr space;
} GreedyAlignMem, PNTR GreedyAlignMemPtr;

Int4 
MegaBlastGreedyAlign PROTO((const UcharPtr s1, Int4 len1,
			     const UcharPtr s2, Int4 len2,
			     Boolean reverse, Int4 xdrop_threshold, 
			     Int4 match_cost, Int4 mismatch_cost,
			     Int4Ptr e1, Int4Ptr e2, GreedyAlignMemPtr abmp, 
			     edit_script_t *S, Uint1 rem));
Int4 
MegaBlastAffineGreedyAlign PROTO((const UcharPtr s1, Int4 len1,
				  const UcharPtr s2, Int4 len2,
				  Boolean reverse, Int4 xdrop_threshold, 
				  Int4 match_cost, Int4 mismatch_cost,
				  Int4 gap_open, Int4 gap_extend,
				  Int4Ptr e1, Int4Ptr e2, 
				  GreedyAlignMemPtr abmp, 
				  edit_script_t *S, Uint1 rem));



GreedyAlignMemPtr 
GreedyAlignMemFree PROTO((GreedyAlignMemPtr abmp));

#ifdef __cplusplus
}
#endif
#endif /* _MBALIGN_H_ */
