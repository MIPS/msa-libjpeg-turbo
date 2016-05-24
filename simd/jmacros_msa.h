/*
 * MIPS MSA optimizations for libjpeg-turbo
 *
 * Copyright (C) 2016-2017, Imagination Technologies.
 * All rights reserved.
 * Authors: Vikram Dattu (vikram.dattu@imgtec.com)
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef __JMACROS_MSA_H__
#define __JMACROS_MSA_H__

#include <stdint.h>
#include <msa.h>

#define ALIGNMENT           16
#define ALIGNMENT_MINUS_1   (ALIGNMENT - 1)
#define ALLOC_ALIGNED(align) __attribute__ ((aligned((align) << 1)))

#ifdef CLANG_BUILD
  #define MSA_ADDVI_B(a, b)  __msa_addvi_b((v16i8) a, b)
  #define MSA_ADDVI_H(a, b)  __msa_addvi_h((v8i16) a, b)
  #define MSA_ADDVI_W(a, b)  __msa_addvi_w((v4i32) a, b)
  #define MSA_ADDVI_D(a, b)  __msa_addvi_d((v2i64) a, b)
  #define MSA_SUBVI_B(a, b)  __msa_subvi_b((v16i8) a, b)
  #define MSA_SUBVI_H(a, b)  __msa_subvi_h((v8i16) a, b)
  #define MSA_SUBVI_W(a, b)  __msa_subvi_w((v4i32) a, b)
  #define MSA_SUBVI_D(a, b)  __msa_subvi_d((v2i64) a, b)
  #define MSA_MULVI_B(a, b)  __msa_mulv_b((v16i8) a, __msa_ldi_b(b))
  #define MSA_MULVI_H(a, b)  __msa_mulv_h((v8i16) a, __msa_ldi_h(b))
  #define MSA_MULVI_W(a, b)  __msa_mulv_w((v4i32) a, __msa_ldi_w(b))
  #define MSA_MULVI_D(a, b)  __msa_mulv_d((v2i64) a, __msa_ldi_d(b))
  #define MSA_SLLI_B(a, b)   __msa_slli_b((v16i8) a, b)
  #define MSA_SLLI_H(a, b)   __msa_slli_h((v8i16) a, b)
  #define MSA_SLLI_W(a, b)   __msa_slli_w((v4i32) a, b)
  #define MSA_SLLI_D(a, b)   __msa_slli_d((v2i64) a, b)
  #define MSA_SRAI_B(a, b)   __msa_srai_b((v16i8) a, b)
  #define MSA_SRAI_H(a, b)   __msa_srai_h((v8i16) a, b)
  #define MSA_SRAI_W(a, b)   __msa_srai_w((v4i32) a, b)
  #define MSA_SRAI_D(a, b)   __msa_srai_d((v2i64) a, b)
  #define MSA_XORI_V(a, b)   __msa_xori_b((v16i8) a, b)
#else
  #define MSA_ADDVI_B(a, b)  (a + b)
  #define MSA_ADDVI_H(a, b)  (a + b)
  #define MSA_ADDVI_W(a, b)  (a + b)
  #define MSA_ADDVI_D(a, b)  (a + b)
  #define MSA_SUBVI_B(a, b)  (a - b)
  #define MSA_SUBVI_H(a, b)  (a - b)
  #define MSA_SUBVI_W(a, b)  (a - b)
  #define MSA_SUBVI_D(a, b)  (a - b)
  #define MSA_MULVI_B(a, b)  (a * b)
  #define MSA_MULVI_H(a, b)  (a * b)
  #define MSA_MULVI_W(a, b)  (a * b)
  #define MSA_MULVI_D(a, b)  (a * b)
  #define MSA_SLLI_B(a, b)   (a << b)
  #define MSA_SLLI_H(a, b)   (a << b)
  #define MSA_SLLI_W(a, b)   (a << b)
  #define MSA_SLLI_D(a, b)   (a << b)
  #define MSA_SRAI_B(a, b)   (a >> b)
  #define MSA_SRAI_H(a, b)   (a >> b)
  #define MSA_SRAI_W(a, b)   (a >> b)
  #define MSA_SRAI_D(a, b)   (a >> b)
  #define MSA_XORI_V(a, b)   (a ^ b)
#endif

#define LD_B(RTYPE, psrc) *((RTYPE *)(psrc))
#define LD_UB(...) LD_B(v16u8, __VA_ARGS__)
#define LD_SB(...) LD_B(v16i8, __VA_ARGS__)

#define LD_H(RTYPE, psrc) *((RTYPE *)(psrc))
#define LD_UH(...) LD_H(v8u16, __VA_ARGS__)
#define LD_SH(...) LD_H(v8i16, __VA_ARGS__)

#define LD_W(RTYPE, psrc) *((RTYPE *)(psrc))
#define LD_UW(...) LD_W(v4u32, __VA_ARGS__)
#define LD_SW(...) LD_W(v4i32, __VA_ARGS__)
#define LD_SP(...) LD_W(v4f32, __VA_ARGS__)

#define LD_D(RTYPE, psrc) *((RTYPE *)(psrc))
#define LD_DP(...) LD_D(v2f64, __VA_ARGS__)

#define ST_B(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
#define ST_UB(...) ST_B(v16u8, __VA_ARGS__)
#define ST_SB(...) ST_B(v16i8, __VA_ARGS__)

#define ST_H(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
#define ST_UH(...) ST_H(v8u16, __VA_ARGS__)
#define ST_SH(...) ST_H(v8i16, __VA_ARGS__)

#define ST_W(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
#define ST_UW(...) ST_W(v4u32, __VA_ARGS__)
#define ST_SW(...) ST_W(v4i32, __VA_ARGS__)
#define ST_SP(...) ST_W(v4f32, __VA_ARGS__)

#define ST_D(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
#define ST_DP(...) ST_D(v2f64, __VA_ARGS__)

#if (__mips_isa_rev >= 6)
  #define LH(psrc) ( {                                    \
    unsigned char *psrc_lh_m = (unsigned char *) (psrc);  \
    unsigned short val_m;                                 \
                                                          \
    asm volatile (                                        \
        "lh  %[val_m],  %[psrc_lh_m]  \n\t"               \
                                                          \
        : [val_m] "=r" (val_m)                            \
        : [psrc_lh_m] "m" (*psrc_lh_m)                    \
    );                                                    \
                                                          \
    val_m;                                                \
  } )

  #define LW(psrc) ( {                                    \
    unsigned char *psrc_lw_m = (unsigned char *) (psrc);  \
    unsigned int val_m;                                   \
                                                          \
    asm volatile (                                        \
        "lw  %[val_m],  %[psrc_lw_m]  \n\t"               \
                                                          \
        : [val_m] "=r" (val_m)                            \
        : [psrc_lw_m] "m" (*psrc_lw_m)                    \
    );                                                    \
                                                          \
    val_m;                                                \
  } )

  #if (__mips == 64)
    #define LD(psrc)( {                                     \
      unsigned char *psrc_ld_m = (unsigned char *) (psrc);  \
      unsigned long long val_m = 0;                         \
                                                            \
      asm volatile (                                        \
          "ld  %[val_m],  %[psrc_ld_m]  \n\t"               \
                                                            \
          : [val_m] "=r" (val_m)                            \
          : [psrc_ld_m] "m" (*psrc_ld_m)                    \
      );                                                    \
                                                            \
      val_m;                                                \
    } )
  #else  // !(__mips == 64)
    #define LD(psrc) ( {                                                   \
      unsigned char *psrc_ld_m = (unsigned char *) (psrc);                 \
      unsigned int val0_m, val1_m;                                         \
      unsigned long long val_m = 0;                                        \
                                                                           \
      val0_m = LW(psrc_ld_m);                                              \
      val1_m = LW(psrc_ld_m + 4);                                          \
                                                                           \
      val_m = (unsigned long long) (val1_m);                               \
      val_m = (unsigned long long) ((val_m << 32) & 0xFFFFFFFF00000000);   \
      val_m = (unsigned long long) (val_m | (unsigned long long) val0_m);  \
                                                                           \
      val_m;                                                               \
    } )
  #endif  // (__mips == 64)

  #define SH(val, pdst) {                                 \
    unsigned char *pdst_sh_m = (unsigned char *) (pdst);  \
    unsigned short val_m = (val);                         \
                                                          \
    asm volatile (                                        \
        "sh  %[val_m],  %[pdst_sh_m]  \n\t"               \
                                                          \
        : [pdst_sh_m] "=m" (*pdst_sh_m)                   \
        : [val_m] "r" (val_m)                             \
    );                                                    \
  }

  #define SW(val, pdst) {                                 \
    unsigned char *pdst_sw_m = (unsigned char *) (pdst);  \
    unsigned int val_m = (val);                           \
                                                          \
    asm volatile (                                        \
        "sw  %[val_m],  %[pdst_sw_m]  \n\t"               \
                                                          \
        : [pdst_sw_m] "=m" (*pdst_sw_m)                   \
        : [val_m] "r" (val_m)                             \
    );                                                    \
  }

  #define SD(val, pdst) {                                 \
    unsigned char *pdst_sd_m = (unsigned char *) (pdst);  \
    unsigned long long val_m = (val);                     \
                                                          \
    asm volatile (                                        \
        "sd  %[val_m],  %[pdst_sd_m]  \n\t"               \
                                                          \
        : [pdst_sd_m] "=m" (*pdst_sd_m)                   \
        : [val_m] "r" (val_m)                             \
    );                                                    \
  }

  #define SW_ZERO(pdst) {                              \
    unsigned char *pdst_m = (unsigned char *) (pdst);  \
                                                       \
    asm volatile (                                     \
        "sw  $0,  %[pdst_m]  \n\t"                     \
                                                       \
        : [pdst_m] "=m" (*pdst_m)                      \
        :                                              \
    );                                                 \
  }
#else  // !(__mips_isa_rev >= 6)
  #define LH(psrc) ( {                                    \
    unsigned char *psrc_lh_m = (unsigned char *) (psrc);  \
    unsigned short val_m;                                 \
                                                          \
    asm volatile (                                        \
        "ulh  %[val_m],  %[psrc_lh_m]  \n\t"              \
                                                          \
        : [val_m] "=r" (val_m)                            \
        : [psrc_lh_m] "m" (*psrc_lh_m)                    \
    );                                                    \
                                                          \
    val_m;                                                \
  } )

  #define LW(psrc) ( {                                    \
    unsigned char *psrc_lw_m = (unsigned char *) (psrc);  \
    unsigned int val_m;                                   \
                                                          \
    asm volatile (                                        \
        "ulw  %[val_m],  %[psrc_lw_m]  \n\t"              \
                                                          \
        : [val_m] "=r" (val_m)                            \
        : [psrc_lw_m] "m" (*psrc_lw_m)                    \
    );                                                    \
                                                          \
    val_m;                                                \
  } )

  #if (__mips == 64)
    #define LD(psrc) ( {                                    \
      unsigned char *psrc_ld_m = (unsigned char *) (psrc);  \
      unsigned long long val_m = 0;                         \
                                                            \
      asm volatile (                                        \
          "uld  %[val_m],  %[psrc_ld_m]  \n\t"              \
                                                            \
          : [val_m] "=r" (val_m)                            \
          : [psrc_ld_m] "m" (*psrc_ld_m)                    \
      );                                                    \
                                                            \
      val_m;                                                \
    } )
  #else  // !(__mips == 64)
    #define LD(psrc) ( {                                                   \
      unsigned char *psrc_ld_m = (unsigned char *) (psrc);                 \
      unsigned int val0_m, val1_m;                                         \
      unsigned long long val_m = 0;                                        \
                                                                           \
      val0_m = LW(psrc_ld_m);                                              \
      val1_m = LW(psrc_ld_m + 4);                                          \
                                                                           \
      val_m = (unsigned long long) (val1_m);                               \
      val_m = (unsigned long long) ((val_m << 32) & 0xFFFFFFFF00000000);   \
      val_m = (unsigned long long) (val_m | (unsigned long long) val0_m);  \
                                                                           \
      val_m;                                                               \
    } )
  #endif  // (__mips == 64)

  #define SH(val, pdst) {                                 \
    unsigned char *pdst_sh_m = (unsigned char *) (pdst);  \
    unsigned short val_m = (val);                         \
                                                          \
    asm volatile (                                        \
        "ush  %[val_m],  %[pdst_sh_m]  \n\t"              \
                                                          \
        : [pdst_sh_m] "=m" (*pdst_sh_m)                   \
        : [val_m] "r" (val_m)                             \
    );                                                    \
  }

  #define SW(val, pdst) {                                 \
    unsigned char *pdst_sw_m = (unsigned char *) (pdst);  \
    unsigned int val_m = (val);                           \
                                                          \
    asm volatile (                                        \
        "usw  %[val_m],  %[pdst_sw_m]  \n\t"              \
                                                          \
        : [pdst_sw_m] "=m" (*pdst_sw_m)                   \
        : [val_m] "r" (val_m)                             \
    );                                                    \
  }

  #define SD(val, pdst) {                                          \
    unsigned char *pdst_sd_m = (unsigned char *) (pdst);           \
    unsigned int val0_m, val1_m;                                   \
                                                                   \
    val0_m = (unsigned int) ((val) & 0x00000000FFFFFFFF);          \
    val1_m = (unsigned int) (((val) >> 32) & 0x00000000FFFFFFFF);  \
                                                                   \
    SW(val0_m, pdst_sd_m);                                         \
    SW(val1_m, pdst_sd_m + 4);                                     \
  }

  #define SW_ZERO(pdst) {                              \
    unsigned char *pdst_m = (unsigned char *) (pdst);  \
                                                       \
    asm volatile (                                     \
        "usw  $0,  %[pdst_m]  \n\t"                    \
                                                       \
        : [pdst_m] "=m" (*pdst_m)                      \
        :                                              \
    );                                                 \
  }
#endif  // (__mips_isa_rev >= 6)

/* Description : Load 4 words with stride
   Arguments   : Inputs  - psrc, stride
                 Outputs - out0, out1, out2, out3
   Details     : Load word in 'out0' from (psrc)
                 Load word in 'out1' from (psrc + stride)
                 Load word in 'out2' from (psrc + 2 * stride)
                 Load word in 'out3' from (psrc + 3 * stride)
*/
#define LW4(psrc, stride, out0, out1, out2, out3) {  \
  out0 = LW((psrc));                                 \
  out1 = LW((psrc) + stride);                        \
  out2 = LW((psrc) + 2 * stride);                    \
  out3 = LW((psrc) + 3 * stride);                    \
}

/* Description : Load double words with stride
   Arguments   : Inputs  - psrc, stride
                 Outputs - out0, out1
   Details     : Load double word in 'out0' from (psrc)
                 Load double word in 'out1' from (psrc + stride)
*/
#define LD2(psrc, stride, out0, out1) {  \
  out0 = LD((psrc));                     \
  out1 = LD((psrc) + stride);            \
}
#define LD4(psrc, stride, out0, out1, out2, out3) {  \
  LD2((psrc), stride, out0, out1);                   \
  LD2((psrc) + 2 * stride, stride, out2, out3);      \
}

/* Description : Store 4 words with stride
   Arguments   : Inputs - in0, in1, in2, in3, pdst, stride
   Details     : Store word from 'in0' to (pdst)
                 Store word from 'in1' to (pdst + stride)
                 Store word from 'in2' to (pdst + 2 * stride)
                 Store word from 'in3' to (pdst + 3 * stride)
*/
#define SW4(in0, in1, in2, in3, pdst, stride) {  \
  SW(in0, (pdst));                               \
  SW(in1, (pdst) + stride);                      \
  SW(in2, (pdst) + 2 * stride);                  \
  SW(in3, (pdst) + 3 * stride);                  \
}

