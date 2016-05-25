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

/* YCC --> RGB CONVERSION */

#include "../jinclude.h"
#include "../jpeglib.h"

#include "jmacros_msa.h"

#define FIX_1_40200  0x166e9
#define FIX_0_34414  0x581a
#define FIX_0_71414  0xb6d2
#define FIX_1_77200  0x1c5a2

#define ROUND_POWER_OF_TWO(value, n)  (((value) + (1 << ((n) - 1))) >> (n))

static inline unsigned char clip_pixel (int val)
{
    return ((val & ~0xFF) ? ((-val) >> 31) & 0xFF : val);
}

#define CALC_R4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cr_w0, cr_w1, cr_w2, cr_w3,  \
                        out_r0, out_r1, out_r2, out_r3) {                    \
  v4i32 const_1_40200_m = { FIX_1_40200, FIX_1_40200, FIX_1_40200,           \
                            FIX_1_40200 };                                   \
                                                                             \
  MUL4(const_1_40200_m, cr_w0, const_1_40200_m, cr_w1,                       \
       const_1_40200_m, cr_w2, const_1_40200_m, cr_w3,                       \
       out_r0, out_r1, out_r2, out_r3);                                      \
  SRARI_W4_SW(out_r0, out_r1, out_r2, out_r3, 16);                           \
  ADD4(y_w0, out_r0, y_w1, out_r1, y_w2, out_r2, y_w3, out_r3,               \
       out_r0, out_r1, out_r2, out_r3);                                      \
  out_r0 = CLIP_SW_0_255(out_r0);                                            \
  out_r1 = CLIP_SW_0_255(out_r1);                                            \
  out_r2 = CLIP_SW_0_255(out_r2);                                            \
  out_r3 = CLIP_SW_0_255(out_r3);                                            \
}

#define CALC_R2_FRM_YUV(y_w0, y_w1, cr_w0, cr_w1, out_r0, out_r1)  {     \
  v4i32 const_1_40200_m = { FIX_1_40200, FIX_1_40200, FIX_1_40200,       \
                            FIX_1_40200 };                               \
                                                                         \
  MUL2(const_1_40200_m, cr_w0, const_1_40200_m, cr_w1, out_r0, out_r1);  \
  SRARI_W2_SW(out_r0, out_r1, 16);                                       \
  ADD2(y_w0, out_r0, y_w1, out_r1, out_r0, out_r1);                      \
  out_r0 = CLIP_SW_0_255(out_r0);                                        \
  out_r1 = CLIP_SW_0_255(out_r1);                                        \
}

#define CALC_G4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,  \
                        cr_w0, cr_w1, cr_w2, cr_w3, out_g0, out_g1, out_g2,  \
                        out_g3) {                                            \
  v4i32 const_0_34414_m = { FIX_0_34414, FIX_0_34414, FIX_0_34414,           \
                            FIX_0_34414 };                                   \
  v4i32 const_0_71414_m = { FIX_0_71414, FIX_0_71414, FIX_0_71414,           \
                            FIX_0_71414 };                                   \
                                                                             \
  MUL4(const_0_34414_m, cb_w0, const_0_34414_m, cb_w1,                       \
       const_0_34414_m, cb_w2, const_0_34414_m, cb_w3,                       \
       out_g0, out_g1, out_g2, out_g3);                                      \
  SUB4(zero, out_g0, zero, out_g1, zero, out_g2, zero, out_g3,               \
       out_g0, out_g1, out_g2, out_g3);                                      \
  out_g0 -= (const_0_71414_m * cr_w0);                                       \
  out_g1 -= (const_0_71414_m * cr_w1);                                       \
  out_g2 -= (const_0_71414_m * cr_w2);                                       \
  out_g3 -= (const_0_71414_m * cr_w3);                                       \
  SRARI_W4_SW(out_g0, out_g1, out_g2, out_g3, 16);                           \
  ADD4(y_w0, out_g0, y_w1, out_g1, y_w2, out_g2, y_w3, out_g3,               \
       out_g0, out_g1, out_g2, out_g3);                                      \
  out_g0 = CLIP_SW_0_255(out_g0);                                            \
  out_g1 = CLIP_SW_0_255(out_g1);                                            \
  out_g2 = CLIP_SW_0_255(out_g2);                                            \
  out_g3 = CLIP_SW_0_255(out_g3);                                            \
}

#define CALC_G2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, cr_w0, cr_w1,         \
                        out_g0, out_g1)  {                              \
  v4i32 const_0_34414_m = { FIX_0_34414, FIX_0_34414, FIX_0_34414,      \
                            FIX_0_34414 };                              \
  v4i32 const_0_71414_m = { FIX_0_71414, FIX_0_71414, FIX_0_71414,      \
                            FIX_0_71414 };                              \
                                                                        \
  MUL2(const_0_34414_m, cb_w0, const_0_34414_m, cb_w1,out_g0, out_g1);  \
  SUB2(zero, out_g0, zero, out_g1, out_g0, out_g1);                     \
  out_g0 -= (const_0_71414_m * cr_w0);                                  \
  out_g1 -= (const_0_71414_m * cr_w1);                                  \
  SRARI_W2_SW(out_g0, out_g1, 16);                                      \
  ADD2(y_w0, out_g0, y_w1, out_g1, out_g0, out_g1);                     \
  out_g0 = CLIP_SW_0_255(out_g0);                                       \
  out_g1 = CLIP_SW_0_255(out_g1);                                       \
}

