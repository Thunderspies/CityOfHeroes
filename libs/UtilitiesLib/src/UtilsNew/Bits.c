/***************************************************************************
 *     Copyright (c) 2008-2008, NCSoft
 *     All Rights Reserved
 *     Confidential Property of NCSoft
 ***************************************************************************/
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/UtilsNew/Bits.h"
#include "utilitieslib/UtilsNew/Array.h"

#define IDXBITS 5
STATIC_ASSERT(sizeof(unsigned) == 4); // otherwise, we need to specify IDXBITS for the platform

#define AIDX(idx)    (idx >> IDXBITS)            // index into the int array
#define BIDX(idx)    (idx & ((1<<IDXBITS)-1))    // bit index

Bits Bits_temp_dbg(void *mem, size_t mem_size DBG_PARMS)
{
    return (Bits)aint_init_dbg(mem, mem_size, ARRAYFLAG_ALLOCA DBG_PARMS_CALL);
}

int Bits_test_dbg(const Bits *hbits, int idx DBG_PARMS)
{
    int aidx = AIDX(idx);
	int size = aint_size_dbg((int const *const *)hbits DBG_PARMS_CALL);
	return aidx < size ? (((*hbits)[aidx] >> BIDX(idx)) & 1) : 0;
}

int Bits_set_dbg(Bits *hbits, int idx DBG_PARMS)
{
    int aidx = AIDX(idx);
	if(aidx >= aint_size_dbg((int const *const *)hbits DBG_PARMS_CALL))
		aint_setsize_dbg((int **)hbits, aidx+1 DBG_PARMS_CALL);
    (*hbits)[aidx] |= 1 << BIDX(idx);
    return 1;
}

int Bits_clear_dbg(Bits *hbits, int idx DBG_PARMS)
{
    int aidx = AIDX(idx);
	if(aidx < aint_size_dbg((int const *const *)hbits DBG_PARMS_CALL))
        (*hbits)[aidx] &= ~(1 << BIDX(idx));
    return 0;
}

int Bits_toggle_dbg(Bits *hbits, int idx DBG_PARMS)
{
    int aidx = AIDX(idx);
	if(aidx >= aint_size_dbg((int const *const *)hbits DBG_PARMS_CALL))
		aint_setsize_dbg((int **)hbits, aidx+1 DBG_PARMS_CALL);
    (*hbits)[aidx] ^= 1 << BIDX(idx);
    return ( ((*hbits)[aidx] >> BIDX(idx)) & 1);
}

int Bits_setvalue_dbg(Bits *hbits, int idx, int on DBG_PARMS)
{
    return on ? Bits_set_dbg(hbits, idx DBG_PARMS_CALL) : Bits_clear_dbg(hbits, idx DBG_PARMS_CALL);
}

void Bits_setrun_dbg(Bits *hbits, int idx, int len DBG_PARMS)
{
    int i;
    for(i = 0; i < len; i++) // this could be faster
        Bits_set_dbg(hbits, idx+i DBG_PARMS_CALL);
}

void Bits_setbytes_dbg(Bits *hbits, const void *mem, int bytecount DBG_PARMS)
{
    int i, j;
    const U8 *bytes = mem;
	aint_setsize_dbg((int **)hbits,
		(bytecount + sizeof(unsigned) - 1) / sizeof(unsigned)
		DBG_PARMS_CALL);
    for(i = 0; bytecount > 0; i++)
    {
        for(j = 0; j < sizeof(unsigned) && j < bytecount; j++)
            (*hbits)[i] |= (unsigned)(bytes[j]) << (j * 8);
        bytes += sizeof(unsigned);
        bytecount -= sizeof(unsigned);
    }
	assert(i == aint_size_dbg((int const *const *)hbits DBG_PARMS_CALL));
}

void Bits_clearall_dbg(Bits *hbits DBG_PARMS)
{
	aint_setsize_dbg((int **)hbits, 0 DBG_PARMS_CALL);
}

void Bits_copy_dbg(Bits *hbits_dst, const Bits *hbits_src DBG_PARMS)
{
	aint_cp_dbg((int **)hbits_dst, (int const *const *)hbits_src,
		0 DBG_PARMS_CALL); // copy all
}

void Bits_and_dbg(Bits *hbits_dst, const Bits *hbits_src DBG_PARMS)
{
    int i;
	int dsize = aint_size_dbg((int const *const *)hbits_dst DBG_PARMS_CALL);
	int ssize = aint_size_dbg((int const *const *)hbits_src DBG_PARMS_CALL);
    for(i = 0; i < MIN(dsize, ssize); i++)
        (*hbits_dst)[i] &= (*hbits_src)[i];
    for(i; i < dsize; i++)
        (*hbits_dst)[i] = 0;
}

void Bits_or_dbg(Bits *hbits_dst, const Bits *hbits_src DBG_PARMS)
{
    int i;
	int dsize = aint_size_dbg((int const *const *)hbits_dst DBG_PARMS_CALL);
	int ssize = aint_size_dbg((int const *const *)hbits_src DBG_PARMS_CALL);
    if(dsize < ssize)
		aint_setsize_dbg((int **)hbits_dst, ssize DBG_PARMS_CALL);
    for(i = 0; i < MIN(dsize, ssize); i++)
        (*hbits_dst)[i] |= (*hbits_src)[i];
    for(i; i < ssize; i++)
        (*hbits_dst)[i] = (*hbits_src)[i];
}

void Bits_xor_dbg(Bits *hbits_dst, const Bits *hbits_src DBG_PARMS)
{
    int i;
	int dsize = aint_size_dbg((int const *const *)hbits_dst DBG_PARMS_CALL);
	int ssize = aint_size_dbg((int const *const *)hbits_src DBG_PARMS_CALL);
    if(dsize < ssize)
		aint_setsize_dbg((int **)hbits_dst, ssize DBG_PARMS_CALL);
    for(i = 0; i < MIN(dsize, ssize); i++)
        (*hbits_dst)[i] ^= (*hbits_src)[i];
    for(i; i < ssize; i++)
        (*hbits_dst)[i] = (*hbits_src)[i];
}

void Bits_not_dbg(Bits *hbits DBG_PARMS)
{
    int i;
	int asize = aint_size_dbg((int const *const *)hbits DBG_PARMS_CALL);
    for(i = 0; i < asize; i++)
        (*hbits)[i] = ~(*hbits)[i];
}

int Bits_countset_dbg(const Bits *hbits DBG_PARMS)
{
    int i;
    int count = 0;
	int asize = aint_size_dbg((int const *const *)hbits DBG_PARMS_CALL);
    for(i = 0; i < asize; i++)
    {
        unsigned chunk;
        for(chunk = (*hbits)[i]; chunk; chunk &= chunk-1)
            count++;
    }
    return count;
}

int Bits_highestset_dbg(const Bits *hbits DBG_PARMS)
{
    int i, j;
	int asize = aint_size_dbg((int const *const *)hbits DBG_PARMS_CALL);
    for(i = asize-1; i >= 0; --i)
        if((*hbits)[i])
            for(j = (1<<IDXBITS)-1; j >= 0; --j)
                if((*hbits)[i] & (1 << j))
                    return i<<IDXBITS | j;
    return 0;
}

int Bits_size_dbg(const Bits *hbits DBG_PARMS)
{
	return aint_size_dbg((int const *const *)hbits DBG_PARMS_CALL) *
		(1 << IDXBITS);
}

void Bits_destroy_dbg(Bits *hbits DBG_PARMS)
{
	aint_destroy_dbg((int **)hbits, NULL DBG_PARMS_CALL);
}