/* Description : Store 4 double words with stride
   Arguments   : Inputs - in0, in1, in2, in3, pdst, stride
   Details     : Store double word from 'in0' to (pdst)
                 Store double word from 'in1' to (pdst + stride)
                 Store double word from 'in2' to (pdst + 2 * stride)
                 Store double word from 'in3' to (pdst + 3 * stride)
*/
#define SD4(in0, in1, in2, in3, pdst, stride) {  \
  SD(in0, (pdst));                               \
  SD(in1, (pdst) + stride);                      \
  SD(in2, (pdst) + 2 * stride);                  \
  SD(in3, (pdst) + 3 * stride);                  \
}

/* Description : Load vectors with 16 byte elements with stride
   Arguments   : Inputs  - psrc, stride
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Load 16 byte elements in 'out0' from (psrc)
                 Load 16 byte elements in 'out1' from (psrc + stride)
*/
#define LD_B2(RTYPE, psrc, stride, out0, out1) {  \
  out0 = LD_B(RTYPE, (psrc));                     \
  out1 = LD_B(RTYPE, (psrc) + stride);            \
}
#define LD_UB2(...) LD_B2(v16u8, __VA_ARGS__)
#define LD_SB2(...) LD_B2(v16i8, __VA_ARGS__)

#define LD_B3(RTYPE, psrc, stride, out0, out1, out2) {  \
  LD_B2(RTYPE, (psrc), stride, out0, out1);             \
  out2 = LD_B(RTYPE, (psrc) + 2 * stride);              \
}
#define LD_UB3(...) LD_B3(v16u8, __VA_ARGS__)
#define LD_SB3(...) LD_B3(v16i8, __VA_ARGS__)

#define LD_B4(RTYPE, psrc, stride, out0, out1, out2, out3) {  \
  LD_B2(RTYPE, (psrc), stride, out0, out1);                   \
  LD_B2(RTYPE, (psrc) + 2 * stride , stride, out2, out3);     \
}
#define LD_UB4(...) LD_B4(v16u8, __VA_ARGS__)
#define LD_SB4(...) LD_B4(v16i8, __VA_ARGS__)

#define LD_B5(RTYPE, psrc, stride, out0, out1, out2, out3, out4) {  \
  LD_B4(RTYPE, (psrc), stride, out0, out1, out2, out3);             \
  out4 = LD_B(RTYPE, (psrc) + 4 * stride);                          \
}
#define LD_UB5(...) LD_B5(v16u8, __VA_ARGS__)
#define LD_SB5(...) LD_B5(v16i8, __VA_ARGS__)

#define LD_B6(RTYPE, psrc, stride, out0, out1, out2, out3, out4, out5) {  \
  LD_B4(RTYPE, (psrc), stride, out0, out1, out2, out3);                   \
  LD_B2(RTYPE, (psrc) + 4 * stride, stride, out4, out5);                  \
}
#define LD_UB6(...) LD_B6(v16u8, __VA_ARGS__)
#define LD_SB6(...) LD_B6(v16i8, __VA_ARGS__)

#define LD_B7(RTYPE, psrc, stride,                             \
              out0, out1, out2, out3, out4, out5, out6) {      \
  LD_B5(RTYPE, (psrc), stride, out0, out1, out2, out3, out4);  \
  LD_B2(RTYPE, (psrc) + 5 * stride, stride, out5, out6);       \
}
#define LD_UB7(...) LD_B7(v16u8, __VA_ARGS__)
#define LD_SB7(...) LD_B7(v16i8, __VA_ARGS__)

#define LD_B8(RTYPE, psrc, stride,                                    \
              out0, out1, out2, out3, out4, out5, out6, out7) {       \
  LD_B4(RTYPE, (psrc), stride, out0, out1, out2, out3);               \
  LD_B4(RTYPE, (psrc) + 4 * stride, stride, out4, out5, out6, out7);  \
}
#define LD_UB8(...) LD_B8(v16u8, __VA_ARGS__)
#define LD_SB8(...) LD_B8(v16i8, __VA_ARGS__)

/* Description : Load vectors with 8 halfword elements with stride
   Arguments   : Inputs  - psrc, stride
                 Outputs - out0, out1
   Details     : Load 8 halfword elements in 'out0' from (psrc)
                 Load 8 halfword elements in 'out1' from (psrc + stride)
*/
#define LD_H2(RTYPE, psrc, stride, out0, out1) {  \
  out0 = LD_H(RTYPE, (psrc));                     \
  out1 = LD_H(RTYPE, (psrc) + (stride));          \
}
#define LD_UH2(...) LD_H2(v8u16, __VA_ARGS__)
#define LD_SH2(...) LD_H2(v8i16, __VA_ARGS__)

#define LD_H4(RTYPE, psrc, stride, out0, out1, out2, out3) {  \
  LD_H2(RTYPE, (psrc), stride, out0, out1);                   \
  LD_H2(RTYPE, (psrc) + 2 * stride, stride, out2, out3);      \
}
#define LD_UH4(...) LD_H4(v8u16, __VA_ARGS__)
#define LD_SH4(...) LD_H4(v8i16, __VA_ARGS__)

#define LD_H6(RTYPE, psrc, stride, out0, out1, out2, out3, out4, out5) {  \
  LD_H4(RTYPE, (psrc), stride, out0, out1, out2, out3);                   \
  LD_H2(RTYPE, (psrc) + 4 * stride, stride, out4, out5);                  \
}
#define LD_UH6(...) LD_H6(v8u16, __VA_ARGS__)
#define LD_SH6(...) LD_H6(v8i16, __VA_ARGS__)

#define LD_H8(RTYPE, psrc, stride,                                    \
              out0, out1, out2, out3, out4, out5, out6, out7) {       \
  LD_H4(RTYPE, (psrc), stride, out0, out1, out2, out3);               \
  LD_H4(RTYPE, (psrc) + 4 * stride, stride, out4, out5, out6, out7);  \
}
#define LD_UH8(...) LD_H8(v8u16, __VA_ARGS__)
#define LD_SH8(...) LD_H8(v8i16, __VA_ARGS__)

#define LD_H16(RTYPE, psrc, stride,                                     \
               out0, out1, out2, out3, out4, out5, out6, out7,          \
               out8, out9, out10, out11, out12, out13, out14, out15) {  \
  LD_H8(RTYPE, (psrc), stride,                                          \
        out0, out1, out2, out3, out4, out5, out6, out7);                \
  LD_H8(RTYPE, (psrc) + 8 * stride, stride,                             \
        out8, out9, out10, out11, out12, out13, out14, out15);          \
}
#define LD_SH16(...) LD_H16(v8i16, __VA_ARGS__)

/* Description : Load 4x4 block of signed halfword elements from 1D source
                 data into 4 vectors (Each vector with 4 signed halfwords)
   Arguments   : Input   - psrc
                 Outputs - out0, out1, out2, out3
*/
#define LD4x4_SH(psrc, out0, out1, out2, out3) {            \
  out0 = LD_SH(psrc);                                       \
  out2 = LD_SH(psrc + 8);                                   \
  out1 = (v8i16) __msa_ilvl_d((v2i64) out0, (v2i64) out0);  \
  out3 = (v8i16) __msa_ilvl_d((v2i64) out2, (v2i64) out2);  \
}

/* Description : Load 2 vectors of signed word elements with stride
   Arguments   : Inputs  - psrc, stride
                 Outputs - out0, out1
                 Return Type - signed word
*/
#define LD_SW2(psrc, stride, out0, out1) {  \
  out0 = LD_SW((psrc));                     \
  out1 = LD_SW((psrc) + stride);            \
}

/* Description : Store vectors of 16 byte elements with stride
   Arguments   : Inputs - in0, in1, pdst, stride
   Details     : Store 16 byte elements from 'in0' to (pdst)
                 Store 16 byte elements from 'in1' to (pdst + stride)
*/
#define ST_B2(RTYPE, in0, in1, pdst, stride) {  \
  ST_B(RTYPE, in0, (pdst));                     \
  ST_B(RTYPE, in1, (pdst) + stride);            \
}
#define ST_UB2(...) ST_B2(v16u8, __VA_ARGS__)
#define ST_SB2(...) ST_B2(v16i8, __VA_ARGS__)

#define ST_B4(RTYPE, in0, in1, in2, in3, pdst, stride) {  \
  ST_B2(RTYPE, in0, in1, (pdst), stride);                 \
  ST_B2(RTYPE, in2, in3, (pdst) + 2 * stride, stride);    \
}
#define ST_UB4(...) ST_B4(v16u8, __VA_ARGS__)
#define ST_SB4(...) ST_B4(v16i8, __VA_ARGS__)

#define ST_B8(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,      \
              pdst, stride) {                                     \
  ST_B4(RTYPE, in0, in1, in2, in3, pdst, stride);                 \
  ST_B4(RTYPE, in4, in5, in6, in7, (pdst) + 4 * stride, stride);  \
}
#define ST_UB8(...) ST_B8(v16u8, __VA_ARGS__)

/* Description : Store vectors of 8 halfword elements with stride
   Arguments   : Inputs - in0, in1, pdst, stride
   Details     : Store 8 halfword elements from 'in0' to (pdst)
                 Store 8 halfword elements from 'in1' to (pdst + stride)
*/
#define ST_H2(RTYPE, in0, in1, pdst, stride) {  \
  ST_H(RTYPE, in0, (pdst));                     \
  ST_H(RTYPE, in1, (pdst) + stride);            \
}
#define ST_UH2(...) ST_H2(v8u16, __VA_ARGS__)
#define ST_SH2(...) ST_H2(v8i16, __VA_ARGS__)

#define ST_H3(RTYPE, in0, in1, in2, pdst, stride) {  \
  ST_H2(RTYPE, in0, in1, (pdst), stride);            \
  ST_H(RTYPE, in2, (pdst) + 2 * stride);             \
}
#define ST_SH3(...) ST_H3(v8i16, __VA_ARGS__)

#define ST_H4(RTYPE, in0, in1, in2, in3, pdst, stride) {  \
  ST_H2(RTYPE, in0, in1, (pdst), stride);                 \
  ST_H2(RTYPE, in2, in3, (pdst) + 2 * stride, stride);    \
}
#define ST_SH4(...) ST_H4(v8i16, __VA_ARGS__)

#define ST_H6(RTYPE, in0, in1, in2, in3, in4, in5, pdst, stride) {  \
  ST_H4(RTYPE, in0, in1, in2, in3, (pdst), stride);                 \
  ST_H2(RTYPE, in4, in5, (pdst) + 4 * stride, stride);              \
}
#define ST_SH6(...) ST_H6(v8i16, __VA_ARGS__)

#define ST_H8(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7, pdst, stride) {  \
  ST_H4(RTYPE, in0, in1, in2, in3, (pdst), stride);                           \
  ST_H4(RTYPE, in4, in5, in6, in7, (pdst) + 4 * stride, stride);              \
}
#define ST_SH8(...) ST_H8(v8i16, __VA_ARGS__)

/* Description : Store vectors of word elements with stride
   Arguments   : Inputs - in0, in1, pdst, stride
   Details     : Store 4 word elements from 'in0' to (pdst)
                 Store 4 word elements from 'in1' to (pdst + stride)
*/
#define ST_SW2(in0, in1, pdst, stride) {  \
  ST_SW(in0, (pdst));                     \
  ST_SW(in1, (pdst) + stride);            \
}
#define ST_SW8(in0, in1, in2, in3, in4, in5, in6, in7,  \
               pdst, stride) {                          \
  ST_SW2(in0, in1, (pdst), stride);                     \
  ST_SW2(in2, in3, (pdst) + 2 * stride, stride);        \
  ST_SW2(in4, in5, (pdst) + 4 * stride, stride);        \
  ST_SW2(in6, in7, (pdst) + 6 * stride, stride);        \
}

/* Description : Store 2x4 byte block to destination memory from input vector
   Arguments   : Inputs - in, stidx, pdst, stride
   Details     : Index 'stidx' halfword element from 'in' vector is copied to
                 the GP register and stored to (pdst)
                 Index 'stidx+1' halfword element from 'in' vector is copied to
                 the GP register and stored to (pdst + stride)
                 Index 'stidx+2' halfword element from 'in' vector is copied to
                 the GP register and stored to (pdst + 2 * stride)
                 Index 'stidx+3' halfword element from 'in' vector is copied to
                 the GP register and stored to (pdst + 3 * stride)
*/
#define ST2x4_UB(in, stidx, pdst, stride) {              \
  unsigned short out0_m, out1_m, out2_m, out3_m;         \
  unsigned char *pblk_2x4_m = (unsigned char *) (pdst);  \
                                                         \
  out0_m = __msa_copy_u_h((v8i16) in, (stidx));          \
  out1_m = __msa_copy_u_h((v8i16) in, (stidx + 1));      \
  out2_m = __msa_copy_u_h((v8i16) in, (stidx + 2));      \
  out3_m = __msa_copy_u_h((v8i16) in, (stidx + 3));      \
                                                         \
  SH(out0_m, pblk_2x4_m);                                \
  SH(out1_m, pblk_2x4_m + stride);                       \
  SH(out2_m, pblk_2x4_m + 2 * stride);                   \
  SH(out3_m, pblk_2x4_m + 3 * stride);                   \
}

/* Description : Store 4x2 byte block to destination memory from input vector
   Arguments   : Inputs - in, pdst, stride
   Details     : Index 0 word element from 'in' vector is copied to the GP
                 register and stored to (pdst)
                 Index 1 word element from 'in' vector is copied to the GP
                 register and stored to (pdst + stride)
*/
#define ST4x2_UB(in, pdst, stride) {                     \
  unsigned int out0_m, out1_m;                           \
  unsigned char *pblk_4x2_m = (unsigned char *) (pdst);  \
                                                         \
  out0_m = __msa_copy_u_w((v4i32) in, 0);                \
  out1_m = __msa_copy_u_w((v4i32) in, 1);                \
                                                         \
  SW(out0_m, pblk_4x2_m);                                \
  SW(out1_m, pblk_4x2_m + stride);                       \
}

/* Description : Store 4x4 byte block to destination memory from input vector
   Arguments   : Inputs - in0, in1, pdst, stride
   Details     : 'Idx0' word element from input vector 'in0' is copied to the
                 GP register and stored to (pdst)
                 'Idx1' word element from input vector 'in0' is copied to the
                 GP register and stored to (pdst + stride)
                 'Idx2' word element from input vector 'in0' is copied to the
                 GP register and stored to (pdst + 2 * stride)
                 'Idx3' word element from input vector 'in0' is copied to the
                 GP register and stored to (pdst + 3 * stride)
*/
#define ST4x4_UB(in0, in1, idx0, idx1, idx2, idx3, pdst, stride) {  \
  unsigned int out0_m, out1_m, out2_m, out3_m;                      \
  unsigned char *pblk_4x4_m = (unsigned char *) (pdst);             \
                                                                    \
  out0_m = __msa_copy_u_w((v4i32) in0, idx0);                       \
  out1_m = __msa_copy_u_w((v4i32) in0, idx1);                       \
  out2_m = __msa_copy_u_w((v4i32) in1, idx2);                       \
  out3_m = __msa_copy_u_w((v4i32) in1, idx3);                       \
                                                                    \
  SW4(out0_m, out1_m, out2_m, out3_m, pblk_4x4_m, stride);          \
}
#define ST4x8_UB(in0, in1, pdst, stride) {                        \
  unsigned char *pblk_4x8 = (unsigned char *) (pdst);             \
                                                                  \
  ST4x4_UB(in0, in0, 0, 1, 2, 3, pblk_4x8, stride);               \
  ST4x4_UB(in1, in1, 0, 1, 2, 3, pblk_4x8 + 4 * stride, stride);  \
}