#define CALC_B4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,  \
                        out_b0, out_b1, out_b2, out_b3) {                    \
  v4i32 const_1_77200_m = { FIX_1_77200, FIX_1_77200, FIX_1_77200,           \
                            FIX_1_77200 };                                   \
                                                                             \
  MUL4(const_1_77200_m, cb_w0, const_1_77200_m, cb_w1,                       \
       const_1_77200_m, cb_w2, const_1_77200_m, cb_w3,                       \
       out_b0, out_b1, out_b2, out_b3);                                      \
  SRARI_W4_SW(out_b0, out_b1, out_b2, out_b3, 16);                           \
  ADD4(y_w0, out_b0, y_w1, out_b1, y_w2, out_b2, y_w3, out_b3,               \
       out_b0, out_b1, out_b2, out_b3);                                      \
  out_b0 = CLIP_SW_0_255(out_b0);                                            \
  out_b1 = CLIP_SW_0_255(out_b1);                                            \
  out_b2 = CLIP_SW_0_255(out_b2);                                            \
  out_b3 = CLIP_SW_0_255(out_b3);                                            \
}

#define CALC_B2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, out_b0, out_b1) {      \
  v4i32 const_1_77200_m = { FIX_1_77200, FIX_1_77200, FIX_1_77200,       \
                            FIX_1_77200 };                               \
                                                                         \
  MUL2(const_1_77200_m, cb_w0, const_1_77200_m, cb_w1, out_b0, out_b1);  \
  SRARI_W2_SW(out_b0, out_b1, 16);                                       \
  ADD2(y_w0, out_b0, y_w1, out_b1, out_b0, out_b1);                      \
  out_b0 = CLIP_SW_0_255(out_b0);                                        \
  out_b1 = CLIP_SW_0_255(out_b1);                                        \
}

#define UNPCK_SB_TO_SW_YCBCR(input_y, input_cb, input_cr,                   \
                             out_y_w0, out_y_w1, out_y_w2, out_y_w3,        \
                             out_cb_w0, out_cb_w1, out_cb_w2, out_cb_w3,    \
                             out_cr_w0, out_cr_w1, out_cr_w2, out_cr_w3) {  \
  v8i16 y_hr_m, y_hl_m, cb_hr_m, cb_hl_m, cr_hr_m, cr_hl_m;                 \
                                                                            \
  UNPCK_UB_SH(input_y, y_hr_m, y_hl_m);                                     \
  UNPCK_SB_SH(input_cb, cb_hr_m, cb_hl_m);                                  \
  UNPCK_SB_SH(input_cr, cr_hr_m, cr_hl_m);                                  \
                                                                            \
  UNPCK_SH_SW(y_hr_m, out_y_w0, out_y_w1);                                  \
  UNPCK_SH_SW(y_hl_m, out_y_w2, out_y_w3);                                  \
                                                                            \
  UNPCK_SH_SW(cb_hr_m, out_cb_w0, out_cb_w1);                               \
  UNPCK_SH_SW(cb_hl_m, out_cb_w2, out_cb_w3);                               \
                                                                            \
  UNPCK_SH_SW(cr_hr_m, out_cr_w0, out_cr_w1);                               \
  UNPCK_SH_SW(cr_hl_m, out_cr_w2, out_cr_w3);                               \
}

#define UNPCK_R_SB_TO_SW_YCBCR(input_y, input_cbcr, out_y_w0, out_y_w1,       \
                               out_cb_w0, out_cb_w1, out_cr_w0, out_cr_w1) {  \
  v8i16 y_hr_m, cb_hr_m, cr_hr_m;                                             \
                                                                              \
  y_hr_m = (v8i16) __msa_ilvr_b((v16i8) zero, (v16i8) input_y);               \
  UNPCK_SB_SH(input_cbcr, cb_hr_m, cr_hr_m);                                  \
                                                                              \
  UNPCK_SH_SW(y_hr_m, out_y_w0, out_y_w1);                                    \
  UNPCK_SH_SW(cb_hr_m, out_cb_w0, out_cb_w1);                                 \
  UNPCK_SH_SW(cr_hr_m, out_cr_w0, out_cr_w1);                                 \
}

