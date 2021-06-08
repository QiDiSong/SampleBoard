/**********************************************************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: 
 * Author     : w00449361
 * Create     : 
 **********************************************************************************************************************/
#ifndef UTILS_H_
#define UTILS_H_


#ifndef U8									/* U8 */
typedef unsigned char   U8;					/* U8 */
#endif										/* U8 */

#ifndef U16									/* U16 */
typedef unsigned short  U16;				/* U16 */
#endif										/* U16 */

#ifndef U32									/* U32 */
typedef unsigned int    U32;				/* U32 */
#endif		

#ifndef S32									/* U32 */
typedef int    S32;				        /* U32 */
#endif									/* U32 */

#define ERROR (-1)
#define OK     (0)

#ifndef BYTE_SWAP32
#define BYTE_SWAP32(value) ((((unsigned)(value)&0x000000ff) << 24) | \
                            (((unsigned)(value)&0x0000ff00) << 8) |            \
                            (((unsigned)(value)&0x00ff0000) >> 8) |            \
                            (((unsigned)(value)&0xff000000) >> 24))
#endif

#ifndef BYTE_SWAP16
#define BYTE_SWAP16(value) ((((unsigned)(value)&0x00ff) << 8) | \
                            (((unsigned)(value)&0xff00) >> 8))
#endif

#endif