/* Description : Store 6x4 byte block to destination memory from input
                 vectors
   Arguments   : Inputs  - in0, in1, pdst, stride
   Details     : Index 0 word element and Index 2 half word element from input
                 vector 'in0' is copied to the GP registers and stored to (pdst)
                 Index 2 word element and Index 6 half word element from input
                 vector 'in0' is copied to the GP registers and stored to
                 (pdst + stride)
                 Index 0 word element and Index 2 half word element from input
                 vector 'in1' is copied to the GP registers and stored to
                 (pdst + 2 * stride)
                 Index 2 word element and Index 6 half word element from input
                 vector 'in1' is copied to the GP registers and stored to
                 (pdst + 3 * stride)
*/
#define ST6x4_UB(in0, in1, pdst, stride) {               \
  unsigned int out0_m, out1_m, out2_m, out3_m;           \
  unsigned short out4_m, out5_m, out6_m, out7_m;         \
  unsigned char *pblk_6x4_m = (unsigned char *) (pdst);  \
                                                         \
  out0_m = __msa_copy_u_w((v4i32) in0, 0);               \
  out1_m = __msa_copy_u_w((v4i32) in0, 2);               \
  out2_m = __msa_copy_u_w((v4i32) in1, 0);               \
  out3_m = __msa_copy_u_w((v4i32) in1, 2);               \
                                                         \
  out4_m = __msa_copy_u_h((v8i16) in0, 2);               \
  out5_m = __msa_copy_u_h((v8i16) in0, 6);               \
  out6_m = __msa_copy_u_h((v8i16) in1, 2);               \
  out7_m = __msa_copy_u_h((v8i16) in1, 6);               \
                                                         \
  SW(out0_m, pblk_6x4_m);                                \
  SH(out4_m, (pblk_6x4_m + 4));                          \
  pblk_6x4_m += stride;                                  \
  SW(out1_m, pblk_6x4_m);                                \
  SH(out5_m, (pblk_6x4_m + 4));                          \
  pblk_6x4_m += stride;                                  \
  SW(out2_m, pblk_6x4_m);                                \
  SH(out6_m, (pblk_6x4_m + 4));                          \
  pblk_6x4_m += stride;                                  \
  SW(out3_m, pblk_6x4_m);                                \
  SH(out7_m, (pblk_6x4_m + 4));                          \
}

/* Description : Store 8x1 byte block to destination memory from input vector
   Arguments   : Inputs - in, pdst
   Details     : Index 0 double word element from 'in' vector is copied to the
                 GP register and stored to (pdst)
*/
#define ST8x1_UB(in, pdst) {               \
  unsigned long long out0_m;               \
  out0_m = __msa_copy_u_d((v2i64) in, 0);  \
  SD(out0_m, pdst);                        \
}

/* Description : Store 8x2 byte block to destination memory from input vector
   Arguments   : Inputs - in, pdst, stride
   Details     : Index 0 double word element from 'in' vector is copied to the
                 GP register and stored to (pdst)
                 Index 1 double word element from 'in' vector is copied to the
                 GP register and stored to (pdst + stride)
*/
#define ST8x2_UB(in, pdst, stride) {                     \
  unsigned long long out0_m, out1_m;                     \
  unsigned char *pblk_8x2_m = (unsigned char *) (pdst);  \
                                                         \
  out0_m = __msa_copy_u_d((v2i64) in, 0);                \
  out1_m = __msa_copy_u_d((v2i64) in, 1);                \
                                                         \
  SD(out0_m, pblk_8x2_m);                                \
  SD(out1_m, pblk_8x2_m + stride);                       \
}

/* Description : average with rounding (in0 + in1 + 1) / 2.
   Arguments   : Inputs  - in0, in1, in2, in3,
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Each unsigned byte element from 'in0' vector is added with
                 each unsigned byte element from 'in1' vector. Then the average
                 with rounding is calculated and written to 'out0'
*/
#define AVER_UB2(RTYPE, in0, in1, in2, in3, out0, out1) {   \
  out0 = (RTYPE) __msa_aver_u_b((v16u8) in0, (v16u8) in1);  \
  out1 = (RTYPE) __msa_aver_u_b((v16u8) in2, (v16u8) in3);  \
}
#define AVER_UB2_UB(...) AVER_UB2(v16u8, __VA_ARGS__)

#define AVER_UB4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  AVER_UB2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  AVER_UB2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define AVER_UB4_UB(...) AVER_UB4(v16u8, __VA_ARGS__)

/* Description : Immediate number of elements to slide with zero
   Arguments   : Inputs  - in0, in1, slide_val
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Byte elements from 'zero_m' vector are slid into 'in0' by
                 value specified in the 'slide_val'
*/
#define SLDI_B2_0(RTYPE, in0, in1, out0, out1, slide_val) {             \
  v16i8 zero_m = { 0 };                                                 \
  out0 = (RTYPE) __msa_sldi_b((v16i8) zero_m, (v16i8) in0, slide_val);  \
  out1 = (RTYPE) __msa_sldi_b((v16i8) zero_m, (v16i8) in1, slide_val);  \
}
#define SLDI_B2_0_UB(...) SLDI_B2_0(v16u8, __VA_ARGS__)
#define SLDI_B2_0_SB(...) SLDI_B2_0(v16i8, __VA_ARGS__)
#define SLDI_B2_0_SW(...) SLDI_B2_0(v4i32, __VA_ARGS__)

#define SLDI_B3_0(RTYPE, in0, in1, in2, out0, out1, out2,  slide_val) {  \
  v16i8 zero_m = { 0 };                                                  \
  SLDI_B2_0(RTYPE, in0, in1, out0, out1, slide_val);                     \
  out2 = (RTYPE) __msa_sldi_b((v16i8) zero_m, (v16i8) in2, slide_val);   \
}
#define SLDI_B3_0_UB(...) SLDI_B3_0(v16u8, __VA_ARGS__)
#define SLDI_B3_0_SB(...) SLDI_B3_0(v16i8, __VA_ARGS__)

#define SLDI_B4_0(RTYPE, in0, in1, in2, in3,            \
                  out0, out1, out2, out3, slide_val) {  \
  SLDI_B2_0(RTYPE, in0, in1, out0, out1, slide_val);    \
  SLDI_B2_0(RTYPE, in2, in3, out2, out3, slide_val);    \
}
#define SLDI_B4_0_UB(...) SLDI_B4_0(v16u8, __VA_ARGS__)
#define SLDI_B4_0_SB(...) SLDI_B4_0(v16i8, __VA_ARGS__)
#define SLDI_B4_0_SH(...) SLDI_B4_0(v8i16, __VA_ARGS__)

/* Description : Immediate number of elements to slide
   Arguments   : Inputs  - in0_0, in0_1, in1_0, in1_1, slide_val
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Byte elements from 'in0_0' vector are slid into 'in1_0' by
                 value specified in the 'slide_val'
*/
#define SLDI_B2(RTYPE, in0_0, in0_1, in1_0, in1_1, out0, out1, slide_val) {  \
  out0 = (RTYPE) __msa_sldi_b((v16i8) in0_0, (v16i8) in1_0, slide_val);      \
  out1 = (RTYPE) __msa_sldi_b((v16i8) in0_1, (v16i8) in1_1, slide_val);      \
}
#define SLDI_B2_UB(...) SLDI_B2(v16u8, __VA_ARGS__)
#define SLDI_B2_SB(...) SLDI_B2(v16i8, __VA_ARGS__)
#define SLDI_B2_SH(...) SLDI_B2(v8i16, __VA_ARGS__)

#define SLDI_B3(RTYPE, in0_0, in0_1, in0_2, in1_0, in1_1, in1_2,         \
                out0, out1, out2, slide_val) {                           \
  SLDI_B2(RTYPE, in0_0, in0_1, in1_0, in1_1, out0, out1, slide_val)      \
  out2 = (RTYPE) __msa_sldi_b((v16i8) in0_2, (v16i8) in1_2, slide_val);  \
}
#define SLDI_B3_SB(...) SLDI_B3(v16i8, __VA_ARGS__)
#define SLDI_B3_UH(...) SLDI_B3(v8u16, __VA_ARGS__)

/* Description : Shuffle byte vector elements as per mask vector
   Arguments   : Inputs  - in0, in1, in2, in3, mask0, mask1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Byte elements from 'in0' & 'in1' are copied selectively to
                 'out0' as per control vector 'mask0'
*/
#define VSHF_B2(RTYPE, in0, in1, in2, in3, mask0, mask1, out0, out1) {   \
  out0 = (RTYPE) __msa_vshf_b((v16i8) mask0, (v16i8) in1, (v16i8) in0);  \
  out1 = (RTYPE) __msa_vshf_b((v16i8) mask1, (v16i8) in3, (v16i8) in2);  \
}
#define VSHF_B2_UB(...) VSHF_B2(v16u8, __VA_ARGS__)
#define VSHF_B2_SB(...) VSHF_B2(v16i8, __VA_ARGS__)
#define VSHF_B2_UH(...) VSHF_B2(v8u16, __VA_ARGS__)
#define VSHF_B2_SH(...) VSHF_B2(v8i16, __VA_ARGS__)

#define VSHF_B3(RTYPE, in0, in1, in2, in3, in4, in5, mask0, mask1, mask2,  \
                out0, out1, out2) {                                        \
  VSHF_B2(RTYPE, in0, in1, in2, in3, mask0, mask1, out0, out1);            \
  out2 = (RTYPE) __msa_vshf_b((v16i8) mask2, (v16i8) in5, (v16i8) in4);    \
}
#define VSHF_B3_SB(...) VSHF_B3(v16i8, __VA_ARGS__)

#define VSHF_B4(RTYPE, in0, in1, mask0, mask1, mask2, mask3,     \
                out0, out1, out2, out3) {                        \
  VSHF_B2(RTYPE, in0, in1, in0, in1, mask0, mask1, out0, out1);  \
  VSHF_B2(RTYPE, in0, in1, in0, in1, mask2, mask3, out2, out3);  \
}
#define VSHF_B4_UB(...) VSHF_B4(v16u8, __VA_ARGS__)
#define VSHF_B4_SB(...) VSHF_B4(v16i8, __VA_ARGS__)
#define VSHF_B4_SH(...) VSHF_B4(v8i16, __VA_ARGS__)

/* Description : Shuffle halfword vector elements as per mask vector
   Arguments   : Inputs  - in0, in1, in2, in3, mask0, mask1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : halfword elements from 'in0' & 'in1' are copied selectively to
                 'out0' as per control vector 'mask0'
*/
#define VSHF_H2(RTYPE, in0, in1, in2, in3, mask0, mask1, out0, out1) {   \
  out0 = (RTYPE) __msa_vshf_h((v8i16) mask0, (v8i16) in1, (v8i16) in0);  \
  out1 = (RTYPE) __msa_vshf_h((v8i16) mask1, (v8i16) in3, (v8i16) in2);  \
}
#define VSHF_H2_SH(...) VSHF_H2(v8i16, __VA_ARGS__)

#define VSHF_H3(RTYPE, in0, in1, in2, in3, in4, in5, mask0, mask1, mask2,  \
                out0, out1, out2) {                                        \
  VSHF_H2(RTYPE, in0, in1, in2, in3, mask0, mask1, out0, out1);            \
  out2 = (RTYPE) __msa_vshf_h((v8i16) mask2, (v8i16) in5, (v8i16) in4);    \
}
#define VSHF_H3_SH(...) VSHF_H3(v8i16, __VA_ARGS__)

/* Description : Shuffle word vector elements as per mask vector
   Arguments   : Inputs  - in0, in1, in2, in3, mask0, mask1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Word elements from 'in0' & 'in1' are copied selectively to
                 'out0' as per control vector 'mask0'
*/
#define VSHF_W2(RTYPE, in0, in1, in2, in3, mask0, mask1, out0, out1) {   \
  out0 = (RTYPE) __msa_vshf_w((v4i32) mask0, (v4i32) in1, (v4i32) in0);  \
  out1 = (RTYPE) __msa_vshf_w((v4i32) mask1, (v4i32) in3, (v4i32) in2);  \
}
#define VSHF_W2_UB(...) VSHF_W2(v16u8, __VA_ARGS__)
#define VSHF_W2_SB(...) VSHF_W2(v16i8, __VA_ARGS__)

/* Description : Dot product of byte vector elements
   Arguments   : Inputs  - mult0, mult1, cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Unsigned byte elements from 'mult0' are multiplied with
                 unsigned byte elements from 'cnst0' producing a result
                 twice the size of input i.e. unsigned halfword.
                 The multiplication result of adjacent odd-even elements
                 are added together and written to the 'out0' vector
*/
#define DOTP_UB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {  \
  out0 = (RTYPE) __msa_dotp_u_h((v16u8) mult0, (v16u8) cnst0);     \
  out1 = (RTYPE) __msa_dotp_u_h((v16u8) mult1, (v16u8) cnst1);     \
}
#define DOTP_UB2_UH(...) DOTP_UB2(v8u16, __VA_ARGS__)

#define DOTP_UB4(RTYPE, mult0, mult1, mult2, mult3,         \
                 cnst0, cnst1, cnst2, cnst3,                \
                 out0, out1, out2, out3) {                  \
  DOTP_UB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);  \
  DOTP_UB2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);  \
}
#define DOTP_UB4_UH(...) DOTP_UB4(v8u16, __VA_ARGS__)

/* Description : Dot product of byte vector elements
   Arguments   : Inputs  - mult0, mult1, cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Signed byte elements from 'mult0' are multiplied with
                 signed byte elements from 'cnst0' producing a result
                 twice the size of input i.e. signed halfword.
                 The multiplication result of adjacent odd-even elements
                 are added together and written to the 'out0' vector
*/
#define DOTP_SB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {  \
  out0 = (RTYPE) __msa_dotp_s_h((v16i8) mult0, (v16i8) cnst0);     \
  out1 = (RTYPE) __msa_dotp_s_h((v16i8) mult1, (v16i8) cnst1);     \
}
#define DOTP_SB2_SH(...) DOTP_SB2(v8i16, __VA_ARGS__)

#define DOTP_SB3(RTYPE, mult0, mult1, mult2, cnst0, cnst1, cnst2,  \
                 out0, out1, out2) {                               \
  DOTP_SB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);         \
  out2 = (RTYPE) __msa_dotp_s_h((v16i8) mult2, (v16i8) cnst2);     \
}
#define DOTP_SB3_SH(...) DOTP_SB3(v8i16, __VA_ARGS__)

#define DOTP_SB4(RTYPE, mult0, mult1, mult2, mult3,                     \
                 cnst0, cnst1, cnst2, cnst3, out0, out1, out2, out3) {  \
  DOTP_SB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);              \
  DOTP_SB2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);              \
}
#define DOTP_SB4_SH(...) DOTP_SB4(v8i16, __VA_ARGS__)

/* Description : Dot product of halfword vector elements
   Arguments   : Inputs  - mult0, mult1, cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Signed halfword elements from 'mult0' are multiplied with
                 signed halfword elements from 'cnst0' producing a result
                 twice the size of input i.e. signed word.
                 The multiplication result of adjacent odd-even elements
                 are added together and written to the 'out0' vector
*/
#define DOTP_SH2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {  \
  out0 = (RTYPE) __msa_dotp_s_w((v8i16) mult0, (v8i16) cnst0);     \
  out1 = (RTYPE) __msa_dotp_s_w((v8i16) mult1, (v8i16) cnst1);     \
}
#define DOTP_SH2_SW(...) DOTP_SH2(v4i32, __VA_ARGS__)