void
yuv_rgb_convert_msa (JSAMPROW p_in_y, JSAMPROW p_in_cb, JSAMPROW p_in_cr,
                     JSAMPROW p_rgb, JDIMENSION out_width)
{
  int y, cb, cr;
  unsigned int col, num_cols_mul_16 = out_width >> 4;
  unsigned int remaining_wd = out_width & 15;
  v16u8 mask_rg = {0, 16, 0, 4, 20, 0, 8, 24, 0, 12, 28, 0, 0, 0, 0, 0};
  v16u8 mask_rgb = {0, 1, 16, 3, 4, 20, 6, 7, 24, 9, 10, 28, 0, 0, 0, 0};
  v16i8 const_128 = {128, 128, 128, 128, 128, 128, 128, 128,
                     128, 128, 128, 128, 128, 128, 128, 128};
  v16u8 out0, out1, out2, input_y = { 0 };
  v16i8 input_cb, input_cr, out_rgb0, out_rgb1, out_rgb2, out_rgb3;
  v4i32 y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3;
  v4i32 cr_w0, cr_w1, cr_w2, cr_w3, out_r0, out_r1, out_r2, out_r3;
  v4i32 out_g0, out_g1, out_g2, out_g3, out_b0, out_b1, out_b2, out_b3;
  v4i32 zero = { 0 };

  for (col = 0; col < num_cols_mul_16; col++) {
    input_y = LD_UB(p_in_y);
    input_cb = LD_SB(p_in_cb);
    input_cr = LD_SB(p_in_cr);

    p_in_y += 16;
    p_in_cb += 16;
    p_in_cr += 16;

    input_cb -= const_128;
    input_cr -= const_128;

    UNPCK_SB_TO_SW_YCBCR(input_y, input_cb, input_cr, y_w0, y_w1, y_w2,
                         y_w3, cb_w0, cb_w1, cb_w2, cb_w3, cr_w0, cr_w1,
                         cr_w2, cr_w3);

    CALC_R4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cr_w0, cr_w1, cr_w2, cr_w3,
                    out_r0, out_r1, out_r2, out_r3);

    CALC_G4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    cr_w0, cr_w1, cr_w2, cr_w3, out_g0, out_g1, out_g2,
                    out_g3);

    VSHF_B2_SB(out_r0, out_g0, out_r1, out_g1, mask_rg, mask_rg, out_rgb0,
               out_rgb1);
    VSHF_B2_SB(out_r2, out_g2, out_r3, out_g3, mask_rg, mask_rg, out_rgb2,
               out_rgb3);

    CALC_B4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    out_b0, out_b1, out_b2, out_b3);

    VSHF_B2_SB(out_rgb0, out_b0, out_rgb1, out_b1, mask_rgb, mask_rgb,
               out_rgb0, out_rgb1);
    VSHF_B2_SB(out_rgb2, out_b2, out_rgb3, out_b3, mask_rgb, mask_rgb,
               out_rgb2, out_rgb3);

    out_rgb0 = __msa_sldi_b(out_rgb0, (v16i8) zero, 12);
    out0 = (v16u8) __msa_sldi_b((v16i8) out_rgb1, (v16i8) out_rgb0, 4);
    out_rgb1 = __msa_sldi_b(out_rgb1, (v16i8) zero, 12);
    out1 = (v16u8) __msa_sldi_b((v16i8) out_rgb2, (v16i8) out_rgb1, 8);
    out_rgb2 = __msa_sldi_b(out_rgb2, (v16i8) zero, 12);
    out2 = (v16u8) __msa_sldi_b((v16i8) out_rgb3, (v16i8) out_rgb2, 12);

    ST_UB(out0, p_rgb);
    p_rgb += 16;
    ST_UB(out1, p_rgb);
    p_rgb += 16;
    ST_UB(out2, p_rgb);
    p_rgb += 16;
  }

  if (remaining_wd > 8) {
    uint64_t in_y, in_cb, in_cr;
    v16i8 input_cbcr = { 0 };

    in_y = LD(p_in_y);
    in_cb = LD(p_in_cb);
    in_cr = LD(p_in_cr);

    p_in_y += 8;
    p_in_cb += 8;
    p_in_cr += 8;

    input_y = (v16u8) __msa_insert_d((v2i64) input_y, 0, in_y);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 0, in_cb);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 1, in_cr);

    input_cbcr -= const_128;

    UNPCK_R_SB_TO_SW_YCBCR(input_y, input_cbcr, y_w0, y_w1, cb_w0, cb_w1,
                           cr_w0, cr_w1);

    CALC_R2_FRM_YUV(y_w0, y_w1, cr_w0, cr_w1, out_r0, out_r1);

    CALC_G2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, cr_w0, cr_w1, out_g0, out_g1);

    VSHF_B2_SB(out_r0, out_g0, out_r1, out_g1, mask_rg, mask_rg, out_rgb0,
               out_rgb1);

    CALC_B2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, out_b0, out_b1);

    VSHF_B2_SB(out_rgb0, out_b0, out_rgb1, out_b1, mask_rgb, mask_rgb,
               out_rgb0, out_rgb1);

    out_rgb0 = __msa_sldi_b(out_rgb0, (v16i8) zero, 12);
    out0 = (v16u8) __msa_sldi_b((v16i8) out_rgb1, (v16i8) out_rgb0, 4);
    out_rgb1 = __msa_sldi_b(out_rgb1, (v16i8) zero, 12);
    out1 = (v16u8) __msa_sldi_b((v16i8) zero, (v16i8) out_rgb1, 8);

    ST_UB(out0, p_rgb);
    p_rgb += 16;
    ST8x1_UB(out1, p_rgb);
    p_rgb += 8;

    remaining_wd -= 8;
  }

  for (col = 0; col < remaining_wd; col++) {
    y  = (int) (p_in_y[col]);
    cb = (int) (p_in_cb[col]) - 128;
    cr = (int) (p_in_cr[col]) - 128;

    /* Range-limiting is essential due to noise introduced by DCT losses. */
    p_rgb[0] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_40200 * cr, 16));
    p_rgb[1] = clip_pixel(y + ROUND_POWER_OF_TWO(((-FIX_0_34414) * cb -
                                                   FIX_0_71414 * cr), 16));
    p_rgb[2] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_77200 * cb, 16));
    p_rgb += 3;
  }
}

