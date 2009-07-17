/*
 * COOK compatible decoder fixed point data types and constants
 * Copyright (c) 2007 Ian Braithwaite
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

/**
 * @file cookdata_fixpoint.h
 * Cook AKA RealAudio G2 compatible decoder
 * fixed point data types and constants
 */

#include <inttypes.h>
typedef int32_t FIXP;           /* Fixed point variable type */
typedef uint16_t FIXPU;         /* Fixed point fraction 0<=x<1 */

typedef FIXP REAL_T;


/* No additional variables in COOKContext
 * for fixed point routines
 */
typedef struct {
} realvars_t;

#ifdef TEST
#define cPI1_8 0xec83  /* 1pi/8 2^16 */
#define cPI2_8 0xb505  /* 2pi/8 2^16 */
#define cPI3_8 0x61f8  /* 3pi/8 2^16 */



static const FIXPU sincos_lookup[2050] = {
    /* x_i = 2^16 sin(i 2pi/8192), 2^16 cos(i 2pi/8192); i=0..1024 */
    0x0000, 0xffff,  0x0032, 0xffff,  0x0065, 0xffff,  0x0097, 0xffff,
    0x00c9, 0xffff,  0x00fb, 0xffff,  0x012e, 0xffff,  0x0160, 0xffff,
    0x0192, 0xffff,  0x01c4, 0xfffe,  0x01f7, 0xfffe,  0x0229, 0xfffe,
    0x025b, 0xfffd,  0x028d, 0xfffd,  0x02c0, 0xfffc,  0x02f2, 0xfffc,
    0x0324, 0xfffb,  0x0356, 0xfffa,  0x0389, 0xfffa,  0x03bb, 0xfff9,
    0x03ed, 0xfff8,  0x0420, 0xfff7,  0x0452, 0xfff7,  0x0484, 0xfff6,
    0x04b6, 0xfff5,  0x04e9, 0xfff4,  0x051b, 0xfff3,  0x054d, 0xfff2,
    0x057f, 0xfff1,  0x05b2, 0xfff0,  0x05e4, 0xffef,  0x0616, 0xffed,
    0x0648, 0xffec,  0x067b, 0xffeb,  0x06ad, 0xffea,  0x06df, 0xffe8,
    0x0711, 0xffe7,  0x0744, 0xffe6,  0x0776, 0xffe4,  0x07a8, 0xffe3,
    0x07da, 0xffe1,  0x080d, 0xffe0,  0x083f, 0xffde,  0x0871, 0xffdc,
    0x08a3, 0xffdb,  0x08d5, 0xffd9,  0x0908, 0xffd7,  0x093a, 0xffd5,
    0x096c, 0xffd4,  0x099e, 0xffd2,  0x09d1, 0xffd0,  0x0a03, 0xffce,
    0x0a35, 0xffcc,  0x0a67, 0xffca,  0x0a9a, 0xffc8,  0x0acc, 0xffc6,
    0x0afe, 0xffc4,  0x0b30, 0xffc1,  0x0b62, 0xffbf,  0x0b95, 0xffbd,
    0x0bc7, 0xffbb,  0x0bf9, 0xffb8,  0x0c2b, 0xffb6,  0x0c5d, 0xffb4,
    0x0c90, 0xffb1,  0x0cc2, 0xffaf,  0x0cf4, 0xffac,  0x0d26, 0xffa9,
    0x0d59, 0xffa7,  0x0d8b, 0xffa4,  0x0dbd, 0xffa2,  0x0def, 0xff9f,
    0x0e21, 0xff9c,  0x0e53, 0xff99,  0x0e86, 0xff96,  0x0eb8, 0xff94,
    0x0eea, 0xff91,  0x0f1c, 0xff8e,  0x0f4e, 0xff8b,  0x0f81, 0xff88,
    0x0fb3, 0xff85,  0x0fe5, 0xff82,  0x1017, 0xff7e,  0x1049, 0xff7b,
    0x107b, 0xff78,  0x10ae, 0xff75,  0x10e0, 0xff71,  0x1112, 0xff6e,
    0x1144, 0xff6b,  0x1176, 0xff67,  0x11a8, 0xff64,  0x11da, 0xff60,
    0x120d, 0xff5d,  0x123f, 0xff59,  0x1271, 0xff56,  0x12a3, 0xff52,
    0x12d5, 0xff4e,  0x1307, 0xff4b,  0x1339, 0xff47,  0x136c, 0xff43,
    0x139e, 0xff3f,  0x13d0, 0xff3b,  0x1402, 0xff38,  0x1434, 0xff34,
    0x1466, 0xff30,  0x1498, 0xff2c,  0x14ca, 0xff28,  0x14fc, 0xff23,
    0x152e, 0xff1f,  0x1561, 0xff1b,  0x1593, 0xff17,  0x15c5, 0xff13,
    0x15f7, 0xff0e,  0x1629, 0xff0a,  0x165b, 0xff06,  0x168d, 0xff01,
    0x16bf, 0xfefd,  0x16f1, 0xfef8,  0x1723, 0xfef4,  0x1755, 0xfeef,
    0x1787, 0xfeeb,  0x17b9, 0xfee6,  0x17eb, 0xfee1,  0x181d, 0xfedd,
    0x1850, 0xfed8,  0x1882, 0xfed3,  0x18b4, 0xfece,  0x18e6, 0xfec9,
    0x1918, 0xfec4,  0x194a, 0xfebf,  0x197c, 0xfeba,  0x19ae, 0xfeb5,
    0x19e0, 0xfeb0,  0x1a12, 0xfeab,  0x1a44, 0xfea6,  0x1a76, 0xfea1,
    0x1aa8, 0xfe9c,  0x1ada, 0xfe97,  0x1b0c, 0xfe91,  0x1b3e, 0xfe8c,
    0x1b70, 0xfe87,  0x1ba2, 0xfe81,  0x1bd4, 0xfe7c,  0x1c06, 0xfe76,
    0x1c38, 0xfe71,  0x1c69, 0xfe6b,  0x1c9b, 0xfe66,  0x1ccd, 0xfe60,
    0x1cff, 0xfe5a,  0x1d31, 0xfe55,  0x1d63, 0xfe4f,  0x1d95, 0xfe49,
    0x1dc7, 0xfe43,  0x1df9, 0xfe3d,  0x1e2b, 0xfe37,  0x1e5d, 0xfe31,
    0x1e8f, 0xfe2b,  0x1ec1, 0xfe25,  0x1ef3, 0xfe1f,  0x1f24, 0xfe19,
    0x1f56, 0xfe13,  0x1f88, 0xfe0d,  0x1fba, 0xfe07,  0x1fec, 0xfe01,
    0x201e, 0xfdfa,  0x2050, 0xfdf4,  0x2082, 0xfdee,  0x20b3, 0xfde7,
    0x20e5, 0xfde1,  0x2117, 0xfdda,  0x2149, 0xfdd4,  0x217b, 0xfdcd,
    0x21ad, 0xfdc7,  0x21de, 0xfdc0,  0x2210, 0xfdb9,  0x2242, 0xfdb3,
    0x2274, 0xfdac,  0x22a6, 0xfda5,  0x22d7, 0xfd9e,  0x2309, 0xfd97,
    0x233b, 0xfd90,  0x236d, 0xfd89,  0x239f, 0xfd83,  0x23d0, 0xfd7c,
    0x2402, 0xfd74,  0x2434, 0xfd6d,  0x2466, 0xfd66,  0x2497, 0xfd5f,
    0x24c9, 0xfd58,  0x24fb, 0xfd51,  0x252d, 0xfd49,  0x255e, 0xfd42,
    0x2590, 0xfd3b,  0x25c2, 0xfd33,  0x25f4, 0xfd2c,  0x2625, 0xfd24,
    0x2657, 0xfd1d,  0x2689, 0xfd15,  0x26ba, 0xfd0e,  0x26ec, 0xfd06,
    0x271e, 0xfcfe,  0x274f, 0xfcf7,  0x2781, 0xfcef,  0x27b3, 0xfce7,
    0x27e4, 0xfcdf,  0x2816, 0xfcd8,  0x2848, 0xfcd0,  0x2879, 0xfcc8,
    0x28ab, 0xfcc0,  0x28dd, 0xfcb8,  0x290e, 0xfcb0,  0x2940, 0xfca8,
    0x2971, 0xfca0,  0x29a3, 0xfc97,  0x29d5, 0xfc8f,  0x2a06, 0xfc87,
    0x2a38, 0xfc7f,  0x2a69, 0xfc76,  0x2a9b, 0xfc6e,  0x2acc, 0xfc66,
    0x2afe, 0xfc5d,  0x2b30, 0xfc55,  0x2b61, 0xfc4c,  0x2b93, 0xfc44,
    0x2bc4, 0xfc3b,  0x2bf6, 0xfc33,  0x2c27, 0xfc2a,  0x2c59, 0xfc21,
    0x2c8a, 0xfc18,  0x2cbc, 0xfc10,  0x2ced, 0xfc07,  0x2d1f, 0xfbfe,
    0x2d50, 0xfbf5,  0x2d82, 0xfbec,  0x2db3, 0xfbe3,  0x2de5, 0xfbda,
    0x2e16, 0xfbd1,  0x2e47, 0xfbc8,  0x2e79, 0xfbbf,  0x2eaa, 0xfbb6,
    0x2edc, 0xfbad,  0x2f0d, 0xfba4,  0x2f3f, 0xfb9a,  0x2f70, 0xfb91,
    0x2fa1, 0xfb88,  0x2fd3, 0xfb7e,  0x3004, 0xfb75,  0x3035, 0xfb6b,
    0x3067, 0xfb62,  0x3098, 0xfb58,  0x30ca, 0xfb4f,  0x30fb, 0xfb45,
    0x312c, 0xfb3c,  0x315e, 0xfb32,  0x318f, 0xfb28,  0x31c0, 0xfb1f,
    0x31f1, 0xfb15,  0x3223, 0xfb0b,  0x3254, 0xfb01,  0x3285, 0xfaf7,
    0x32b7, 0xfaed,  0x32e8, 0xfae3,  0x3319, 0xfad9,  0x334a, 0xfacf,
    0x337c, 0xfac5,  0x33ad, 0xfabb,  0x33de, 0xfab1,  0x340f, 0xfaa7,
    0x3440, 0xfa9c,  0x3472, 0xfa92,  0x34a3, 0xfa88,  0x34d4, 0xfa7d,
    0x3505, 0xfa73,  0x3536, 0xfa69,  0x3568, 0xfa5e,  0x3599, 0xfa54,
    0x35ca, 0xfa49,  0x35fb, 0xfa3e,  0x362c, 0xfa34,  0x365d, 0xfa29,
    0x368e, 0xfa1f,  0x36c0, 0xfa14,  0x36f1, 0xfa09,  0x3722, 0xf9fe,
    0x3753, 0xf9f3,  0x3784, 0xf9e8,  0x37b5, 0xf9de,  0x37e6, 0xf9d3,
    0x3817, 0xf9c8,  0x3848, 0xf9bd,  0x3879, 0xf9b2,  0x38aa, 0xf9a6,
    0x38db, 0xf99b,  0x390c, 0xf990,  0x393d, 0xf985,  0x396e, 0xf97a,
    0x399f, 0xf96e,  0x39d0, 0xf963,  0x3a01, 0xf958,  0x3a32, 0xf94c,
    0x3a63, 0xf941,  0x3a94, 0xf935,  0x3ac5, 0xf92a,  0x3af6, 0xf91e,
    0x3b27, 0xf913,  0x3b58, 0xf907,  0x3b88, 0xf8fb,  0x3bb9, 0xf8f0,
    0x3bea, 0xf8e4,  0x3c1b, 0xf8d8,  0x3c4c, 0xf8cc,  0x3c7d, 0xf8c0,
    0x3cae, 0xf8b4,  0x3cde, 0xf8a9,  0x3d0f, 0xf89d,  0x3d40, 0xf891,
    0x3d71, 0xf885,  0x3da2, 0xf878,  0x3dd2, 0xf86c,  0x3e03, 0xf860,
    0x3e34, 0xf854,  0x3e65, 0xf848,  0x3e95, 0xf83b,  0x3ec6, 0xf82f,
    0x3ef7, 0xf823,  0x3f28, 0xf816,  0x3f58, 0xf80a,  0x3f89, 0xf7fe,
    0x3fba, 0xf7f1,  0x3fea, 0xf7e5,  0x401b, 0xf7d8,  0x404c, 0xf7cb,
    0x407c, 0xf7bf,  0x40ad, 0xf7b2,  0x40de, 0xf7a5,  0x410e, 0xf799,
    0x413f, 0xf78c,  0x416f, 0xf77f,  0x41a0, 0xf772,  0x41d1, 0xf765,
    0x4201, 0xf758,  0x4232, 0xf74b,  0x4262, 0xf73e,  0x4293, 0xf731,
    0x42c3, 0xf724,  0x42f4, 0xf717,  0x4324, 0xf70a,  0x4355, 0xf6fd,
    0x4385, 0xf6ef,  0x43b6, 0xf6e2,  0x43e6, 0xf6d5,  0x4417, 0xf6c7,
    0x4447, 0xf6ba,  0x4478, 0xf6ad,  0x44a8, 0xf69f,  0x44d9, 0xf692,
    0x4509, 0xf684,  0x4539, 0xf677,  0x456a, 0xf669,  0x459a, 0xf65b,
    0x45cb, 0xf64e,  0x45fb, 0xf640,  0x462b, 0xf632,  0x465c, 0xf624,
    0x468c, 0xf616,  0x46bc, 0xf609,  0x46ec, 0xf5fb,  0x471d, 0xf5ed,
    0x474d, 0xf5df,  0x477d, 0xf5d1,  0x47ae, 0xf5c3,  0x47de, 0xf5b5,
    0x480e, 0xf5a6,  0x483e, 0xf598,  0x486f, 0xf58a,  0x489f, 0xf57c,
    0x48cf, 0xf56e,  0x48ff, 0xf55f,  0x492f, 0xf551,  0x495f, 0xf543,
    0x4990, 0xf534,  0x49c0, 0xf526,  0x49f0, 0xf517,  0x4a20, 0xf509,
    0x4a50, 0xf4fa,  0x4a80, 0xf4eb,  0x4ab0, 0xf4dd,  0x4ae0, 0xf4ce,
    0x4b10, 0xf4bf,  0x4b40, 0xf4b1,  0x4b71, 0xf4a2,  0x4ba1, 0xf493,
    0x4bd1, 0xf484,  0x4c01, 0xf475,  0x4c31, 0xf466,  0x4c61, 0xf457,
    0x4c90, 0xf448,  0x4cc0, 0xf439,  0x4cf0, 0xf42a,  0x4d20, 0xf41b,
    0x4d50, 0xf40c,  0x4d80, 0xf3fd,  0x4db0, 0xf3ed,  0x4de0, 0xf3de,
    0x4e10, 0xf3cf,  0x4e40, 0xf3c0,  0x4e70, 0xf3b0,  0x4e9f, 0xf3a1,
    0x4ecf, 0xf391,  0x4eff, 0xf382,  0x4f2f, 0xf372,  0x4f5f, 0xf363,
    0x4f8e, 0xf353,  0x4fbe, 0xf343,  0x4fee, 0xf334,  0x501e, 0xf324,
    0x504d, 0xf314,  0x507d, 0xf304,  0x50ad, 0xf2f5,  0x50dd, 0xf2e5,
    0x510c, 0xf2d5,  0x513c, 0xf2c5,  0x516c, 0xf2b5,  0x519b, 0xf2a5,
    0x51cb, 0xf295,  0x51fb, 0xf285,  0x522a, 0xf275,  0x525a, 0xf265,
    0x5289, 0xf254,  0x52b9, 0xf244,  0x52e8, 0xf234,  0x5318, 0xf224,
    0x5348, 0xf213,  0x5377, 0xf203,  0x53a7, 0xf1f3,  0x53d6, 0xf1e2,
    0x5406, 0xf1d2,  0x5435, 0xf1c1,  0x5464, 0xf1b1,  0x5494, 0xf1a0,
    0x54c3, 0xf18f,  0x54f3, 0xf17f,  0x5522, 0xf16e,  0x5552, 0xf15d,
    0x5581, 0xf14c,  0x55b0, 0xf13c,  0x55e0, 0xf12b,  0x560f, 0xf11a,
    0x563e, 0xf109,  0x566e, 0xf0f8,  0x569d, 0xf0e7,  0x56cc, 0xf0d6,
    0x56fc, 0xf0c5,  0x572b, 0xf0b4,  0x575a, 0xf0a3,  0x5789, 0xf092,
    0x57b9, 0xf080,  0x57e8, 0xf06f,  0x5817, 0xf05e,  0x5846, 0xf04d,
    0x5875, 0xf03b,  0x58a5, 0xf02a,  0x58d4, 0xf018,  0x5903, 0xf007,
    0x5932, 0xeff5,  0x5961, 0xefe4,  0x5990, 0xefd2,  0x59bf, 0xefc1,
    0x59ee, 0xefaf,  0x5a1d, 0xef9d,  0x5a4c, 0xef8c,  0x5a7b, 0xef7a,
    0x5aaa, 0xef68,  0x5ad9, 0xef56,  0x5b08, 0xef45,  0x5b37, 0xef33,
    0x5b66, 0xef21,  0x5b95, 0xef0f,  0x5bc4, 0xeefd,  0x5bf3, 0xeeeb,
    0x5c22, 0xeed9,  0x5c51, 0xeec7,  0x5c80, 0xeeb4,  0x5caf, 0xeea2,
    0x5cde, 0xee90,  0x5d0c, 0xee7e,  0x5d3b, 0xee6b,  0x5d6a, 0xee59,
    0x5d99, 0xee47,  0x5dc8, 0xee34,  0x5df6, 0xee22,  0x5e25, 0xee0f,
    0x5e54, 0xedfd,  0x5e83, 0xedea,  0x5eb1, 0xedd8,  0x5ee0, 0xedc5,
    0x5f0f, 0xedb3,  0x5f3d, 0xeda0,  0x5f6c, 0xed8d,  0x5f9b, 0xed7a,
    0x5fc9, 0xed68,  0x5ff8, 0xed55,  0x6026, 0xed42,  0x6055, 0xed2f,
    0x6084, 0xed1c,  0x60b2, 0xed09,  0x60e1, 0xecf6,  0x610f, 0xece3,
    0x613e, 0xecd0,  0x616c, 0xecbd,  0x619b, 0xecaa,  0x61c9, 0xec97,
    0x61f8, 0xec83,  0x6226, 0xec70,  0x6254, 0xec5d,  0x6283, 0xec4a,
    0x62b1, 0xec36,  0x62e0, 0xec23,  0x630e, 0xec0f,  0x633c, 0xebfc,
    0x636b, 0xebe8,  0x6399, 0xebd5,  0x63c7, 0xebc1,  0x63f5, 0xebae,
    0x6424, 0xeb9a,  0x6452, 0xeb86,  0x6480, 0xeb73,  0x64ae, 0xeb5f,
    0x64dd, 0xeb4b,  0x650b, 0xeb37,  0x6539, 0xeb23,  0x6567, 0xeb0f,
    0x6595, 0xeafc,  0x65c3, 0xeae8,  0x65f2, 0xead4,  0x6620, 0xeac0,
    0x664e, 0xeaab,  0x667c, 0xea97,  0x66aa, 0xea83,  0x66d8, 0xea6f,
    0x6706, 0xea5b,  0x6734, 0xea47,  0x6762, 0xea32,  0x6790, 0xea1e,
    0x67be, 0xea0a,  0x67ec, 0xe9f5,  0x681a, 0xe9e1,  0x6848, 0xe9cc,
    0x6876, 0xe9b8,  0x68a3, 0xe9a3,  0x68d1, 0xe98f,  0x68ff, 0xe97a,
    0x692d, 0xe966,  0x695b, 0xe951,  0x6989, 0xe93c,  0x69b6, 0xe927,
    0x69e4, 0xe913,  0x6a12, 0xe8fe,  0x6a40, 0xe8e9,  0x6a6d, 0xe8d4,
    0x6a9b, 0xe8bf,  0x6ac9, 0xe8aa,  0x6af6, 0xe895,  0x6b24, 0xe880,
    0x6b52, 0xe86b,  0x6b7f, 0xe856,  0x6bad, 0xe841,  0x6bdb, 0xe82c,
    0x6c08, 0xe817,  0x6c36, 0xe801,  0x6c63, 0xe7ec,  0x6c91, 0xe7d7,
    0x6cbe, 0xe7c2,  0x6cec, 0xe7ac,  0x6d19, 0xe797,  0x6d47, 0xe781,
    0x6d74, 0xe76c,  0x6da2, 0xe756,  0x6dcf, 0xe741,  0x6dfc, 0xe72b,
    0x6e2a, 0xe716,  0x6e57, 0xe700,  0x6e85, 0xe6ea,  0x6eb2, 0xe6d5,
    0x6edf, 0xe6bf,  0x6f0d, 0xe6a9,  0x6f3a, 0xe693,  0x6f67, 0xe67d,
    0x6f94, 0xe667,  0x6fc2, 0xe652,  0x6fef, 0xe63c,  0x701c, 0xe626,
    0x7049, 0xe610,  0x7076, 0xe5f9,  0x70a3, 0xe5e3,  0x70d1, 0xe5cd,
    0x70fe, 0xe5b7,  0x712b, 0xe5a1,  0x7158, 0xe58b,  0x7185, 0xe574,
    0x71b2, 0xe55e,  0x71df, 0xe548,  0x720c, 0xe531,  0x7239, 0xe51b,
    0x7266, 0xe504,  0x7293, 0xe4ee,  0x72c0, 0xe4d7,  0x72ed, 0xe4c1,
    0x731a, 0xe4aa,  0x7347, 0xe494,  0x7373, 0xe47d,  0x73a0, 0xe466,
    0x73cd, 0xe450,  0x73fa, 0xe439,  0x7427, 0xe422,  0x7454, 0xe40b,
    0x7480, 0xe3f4,  0x74ad, 0xe3de,  0x74da, 0xe3c7,  0x7507, 0xe3b0,
    0x7533, 0xe399,  0x7560, 0xe382,  0x758d, 0xe36b,  0x75b9, 0xe353,
    0x75e6, 0xe33c,  0x7612, 0xe325,  0x763f, 0xe30e,  0x766c, 0xe2f7,
    0x7698, 0xe2df,  0x76c5, 0xe2c8,  0x76f1, 0xe2b1,  0x771e, 0xe299,
    0x774a, 0xe282,  0x7777, 0xe26b,  0x77a3, 0xe253,  0x77d0, 0xe23c,
    0x77fc, 0xe224,  0x7828, 0xe20d,  0x7855, 0xe1f5,  0x7881, 0xe1dd,
    0x78ad, 0xe1c6,  0x78da, 0xe1ae,  0x7906, 0xe196,  0x7932, 0xe17e,
    0x795f, 0xe167,  0x798b, 0xe14f,  0x79b7, 0xe137,  0x79e3, 0xe11f,
    0x7a10, 0xe107,  0x7a3c, 0xe0ef,  0x7a68, 0xe0d7,  0x7a94, 0xe0bf,
    0x7ac0, 0xe0a7,  0x7aec, 0xe08f,  0x7b18, 0xe077,  0x7b44, 0xe05e,
    0x7b70, 0xe046,  0x7b9c, 0xe02e,  0x7bc8, 0xe016,  0x7bf4, 0xdffd,
    0x7c20, 0xdfe5,  0x7c4c, 0xdfcd,  0x7c78, 0xdfb4,  0x7ca4, 0xdf9c,
    0x7cd0, 0xdf83,  0x7cfc, 0xdf6b,  0x7d28, 0xdf52,  0x7d54, 0xdf39,
    0x7d7f, 0xdf21,  0x7dab, 0xdf08,  0x7dd7, 0xdef0,  0x7e03, 0xded7,
    0x7e2f, 0xdebe,  0x7e5a, 0xdea5,  0x7e86, 0xde8c,  0x7eb2, 0xde74,
    0x7edd, 0xde5b,  0x7f09, 0xde42,  0x7f35, 0xde29,  0x7f60, 0xde10,
    0x7f8c, 0xddf7,  0x7fb7, 0xddde,  0x7fe3, 0xddc5,  0x800f, 0xddab,
    0x803a, 0xdd92,  0x8066, 0xdd79,  0x8091, 0xdd60,  0x80bc, 0xdd47,
    0x80e8, 0xdd2d,  0x8113, 0xdd14,  0x813f, 0xdcfb,  0x816a, 0xdce1,
    0x8195, 0xdcc8,  0x81c1, 0xdcae,  0x81ec, 0xdc95,  0x8217, 0xdc7b,
    0x8243, 0xdc62,  0x826e, 0xdc48,  0x8299, 0xdc2f,  0x82c4, 0xdc15,
    0x82f0, 0xdbfb,  0x831b, 0xdbe1,  0x8346, 0xdbc8,  0x8371, 0xdbae,
    0x839c, 0xdb94,  0x83c7, 0xdb7a,  0x83f2, 0xdb60,  0x841d, 0xdb46,
    0x8449, 0xdb2c,  0x8474, 0xdb12,  0x849f, 0xdaf8,  0x84ca, 0xdade,
    0x84f5, 0xdac4,  0x851f, 0xdaaa,  0x854a, 0xda90,  0x8575, 0xda76,
    0x85a0, 0xda5c,  0x85cb, 0xda41,  0x85f6, 0xda27,  0x8621, 0xda0d,
    0x864c, 0xd9f2,  0x8676, 0xd9d8,  0x86a1, 0xd9be,  0x86cc, 0xd9a3,
    0x86f7, 0xd989,  0x8721, 0xd96e,  0x874c, 0xd954,  0x8777, 0xd939,
    0x87a1, 0xd91e,  0x87cc, 0xd904,  0x87f6, 0xd8e9,  0x8821, 0xd8ce,
    0x884c, 0xd8b4,  0x8876, 0xd899,  0x88a1, 0xd87e,  0x88cb, 0xd863,
    0x88f6, 0xd848,  0x8920, 0xd82d,  0x894a, 0xd812,  0x8975, 0xd7f8,
    0x899f, 0xd7dc,  0x89ca, 0xd7c1,  0x89f4, 0xd7a6,  0x8a1e, 0xd78b,
    0x8a49, 0xd770,  0x8a73, 0xd755,  0x8a9d, 0xd73a,  0x8ac7, 0xd71f,
    0x8af2, 0xd703,  0x8b1c, 0xd6e8,  0x8b46, 0xd6cd,  0x8b70, 0xd6b1,
    0x8b9a, 0xd696,  0x8bc5, 0xd67a,  0x8bef, 0xd65f,  0x8c19, 0xd644,
    0x8c43, 0xd628,  0x8c6d, 0xd60c,  0x8c97, 0xd5f1,  0x8cc1, 0xd5d5,
    0x8ceb, 0xd5ba,  0x8d15, 0xd59e,  0x8d3f, 0xd582,  0x8d69, 0xd566,
    0x8d93, 0xd54b,  0x8dbc, 0xd52f,  0x8de6, 0xd513,  0x8e10, 0xd4f7,
    0x8e3a, 0xd4db,  0x8e64, 0xd4bf,  0x8e8d, 0xd4a3,  0x8eb7, 0xd487,
    0x8ee1, 0xd46b,  0x8f0b, 0xd44f,  0x8f34, 0xd433,  0x8f5e, 0xd417,
    0x8f88, 0xd3fb,  0x8fb1, 0xd3df,  0x8fdb, 0xd3c2,  0x9004, 0xd3a6,
    0x902e, 0xd38a,  0x9057, 0xd36d,  0x9081, 0xd351,  0x90aa, 0xd335,
    0x90d4, 0xd318,  0x90fd, 0xd2fc,  0x9127, 0xd2df,  0x9150, 0xd2c3,
    0x9179, 0xd2a6,  0x91a3, 0xd28a,  0x91cc, 0xd26d,  0x91f5, 0xd250,
    0x921f, 0xd234,  0x9248, 0xd217,  0x9271, 0xd1fa,  0x929a, 0xd1de,
    0x92c4, 0xd1c1,  0x92ed, 0xd1a4,  0x9316, 0xd187,  0x933f, 0xd16a,
    0x9368, 0xd14d,  0x9391, 0xd130,  0x93ba, 0xd113,  0x93e3, 0xd0f6,
    0x940c, 0xd0d9,  0x9435, 0xd0bc,  0x945e, 0xd09f,  0x9487, 0xd082,
    0x94b0, 0xd065,  0x94d9, 0xd047,  0x9502, 0xd02a,  0x952b, 0xd00d,
    0x9554, 0xcff0,  0x957d, 0xcfd2,  0x95a5, 0xcfb5,  0x95ce, 0xcf98,
    0x95f7, 0xcf7a,  0x9620, 0xcf5d,  0x9648, 0xcf3f,  0x9671, 0xcf22,
    0x969a, 0xcf04,  0x96c2, 0xcee7,  0x96eb, 0xcec9,  0x9713, 0xceab,
    0x973c, 0xce8e,  0x9765, 0xce70,  0x978d, 0xce52,  0x97b6, 0xce34,
    0x97de, 0xce17,  0x9807, 0xcdf9,  0x982f, 0xcddb,  0x9857, 0xcdbd,
    0x9880, 0xcd9f,  0x98a8, 0xcd81,  0x98d0, 0xcd63,  0x98f9, 0xcd45,
    0x9921, 0xcd27,  0x9949, 0xcd09,  0x9972, 0xcceb,  0x999a, 0xcccd,
    0x99c2, 0xccae,  0x99ea, 0xcc90,  0x9a12, 0xcc72,  0x9a3a, 0xcc54,
    0x9a63, 0xcc35,  0x9a8b, 0xcc17,  0x9ab3, 0xcbf9,  0x9adb, 0xcbda,
    0x9b03, 0xcbbc,  0x9b2b, 0xcb9e,  0x9b53, 0xcb7f,  0x9b7b, 0xcb61,
    0x9ba3, 0xcb42,  0x9bca, 0xcb23,  0x9bf2, 0xcb05,  0x9c1a, 0xcae6,
    0x9c42, 0xcac7,  0x9c6a, 0xcaa9,  0x9c92, 0xca8a,  0x9cb9, 0xca6b,
    0x9ce1, 0xca4d,  0x9d09, 0xca2e,  0x9d31, 0xca0f,  0x9d58, 0xc9f0,
    0x9d80, 0xc9d1,  0x9da7, 0xc9b2,  0x9dcf, 0xc993,  0x9df7, 0xc974,
    0x9e1e, 0xc955,  0x9e46, 0xc936,  0x9e6d, 0xc917,  0x9e95, 0xc8f8,
    0x9ebc, 0xc8d9,  0x9ee3, 0xc8ba,  0x9f0b, 0xc89a,  0x9f32, 0xc87b,
    0x9f5a, 0xc85c,  0x9f81, 0xc83c,  0x9fa8, 0xc81d,  0x9fd0, 0xc7fe,
    0x9ff7, 0xc7de,  0xa01e, 0xc7bf,  0xa045, 0xc7a0,  0xa06c, 0xc780,
    0xa094, 0xc761,  0xa0bb, 0xc741,  0xa0e2, 0xc721,  0xa109, 0xc702,
    0xa130, 0xc6e2,  0xa157, 0xc6c2,  0xa17e, 0xc6a3,  0xa1a5, 0xc683,
    0xa1cc, 0xc663,  0xa1f3, 0xc644,  0xa21a, 0xc624,  0xa241, 0xc604,
    0xa268, 0xc5e4,  0xa28e, 0xc5c4,  0xa2b5, 0xc5a4,  0xa2dc, 0xc584,
    0xa303, 0xc564,  0xa32a, 0xc544,  0xa350, 0xc524,  0xa377, 0xc504,
    0xa39e, 0xc4e4,  0xa3c4, 0xc4c4,  0xa3eb, 0xc4a4,  0xa412, 0xc483,
    0xa438, 0xc463,  0xa45f, 0xc443,  0xa485, 0xc423,  0xa4ac, 0xc402,
    0xa4d2, 0xc3e2,  0xa4f9, 0xc3c2,  0xa51f, 0xc3a1,  0xa545, 0xc381,
    0xa56c, 0xc360,  0xa592, 0xc340,  0xa5b8, 0xc31f,  0xa5df, 0xc2ff,
    0xa605, 0xc2de,  0xa62b, 0xc2be,  0xa652, 0xc29d,  0xa678, 0xc27c,
    0xa69e, 0xc25c,  0xa6c4, 0xc23b,  0xa6ea, 0xc21a,  0xa710, 0xc1f9,
    0xa736, 0xc1d8,  0xa75c, 0xc1b8,  0xa782, 0xc197,  0xa7a8, 0xc176,
    0xa7ce, 0xc155,  0xa7f4, 0xc134,  0xa81a, 0xc113,  0xa840, 0xc0f2,
    0xa866, 0xc0d1,  0xa88c, 0xc0b0,  0xa8b2, 0xc08f,  0xa8d7, 0xc06e,
    0xa8fd, 0xc04c,  0xa923, 0xc02b,  0xa949, 0xc00a,  0xa96e, 0xbfe9,
    0xa994, 0xbfc7,  0xa9ba, 0xbfa6,  0xa9df, 0xbf85,  0xaa05, 0xbf63,
    0xaa2a, 0xbf42,  0xaa50, 0xbf21,  0xaa76, 0xbeff,  0xaa9b, 0xbede,
    0xaac1, 0xbebc,  0xaae6, 0xbe9b,  0xab0b, 0xbe79,  0xab31, 0xbe57,
    0xab56, 0xbe36,  0xab7b, 0xbe14,  0xaba1, 0xbdf2,  0xabc6, 0xbdd1,
    0xabeb, 0xbdaf,  0xac11, 0xbd8d,  0xac36, 0xbd6b,  0xac5b, 0xbd4a,
    0xac80, 0xbd28,  0xaca5, 0xbd06,  0xacca, 0xbce4,  0xacef, 0xbcc2,
    0xad14, 0xbca0,  0xad39, 0xbc7e,  0xad5e, 0xbc5c,  0xad83, 0xbc3a,
    0xada8, 0xbc18,  0xadcd, 0xbbf6,  0xadf2, 0xbbd4,  0xae17, 0xbbb1,
    0xae3c, 0xbb8f,  0xae61, 0xbb6d,  0xae85, 0xbb4b,  0xaeaa, 0xbb28,
    0xaecf, 0xbb06,  0xaef4, 0xbae4,  0xaf18, 0xbac1,  0xaf3d, 0xba9f,
    0xaf62, 0xba7d,  0xaf86, 0xba5a,  0xafab, 0xba38,  0xafcf, 0xba15,
    0xaff4, 0xb9f3,  0xb018, 0xb9d0,  0xb03d, 0xb9ae,  0xb061, 0xb98b,
    0xb086, 0xb968,  0xb0aa, 0xb946,  0xb0ce, 0xb923,  0xb0f3, 0xb900,
    0xb117, 0xb8dd,  0xb13b, 0xb8bb,  0xb160, 0xb898,  0xb184, 0xb875,
    0xb1a8, 0xb852,  0xb1cc, 0xb82f,  0xb1f0, 0xb80c,  0xb215, 0xb7e9,
    0xb239, 0xb7c6,  0xb25d, 0xb7a3,  0xb281, 0xb780,  0xb2a5, 0xb75d,
    0xb2c9, 0xb73a,  0xb2ed, 0xb717,  0xb311, 0xb6f4,  0xb335, 0xb6d1,
    0xb358, 0xb6ad,  0xb37c, 0xb68a,  0xb3a0, 0xb667,  0xb3c4, 0xb644,
    0xb3e8, 0xb620,  0xb40b, 0xb5fd,  0xb42f, 0xb5da,  0xb453, 0xb5b6,
    0xb477, 0xb593,  0xb49a, 0xb56f,  0xb4be, 0xb54c,  0xb4e1, 0xb528,
    0xb505, 0xb505
};