#define DOTP_SH4(RTYPE, mult0, mult1, mult2, mult3,         \
                 cnst0, cnst1, cnst2, cnst3,                \
                 out0, out1, out2, out3) {                  \
  DOTP_SH2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);  \
  DOTP_SH2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);  \
}
#define DOTP_SH4_SW(...) DOTP_SH4(v4i32, __VA_ARGS__)

/* Description : Dot product of word vector elements
   Arguments   : Inputs  - mult0, mult1, cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Signed word elements from 'mult0' are multiplied with
                 signed word elements from 'cnst0' producing a result
                 twice the size of input i.e. signed double word.
                 The multiplication result of adjacent odd-even elements
                 are added together and written to the 'out0' vector
*/
#define DOTP_SW2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {  \
  out0 = (RTYPE) __msa_dotp_s_d((v4i32) mult0, (v4i32) cnst0);     \
  out1 = (RTYPE) __msa_dotp_s_d((v4i32) mult1, (v4i32) cnst1);     \
}
#define DOTP_SW2_SD(...) DOTP_SW2(v2i64, __VA_ARGS__)

/* Description : Dot product & addition of byte vector elements
   Arguments   : Inputs  - mult0, mult1, cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Signed byte elements from 'mult0' are multiplied with
                 signed byte elements from 'cnst0' producing a result
                 twice the size of input i.e. signed halfword.
                 The multiplication result of adjacent odd-even elements
                 are added to the 'out0' vector
*/
#define DPADD_SB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {             \
  out0 = (RTYPE) __msa_dpadd_s_h((v8i16) out0, (v16i8) mult0, (v16i8) cnst0);  \
  out1 = (RTYPE) __msa_dpadd_s_h((v8i16) out1, (v16i8) mult1, (v16i8) cnst1);  \
}
#define DPADD_SB2_SH(...) DPADD_SB2(v8i16, __VA_ARGS__)

#define DPADD_SB4(RTYPE, mult0, mult1, mult2, mult3,                     \
                  cnst0, cnst1, cnst2, cnst3, out0, out1, out2, out3) {  \
  DPADD_SB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);              \
  DPADD_SB2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);              \
}
#define DPADD_SB4_SH(...) DPADD_SB4(v8i16, __VA_ARGS__)

/* Description : Dot product & addition of byte vector elements
   Arguments   : Inputs  - mult0, mult1, cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - unsigned halfword
   Details     : Unsigned byte elements from 'mult0' are multiplied with
                 unsigned byte elements from 'cnst0' producing a result
                 twice the size of input i.e. unsigned halfword.
                 The multiplication result of adjacent odd-even elements
                 are added to the 'out0' vector
*/
#define DPADD_UB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {             \
  out0 = (RTYPE) __msa_dpadd_u_h((v8u16) out0, (v16u8) mult0, (v16u8) cnst0);  \
  out1 = (RTYPE) __msa_dpadd_u_h((v8u16) out1, (v16u8) mult1, (v16u8) cnst1);  \
}
#define DPADD_UB2_UH(...) DPADD_UB2(v8u16, __VA_ARGS__)

#define DPADD_UB4(RTYPE, mult0, mult1, mult2, mult3,                     \
                  cnst0, cnst1, cnst2, cnst3, out0, out1, out2, out3) {  \
  DPADD_UB2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);              \
  DPADD_UB2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);              \
}
#define DPADD_UB4_UH(...) DPADD_UB4(v8u16, __VA_ARGS__)

/* Description : Dot product & addition of halfword vector elements
   Arguments   : Inputs  - mult0, mult1, cnst0, cnst1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Signed halfword elements from 'mult0' are multiplied with
                 signed halfword elements from 'cnst0' producing a result
                 twice the size of input i.e. signed word.
                 The multiplication result of adjacent odd-even elements
                 are added to the 'out0' vector
*/
#define DPADD_SH2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1) {             \
  out0 = (RTYPE) __msa_dpadd_s_w((v4i32) out0, (v8i16) mult0, (v8i16) cnst0);  \
  out1 = (RTYPE) __msa_dpadd_s_w((v4i32) out1, (v8i16) mult1, (v8i16) cnst1);  \
}
#define DPADD_SH2_SW(...) DPADD_SH2(v4i32, __VA_ARGS__)

#define DPADD_SH4(RTYPE, mult0, mult1, mult2, mult3,                     \
                  cnst0, cnst1, cnst2, cnst3, out0, out1, out2, out3) {  \
  DPADD_SH2(RTYPE, mult0, mult1, cnst0, cnst1, out0, out1);              \
  DPADD_SH2(RTYPE, mult2, mult3, cnst2, cnst3, out2, out3);              \
}
#define DPADD_SH4_SW(...) DPADD_SH4(v4i32, __VA_ARGS__)

/* Description : Dot product & addition of double word vector elements
   Arguments   : Inputs  - mult0, mult1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Each signed word element from 'mult0' is multiplied with itself
                 producing a intermediate result twice the size of it
                 i.e. signed double word
                 The multiplication result of adjacent odd-even elements
                 are added to the 'out0' vector
*/
#define DPADD_SD2(RTYPE, mult0, mult1, out0, out1) {                           \
  out0 = (RTYPE) __msa_dpadd_s_d((v2i64) out0, (v4i32) mult0, (v4i32) mult0);  \
  out1 = (RTYPE) __msa_dpadd_s_d((v2i64) out1, (v4i32) mult1, (v4i32) mult1);  \
}
#define DPADD_SD2_SD(...) DPADD_SD2(v2i64, __VA_ARGS__)

/* Description : Minimum values between unsigned elements of
                 either vector are copied to the output vector
   Arguments   : Inputs  - in0, in1, min_vec
                 Outputs - in place operation
                 Return Type - as per RTYPE
   Details     : Minimum of unsigned halfword element values from 'in0' and
                 'min_vec' are written to output vector 'in0'
*/
#define MIN_UH2(RTYPE, in0, in1, min_vec) {           \
  in0 = (RTYPE) __msa_min_u_h((v8u16) in0, min_vec);  \
  in1 = (RTYPE) __msa_min_u_h((v8u16) in1, min_vec);  \
}
#define MIN_UH2_UH(...) MIN_UH2(v8u16, __VA_ARGS__)

#define MIN_UH4(RTYPE, in0, in1, in2, in3, min_vec) {  \
  MIN_UH2(RTYPE, in0, in1, min_vec);                   \
  MIN_UH2(RTYPE, in2, in3, min_vec);                   \
}
#define MIN_UH4_UH(...) MIN_UH4(v8u16, __VA_ARGS__)

/* Description : Clips all halfword elements of input vector between min & max
                 out = (in < min) ? min : ((in > max) ? max : in)
   Arguments   : Inputs - in, min, max
                 Output - out_m
                 Return Type - signed halfword
*/
#define CLIP_SH(in, min, max) ( {                     \
  v8i16 out_m;                                        \
                                                      \
  out_m = __msa_max_s_h((v8i16) min, (v8i16) in);     \
  out_m = __msa_min_s_h((v8i16) max, (v8i16) out_m);  \
  out_m;                                              \
} )

/* Description : Clips all signed halfword elements of input vector
                 between 0 & 255
   Arguments   : Input  - in
                 Output - out_m
                 Return Type - signed halfword
*/
#define CLIP_SH_0_255(in) ( {                           \
  v8i16 max_m = __msa_ldi_h(255);                       \
  v8i16 out_m;                                          \
                                                        \
  out_m = __msa_maxi_s_h((v8i16) in, 0);                \
  out_m = __msa_min_s_h((v8i16) max_m, (v8i16) out_m);  \
  out_m;                                                \
} )
#define CLIP_SH2_0_255(in0, in1) {  \
  in0 = CLIP_SH_0_255(in0);         \
  in1 = CLIP_SH_0_255(in1);         \
}
#define CLIP_SH4_0_255(in0, in1, in2, in3) {  \
  CLIP_SH2_0_255(in0, in1);                   \
  CLIP_SH2_0_255(in2, in3);                   \
}

/* Description : Clips all unsigned halfword elements of input vector
                 between 0 & 255
   Arguments   : Input  - in
                 Output - out_m
                 Return Type - unsigned halfword
*/
#define CLIP_UH_0_255(in) ( {                           \
  v8u16 max_m = __msa_ldi_h(255);                       \
  v8u16 out_m;                                          \
                                                        \
  out_m = __msa_maxi_u_h((v8u16) in, 0);                \
  out_m = __msa_min_u_h((v8u16) max_m, (v8u16) out_m);  \
  out_m;                                                \
} )

/* Description : Clips all signed word elements of input vector
                 between 0 & 255
   Arguments   : Input  - in
                 Output - out_m
                 Return Type - signed word
*/
#define CLIP_SW_0_255(in) ( {                           \
  v4i32 max_m = __msa_ldi_w(255);                       \
  v4i32 out_m;                                          \
                                                        \
  out_m = __msa_maxi_s_w((v4i32) in, 0);                \
  out_m = __msa_min_s_w((v4i32) max_m, (v4i32) out_m);  \
  out_m;                                                \
} )

/* Description : Horizontal addition of 4 signed word elements of input vector
   Arguments   : Input  - in       (signed word vector)
                 Output - sum_m    (i32 sum)
                 Return Type - signed word (GP)
   Details     : 4 signed word elements of 'in' vector are added together and
                 the resulting integer sum is returned
*/
#define HADD_SW_S32(in) ( {                         \
  v2i64 res0_m, res1_m;                             \
  int sum_m;                                        \
                                                    \
  res0_m = __msa_hadd_s_d((v4i32) in, (v4i32) in);  \
  res1_m = __msa_splati_d(res0_m, 1);               \
  res0_m = res0_m + res1_m;                         \
  sum_m = __msa_copy_s_w((v4i32) res0_m, 0);        \
  sum_m;                                            \
} )

/* Description : Horizontal addition of 8 signed halfword elements
   Arguments   : Inputs  - in       (signed halfword vector)
                 Outputs - res_m    (s32 sum)
                 Return Type - unsigned word
   Details     : 8 signed halfword elements of input vector are added
                 together and the resulting integer sum is returned
*/
#define HADD_SH_S32(in) ( {                         \
  v4i32 temp_m;                                     \
  int res_m;                                        \
                                                    \
  temp_m = __msa_hadd_s_w((v8i16) in, (v8i16) in);  \
  res_m = HADD_SW_S32(temp_m);                      \
  res_m;                                            \
} )

/* Description : Horizontal addition of 8 unsigned halfword elements
   Arguments   : Inputs  - in       (unsigned halfword vector)
                 Outputs - sum_m    (u32 sum)
                 Return Type - unsigned word
   Details     : 8 unsigned halfword elements of input vector are added
                 together and the resulting integer sum is returned
*/
#define HADD_UH_U32(in) ( {                            \
  v4u32 res_m;                                         \
  v2u64 res0_m, res1_m;                                \
  unsigned int sum_m;                                  \
                                                       \
  res_m = __msa_hadd_u_w((v8u16) in, (v8u16) in);      \
  res0_m = __msa_hadd_u_d(res_m, res_m);               \
  res1_m = (v2u64) __msa_splati_d((v2i64) res0_m, 1);  \
  res0_m = res0_m + res1_m;                            \
  sum_m = __msa_copy_u_w((v4i32) res0_m, 0);           \
  sum_m;                                               \
} )

/* Description : Horizontal addition of signed byte vector elements
   Arguments   : Inputs  - in0, in1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Each signed odd byte element from 'in0' is added to
                 even signed byte element from 'in0' (pairwise) and the
                 halfword result is written in 'out0'
*/
#define HADD_SB2(RTYPE, in0, in1, out0, out1) {             \
  out0 = (RTYPE) __msa_hadd_s_h((v16i8) in0, (v16i8) in0);  \
  out1 = (RTYPE) __msa_hadd_s_h((v16i8) in1, (v16i8) in1);  \
}
#define HADD_SB2_SH(...) HADD_SB2(v8i16, __VA_ARGS__)

#define HADD_SB4(RTYPE, in0, in1, in2, in3, out0, out1, out2, out3) {  \
  HADD_SB2(RTYPE, in0, in1, out0, out1);                               \
  HADD_SB2(RTYPE, in2, in3, out2, out3);                               \
}
#define HADD_SB4_UH(...) HADD_SB4(v8u16, __VA_ARGS__)
#define HADD_SB4_SH(...) HADD_SB4(v8i16, __VA_ARGS__)

/* Description : Horizontal addition of unsigned byte vector elements
   Arguments   : Inputs  - in0, in1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Each unsigned odd byte element from 'in0' is added to
                 even unsigned byte element from 'in0' (pairwise) and the
                 halfword result is written to 'out0'
*/
#define HADD_UB2(RTYPE, in0, in1, out0, out1) {             \
  out0 = (RTYPE) __msa_hadd_u_h((v16u8) in0, (v16u8) in0);  \
  out1 = (RTYPE) __msa_hadd_u_h((v16u8) in1, (v16u8) in1);  \
}
#define HADD_UB2_UH(...) HADD_UB2(v8u16, __VA_ARGS__)

#define HADD_UB3(RTYPE, in0, in1, in2, out0, out1, out2) {  \
  HADD_UB2(RTYPE, in0, in1, out0, out1);                    \
  out2 = (RTYPE) __msa_hadd_u_h((v16u8) in2, (v16u8) in2);  \
}
#define HADD_UB3_UH(...) HADD_UB3(v8u16, __VA_ARGS__)

#define HADD_UB4(RTYPE, in0, in1, in2, in3, out0, out1, out2, out3) {  \
  HADD_UB2(RTYPE, in0, in1, out0, out1);                               \
  HADD_UB2(RTYPE, in2, in3, out2, out3);                               \
}
#define HADD_UB4_UB(...) HADD_UB4(v16u8, __VA_ARGS__)
#define HADD_UB4_UH(...) HADD_UB4(v8u16, __VA_ARGS__)
#define HADD_UB4_SH(...) HADD_UB4(v8i16, __VA_ARGS__)

/* Description : Horizontal subtraction of unsigned byte vector elements
   Arguments   : Inputs  - in0, in1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Each unsigned odd byte element from 'in0' is subtracted from
                 even unsigned byte element from 'in0' (pairwise) and the
                 halfword result is written to 'out0'
*/
#define HSUB_UB2(RTYPE, in0, in1, out0, out1) {             \
  out0 = (RTYPE) __msa_hsub_u_h((v16u8) in0, (v16u8) in0);  \
  out1 = (RTYPE) __msa_hsub_u_h((v16u8) in1, (v16u8) in1);  \
}
#define HSUB_UB2_UH(...) HSUB_UB2(v8u16, __VA_ARGS__)
#define HSUB_UB2_SH(...) HSUB_UB2(v8i16, __VA_ARGS__)
#define HSUB_UB2_SW(...) HSUB_UB2(v4i32, __VA_ARGS__)

#define HSUB_UB4(RTYPE, in0, in1, in2, in3, out0, out1, out2, out3) {  \
  HSUB_UB2(RTYPE, in0, in1, out0, out1);                               \
  HSUB_UB2(RTYPE, in2, in3, out2, out3);                               \
}
#define HSUB_UB4_UH(...) HSUB_UB4(v8u16, __VA_ARGS__)
#define HSUB_UB4_SH(...) HSUB_UB4(v8i16, __VA_ARGS__)