void
yuv_bgr_convert_msa (JSAMPROW p_in_y, JSAMPROW p_in_cb, JSAMPROW p_in_cr,
                     JSAMPROW p_rgb, JDIMENSION out_width)
{
  int y, cb, cr;
  unsigned int col, num_cols_mul_16 = out_width >> 4;
  unsigned int remaining_wd = out_width & 15;
  v16u8 mask_bg = {0, 16, 0, 4, 20, 0, 8, 24, 0, 12, 28, 0, 0, 0, 0, 0};
  v16u8 mask_rgb = {0, 1, 16, 3, 4, 20, 6, 7, 24, 9, 10, 28, 0, 0, 0, 0};
  v16i8 const_128 = {128, 128, 128, 128, 128, 128, 128, 128,
                     128, 128, 128, 128, 128, 128, 128, 128};
  v16u8 out0, out1, out2, input_y = { 0 };
  v16i8 input_cb, input_cr, out_rgb0, out_rgb1, out_rgb2, out_rgb3;
  v4i32 y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3;
  v4i32 cr_w0, cr_w1, cr_w2, cr_w3, out_r0, out_r1, out_r2, out_r3;
  v4i32 out_g0, out_g1, out_g2, out_g3, out_b0, out_b1, out_b2, out_b3;
  v4i32 zero = { 0 };

  for (col = 0; col < num_cols_mul_16; col++) {
    input_y = LD_UB(p_in_y);
    input_cb = LD_SB(p_in_cb);
    input_cr = LD_SB(p_in_cr);

    p_in_y += 16;
    p_in_cb += 16;
    p_in_cr += 16;

    input_cb -= const_128;
    input_cr -= const_128;

    UNPCK_SB_TO_SW_YCBCR(input_y, input_cb, input_cr, y_w0, y_w1, y_w2,
                         y_w3, cb_w0, cb_w1, cb_w2, cb_w3, cr_w0, cr_w1,
                         cr_w2, cr_w3);

    CALC_B4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    out_b0, out_b1, out_b2, out_b3);

    CALC_G4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    cr_w0, cr_w1, cr_w2, cr_w3, out_g0, out_g1, out_g2,
                    out_g3);

    VSHF_B2_SB(out_b0, out_g0, out_b1, out_g1, mask_bg, mask_bg, out_rgb0,
               out_rgb1);
    VSHF_B2_SB(out_b2, out_g2, out_b3, out_g3, mask_bg, mask_bg, out_rgb2,
               out_rgb3);

    CALC_R4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cr_w0, cr_w1, cr_w2, cr_w3,
                    out_r0, out_r1, out_r2, out_r3);

    VSHF_B2_SB(out_rgb0, out_r0, out_rgb1, out_r1, mask_rgb, mask_rgb,
               out_rgb0, out_rgb1);
    VSHF_B2_SB(out_rgb2, out_r2, out_rgb3, out_r3, mask_rgb, mask_rgb,
               out_rgb2, out_rgb3);

    out_rgb0 = __msa_sldi_b(out_rgb0, (v16i8) zero, 12);
    out0 = (v16u8) __msa_sldi_b((v16i8) out_rgb1, (v16i8) out_rgb0, 4);
    out_rgb1 = __msa_sldi_b(out_rgb1, (v16i8) zero, 12);
    out1 = (v16u8) __msa_sldi_b((v16i8) out_rgb2, (v16i8) out_rgb1, 8);
    out_rgb2 = __msa_sldi_b(out_rgb2, (v16i8) zero, 12);
    out2 = (v16u8) __msa_sldi_b((v16i8) out_rgb3, (v16i8) out_rgb2, 12);

    ST_UB(out0, p_rgb);
    p_rgb += 16;
    ST_UB(out1, p_rgb);
    p_rgb += 16;
    ST_UB(out2, p_rgb);
    p_rgb += 16;
  }

  if (remaining_wd > 8) {
    uint64_t in_y, in_cb, in_cr;
    v16i8 input_cbcr = { 0 };

    in_y = LD(p_in_y);
    in_cb = LD(p_in_cb);
    in_cr = LD(p_in_cr);

    p_in_y += 8;
    p_in_cb += 8;
    p_in_cr += 8;

    input_y = (v16u8) __msa_insert_d((v2i64) input_y, 0, in_y);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 0, in_cb);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 1, in_cr);

    input_cbcr -= const_128;

    UNPCK_R_SB_TO_SW_YCBCR(input_y, input_cbcr, y_w0, y_w1, cb_w0, cb_w1,
                           cr_w0, cr_w1);

    CALC_B2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, out_b0, out_b1);

    CALC_G2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, cr_w0, cr_w1, out_g0, out_g1);

    VSHF_B2_SB(out_b0, out_g0, out_b1, out_g1, mask_bg, mask_bg, out_rgb0,
               out_rgb1);

    CALC_R2_FRM_YUV(y_w0, y_w1, cr_w0, cr_w1, out_r0, out_r1);

    VSHF_B2_SB(out_rgb0, out_r0, out_rgb1, out_r1, mask_rgb, mask_rgb,
               out_rgb0, out_rgb1);

    out_rgb0 = __msa_sldi_b(out_rgb0, (v16i8) zero, 12);
    out0 = (v16u8) __msa_sldi_b((v16i8) out_rgb1, (v16i8) out_rgb0, 4);
    out_rgb1 = __msa_sldi_b(out_rgb1, (v16i8) zero, 12);
    out1 = (v16u8) __msa_sldi_b((v16i8) zero, (v16i8) out_rgb1, 8);

    ST_UB(out0, p_rgb);
    p_rgb += 16;
    ST8x1_UB(out1, p_rgb);
    p_rgb += 8;

    remaining_wd -= 8;
  }

  for (col = 0; col < remaining_wd; col++) {
    y  = (int) (p_in_y[col]);
    cb = (int) (p_in_cb[col]) - 128;
    cr = (int) (p_in_cr[col]) - 128;

    /* Range-limiting is essential due to noise introduced by DCT losses. */
    p_rgb[0] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_77200 * cb, 16));
    p_rgb[1] = clip_pixel(y + ROUND_POWER_OF_TWO(((-FIX_0_34414) * cb -
                                                   FIX_0_71414 * cr), 16));
    p_rgb[2] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_40200 * cr, 16));
    p_rgb += 3;
  }
}