#endif

static const FIXPU pow128_tab[128] = {
    /* x_i = 2^(15+i/128) */
    0x8000, 0x80b2, 0x8165, 0x8219, 0x82ce, 0x8383, 0x843a, 0x84f2, 0x85ab,
    0x8665, 0x871f, 0x87db, 0x8898, 0x8956, 0x8a15, 0x8ad5, 0x8b96, 0x8c58,
    0x8d1b, 0x8ddf, 0x8ea4, 0x8f6b, 0x9032, 0x90fa, 0x91c4, 0x928e, 0x935a,
    0x9427, 0x94f5, 0x95c4, 0x9694, 0x9765, 0x9838, 0x990c, 0x99e0, 0x9ab6,
    0x9b8d, 0x9c65, 0x9d3f, 0x9e19, 0x9ef5, 0x9fd2, 0xa0b0, 0xa190, 0xa270,
    0xa352, 0xa435, 0xa519, 0xa5ff, 0xa6e6, 0xa7ce, 0xa8b7, 0xa9a1, 0xaa8d,
    0xab7a, 0xac69, 0xad58, 0xae49, 0xaf3b, 0xb02f, 0xb124, 0xb21a, 0xb312,
    0xb40b, 0xb505, 0xb601, 0xb6fe, 0xb7fc, 0xb8fc, 0xb9fd, 0xbaff, 0xbc03,
    0xbd09, 0xbe0f, 0xbf18, 0xc021, 0xc12c, 0xc239, 0xc347, 0xc456, 0xc567,
    0xc67a, 0xc78d, 0xc8a3, 0xc9ba, 0xcad2, 0xcbec, 0xcd08, 0xce25, 0xcf43,
    0xd063, 0xd185, 0xd2a8, 0xd3cd, 0xd4f3, 0xd61b, 0xd745, 0xd870, 0xd99d,
    0xdacc, 0xdbfc, 0xdd2e, 0xde61, 0xdf96, 0xe0cd, 0xe205, 0xe340, 0xe47b,
    0xe5b9, 0xe6f8, 0xe839, 0xe97c, 0xeac1, 0xec07, 0xed4f, 0xee99, 0xefe5,
    0xf132, 0xf281, 0xf3d3, 0xf525, 0xf67a, 0xf7d1, 0xf929, 0xfa84, 0xfbe0,
    0xfd3e, 0xfe9e
};