/* Description : Horizontal subtraction of signed halfword vector elements
   Arguments   : Inputs  - in0, in1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Each signed odd halfword element from 'in0' is subtracted from
                 even signed halfword element from 'in0' (pairwise) and the
                 word result is written to 'out0'
*/
#define HSUB_UH2(RTYPE, in0, in1, out0, out1) {             \
  out0 = (RTYPE) __msa_hsub_s_w((v8i16) in0, (v8i16) in0);  \
  out1 = (RTYPE) __msa_hsub_s_w((v8i16) in1, (v8i16) in1);  \
}
#define HSUB_UH2_SW(...) HSUB_UH2(v4i32, __VA_ARGS__)

/* Description : SAD (Sum of Absolute Difference)
   Arguments   : Inputs  - in0, in1, ref0, ref1
                 Outputs - sad_m                 (halfword vector)
                 Return Type - unsigned halfword
   Details     : Absolute difference of all the byte elements from 'in0' with
                 'ref0' is calculated and preserved in 'diff0'. Then even-odd
                 pairs are added together to generate 8 halfword results.
*/
#define SAD_UB2_UH(in0, in1, ref0, ref1) ( {                  \
  v16u8 diff0_m, diff1_m;                                     \
  v8u16 sad_m = { 0 };                                        \
                                                              \
  diff0_m = __msa_asub_u_b((v16u8) in0, (v16u8) ref0);        \
  diff1_m = __msa_asub_u_b((v16u8) in1, (v16u8) ref1);        \
                                                              \
  sad_m += __msa_hadd_u_h((v16u8) diff0_m, (v16u8) diff0_m);  \
  sad_m += __msa_hadd_u_h((v16u8) diff1_m, (v16u8) diff1_m);  \
                                                              \
  sad_m;                                                      \
} )

/* Description : Set element n input vector to GPR value
   Arguments   : Inputs - in0, in1, in2, in3
                 Output - out
                 Return Type - as per RTYPE
   Details     : Set element 0 in vector 'out' to value specified in 'in0'
*/
#define INSERT_W2(RTYPE, in0, in1, out) {             \
  out = (RTYPE) __msa_insert_w((v4i32) out, 0, in0);  \
  out = (RTYPE) __msa_insert_w((v4i32) out, 1, in1);  \
}
#define INSERT_W2_UB(...) INSERT_W2(v16u8, __VA_ARGS__)
#define INSERT_W2_SB(...) INSERT_W2(v16i8, __VA_ARGS__)

#define INSERT_W4(RTYPE, in0, in1, in2, in3, out) {   \
  out = (RTYPE) __msa_insert_w((v4i32) out, 0, in0);  \
  out = (RTYPE) __msa_insert_w((v4i32) out, 1, in1);  \
  out = (RTYPE) __msa_insert_w((v4i32) out, 2, in2);  \
  out = (RTYPE) __msa_insert_w((v4i32) out, 3, in3);  \
}
#define INSERT_W4_UB(...) INSERT_W4(v16u8, __VA_ARGS__)
#define INSERT_W4_SB(...) INSERT_W4(v16i8, __VA_ARGS__)
#define INSERT_W4_SW(...) INSERT_W4(v4i32, __VA_ARGS__)

#define INSERT_D2(RTYPE, in0, in1, out) {             \
  out = (RTYPE) __msa_insert_d((v2i64) out, 0, in0);  \
  out = (RTYPE) __msa_insert_d((v2i64) out, 1, in1);  \
}
#define INSERT_D2_UB(...) INSERT_D2(v16u8, __VA_ARGS__)
#define INSERT_D2_SB(...) INSERT_D2(v16i8, __VA_ARGS__)
#define INSERT_D2_SH(...) INSERT_D2(v8i16, __VA_ARGS__)
#define INSERT_D2_SD(...) INSERT_D2(v2i64, __VA_ARGS__)

/* Description : Interleave even byte elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even byte elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'
*/
#define ILVEV_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvev_b((v16i8) in1, (v16i8) in0);  \
  out1 = (RTYPE) __msa_ilvev_b((v16i8) in3, (v16i8) in2);  \
}
#define ILVEV_B2_UB(...) ILVEV_B2(v16u8, __VA_ARGS__)
#define ILVEV_B2_SB(...) ILVEV_B2(v16i8, __VA_ARGS__)
#define ILVEV_B2_UH(...) ILVEV_B2(v8u16, __VA_ARGS__)
#define ILVEV_B2_SH(...) ILVEV_B2(v8i16, __VA_ARGS__)
#define ILVEV_B2_SD(...) ILVEV_B2(v2i64, __VA_ARGS__)

/* Description : Interleave even halfword elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even halfword elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'
*/
#define ILVEV_H2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvev_h((v8i16) in1, (v8i16) in0);  \
  out1 = (RTYPE) __msa_ilvev_h((v8i16) in3, (v8i16) in2);  \
}
#define ILVEV_H2_UB(...) ILVEV_H2(v16u8, __VA_ARGS__)
#define ILVEV_H2_UH(...) ILVEV_H2(v8u16, __VA_ARGS__)
#define ILVEV_H2_SH(...) ILVEV_H2(v8i16, __VA_ARGS__)
#define ILVEV_H2_SW(...) ILVEV_H2(v4i32, __VA_ARGS__)

/* Description : Interleave even word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even word elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'
*/
#define ILVEV_W2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvev_w((v4i32) in1, (v4i32) in0);  \
  out1 = (RTYPE) __msa_ilvev_w((v4i32) in3, (v4i32) in2);  \
}
#define ILVEV_W2_UB(...) ILVEV_W2(v16u8, __VA_ARGS__)
#define ILVEV_W2_SB(...) ILVEV_W2(v16i8, __VA_ARGS__)
#define ILVEV_W2_UH(...) ILVEV_W2(v8u16, __VA_ARGS__)
#define ILVEV_W2_SD(...) ILVEV_W2(v2i64, __VA_ARGS__)

/* Description : Interleave even double word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even double word elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'
*/
#define ILVEV_D2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvev_d((v2i64) in1, (v2i64) in0);  \
  out1 = (RTYPE) __msa_ilvev_d((v2i64) in3, (v2i64) in2);  \
}
#define ILVEV_D2_UB(...) ILVEV_D2(v16u8, __VA_ARGS__)
#define ILVEV_D2_SB(...) ILVEV_D2(v16i8, __VA_ARGS__)
#define ILVEV_D2_SW(...) ILVEV_D2(v4i32, __VA_ARGS__)

/* Description : Interleave left half of byte elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Left half of byte elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'.
*/
#define ILVL_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvl_b((v16i8) in0, (v16i8) in1);  \
  out1 = (RTYPE) __msa_ilvl_b((v16i8) in2, (v16i8) in3);  \
}
#define ILVL_B2_UB(...) ILVL_B2(v16u8, __VA_ARGS__)
#define ILVL_B2_SB(...) ILVL_B2(v16i8, __VA_ARGS__)
#define ILVL_B2_UH(...) ILVL_B2(v8u16, __VA_ARGS__)
#define ILVL_B2_SH(...) ILVL_B2(v8i16, __VA_ARGS__)

#define ILVL_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVL_B2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVL_B2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVL_B4_UB(...) ILVL_B4(v16u8, __VA_ARGS__)
#define ILVL_B4_SB(...) ILVL_B4(v16i8, __VA_ARGS__)
#define ILVL_B4_UH(...) ILVL_B4(v8u16, __VA_ARGS__)
#define ILVL_B4_SH(...) ILVL_B4(v8i16, __VA_ARGS__)

/* Description : Interleave left half of halfword elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Left half of halfword elements of 'in0' and 'in1' are
                 interleaved and written to 'out0'.
*/
#define ILVL_H2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvl_h((v8i16) in0, (v8i16) in1);  \
  out1 = (RTYPE) __msa_ilvl_h((v8i16) in2, (v8i16) in3);  \
}
#define ILVL_H2_SH(...) ILVL_H2(v8i16, __VA_ARGS__)
#define ILVL_H2_SW(...) ILVL_H2(v4i32, __VA_ARGS__)

#define ILVL_H4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVL_H2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVL_H2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVL_H4_SH(...) ILVL_H4(v8i16, __VA_ARGS__)
#define ILVL_H4_SW(...) ILVL_H4(v4i32, __VA_ARGS__)

/* Description : Interleave left half of word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Left half of word elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'.
*/
#define ILVL_W2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvl_w((v4i32) in0, (v4i32) in1);  \
  out1 = (RTYPE) __msa_ilvl_w((v4i32) in2, (v4i32) in3);  \
}
#define ILVL_W2_UB(...) ILVL_W2(v16u8, __VA_ARGS__)
#define ILVL_W2_SB(...) ILVL_W2(v16i8, __VA_ARGS__)
#define ILVL_W2_SH(...) ILVL_W2(v8i16, __VA_ARGS__)

/* Description : Interleave right half of byte elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Right half of byte elements of 'in0' and 'in1' are interleaved
                 and written to out0.
*/
#define ILVR_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvr_b((v16i8) in0, (v16i8) in1);  \
  out1 = (RTYPE) __msa_ilvr_b((v16i8) in2, (v16i8) in3);  \
}
#define ILVR_B2_UB(...) ILVR_B2(v16u8, __VA_ARGS__)
#define ILVR_B2_SB(...) ILVR_B2(v16i8, __VA_ARGS__)
#define ILVR_B2_UH(...) ILVR_B2(v8u16, __VA_ARGS__)
#define ILVR_B2_SH(...) ILVR_B2(v8i16, __VA_ARGS__)
#define ILVR_B2_SW(...) ILVR_B2(v4i32, __VA_ARGS__)

#define ILVR_B3(RTYPE, in0, in1, in2, in3, in4, in5, out0, out1, out2) {  \
  ILVR_B2(RTYPE, in0, in1, in2, in3, out0, out1);                         \
  out2 = (RTYPE) __msa_ilvr_b((v16i8) in4, (v16i8) in5);                  \
}
#define ILVR_B3_UB(...) ILVR_B3(v16u8, __VA_ARGS__)
#define ILVR_B3_UH(...) ILVR_B3(v8u16, __VA_ARGS__)
#define ILVR_B3_SH(...) ILVR_B3(v8i16, __VA_ARGS__)

#define ILVR_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVR_B2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVR_B2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVR_B4_UB(...) ILVR_B4(v16u8, __VA_ARGS__)
#define ILVR_B4_SB(...) ILVR_B4(v16i8, __VA_ARGS__)
#define ILVR_B4_UH(...) ILVR_B4(v8u16, __VA_ARGS__)
#define ILVR_B4_SH(...) ILVR_B4(v8i16, __VA_ARGS__)
#define ILVR_B4_SW(...) ILVR_B4(v4i32, __VA_ARGS__)

#define ILVR_B8(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,     \
                in8, in9, in10, in11, in12, in13, in14, in15,      \
                out0, out1, out2, out3, out4, out5, out6, out7) {  \
  ILVR_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,           \
          out0, out1, out2, out3);                                 \
  ILVR_B4(RTYPE, in8, in9, in10, in11, in12, in13, in14, in15,     \
          out4, out5, out6, out7);                                 \
}
#define ILVR_B8_UH(...) ILVR_B8(v8u16, __VA_ARGS__)

/* Description : Interleave right half of halfword elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Right half of halfword elements of 'in0' and 'in1' are
                 interleaved and written to 'out0'.
*/
#define ILVR_H2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvr_h((v8i16) in0, (v8i16) in1);  \
  out1 = (RTYPE) __msa_ilvr_h((v8i16) in2, (v8i16) in3);  \
}
#define ILVR_H2_UB(...) ILVR_H2(v16u8, __VA_ARGS__)
#define ILVR_H2_SH(...) ILVR_H2(v8i16, __VA_ARGS__)
#define ILVR_H2_SW(...) ILVR_H2(v4i32, __VA_ARGS__)

#define ILVR_H3(RTYPE, in0, in1, in2, in3, in4, in5, out0, out1, out2) {  \
  ILVR_H2(RTYPE, in0, in1, in2, in3, out0, out1);                         \
  out2 = (RTYPE) __msa_ilvr_h((v8i16) in4, (v8i16) in5);                  \
}
#define ILVR_H3_SH(...) ILVR_H3(v8i16, __VA_ARGS__)

#define ILVR_H4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVR_H2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVR_H2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVR_H4_UB(...) ILVR_H4(v16u8, __VA_ARGS__)
#define ILVR_H4_SH(...) ILVR_H4(v8i16, __VA_ARGS__)
#define ILVR_H4_SW(...) ILVR_H4(v4i32, __VA_ARGS__)

#define ILVR_W2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvr_w((v4i32) in0, (v4i32) in1);  \
  out1 = (RTYPE) __msa_ilvr_w((v4i32) in2, (v4i32) in3);  \
}
#define ILVR_W2_UB(...) ILVR_W2(v16u8, __VA_ARGS__)
#define ILVR_W2_SB(...) ILVR_W2(v16i8, __VA_ARGS__)
#define ILVR_W2_SH(...) ILVR_W2(v8i16, __VA_ARGS__)

#define ILVR_W4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVR_W2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVR_W2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVR_W4_SB(...) ILVR_W4(v16i8, __VA_ARGS__)
#define ILVR_W4_UB(...) ILVR_W4(v16u8, __VA_ARGS__)

/* Description : Interleave right half of double word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Right half of double word elements of 'in0' and 'in1' are
                 interleaved and written to 'out0'.
*/
#define ILVR_D2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_ilvr_d((v2i64) in0, (v2i64) in1);  \
  out1 = (RTYPE) __msa_ilvr_d((v2i64) in2, (v2i64) in3);  \
}
#define ILVR_D2_UB(...) ILVR_D2(v16u8, __VA_ARGS__)
#define ILVR_D2_SB(...) ILVR_D2(v16i8, __VA_ARGS__)
#define ILVR_D2_SH(...) ILVR_D2(v8i16, __VA_ARGS__)

#define ILVR_D3(RTYPE, in0, in1, in2, in3, in4, in5, out0, out1, out2) {  \
  ILVR_D2(RTYPE, in0, in1, in2, in3, out0, out1);                         \
  out2 = (RTYPE) __msa_ilvr_d((v2i64) in4, (v2i64) in5);                  \
}
#define ILVR_D3_SB(...) ILVR_D3(v16i8, __VA_ARGS__)

#define ILVR_D4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  ILVR_D2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ILVR_D2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ILVR_D4_SB(...) ILVR_D4(v16i8, __VA_ARGS__)
#define ILVR_D4_UB(...) ILVR_D4(v16u8, __VA_ARGS__)

/* Description : Interleave both left and right half of input vectors
   Arguments   : Inputs  - in0, in1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Right half of byte elements from 'in0' and 'in1' are
                 interleaved and written to 'out0'
*/
#define ILVRL_B2(RTYPE, in0, in1, out0, out1) {           \
  out0 = (RTYPE) __msa_ilvr_b((v16i8) in0, (v16i8) in1);  \
  out1 = (RTYPE) __msa_ilvl_b((v16i8) in0, (v16i8) in1);  \
}
#define ILVRL_B2_UB(...) ILVRL_B2(v16u8, __VA_ARGS__)
#define ILVRL_B2_SB(...) ILVRL_B2(v16i8, __VA_ARGS__)
#define ILVRL_B2_UH(...) ILVRL_B2(v8u16, __VA_ARGS__)
#define ILVRL_B2_SH(...) ILVRL_B2(v8i16, __VA_ARGS__)
#define ILVRL_B2_SW(...) ILVRL_B2(v4i32, __VA_ARGS__)