void
yuv_rgba_convert_msa (JSAMPROW p_in_y, JSAMPROW p_in_cb, JSAMPROW p_in_cr,
                      JSAMPROW p_rgb, JDIMENSION out_width)
{
  int y, cb, cr;
  unsigned int col, num_cols_mul_16 = out_width >> 4;
  unsigned int remaining_wd = out_width & 15;
  v16u8 mask = {0, 16, 4, 20, 8, 24, 12, 28, 0, 0, 0, 0, 0, 0, 0, 0};
  v16i8 const_128 = {128, 128, 128, 128, 128, 128, 128, 128,
                     128, 128, 128, 128, 128, 128, 128, 128};
  v16i8 alpha = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  v16u8 out0, out1, out2, out3, input_y = { 0 };
  v16i8 input_cb, input_cr, out_rg0, out_rg1, out_rg2, out_rg3;
  v16i8 out_ba0, out_ba1, out_ba2, out_ba3;
  v4i32 y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3;
  v4i32 cr_w0, cr_w1, cr_w2, cr_w3, out_r0, out_r1, out_r2, out_r3;
  v4i32 out_g0, out_g1, out_g2, out_g3, out_b0, out_b1, out_b2, out_b3;
  v4i32 zero = { 0 };

  for (col = 0; col < num_cols_mul_16; col++) {
    input_y = LD_UB(p_in_y);
    input_cb = LD_SB(p_in_cb);
    input_cr = LD_SB(p_in_cr);

    p_in_y += 16;
    p_in_cb += 16;
    p_in_cr += 16;

    input_cb -= const_128;
    input_cr -= const_128;

    UNPCK_SB_TO_SW_YCBCR(input_y, input_cb, input_cr, y_w0, y_w1, y_w2,
                         y_w3, cb_w0, cb_w1, cb_w2, cb_w3, cr_w0, cr_w1,
                         cr_w2, cr_w3);

    CALC_R4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cr_w0, cr_w1, cr_w2, cr_w3,
                    out_r0, out_r1, out_r2, out_r3);

    CALC_G4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    cr_w0, cr_w1, cr_w2, cr_w3, out_g0, out_g1, out_g2,
                    out_g3);

    VSHF_B2_SB(out_r0, out_g0, out_r1, out_g1, mask, mask, out_rg0,
               out_rg1);
    VSHF_B2_SB(out_r2, out_g2, out_r3, out_g3, mask, mask, out_rg2,
               out_rg3);

    CALC_B4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    out_b0, out_b1, out_b2, out_b3);

    VSHF_B2_SB(out_b0, alpha, out_b1, alpha, mask, mask, out_ba0, out_ba1);
    VSHF_B2_SB(out_b2, alpha, out_b3, alpha, mask, mask, out_ba2, out_ba3);

    ILVR_H4_UB(out_ba0, out_rg0, out_ba1, out_rg1, out_ba2, out_rg2,
               out_ba3, out_rg3, out0, out1, out2, out3);

    ST_UB4(out0, out1, out2, out3, p_rgb, 16);
    p_rgb += 16 * 4;
  }

  if (remaining_wd > 8) {
    uint64_t in_y, in_cb, in_cr;
    v16i8 input_cbcr = { 0 };

    in_y = LD(p_in_y);
    in_cb = LD(p_in_cb);
    in_cr = LD(p_in_cr);

    p_in_y += 8;
    p_in_cb += 8;
    p_in_cr += 8;

    input_y = (v16u8) __msa_insert_d((v2i64) input_y, 0, in_y);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 0, in_cb);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 1, in_cr);

    input_cbcr -= const_128;

    UNPCK_R_SB_TO_SW_YCBCR(input_y, input_cbcr, y_w0, y_w1, cb_w0, cb_w1,
                           cr_w0, cr_w1);

    CALC_R2_FRM_YUV(y_w0, y_w1, cr_w0, cr_w1, out_r0, out_r1);

    CALC_G2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, cr_w0, cr_w1, out_g0, out_g1);

    VSHF_B2_SB(out_r0, out_g0, out_r1, out_g1, mask, mask, out_rg0,
               out_rg1);

    CALC_B2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, out_b0, out_b1);

    VSHF_B2_SB(out_b0, alpha, out_b1, alpha, mask, mask, out_ba0, out_ba1);

    ILVR_H2_UB(out_ba0, out_rg0, out_ba1, out_rg1, out0, out1);

    ST_UB2(out0, out1, p_rgb, 16);
    p_rgb += 16 * 2;

    remaining_wd -= 8;
  }

  for (col = 0; col < remaining_wd; col++) {
    y  = (int) (p_in_y[col]);
    cb = (int) (p_in_cb[col]) - 128;
    cr = (int) (p_in_cr[col]) - 128;

    /* Range-limiting is essential due to noise introduced by DCT losses. */
    p_rgb[0] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_40200 * cr, 16));
    p_rgb[1] = clip_pixel(y + ROUND_POWER_OF_TWO(((-FIX_0_34414) * cb -
                                                   FIX_0_71414 * cr), 16));
    p_rgb[2] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_77200 * cb, 16));
    p_rgb[3] = 0xFF;
    p_rgb += 4;
  }
}

