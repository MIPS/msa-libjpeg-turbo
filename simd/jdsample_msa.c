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

#define JPEG_INTERNALS
#include "../jinclude.h"
#include "../jpeglib.h"
#include "../jsimd.h"

#include "jmacros_msa.h"

/*
 * Fancy processing for the common case of 2:1 horizontal and 1:1 vertical.
 *
 * The centers of the output pixels are 1/4 and 3/4 of the way between input
 * pixel centers.
 *
 * height = 1 to 4, width = multiple of 16
*/
void
image_upsample_fancy_h2v1_msa (JSAMPARRAY src, JSAMPARRAY dst,
                               JDIMENSION width, JDIMENSION height)
{
  JDIMENSION col, row;
  v8i16 left_r, left_l, cur_r, cur_l, right_r, right_l;
  v8i16 zero = {0};
  v16i8 out0, out1, tmp_r, tmp_l, cur, left, right;

  for (row = 0; row < height; row++) {
    /* Prepare first 16 width column */
    cur = LD_SB(src[row]);
    tmp_r = LD_SB(src[row] + 16);
    tmp_l = __msa_splati_b(cur, 0);

    for (col = 0; col < width; col += 16) {
      left = __msa_sldi_b(cur, tmp_l, 15); /* -1 0 1 2 ... 12 13 14 */
      right = __msa_sldi_b(tmp_r, cur, 1); /* 1 2 3 4 ... 14 15 16 */

      /* Promote data to 16 bit */
      ILVRL_B2_SH(zero, left, left_r, left_l);
      ILVRL_B2_SH(zero, right, right_r, right_l);
      ILVRL_B2_SH(zero, cur, cur_r, cur_l);

      cur_l = cur_l + MSA_SLLI_H(cur_l, 1);
      cur_r = cur_r + MSA_SLLI_H(cur_r, 1);

      left_l += MSA_ADDVI_H(cur_l, 1);
      left_r += MSA_ADDVI_H(cur_r, 1);

      right_l += MSA_ADDVI_H(cur_l, 2);
      right_r += MSA_ADDVI_H(cur_r, 2);

      SRAI_H4_SH(left_r, left_l, right_r, right_l, 2);

      left = __msa_pckev_b((v16i8) left_l, (v16i8) left_r);
      right = __msa_pckev_b((v16i8) right_l, (v16i8) right_r);

      ILVRL_B2_SB(right, left, out0, out1);

      ST_SB2(out0, out1, dst[row] + (col << 1), 16);

      /* Prepare for next 16 width column */
      tmp_l = cur;
      cur = tmp_r;
      tmp_r = LD_SB(src[row] + col + 32);
    }
    /* Re-substitute last pixel */
    *(dst[row] + 2 * width - 1) = *(src[row] + width - 1);
  }
}

void
jsimd_h2v1_fancy_upsample_msa (int max_v_samp_factor,
                               JDIMENSION downsampled_width,
                               JSAMPARRAY input_data,
                               JSAMPARRAY * output_data_ptr)
{
  image_upsample_fancy_h2v1_msa(input_data, *output_data_ptr,
                                downsampled_width, max_v_samp_factor);
}