/* dither_table and quant_centroid_table.
 * Index 1: [0] - scaled by 2^13, [1] - scaled by 2^13 / sqrt(2)
 * Index 2: [0..7] - category
 * Index 3: [0] - dither_table, [1..13] - quant_centroid_table
 */
static const FIXP quant_tables[2][8][14] = {{{
    0x00000000, 0x0645a1cb, 0x0c2d0e56, 0x11eb851f, 0x17a1cac1, 0x1d4fdf3b,
    0x22ed9168, 0x28a7ef9e, 0x2e49ba5e, 0x33eb851f, 0x39916873, 0x3f126e98,
    0x449ba5e3, 0x4b958106
},{
    0x00000000, 0x08b43958, 0x10f5c28f, 0x19020c4a, 0x2116872b, 0x2922d0e5,
    0x3126e979, 0x38fdf3b6, 0x411eb852, 0x49eb851f, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x00000000, 0x0bef9db2, 0x176c8b44, 0x22e147ae, 0x2e1cac08, 0x39581062,
    0x450e5604, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x00000000, 0x10189375, 0x20000000, 0x2fe353f8, 0x3fc28f5c, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x00000000, 0x1522d0e5, 0x2b3f7cee, 0x3fba5e35, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x02d413cd, 0x1a83126f, 0x37db22d1, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x04000000, 0x1f6c8b44, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x0b504f33, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
}},{{
    0x00000000, 0x046f5a70, 0x089c1768, 0x0cabddd3, 0x10b5d955, 0x14ba09ed,
    0x18b2a4b4, 0x1cbf85aa, 0x20bb05e5, 0x24b68620, 0x28b4ebcf, 0x2c994066,
    0x30835fe6, 0x35722a5e
},{
    0x00000000, 0x062797a1, 0x0bfe1683, 0x11aeee7a, 0x1765915b, 0x1d166952,
    0x22c17660, 0x284ca76c, 0x2e0bfaaa, 0x3444f306, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x00000000, 0x0870a594, 0x1090326a, 0x18a9f456, 0x209b29e3, 0x288c5f70,
    0x30d478a5, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x00000000, 0x0b61afee, 0x16a09e66, 0x21dca76a, 0x2d15caf9, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x00000000, 0x0ef20652, 0x1e94b968, 0x2d100010, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x02000000, 0x12bf2f44, 0x277f041b, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x02d413cd, 0x16385a03, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
},{
    0x08000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000
}}};