void
yuv_bgra_convert_msa (JSAMPROW p_in_y, JSAMPROW p_in_cb, JSAMPROW p_in_cr,
                      JSAMPROW p_rgb, JDIMENSION out_width)
{
  int y, cb, cr;
  unsigned int col, num_cols_mul_16 = out_width >> 4;
  unsigned int remaining_wd = out_width & 15;
  v16u8 mask = {0, 16, 4, 20, 8, 24, 12, 28, 0, 0, 0, 0, 0, 0, 0, 0};
  v16i8 const_128 = {128, 128, 128, 128, 128, 128, 128, 128,
                     128, 128, 128, 128, 128, 128, 128, 128};
  v16i8 alpha = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  v16u8 out0, out1, out2, out3, input_y = { 0 };
  v16i8 input_cb, input_cr, out_bg0, out_bg1, out_bg2, out_bg3;
  v16i8 out_ra0, out_ra1, out_ra2, out_ra3;
  v4i32 y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3;
  v4i32 cr_w0, cr_w1, cr_w2, cr_w3, out_r0, out_r1, out_r2, out_r3;
  v4i32 out_g0, out_g1, out_g2, out_g3, out_b0, out_b1, out_b2, out_b3;
  v4i32 zero = { 0 };

  for (col = 0; col < num_cols_mul_16; col++) {
    input_y = LD_UB(p_in_y);
    input_cb = LD_SB(p_in_cb);
    input_cr = LD_SB(p_in_cr);

    p_in_y += 16;
    p_in_cb += 16;
    p_in_cr += 16;

    input_cb -= const_128;
    input_cr -= const_128;

    UNPCK_SB_TO_SW_YCBCR(input_y, input_cb, input_cr, y_w0, y_w1, y_w2,
                         y_w3, cb_w0, cb_w1, cb_w2, cb_w3, cr_w0, cr_w1,
                         cr_w2, cr_w3);

    CALC_B4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    out_b0, out_b1, out_b2, out_b3);

    CALC_G4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    cr_w0, cr_w1, cr_w2, cr_w3, out_g0, out_g1, out_g2,
                    out_g3);

    VSHF_B2_SB(out_b0, out_g0, out_b1, out_g1, mask, mask, out_bg0,
               out_bg1);
    VSHF_B2_SB(out_b2, out_g2, out_b3, out_g3, mask, mask, out_bg2,
               out_bg3);

    CALC_R4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cr_w0, cr_w1, cr_w2, cr_w3,
                    out_r0, out_r1, out_r2, out_r3);

    VSHF_B2_SB(out_r0, alpha, out_r1, alpha, mask, mask, out_ra0, out_ra1);
    VSHF_B2_SB(out_r2, alpha, out_r3, alpha, mask, mask, out_ra2, out_ra3);

    ILVR_H4_UB(out_ra0, out_bg0, out_ra1, out_bg1, out_ra2, out_bg2,
               out_ra3, out_bg3, out0, out1, out2, out3);

    ST_UB4(out0, out1, out2, out3, p_rgb, 16);
    p_rgb += 16 * 4;
  }

  if (remaining_wd > 8) {
    uint64_t in_y, in_cb, in_cr;
    v16i8 input_cbcr = { 0 };

    in_y = LD(p_in_y);
    in_cb = LD(p_in_cb);
    in_cr = LD(p_in_cr);

    p_in_y += 8;
    p_in_cb += 8;
    p_in_cr += 8;

    input_y = (v16u8) __msa_insert_d((v2i64) input_y, 0, in_y);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 0, in_cb);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 1, in_cr);

    input_cbcr -= const_128;

    UNPCK_R_SB_TO_SW_YCBCR(input_y, input_cbcr, y_w0, y_w1, cb_w0, cb_w1,
                           cr_w0, cr_w1);

    CALC_B2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, out_b0, out_b1);

    CALC_G2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, cr_w0, cr_w1, out_g0, out_g1);

    VSHF_B2_SB(out_b0, out_g0, out_b1, out_g1, mask, mask, out_bg0,
               out_bg1);

    CALC_R2_FRM_YUV(y_w0, y_w1, cr_w0, cr_w1, out_r0, out_r1);

    VSHF_B2_SB(out_r0, alpha, out_r1, alpha, mask, mask, out_ra0, out_ra1);

    ILVR_H2_UB(out_ra0, out_bg0, out_ra1, out_bg1, out0, out1);

    ST_UB2(out0, out1, p_rgb, 16);
    p_rgb += 16 * 2;

    remaining_wd -= 8;
  }

  for (col = 0; col < remaining_wd; col++) {
    y  = (int) (p_in_y[col]);
    cb = (int) (p_in_cb[col]) - 128;
    cr = (int) (p_in_cr[col]) - 128;

    p_rgb[0] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_77200 * cb, 16));
    p_rgb[1] = clip_pixel(y + ROUND_POWER_OF_TWO(((-FIX_0_34414) * cb -
                                                   FIX_0_71414 * cr), 16));
    p_rgb[2] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_40200 * cr, 16));
    p_rgb[3] = 0xFF;
    p_rgb += 4;
  }
}