#define ILVRL_H2(RTYPE, in0, in1, out0, out1) {           \
  out0 = (RTYPE) __msa_ilvr_h((v8i16) in0, (v8i16) in1);  \
  out1 = (RTYPE) __msa_ilvl_h((v8i16) in0, (v8i16) in1);  \
}
#define ILVRL_H2_UB(...) ILVRL_H2(v16u8, __VA_ARGS__)
#define ILVRL_H2_SB(...) ILVRL_H2(v16i8, __VA_ARGS__)
#define ILVRL_H2_SH(...) ILVRL_H2(v8i16, __VA_ARGS__)
#define ILVRL_H2_SW(...) ILVRL_H2(v4i32, __VA_ARGS__)
#define ILVRL_H2_UW(...) ILVRL_H2(v4u32, __VA_ARGS__)

#define ILVRL_W2(RTYPE, in0, in1, out0, out1) {           \
  out0 = (RTYPE) __msa_ilvr_w((v4i32) in0, (v4i32) in1);  \
  out1 = (RTYPE) __msa_ilvl_w((v4i32) in0, (v4i32) in1);  \
}
#define ILVRL_W2_UB(...) ILVRL_W2(v16u8, __VA_ARGS__)
#define ILVRL_W2_SH(...) ILVRL_W2(v8i16, __VA_ARGS__)
#define ILVRL_W2_SW(...) ILVRL_W2(v4i32, __VA_ARGS__)

/* Description : Maximum values between signed elements of vector and
                 5-bit signed immediate value are copied to the output vector
   Arguments   : Inputs  - in0, in1, in2, in3, max_val
                 Outputs - in place operation
                 Return Type - unsigned halfword
   Details     : Maximum of signed halfword element values from 'in0' and
                 'max_val' are written in place
*/
#define MAXI_SH2(RTYPE, in0, in1, max_val) {             \
  in0 = (RTYPE) __msa_maxi_s_h((v8i16) in0, (max_val));  \
  in1 = (RTYPE) __msa_maxi_s_h((v8i16) in1, (max_val));  \
}
#define MAXI_SH2_UH(...) MAXI_SH2(v8u16, __VA_ARGS__)
#define MAXI_SH2_SH(...) MAXI_SH2(v8i16, __VA_ARGS__)

#define MAXI_SH4(RTYPE, in0, in1, in2, in3, max_val) {  \
  MAXI_SH2(RTYPE, in0, in1, max_val);                   \
  MAXI_SH2(RTYPE, in2, in3, max_val);                   \
}
#define MAXI_SH4_UH(...) MAXI_SH4(v8u16, __VA_ARGS__)
#define MAXI_SH4_SH(...) MAXI_SH4(v8i16, __VA_ARGS__)

/* Description : Saturate the halfword element values to the max
                 unsigned value of (sat_val + 1) bits
                 The element data width remains unchanged
   Arguments   : Inputs  - in0, in1, sat_val
                 Outputs - in place operation
                 Return Type - as per RTYPE
   Details     : Each unsigned halfword element from 'in0' is saturated to the
                 value generated with (sat_val + 1) bit range.
                 The results are written in place
*/
#define SAT_UH2(RTYPE, in0, in1, sat_val) {           \
  in0 = (RTYPE) __msa_sat_u_h((v8u16) in0, sat_val);  \
  in1 = (RTYPE) __msa_sat_u_h((v8u16) in1, sat_val);  \
}
#define SAT_UH2_UH(...) SAT_UH2(v8u16, __VA_ARGS__)
#define SAT_UH2_SH(...) SAT_UH2(v8i16, __VA_ARGS__)

#define SAT_UH4(RTYPE, in0, in1, in2, in3, sat_val) {  \
  SAT_UH2(RTYPE, in0, in1, sat_val);                   \
  SAT_UH2(RTYPE, in2, in3, sat_val);                   \
}
#define SAT_UH4_UH(...) SAT_UH4(v8u16, __VA_ARGS__)
#define SAT_UH4_SH(...) SAT_UH4(v8i16, __VA_ARGS__)

/* Description : Saturate the halfword element values to the max
                 unsigned value of (sat_val + 1) bits
                 The element data width remains unchanged
   Arguments   : Inputs  - in0, in1, sat_val
                 Outputs - in place operation
                 Return Type - as per RTYPE
   Details     : Each unsigned halfword element from 'in0' is saturated to the
                 value generated with (sat_val + 1) bit range
                 The results are written in place
*/
#define SAT_SH2(RTYPE, in0, in1, sat_val) {           \
  in0 = (RTYPE) __msa_sat_s_h((v8i16) in0, sat_val);  \
  in1 = (RTYPE) __msa_sat_s_h((v8i16) in1, sat_val);  \
}
#define SAT_SH2_SH(...) SAT_SH2(v8i16, __VA_ARGS__)

#define SAT_SH3(RTYPE, in0, in1, in2, sat_val) {      \
  SAT_SH2(RTYPE, in0, in1, sat_val);                  \
  in2 = (RTYPE) __msa_sat_s_h((v8i16) in2, sat_val);  \
}
#define SAT_SH3_SH(...) SAT_SH3(v8i16, __VA_ARGS__)

#define SAT_SH4(RTYPE, in0, in1, in2, in3, sat_val) {  \
  SAT_SH2(RTYPE, in0, in1, sat_val);                   \
  SAT_SH2(RTYPE, in2, in3, sat_val);                   \
}
#define SAT_SH4_SH(...) SAT_SH4(v8i16, __VA_ARGS__)

/* Description : Saturate the word element values to the max
                 unsigned value of (sat_val + 1) bits
                 The element data width remains unchanged
   Arguments   : Inputs  - in0, in1, sat_val
                 Outputs - in place operation
                 Return Type - as per RTYPE
   Details     : Each unsigned word element from 'in0' is saturated to the
                 value generated with (sat_val + 1) bit range
                 The results are written in place
*/
#define SAT_SW2(RTYPE, in0, in1, sat_val) {           \
  in0 = (RTYPE) __msa_sat_s_w((v4i32) in0, sat_val);  \
  in1 = (RTYPE) __msa_sat_s_w((v4i32) in1, sat_val);  \
}
#define SAT_SW2_SW(...) SAT_SW2(v4i32, __VA_ARGS__)

#define SAT_SW4(RTYPE, in0, in1, in2, in3, sat_val) {  \
  SAT_SW2(RTYPE, in0, in1, sat_val);                   \
  SAT_SW2(RTYPE, in2, in3, sat_val);                   \
}
#define SAT_SW4_SW(...) SAT_SW4(v4i32, __VA_ARGS__)

/* Description : Indexed halfword element values are replicated to all
                 elements in output vector
   Arguments   : Inputs  - in, idx0, idx1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : 'idx0' element value from 'in' vector is replicated to all
                  elements in 'out0' vector
                  Valid index range for halfword operation is 0-7
*/
#define SPLATI_H2(RTYPE, in, idx0, idx1, out0, out1) {  \
  out0 = (RTYPE) __msa_splati_h((v8i16) in, idx0);      \
  out1 = (RTYPE) __msa_splati_h((v8i16) in, idx1);      \
}
#define SPLATI_H2_SB(...) SPLATI_H2(v16i8, __VA_ARGS__)
#define SPLATI_H2_SH(...) SPLATI_H2(v8i16, __VA_ARGS__)

#define SPLATI_H3(RTYPE, in, idx0, idx1, idx2,      \
                  out0, out1, out2) {               \
  SPLATI_H2(RTYPE, in, idx0, idx1, out0, out1);     \
  out2 = (RTYPE) __msa_splati_h((v8i16) in, idx2);  \
}
#define SPLATI_H3_SB(...) SPLATI_H3(v16i8, __VA_ARGS__)
#define SPLATI_H3_SH(...) SPLATI_H3(v8i16, __VA_ARGS__)

#define SPLATI_H4(RTYPE, in, idx0, idx1, idx2, idx3,  \
                  out0, out1, out2, out3) {           \
  SPLATI_H2(RTYPE, in, idx0, idx1, out0, out1);       \
  SPLATI_H2(RTYPE, in, idx2, idx3, out2, out3);       \
}
#define SPLATI_H4_UB(...) SPLATI_H4(v16u8, __VA_ARGS__)
#define SPLATI_H4_SB(...) SPLATI_H4(v16i8, __VA_ARGS__)
#define SPLATI_H4_SH(...) SPLATI_H4(v8i16, __VA_ARGS__)

/* Description : Indexed word element values are replicated to all
                 elements in output vector
   Arguments   : Inputs  - in, stidx
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : 'stidx' element value from 'in' vector is replicated to all
                 elements in 'out0' vector
                 'stidx + 1' element value from 'in' vector is replicated to all
                 elements in 'out1' vector
                 Valid index range for word operation is 0-3
*/
#define SPLATI_W2(RTYPE, in, stidx, out0, out1) {        \
  out0 = (RTYPE) __msa_splati_w((v4i32) in, stidx);      \
  out1 = (RTYPE) __msa_splati_w((v4i32) in, (stidx+1));  \
}
#define SPLATI_W2_SH(...) SPLATI_W2(v8i16, __VA_ARGS__)
#define SPLATI_W2_SW(...) SPLATI_W2(v4i32, __VA_ARGS__)

#define SPLATI_W4(RTYPE, in, out0, out1, out2, out3) {  \
  SPLATI_W2(RTYPE, in, 0, out0, out1);                  \
  SPLATI_W2(RTYPE, in, 2, out2, out3);                  \
}
#define SPLATI_W4_SH(...) SPLATI_W4(v8i16, __VA_ARGS__)
#define SPLATI_W4_SW(...) SPLATI_W4(v4i32, __VA_ARGS__)

/* Description : Pack even byte elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even byte elements of 'in0' are copied to the left half of
                 'out0' & even byte elements of 'in1' are copied to the right
                 half of 'out0'.
*/
#define PCKEV_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_pckev_b((v16i8) in0, (v16i8) in1);  \
  out1 = (RTYPE) __msa_pckev_b((v16i8) in2, (v16i8) in3);  \
}
#define PCKEV_B2_SB(...) PCKEV_B2(v16i8, __VA_ARGS__)
#define PCKEV_B2_UB(...) PCKEV_B2(v16u8, __VA_ARGS__)
#define PCKEV_B2_SH(...) PCKEV_B2(v8i16, __VA_ARGS__)
#define PCKEV_B2_SW(...) PCKEV_B2(v4i32, __VA_ARGS__)

#define PCKEV_B3(RTYPE, in0, in1, in2, in3, in4, in5, out0, out1, out2) {  \
  PCKEV_B2(RTYPE, in0, in1, in2, in3, out0, out1);                         \
  out2 = (RTYPE) __msa_pckev_b((v16i8) in4, (v16i8) in5);                  \
}
#define PCKEV_B3_UB(...) PCKEV_B3(v16u8, __VA_ARGS__)
#define PCKEV_B3_SB(...) PCKEV_B3(v16i8, __VA_ARGS__)

#define PCKEV_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  PCKEV_B2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  PCKEV_B2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define PCKEV_B4_SB(...) PCKEV_B4(v16i8, __VA_ARGS__)
#define PCKEV_B4_UB(...) PCKEV_B4(v16u8, __VA_ARGS__)
#define PCKEV_B4_SH(...) PCKEV_B4(v8i16, __VA_ARGS__)
#define PCKEV_B4_SW(...) PCKEV_B4(v4i32, __VA_ARGS__)

/* Description : Pack even halfword elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even halfword elements of 'in0' are copied to the left half of
                 'out0' & even halfword elements of 'in1' are copied to the
                 right half of 'out0'.
*/
#define PCKEV_H2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_pckev_h((v8i16) in0, (v8i16) in1);  \
  out1 = (RTYPE) __msa_pckev_h((v8i16) in2, (v8i16) in3);  \
}
#define PCKEV_H2_UH(...) PCKEV_H2(v8u16, __VA_ARGS__)
#define PCKEV_H2_SH(...) PCKEV_H2(v8i16, __VA_ARGS__)
#define PCKEV_H2_SW(...) PCKEV_H2(v4i32, __VA_ARGS__)

#define PCKEV_H4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  PCKEV_H2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  PCKEV_H2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define PCKEV_H4_SH(...) PCKEV_H4(v8i16, __VA_ARGS__)
#define PCKEV_H4_UH(...) PCKEV_H4(v8u16, __VA_ARGS__)
#define PCKEV_H4_SW(...) PCKEV_H4(v4i32, __VA_ARGS__)

/* Description : Pack even double word elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even double elements of 'in0' are copied to the left half of
                 'out0' & even double elements of 'in1' are copied to the right
                 half of 'out0'.
*/
#define PCKEV_D2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_pckev_d((v2i64) in0, (v2i64) in1);  \
  out1 = (RTYPE) __msa_pckev_d((v2i64) in2, (v2i64) in3);  \
}
#define PCKEV_D2_UB(...) PCKEV_D2(v16u8, __VA_ARGS__)
#define PCKEV_D2_SB(...) PCKEV_D2(v16i8, __VA_ARGS__)
#define PCKEV_D2_SH(...) PCKEV_D2(v8i16, __VA_ARGS__)
#define PCKEV_D2_UH(...) PCKEV_D2(v8u16, __VA_ARGS__)

#define PCKEV_D4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  PCKEV_D2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  PCKEV_D2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define PCKEV_D4_UB(...) PCKEV_D4(v16u8, __VA_ARGS__)

/* Description : Pack odd byte elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Odd byte elements of 'in0' are copied to the left half of
                 'out0' & odd byte elements of 'in1' are copied to the right
                 half of 'out0'.
*/
#define PCKOD_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_pckod_b((v16i8) in0, (v16i8) in1);  \
  out1 = (RTYPE) __msa_pckod_b((v16i8) in2, (v16i8) in3);  \
}
#define PCKOD_B2_UB(...) PCKOD_B2(v16u8, __VA_ARGS__)

#define PCKOD_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  PCKOD_B2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  PCKOD_B2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define PCKOD_B4_UB(...) PCKOD_B4(v16u8, __VA_ARGS__)

/* Description : Pack odd double word elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Odd double word elements of 'in0' are copied to the left half
                 of 'out0' & odd double word elements of 'in1' are copied to
                 the right half of 'out0'.
*/
#define PCKOD_D2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) __msa_pckod_d((v2i64) in0, (v2i64) in1);  \
  out1 = (RTYPE) __msa_pckod_d((v2i64) in2, (v2i64) in3);  \
}
#define PCKOD_D2_UB(...) PCKOD_D2(v16u8, __VA_ARGS__)
#define PCKOD_D2_SH(...) PCKOD_D2(v8i16, __VA_ARGS__)
#define PCKOD_D2_SD(...) PCKOD_D2(v2i64, __VA_ARGS__)

/* Description : Average of signed halfword elements -> (a + b) / 2
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3
                 Return Type - as per RTYPE
   Details     : Each signed halfword element from 'in0' is added to each
                 signed halfword element of 'in1' with full precision resulting
                 in one extra bit in the result. The result is then divided by
                 2 and written to 'out0'
*/
#define AVE_SH4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3) {                       \
  out0 = (RTYPE) __msa_ave_s_h((v8i16) in0, (v8i16) in1);       \
  out1 = (RTYPE) __msa_ave_s_h((v8i16) in2, (v8i16) in3);       \
  out2 = (RTYPE) __msa_ave_s_h((v8i16) in4, (v8i16) in5);       \
  out3 = (RTYPE) __msa_ave_s_h((v8i16) in6, (v8i16) in7);       \
}
#define AVE_SH4_SH(...) AVE_SH4(v8i16, __VA_ARGS__)