static const FIXPU cplscale2[3] = {
    /* 2^16 C_ij */
    0xf3f9, 0xb505, 0x4d8b
};
static const FIXPU cplscale3[7] = {
    /* 2^16 C_ij */
    0xfb35, 0xefdf, 0xe03d, 0xb505, 0x7b81, 0x596e, 0x314d
};
static const FIXPU cplscale4[15] = {
    /* 2^16 C_ij */
    0xfdd2, 0xf927, 0xf3f9, 0xee1d, 0xe749, 0xdee9, 0xd381, 0xb505, 0x903b,
    0x7de2, 0x6dbe, 0x5e02, 0x4d8b, 0x3ad1, 0x2155
};
static const FIXPU cplscale5[31] = {
    /* 2^16 C_ij */
    0xfef5, 0xfcce, 0xfa8e, 0xf832, 0xf5b5, 0xf314, 0xf049, 0xed4c, 0xea12,
    0xe68e, 0xe2ab, 0xde4b, 0xd938, 0xd30b, 0xcab6, 0xb505, 0x9c59, 0x90e8,
    0x8778, 0x7ef9, 0x76fc, 0x6f45, 0x67ab, 0x600e, 0x5850, 0x504d, 0x47db,
    0x3ebd, 0x3486, 0x2853, 0x1715
};
static const FIXPU cplscale6[63] = {
    /* 2^16 C_ij */
    0xff7d, 0xfe74, 0xfd65, 0xfc50, 0xfb35, 0xfa14, 0xf8eb, 0xf7bb, 0xf683,
    0xf543, 0xf3f9, 0xf2a6, 0xf148, 0xefdf, 0xee6a, 0xece6, 0xeb54, 0xe9b2,
    0xe7fd, 0xe634, 0xe453, 0xe258, 0xe03d, 0xddff, 0xdb94, 0xd8f4, 0xd610,
    0xd2d2, 0xcf13, 0xca8c, 0xc47c, 0xb505, 0xa41a, 0x9c90, 0x9685, 0x913a,
    0x8c67, 0x87e5, 0x839c, 0x7f7e, 0x7b81, 0x779b, 0x73c7, 0x6fff, 0x6c3f,
    0x6883, 0x64c7, 0x6107, 0x5d40, 0x596e, 0x558d, 0x5198, 0x4d8b, 0x495f,
    0x450d, 0x408b, 0x3bcd, 0x36c1, 0x314d, 0x2b4a, 0x246e, 0x1c1a, 0x1029
};