void
yuv_argb_convert_msa (JSAMPROW p_in_y, JSAMPROW p_in_cb, JSAMPROW p_in_cr,
                      JSAMPROW p_rgb, JDIMENSION out_width)
{
  int y, cb, cr;
  unsigned int col, num_cols_mul_16 = out_width >> 4;
  unsigned int remaining_wd = out_width & 15;
  v16u8 mask = {0, 16, 4, 20, 8, 24, 12, 28, 0, 0, 0, 0, 0, 0, 0, 0};
  v16i8 const_128 = {128, 128, 128, 128, 128, 128, 128, 128,
                     128, 128, 128, 128, 128, 128, 128, 128};
  v16i8 alpha = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  v16u8 out0, out1, out2, out3, input_y = { 0 };
  v16i8 input_cb, input_cr, out_ar0, out_ar1, out_ar2, out_ar3;
  v16i8 out_gb0, out_gb1, out_gb2, out_gb3;
  v4i32 y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3;
  v4i32 cr_w0, cr_w1, cr_w2, cr_w3, out_r0, out_r1, out_r2, out_r3;
  v4i32 out_g0, out_g1, out_g2, out_g3, out_b0, out_b1, out_b2, out_b3;
  v4i32 zero = { 0 };

  for (col = 0; col < num_cols_mul_16; col++) {
    input_y = LD_UB(p_in_y);
    input_cb = LD_SB(p_in_cb);
    input_cr = LD_SB(p_in_cr);

    p_in_y += 16;
    p_in_cb += 16;
    p_in_cr += 16;

    input_cb -= const_128;
    input_cr -= const_128;

    UNPCK_SB_TO_SW_YCBCR(input_y, input_cb, input_cr, y_w0, y_w1, y_w2,
                         y_w3, cb_w0, cb_w1, cb_w2, cb_w3, cr_w0, cr_w1,
                         cr_w2, cr_w3);

    CALC_R4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cr_w0, cr_w1, cr_w2, cr_w3,
                    out_r0, out_r1, out_r2, out_r3);

    CALC_G4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    cr_w0, cr_w1, cr_w2, cr_w3, out_g0, out_g1, out_g2,
                    out_g3);

    VSHF_B2_SB(alpha, out_r0, alpha, out_r1, mask, mask, out_ar0, out_ar1);
    VSHF_B2_SB(alpha, out_r2, alpha, out_r3, mask, mask, out_ar2, out_ar3);

    CALC_B4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    out_b0, out_b1, out_b2, out_b3);

    VSHF_B2_SB(out_g0, out_b0, out_g1, out_b1, mask, mask, out_gb0,
               out_gb1);
    VSHF_B2_SB(out_g2, out_b2, out_g3, out_b3, mask, mask, out_gb2,
               out_gb3);

    ILVR_H4_UB(out_gb0, out_ar0, out_gb1, out_ar1, out_gb2, out_ar2,
               out_gb3, out_ar3, out0, out1, out2, out3);

    ST_UB4(out0, out1, out2, out3, p_rgb, 16);
    p_rgb += 16 * 4;
  }

  if (remaining_wd > 8) {
    uint64_t in_y, in_cb, in_cr;
    v16i8 input_cbcr = { 0 };

    in_y = LD(p_in_y);
    in_cb = LD(p_in_cb);
    in_cr = LD(p_in_cr);

    p_in_y += 8;
    p_in_cb += 8;
    p_in_cr += 8;

    input_y = (v16u8) __msa_insert_d((v2i64) input_y, 0, in_y);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 0, in_cb);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 1, in_cr);

    input_cbcr -= const_128;

    UNPCK_R_SB_TO_SW_YCBCR(input_y, input_cbcr, y_w0, y_w1, cb_w0, cb_w1,
                           cr_w0, cr_w1);

    CALC_R2_FRM_YUV(y_w0, y_w1, cr_w0, cr_w1, out_r0, out_r1);

    CALC_G2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, cr_w0, cr_w1, out_g0, out_g1);

    VSHF_B2_SB(alpha, out_r0, alpha, out_r1, mask, mask, out_ar0, out_ar1);

    CALC_B2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, out_b0, out_b1);

    VSHF_B2_SB(out_g0, out_b0, out_g1, out_b1, mask, mask, out_gb0,
               out_gb1);

    ILVR_H2_UB(out_gb0, out_ar0, out_gb1, out_ar1, out0, out1);

    ST_UB2(out0, out1, p_rgb, 16);
    p_rgb += 16 * 2;

    remaining_wd -= 8;
  }

  for (col = 0; col < remaining_wd; col++) {
    y  = (int) (p_in_y[col]);
    cb = (int) (p_in_cb[col]) - 128;
    cr = (int) (p_in_cr[col]) - 128;

    p_rgb[0] = 0xFF;
    p_rgb[1] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_40200 * cr, 16));
    p_rgb[2] = clip_pixel(y + ROUND_POWER_OF_TWO(((-FIX_0_34414) * cb -
                                                   FIX_0_71414 * cr), 16));
    p_rgb[3] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_77200 * cb, 16));
    p_rgb += 4;
  }
}