/* Description : Addition of signed halfword elements and signed saturation
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Signed halfword elements from 'in0' are added to signed
                 halfword elements of 'in1'. The result is then signed saturated
                 between halfword data type range
*/
#define ADDS_SH2(RTYPE, in0, in1, in2, in3, out0, out1) {   \
  out0 = (RTYPE) __msa_adds_s_h((v8i16) in0, (v8i16) in1);  \
  out1 = (RTYPE) __msa_adds_s_h((v8i16) in2, (v8i16) in3);  \
}
#define ADDS_SH2_SH(...) ADDS_SH2(v8i16, __VA_ARGS__)

#define ADDS_SH4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  ADDS_SH2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ADDS_SH2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ADDS_SH4_UH(...) ADDS_SH4(v8u16, __VA_ARGS__)
#define ADDS_SH4_SH(...) ADDS_SH4(v8i16, __VA_ARGS__)

/* Description : Shift left all elements of vector (generic for all data types)
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in place operation
                 Return Type - as per input vector RTYPE
   Details     : Each element of vector 'in0' is left shifted by 'shift' and
                 the result is written in-place.
*/
#define SLLI_2V(in0, in1, shift)  {  \
  in0 = in0 << shift;                \
  in1 = in1 << shift;                \
}
#define SLLI_4V(in0, in1, in2, in3, shift) {  \
  SLLI_2V(in0, in1, shift);                   \
  SLLI_2V(in2, in3, shift);                   \
}

/* Description : Shift left all elements of byte vector
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in place operation
                 Return Type - as per input vector RTYPE
   Details     : Each element of vector 'in0' is left shifted by 'shift' and
                 the result is written in-place.
*/
#define SLLI_B4(RTYPE, in0, in1, in2, in3, shift_val) {  \
  in0 = (RTYPE) MSA_SLLI_B(in0, shift_val);              \
  in1 = (RTYPE) MSA_SLLI_B(in1, shift_val);              \
  in2 = (RTYPE) MSA_SLLI_B(in2, shift_val);              \
  in3 = (RTYPE) MSA_SLLI_B(in3, shift_val);              \
}
#define SLLI_B4_SB(...) SLLI_B4(v16i8, __VA_ARGS__)
#define SLLI_B4_UB(...) SLLI_B4(v16u8, __VA_ARGS__)

/* Description : Shift left all elements of half-word vector
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in place operation
                 Return Type - as per input vector RTYPE
   Details     : Each element of vector 'in0' is left shifted by 'shift' and
                 the result is written in-place.
*/
#define SLLI_H4(RTYPE, in0, in1, in2, in3, shift_val) {  \
  in0 = (RTYPE) MSA_SLLI_H(in0, shift_val);              \
  in1 = (RTYPE) MSA_SLLI_H(in1, shift_val);              \
  in2 = (RTYPE) MSA_SLLI_H(in2, shift_val);              \
  in3 = (RTYPE) MSA_SLLI_H(in3, shift_val);              \
}
#define SLLI_H4_SH(...) SLLI_H4(v8i16, __VA_ARGS__)
#define SLLI_H4_UH(...) SLLI_H4(v8u16, __VA_ARGS__)

/* Description : Shift left all elements of word vector
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in place operation
                 Return Type - as per input vector RTYPE
   Details     : Each element of vector 'in0' is left shifted by 'shift' and
                 the result is written in-place.
*/
#define SLLI_W4(RTYPE, in0, in1, in2, in3, shift_val) {  \
  in0 = (RTYPE) MSA_SLLI_W(in0, shift_val);              \
  in1 = (RTYPE) MSA_SLLI_W(in1, shift_val);              \
  in2 = (RTYPE) MSA_SLLI_W(in2, shift_val);              \
  in3 = (RTYPE) MSA_SLLI_W(in3, shift_val);              \
}
#define SLLI_W4_SW(...) SLLI_W4(v4i32, __VA_ARGS__)
#define SLLI_W4_UW(...) SLLI_W4(v4u32, __VA_ARGS__)

/* Description : Arithmetic shift right all elements of vector
                 (generic for all data types)
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in place operation
                 Return Type - as per input vector RTYPE
   Details     : Each element of vector 'in0' is right shifted by 'shift' and
                 the result is written in-place. 'shift' is a GP variable.
*/
#define SRA_2V(in0, in1, shift) {  \
  in0 = in0 >> shift;              \
  in1 = in1 >> shift;              \
}
#define SRA_4V(in0, in1, in2, in3, shift) {  \
  SRA_2V(in0, in1, shift);                   \
  SRA_2V(in2, in3, shift);                   \
}

/* Description : Arithmetic shift right all elements of byte vector
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in place operation
                 Return Type - as per input vector RTYPE
   Details     : Each element of vector 'in0' is right shifted by 'shift' and
                 the result is written in-place. 'shift' is a GP variable.
*/
#define SRAI_B4(RTYPE, in0, in1, in2, in3, shift_val) {  \
  in0 = (RTYPE) MSA_SRAI_B(in0, shift_val);              \
  in1 = (RTYPE) MSA_SRAI_B(in1, shift_val);              \
  in2 = (RTYPE) MSA_SRAI_B(in2, shift_val);              \
  in3 = (RTYPE) MSA_SRAI_B(in3, shift_val);              \
}
#define SRAI_B4_SB(...) SRAI_B4(v16i8, __VA_ARGS__)
#define SRAI_B4_UB(...) SRAI_B4(v16u8, __VA_ARGS__)

/* Description : Arithmetic shift right all elements of half-word vector
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in place operation
                 Return Type - as per input vector RTYPE
   Details     : Each element of vector 'in0' is right shifted by 'shift' and
                 the result is written in-place. 'shift' is a GP variable.
*/
#define SRAI_H2(RTYPE, in0, in1, shift) {          \
  in0 = (RTYPE) __msa_srai_h((v8i16) in0, shift);  \
  in1 = (RTYPE) __msa_srai_h((v8i16) in1, shift);  \
}
#define SRAI_H4(RTYPE, in0, in1, in2, in3, shift) {  \
  SRAI_H2(RTYPE, in0, in1, shift);                   \
  SRAI_H2(RTYPE, in2, in3, shift);                   \
}
#define SRAI_H4_SH(...) SRAI_H4(v8i16, __VA_ARGS__)
#define SRAI_H4_UH(...) SRAI_H4(v8u16, __VA_ARGS__)

/* Description : Arithmetic shift right all elements of word vector
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in place operation
                 Return Type - as per input vector RTYPE
   Details     : Each element of vector 'in0' is right shifted by 'shift' and
                 the result is written in-place. 'shift' is a GP variable.
*/
#define SRAI_W4(RTYPE, in0, in1, in2, in3, shift_val) {  \
  in0 = (RTYPE) MSA_SRAI_W(in0, shift_val);              \
  in1 = (RTYPE) MSA_SRAI_W(in1, shift_val);              \
  in2 = (RTYPE) MSA_SRAI_W(in2, shift_val);              \
  in3 = (RTYPE) MSA_SRAI_W(in3, shift_val);              \
}
#define SRAI_W4_SW(...) SRAI_W4(v4i32, __VA_ARGS__)
#define SRAI_W4_UW(...) SRAI_W4(v4u32, __VA_ARGS__)

/* Description : Shift right arithmetic rounded words
   Arguments   : Inputs  - in0, in1, shift
                 Outputs - in place operation
                 Return Type - as per RTYPE
   Details     : Each element of vector 'in0' is shifted right arithmetically by
                 the number of bits in the corresponding element in the vector
                 'shift'. The last discarded bit is added to shifted value for
                 rounding and the result is written in-place.
                 'shift' is a vector.
*/
#define SRAR_W2(RTYPE, in0, in1, shift) {                  \
  in0 = (RTYPE) __msa_srar_w((v4i32) in0, (v4i32) shift);  \
  in1 = (RTYPE) __msa_srar_w((v4i32) in1, (v4i32) shift);  \
}

#define SRAR_W4(RTYPE, in0, in1, in2, in3, shift) {  \
  SRAR_W2(RTYPE, in0, in1, shift);                   \
  SRAR_W2(RTYPE, in2, in3, shift);                   \
}
#define SRAR_W4_SW(...) SRAR_W4(v4i32, __VA_ARGS__)

/* Description : Shift right arithmetic rounded (immediate)
   Arguments   : Inputs  - in0, in1, shift
                 Outputs - in place operation
                 Return Type - as per RTYPE
   Details     : Each element of vector 'in0' is shifted right arithmetically by
                 the value in 'shift'. The last discarded bit is added to the
                 shifted value for rounding and the result is written in-place.
                 'shift' is an immediate value.
*/
#define SRARI_H2(RTYPE, in0, in1, shift) {          \
  in0 = (RTYPE) __msa_srari_h((v8i16) in0, shift);  \
  in1 = (RTYPE) __msa_srari_h((v8i16) in1, shift);  \
}
#define SRARI_H2_UH(...) SRARI_H2(v8u16, __VA_ARGS__)
#define SRARI_H2_SH(...) SRARI_H2(v8i16, __VA_ARGS__)

#define SRARI_H4(RTYPE, in0, in1, in2, in3, shift) {  \
  SRARI_H2(RTYPE, in0, in1, shift);                   \
  SRARI_H2(RTYPE, in2, in3, shift);                   \
}
#define SRARI_H4_UH(...) SRARI_H4(v8u16, __VA_ARGS__)
#define SRARI_H4_SH(...) SRARI_H4(v8i16, __VA_ARGS__)

#define SRARI_W2(RTYPE, in0, in1, shift) {          \
  in0 = (RTYPE) __msa_srari_w((v4i32) in0, shift);  \
  in1 = (RTYPE) __msa_srari_w((v4i32) in1, shift);  \
}
#define SRARI_W2_SW(...) SRARI_W2(v4i32, __VA_ARGS__)

#define SRARI_W4(RTYPE, in0, in1, in2, in3, shift) {  \
  SRARI_W2(RTYPE, in0, in1, shift);                   \
  SRARI_W2(RTYPE, in2, in3, shift);                   \
}
#define SRARI_W4_SW(...) SRARI_W4(v4i32, __VA_ARGS__)

/* Description : Logical shift right all elements of vector (immediate)
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - out0, out1, out2, out3
                 Return Type - as per RTYPE
   Details     : Each element of vector 'in0' is right shifted by 'shift' and
                 the result is written in-place. 'shift' is an immediate value.
*/
#define SRLI_H4(RTYPE, in0, in1, in2, in3, out0, out1, out2, out3, shift) {  \
  out0 = (RTYPE) __msa_srli_h((v8i16) in0, shift);                           \
  out1 = (RTYPE) __msa_srli_h((v8i16) in1, shift);                           \
  out2 = (RTYPE) __msa_srli_h((v8i16) in2, shift);                           \
  out3 = (RTYPE) __msa_srli_h((v8i16) in3, shift);                           \
}
#define SRLI_H4_SH(...) SRLI_H4(v8i16, __VA_ARGS__)

/* Description : Multiplication of pairs of vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element from 'in0' is multiplied with elements from 'in1'
                 and the result is written to 'out0'
*/
#define MUL2(in0, in1, in2, in3, out0, out1) {  \
  out0 = in0 * in1;                             \
  out1 = in2 * in3;                             \
}
#define MUL4(in0, in1, in2, in3, in4, in5, in6, in7,  \
             out0, out1, out2, out3) {                \
  MUL2(in0, in1, in2, in3, out0, out1);               \
  MUL2(in4, in5, in6, in7, out2, out3);               \
}

/* Description : Addition of 2 pairs of vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element in 'in0' is added to 'in1' and result is written
                 to 'out0'.
*/
#define ADD2(in0, in1, in2, in3, out0, out1) {  \
  out0 = in0 + in1;                             \
  out1 = in2 + in3;                             \
}
#define ADD4(in0, in1, in2, in3, in4, in5, in6, in7,  \
             out0, out1, out2, out3) {                \
  ADD2(in0, in1, in2, in3, out0, out1);               \
  ADD2(in4, in5, in6, in7, out2, out3);               \
}

/* Description : Addition of 2 pairs of byte vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element in 'in0' is added to 'in1' and result is written
                 to 'out0'.
*/
#define ADDVI_B2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) MSA_ADDVI_B(in0, in1);                    \
  out1 = (RTYPE) MSA_ADDVI_B(in2, in3);                    \
}
#define ADDVI_B2_SB(...) ADDVI_B2(v16i8, __VA_ARGS__)
#define ADDVI_B2_UB(...) ADDVI_B2(v16u8, __VA_ARGS__)

#define ADDVI_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  ADDVI_B2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ADDVI_B2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ADDVI_B4_SB(...) ADDVI_B4(v16i8, __VA_ARGS__)
#define ADDVI_B4_UB(...) ADDVI_B4(v16u8, __VA_ARGS__)

/* Description : Addition of 2 pairs of half-word vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element in 'in0' is added to 'in1' and result is written
                 to 'out0'.
*/
#define ADDVI_H2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) MSA_ADDVI_H(in0, in1);                    \
  out1 = (RTYPE) MSA_ADDVI_H(in2, in3);                    \
}
#define ADDVI_H2_SH(...) ADDVI_H2(v8i16, __VA_ARGS__)
#define ADDVI_H2_UH(...) ADDVI_H2(v8u16, __VA_ARGS__)

#define ADDVI_H4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  ADDVI_H2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ADDVI_H2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ADDVI_H4_SH(...) ADDVI_H4(v8i16, __VA_ARGS__)
#define ADDVI_H4_UH(...) ADDVI_H4(v8u16, __VA_ARGS__)

/* Description : Addition of 2 pairs of word vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element in 'in0' is added to 'in1' and result is written
                 to 'out0'.
*/
#define ADDVI_W2(RTYPE, in0, in1, in2, in3, out0, out1) {  \
  out0 = (RTYPE) MSA_ADDVI_W(in0, in1);                    \
  out1 = (RTYPE) MSA_ADDVI_W(in2, in3);                    \
}
#define ADDVI_W2_SW(...) ADDVI_W2(v4i32, __VA_ARGS__)
#define ADDVI_W2_UW(...) ADDVI_W2(v4u32, __VA_ARGS__)

#define ADDVI_W4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3) {                       \
  ADDVI_W2(RTYPE, in0, in1, in2, in3, out0, out1);               \
  ADDVI_W2(RTYPE, in4, in5, in6, in7, out2, out3);               \
}
#define ADDVI_W4_SW(...) ADDVI_W4(v4i32, __VA_ARGS__)
#define ADDVI_W4_UW(...) ADDVI_W4(v4u32, __VA_ARGS__)

/* Description : Subtraction of 2 pairs of vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element in 'in1' is subtracted from 'in0' and result is
                 written to 'out0'.
*/
#define SUB2(in0, in1, in2, in3, out0, out1) {  \
  out0 = in0 - in1;                             \
  out1 = in2 - in3;                             \
}
#define SUB4(in0, in1, in2, in3, in4, in5, in6, in7,  \
             out0, out1, out2, out3) {                \
  out0 = in0 - in1;                                   \
  out1 = in2 - in3;                                   \
  out2 = in4 - in5;                                   \
  out3 = in6 - in7;                                   \
}

/* Description : Sign extend byte elements from right half of the vector
   Arguments   : Input  - in    (byte vector)
                 Output - out   (sign extended halfword vector)
                 Return Type - signed halfword
   Details     : Sign bit of byte elements from input vector 'in' is
                 extracted and interleaved with same vector 'in0' to generate
                 8 halfword elements keeping sign intact
*/
#define UNPCK_R_SB_SH(in, out)                       \
{                                                    \
    v16i8 sign_m;                                    \
                                                     \
    sign_m = __msa_clti_s_b((v16i8) in, 0);          \
    out = (v8i16) __msa_ilvr_b(sign_m, (v16i8) in);  \
}

