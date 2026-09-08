/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef BASEINV_H__
#define BASEINV_H__

typedef struct RoomDetail RoomDetail;
typedef struct Detail Detail;

extern RoomDetail **g_ppDetailInv;


RoomDetail * baseinv_Add(const Detail *info);
void baseinv_RemoveIdx(int idx);
void baseinv_Remove(RoomDetail *pDetail);


#endif /* #ifndef BASEINV_H__ */

/* End of File */