void
yuv_abgr_convert_msa (JSAMPROW p_in_y, JSAMPROW p_in_cb, JSAMPROW p_in_cr,
                      JSAMPROW p_rgb, JDIMENSION out_width)
{
  int y, cb, cr;
  unsigned int col, num_cols_mul_16 = out_width >> 4;
  unsigned int remaining_wd = out_width & 15;
  v16u8 mask = {0, 16, 4, 20, 8, 24, 12, 28, 0, 0, 0, 0, 0, 0, 0, 0};
  v16i8 const_128 = {128, 128, 128, 128, 128, 128, 128, 128,
                     128, 128, 128, 128, 128, 128, 128, 128};
  v16i8 alpha = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  v16u8 out0, out1, out2, out3, input_y = { 0 };
  v16i8 input_cb, input_cr, out_ab0, out_ab1, out_ab2, out_ab3;
  v16i8 out_gr0, out_gr1, out_gr2, out_gr3;
  v4i32 y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3;
  v4i32 cr_w0, cr_w1, cr_w2, cr_w3, out_r0, out_r1, out_r2, out_r3;
  v4i32 out_g0, out_g1, out_g2, out_g3, out_b0, out_b1, out_b2, out_b3;
  v4i32 zero = { 0 };

  for (col = 0; col < num_cols_mul_16; col++) {
    input_y = LD_UB(p_in_y);
    input_cb = LD_SB(p_in_cb);
    input_cr = LD_SB(p_in_cr);

    p_in_y += 16;
    p_in_cb += 16;
    p_in_cr += 16;

    input_cb -= const_128;
    input_cr -= const_128;

    UNPCK_SB_TO_SW_YCBCR(input_y, input_cb, input_cr, y_w0, y_w1, y_w2,
                         y_w3, cb_w0, cb_w1, cb_w2, cb_w3, cr_w0, cr_w1,
                         cr_w2, cr_w3);

    CALC_B4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    out_b0, out_b1, out_b2, out_b3);

    CALC_G4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cb_w0, cb_w1, cb_w2, cb_w3,
                    cr_w0, cr_w1, cr_w2, cr_w3, out_g0, out_g1, out_g2,
                    out_g3);

    VSHF_B2_SB(alpha, out_b0, alpha, out_b1, mask, mask, out_ab0, out_ab1);
    VSHF_B2_SB(alpha, out_b2, alpha, out_b3, mask, mask, out_ab2, out_ab3);

    CALC_R4_FRM_YUV(y_w0, y_w1, y_w2, y_w3, cr_w0, cr_w1, cr_w2, cr_w3,
                    out_r0, out_r1, out_r2, out_r3);

    VSHF_B2_SB(out_g0, out_r0, out_g1, out_r1, mask, mask, out_gr0,
               out_gr1);
    VSHF_B2_SB(out_g2, out_r2, out_g3, out_r3, mask, mask, out_gr2,
               out_gr3);

    ILVR_H4_UB(out_gr0, out_ab0, out_gr1, out_ab1, out_gr2, out_ab2,
               out_gr3, out_ab3, out0, out1, out2, out3);

    ST_UB4(out0, out1, out2, out3, p_rgb, 16);
    p_rgb += 16 * 4;
  }

  if (remaining_wd > 8) {
    uint64_t in_y, in_cb, in_cr;
    v16i8 input_cbcr = { 0 };

    in_y = LD(p_in_y);
    in_cb = LD(p_in_cb);
    in_cr = LD(p_in_cr);

    p_in_y += 8;
    p_in_cb += 8;
    p_in_cr += 8;

    input_y = (v16u8) __msa_insert_d((v2i64) input_y, 0, in_y);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 0, in_cb);
    input_cbcr = (v16i8) __msa_insert_d((v2i64) input_cbcr, 1, in_cr);

    input_cbcr -= const_128;

    UNPCK_R_SB_TO_SW_YCBCR(input_y, input_cbcr, y_w0, y_w1, cb_w0, cb_w1,
                           cr_w0, cr_w1);

    CALC_B2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, out_b0, out_b1);

    CALC_G2_FRM_YUV(y_w0, y_w1, cb_w0, cb_w1, cr_w0, cr_w1, out_g0, out_g1);

    VSHF_B2_SB(alpha, out_b0, alpha, out_b1, mask, mask, out_ab0, out_ab1);

    CALC_R2_FRM_YUV(y_w0, y_w1, cr_w0, cr_w1, out_r0, out_r1);

    VSHF_B2_SB(out_g0, out_r0, out_g1, out_r1, mask, mask, out_gr0,
               out_gr1);

    ILVR_H2_UB(out_gr0, out_ab0, out_gr1, out_ab1, out0, out1);

    ST_UB2(out0, out1, p_rgb, 16);
    p_rgb += 16 * 2;

    remaining_wd -= 8;
  }

  for (col = 0; col < remaining_wd; col++) {
    y  = (int) (p_in_y[col]);
    cb = (int) (p_in_cb[col]) - 128;
    cr = (int) (p_in_cr[col]) - 128;

    p_rgb[0] = 0xFF;
    p_rgb[1] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_77200 * cb, 16));
    p_rgb[2] = clip_pixel(y + ROUND_POWER_OF_TWO(((-FIX_0_34414) * cb -
                                                   FIX_0_71414 * cr), 16));
    p_rgb[3] = clip_pixel(y + ROUND_POWER_OF_TWO(FIX_1_40200 * cr, 16));
    p_rgb += 4;
  }
}

void
jsimd_ycc_extrgb_convert_msa (JDIMENSION out_width,
                              JSAMPIMAGE input_buf, unsigned int input_row,
                              JSAMPARRAY output_buf, int num_rows)
{
  while (--num_rows >= 0) {
    yuv_rgb_convert_msa(input_buf[0][input_row], input_buf[1][input_row],
                        input_buf[2][input_row], *output_buf++, out_width);
    input_row++;
  }
}

void
jsimd_ycc_extrgbx_convert_msa (JDIMENSION out_width,
                               JSAMPIMAGE input_buf, unsigned int input_row,
                               JSAMPARRAY output_buf, int num_rows)
{
  while (--num_rows >= 0) {
    yuv_rgba_convert_msa(input_buf[0][input_row], input_buf[1][input_row],
                         input_buf[2][input_row], *output_buf++, out_width);
    input_row++;
  }
}

void
jsimd_ycc_extbgr_convert_msa (JDIMENSION out_width,
                              JSAMPIMAGE input_buf, unsigned int input_row,
                              JSAMPARRAY output_buf, int num_rows)
{
  while (--num_rows >= 0) {
    yuv_bgr_convert_msa(input_buf[0][input_row], input_buf[1][input_row],
                        input_buf[2][input_row], *output_buf++, out_width);
    input_row++;
  }
}

void
jsimd_ycc_extbgrx_convert_msa (JDIMENSION out_width,
                               JSAMPIMAGE input_buf, unsigned int input_row,
                               JSAMPARRAY output_buf, int num_rows)
{
  while (--num_rows >= 0) {
    yuv_bgra_convert_msa(input_buf[0][input_row], input_buf[1][input_row],
                         input_buf[2][input_row], *output_buf++, out_width);
    input_row++;
  }
}

void
jsimd_ycc_extxbgr_convert_msa (JDIMENSION out_width,
                               JSAMPIMAGE input_buf, unsigned int input_row,
                               JSAMPARRAY output_buf, int num_rows)
{
  while (--num_rows >= 0) {
    yuv_abgr_convert_msa(input_buf[0][input_row], input_buf[1][input_row],
                         input_buf[2][input_row], *output_buf++, out_width);
    input_row++;
  }
}

void
jsimd_ycc_extxrgb_convert_msa (JDIMENSION out_width,
                               JSAMPIMAGE input_buf, unsigned int input_row,
                               JSAMPARRAY output_buf, int num_rows)
{
  while (--num_rows >= 0) {
    yuv_argb_convert_msa(input_buf[0][input_row], input_buf[1][input_row],
                         input_buf[2][input_row], *output_buf++, out_width);
    input_row++;
  }
}