/* Description : Sign extend halfword elements from right half of the vector
   Arguments   : Input  - in    (halfword vector)
                 Output - out   (sign extended word vector)
                 Return Type - signed word
   Details     : Sign bit of halfword elements from input vector 'in' is
                 extracted and interleaved with same vector 'in0' to generate
                 4 word elements keeping sign intact
*/
#define UNPCK_R_SH_SW(in, out) {                   \
  v8i16 sign_m;                                    \
                                                   \
  sign_m = __msa_clti_s_h((v8i16) in, 0);          \
  out = (v4i32) __msa_ilvr_h(sign_m, (v8i16) in);  \
}

/* Description : Sign extend byte elements from input vector and return
                 halfword results in pair of vectors
   Arguments   : Input   - in           (byte vector)
                 Outputs - out0, out1   (sign extended halfword vectors)
                 Return Type - signed halfword
   Details     : Sign bit of byte elements from input vector 'in' is
                 extracted and interleaved right with same vector 'in0' to
                 generate 8 signed halfword elements in 'out0'
                 Then interleaved left with same vector 'in0' to
                 generate 8 signed halfword elements in 'out1'
*/
#define UNPCK_SB_SH(in, out0, out1) {     \
  v16i8 tmp_m;                            \
                                          \
  tmp_m = __msa_clti_s_b((v16i8) in, 0);  \
  ILVRL_B2_SH(tmp_m, in, out0, out1);     \
}

/* Description : Zero extend unsigned byte elements to halfword elements
   Arguments   : Input   - in          (unsigned byte vector)
                 Outputs - out0, out1  (unsigned  halfword vectors)
                 Return Type - signed halfword
   Details     : Zero extended right half of vector is returned in 'out0'
                 Zero extended left half of vector is returned in 'out1'
*/
#define UNPCK_UB_SH(in, out0, out1) {   \
  v16i8 zero_m = { 0 };                 \
                                        \
  ILVRL_B2_SH(zero_m, in, out0, out1);  \
}

/* Description : Sign extend halfword elements from input vector and return
                 the result in pair of vectors
   Arguments   : Input   - in            (halfword vector)
                 Outputs - out0, out1   (sign extended word vectors)
                 Return Type - signed word
   Details     : Sign bit of halfword elements from input vector 'in' is
                 extracted and interleaved right with same vector 'in0' to
                 generate 4 signed word elements in 'out0'
                 Then interleaved left with same vector 'in0' to
                 generate 4 signed word elements in 'out1'
*/
#define UNPCK_SH_SW(in, out0, out1) {     \
  v8i16 tmp_m;                            \
                                          \
  tmp_m = __msa_clti_s_h((v8i16) in, 0);  \
  ILVRL_H2_SW(tmp_m, in, out0, out1);     \
}

/* Description : Swap two variables
   Arguments   : Inputs  - in0, in1
                 Outputs - in-place operation
   Details     : Swap values of two input variables using xor
*/
#define SWAP(in0, in1) {  \
  in0 = in0 ^ in1;        \
  in1 = in0 ^ in1;        \
  in0 = in0 ^ in1;        \
}

/* Description : Butterfly of 4 input vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1, out2, out3
   Details     : Butterfly operation
*/
#define BUTTERFLY_4(in0, in1, in2, in3, out0, out1, out2, out3) {  \
  out0 = in0 + in3;                                                \
  out1 = in1 + in2;                                                \
                                                                   \
  out2 = in1 - in2;                                                \
  out3 = in0 - in3;                                                \
}

/* Description : Butterfly of 8 input vectors
   Arguments   : Inputs  - in0 ...  in7
                 Outputs - out0 .. out7
   Details     : Butterfly operation
*/
#define BUTTERFLY_8(in0, in1, in2, in3, in4, in5, in6, in7,            \
                    out0, out1, out2, out3, out4, out5, out6, out7) {  \
  out0 = in0 + in7;                                                    \
  out1 = in1 + in6;                                                    \
  out2 = in2 + in5;                                                    \
  out3 = in3 + in4;                                                    \
                                                                       \
  out4 = in3 - in4;                                                    \
  out5 = in2 - in5;                                                    \
  out6 = in1 - in6;                                                    \
  out7 = in0 - in7;                                                    \
}

/* Description : Transpose input 4x4 byte block
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1, out2, out3
                 Return Type - unsigned byte
*/
#define TRANSPOSE4x4_UB_UB(in0, in1, in2, in3, out0, out1, out2, out3) {  \
  v16i8 zero_m = { 0 };                                                   \
  v16i8 s0_m, s1_m, s2_m, s3_m;                                           \
                                                                          \
  ILVR_D2_SB(in1, in0, in3, in2, s0_m, s1_m);                             \
  ILVRL_B2_SB(s1_m, s0_m, s2_m, s3_m);                                    \
                                                                          \
  out0 = (v16u8) __msa_ilvr_b(s3_m, s2_m);                                \
  out1 = (v16u8) __msa_sldi_b(zero_m, (v16i8) out0, 4);                   \
  out2 = (v16u8) __msa_sldi_b(zero_m, (v16i8) out1, 4);                   \
  out3 = (v16u8) __msa_sldi_b(zero_m, (v16i8) out2, 4);                   \
}

/* Description : Transpose input 4x8 byte block
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - as per RTYPE
*/
#define TRANSPOSE4x8_UB(RTYPE, in0, in1, in2, in3, out0, out1, \
                        out2, out3, out4, out5, out6, out7) {  \
  v16i8 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                        \
                                                               \
  ILVR_B2_SB(in2, in0, in3, in1, tmp0_m, tmp1_m);              \
  ILVRL_B2_SB(tmp1_m, tmp0_m, tmp2_m, tmp3_m);                 \
  ILVRL_W2(RTYPE, zero, tmp2_m, out0, out2);                   \
  ILVRL_W2(RTYPE, zero, tmp3_m, out4, out6);                   \
  SLDI_B2_0(RTYPE, out0, out2, out1, out3, 8);                 \
  SLDI_B2_0(RTYPE, out4, out6, out5, out7, 8);                 \
}
#define TRANSPOSE4x8_UB_UB(...) TRANSPOSE4x8_UB(v16u8, __VA_ARGS__)
#define TRANSPOSE4x8_UB_UH(...) TRANSPOSE4x8_UB(v8u16, __VA_ARGS__)

/* Description : Transpose input 8x4 byte block into 4x8
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3
                 Return Type - as per RTYPE
*/
#define TRANSPOSE8x4_UB(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                        out0, out1, out2, out3) {                       \
  v16i8 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                 \
                                                                        \
  ILVEV_W2_SB(in0, in4, in1, in5, tmp0_m, tmp1_m);                      \
  tmp2_m = __msa_ilvr_b(tmp1_m, tmp0_m);                                \
  ILVEV_W2_SB(in2, in6, in3, in7, tmp0_m, tmp1_m);                      \
                                                                        \
  tmp3_m = __msa_ilvr_b(tmp1_m, tmp0_m);                                \
  ILVRL_H2_SB(tmp3_m, tmp2_m, tmp0_m, tmp1_m);                          \
                                                                        \
  ILVRL_W2(RTYPE, tmp1_m, tmp0_m, out0, out2);                          \
  out1 = (RTYPE) __msa_ilvl_d((v2i64) out2, (v2i64) out0);              \
  out3 = (RTYPE) __msa_ilvl_d((v2i64) out0, (v2i64) out2);              \
}
#define TRANSPOSE8x4_UB_UB(...) TRANSPOSE8x4_UB(v16u8, __VA_ARGS__)
#define TRANSPOSE8x4_UB_UH(...) TRANSPOSE8x4_UB(v8u16, __VA_ARGS__)

/* Description : Transpose input 8x8 byte block
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - as per RTYPE
*/
#define TRANSPOSE8x8_UB(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,     \
                        out0, out1, out2, out3, out4, out5, out6, out7) {  \
  v16i8 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                    \
  v16i8 tmp4_m, tmp5_m, tmp6_m, tmp7_m;                                    \
                                                                           \
  ILVR_B4_SB(in2, in0, in3, in1, in6, in4, in7, in5,                       \
             tmp0_m, tmp1_m, tmp2_m, tmp3_m);                              \
  ILVRL_B2_SB(tmp1_m, tmp0_m, tmp4_m, tmp5_m);                             \
  ILVRL_B2_SB(tmp3_m, tmp2_m, tmp6_m, tmp7_m);                             \
  ILVRL_W2(RTYPE, tmp6_m, tmp4_m, out0, out2);                             \
  ILVRL_W2(RTYPE, tmp7_m, tmp5_m, out4, out6);                             \
  SLDI_B2_0(RTYPE, out0, out2, out1, out3, 8);                             \
  SLDI_B2_0(RTYPE, out4, out6, out5, out7, 8);                             \
}
#define TRANSPOSE8x8_UB_UB(...) TRANSPOSE8x8_UB(v16u8, __VA_ARGS__)
#define TRANSPOSE8x8_UB_UH(...) TRANSPOSE8x8_UB(v8u16, __VA_ARGS__)

/* Description : Transpose 4x4 block with half word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1, out2, out3
                 Return Type - signed halfword
*/
#define TRANSPOSE4x4_SH_SH(in0, in1, in2, in3, out0, out1, out2, out3) {  \
  v8i16 s0_m, s1_m;                                                       \
                                                                          \
  ILVR_H2_SH(in1, in0, in3, in2, s0_m, s1_m);                             \
  ILVRL_W2_SH(s1_m, s0_m, out0, out2);                                    \
  out1 = (v8i16) __msa_ilvl_d((v2i64) out0, (v2i64) out0);                \
  out3 = (v8i16) __msa_ilvl_d((v2i64) out0, (v2i64) out2);                \
}

/* Description : Transpose 4x8 block with half word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - signed halfword
*/
#define TRANSPOSE4X8_SH_SH(in0, in1, in2, in3, in4, in5, in6, in7,            \
                           out0, out1, out2, out3, out4, out5, out6, out7) {  \
  v8i16 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                       \
  v8i16 tmp0_n, tmp1_n, tmp2_n, tmp3_n;                                       \
  v8i16 zero_m = { 0 };                                                       \
                                                                              \
  ILVR_H4_SH(in1, in0, in3, in2, in5, in4, in7, in6,                          \
             tmp0_n, tmp1_n, tmp2_n, tmp3_n);                                 \
  ILVRL_W2_SH(tmp1_n, tmp0_n, tmp0_m, tmp2_m);                                \
  ILVRL_W2_SH(tmp3_n, tmp2_n, tmp1_m, tmp3_m);                                \
                                                                              \
  out0 = (v8i16) __msa_ilvr_d((v2i64) tmp1_m, (v2i64) tmp0_m);                \
  out1 = (v8i16) __msa_ilvl_d((v2i64) tmp1_m, (v2i64) tmp0_m);                \
  out2 = (v8i16) __msa_ilvr_d((v2i64) tmp3_m, (v2i64) tmp2_m);                \
  out3 = (v8i16) __msa_ilvl_d((v2i64) tmp3_m, (v2i64) tmp2_m);                \
                                                                              \
  out4 = zero_m;                                                              \
  out5 = zero_m;                                                              \
  out6 = zero_m;                                                              \
  out7 = zero_m;                                                              \
}

/* Description : Transpose 8x4 block with half word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - signed halfword
*/
#define TRANSPOSE8X4_SH_SH(in0, in1, in2, in3, out0, out1, out2, out3) {  \
  v8i16 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                   \
                                                                          \
  ILVR_H2_SH(in1, in0, in3, in2, tmp0_m, tmp1_m);                         \
  ILVL_H2_SH(in1, in0, in3, in2, tmp2_m, tmp3_m);                         \
  ILVR_W2_SH(tmp1_m, tmp0_m, tmp3_m, tmp2_m, out0, out2);                 \
  ILVL_W2_SH(tmp1_m, tmp0_m, tmp3_m, tmp2_m, out1, out3);                 \
}

/* Description : Transpose 8x8 block with half word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - as per RTYPE
*/
#define TRANSPOSE8x8_H(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,     \
                       out0, out1, out2, out3, out4, out5, out6, out7) {  \
  v8i16 s0_m, s1_m;                                                       \
  v8i16 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                   \
  v8i16 tmp4_m, tmp5_m, tmp6_m, tmp7_m;                                   \
                                                                          \
  ILVR_H2_SH(in6, in4, in7, in5, s0_m, s1_m);                             \
  ILVRL_H2_SH(s1_m, s0_m, tmp0_m, tmp1_m);                                \
  ILVL_H2_SH(in6, in4, in7, in5, s0_m, s1_m);                             \
  ILVRL_H2_SH(s1_m, s0_m, tmp2_m, tmp3_m);                                \
  ILVR_H2_SH(in2, in0, in3, in1, s0_m, s1_m);                             \
  ILVRL_H2_SH(s1_m, s0_m, tmp4_m, tmp5_m);                                \
  ILVL_H2_SH(in2, in0, in3, in1, s0_m, s1_m);                             \
  ILVRL_H2_SH(s1_m, s0_m, tmp6_m, tmp7_m);                                \
  PCKEV_D4(RTYPE, tmp0_m, tmp4_m, tmp1_m, tmp5_m, tmp2_m, tmp6_m,         \
           tmp3_m, tmp7_m, out0, out2, out4, out6);                       \
  out1 = (RTYPE) __msa_pckod_d((v2i64) tmp0_m, (v2i64) tmp4_m);           \
  out3 = (RTYPE) __msa_pckod_d((v2i64) tmp1_m, (v2i64) tmp5_m);           \
  out5 = (RTYPE) __msa_pckod_d((v2i64) tmp2_m, (v2i64) tmp6_m);           \
  out7 = (RTYPE) __msa_pckod_d((v2i64) tmp3_m, (v2i64) tmp7_m);           \
}
#define TRANSPOSE8x8_UH_UH(...) TRANSPOSE8x8_H(v8u16, __VA_ARGS__)
#define TRANSPOSE8x8_SH_SH(...) TRANSPOSE8x8_H(v8i16, __VA_ARGS__)

/* Description : Transpose 4x4 block with word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1, out2, out3
                 Return Type - as per RTYPE
*/
#define TRANSPOSE4x4_W(RTYPE, in0, in1, in2, in3, out0, out1, out2, out3) {  \
  v4i32 s0_m, s1_m, s2_m, s3_m;                                              \
                                                                             \
  ILVRL_W2_SW(in1, in0, s0_m, s1_m);                                         \
  ILVRL_W2_SW(in3, in2, s2_m, s3_m);                                         \
                                                                             \
  out0 = (RTYPE) __msa_ilvr_d((v2i64) s2_m, (v2i64) s0_m);                   \
  out1 = (RTYPE) __msa_ilvl_d((v2i64) s2_m, (v2i64) s0_m);                   \
  out2 = (RTYPE) __msa_ilvr_d((v2i64) s3_m, (v2i64) s1_m);                   \
  out3 = (RTYPE) __msa_ilvl_d((v2i64) s3_m, (v2i64) s1_m);                   \
}

#define TRANSPOSE4x4_SW_SW(...) TRANSPOSE4x4_W(v4i32, __VA_ARGS__)
#define TRANSPOSE4x4_SP_SP(...) TRANSPOSE4x4_W(v4f32, __VA_ARGS__)

#endif  /* __JMACROS_MSA_H__ */
