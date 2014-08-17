/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
  SKIA_SRC_DIR=.....
  export SKIA_OUT=......
  export GYP_DEFINES=skia_shared_lib=1
  export GYP_GENERATORS="ninja"
  $SKIA_SRC_DIR/gyp_skia
  ninja -C $SKIA_OUT skia_lib
  cc -L $SKIA_OUT/Debug/lib -lskia \
    $SKIA_SRC_DIR/tests/test_libskia.c -o test_libskia
  LD_LIBRARY_PATH=$SKIA_OUT/Debug/lib ./test_libskia
  open test_libskia_out.png
*/

#include <stdio.h>
#include "../include/skia.h"

void dragon_recursive(sk_surface_t* canvas, sk_paint_t* paint, int iteration, int zig) {
    sk_save_matrix_and_clip(canvas, NULL);
    if (iteration < 1) {
        const sk_color_t colors[]
            = {0xFF0F9D58, 0xFFDB4437, 0xFF4285F4, 0xFFF4B400};
        sk_rect_t rect = {0.0, 0.0, 1.0, 1.0};
        sk_paint_set_color(paint, colors[rand() % 4 ]);
        sk_clip_rect(canvas, rect);
        sk_draw_paint(canvas, paint);
    } else {
        sk_scale(canvas, 0.707106781, 0.707106781);
        sk_rotate(canvas, (zig ? -45.0 : 45.0));
        dragon_recursive(canvas, paint, iteration - 1, 0);
        sk_rotate(canvas, 45.0);
        sk_translate(canvas, 0.707106781, 0.0);
        sk_rotate(canvas, (zig ? 45.0 : -135));
        dragon_recursive(canvas, paint, iteration - 1, 1);
    }
    sk_restore(canvas);
}
void dragon(sk_surface_t* canvas, sk_paint_t* paint, int width, int height) {
    float W = ((width < height) ? width : height);
    sk_save_matrix_and_clip(canvas, NULL);
    sk_translate(canvas, width * 0.4, height * 0.25);
    sk_scale(canvas, W, W);
    dragon_recursive(canvas, paint, 16, 0);
    sk_restore(canvas);
}


int main() {
    int return_value = 1;
    sk_surface_t* sk_surface = NULL;
    sk_paint_t* sk_paint = NULL;
    sk_image_t* sk_image = NULL;
    sk_data_t* sk_data = NULL;
    FILE* out_file = NULL;
    const void* data = NULL;
    size_t size = 0;
    sk_isize_t surface_size = {1024, 768};

    sk_surface = sk_new_raster_surface(surface_size);
    if (!sk_surface) {
        goto done;
    }

    sk_paint = sk_new_paint();
    if (!sk_paint) {
        goto done;
    }

    sk_paint_set_color(sk_paint, sk_color_from_argb(255, 255, 255, 255));

    sk_draw_paint(sk_surface, sk_paint);

    dragon(sk_surface, sk_paint, 1024, 768);

    /* sk_paint_set_color(sk_paint, sk_color_from_argb(255, 255, 0, 0)); */
    /* sk_clip_rect(sk_surface, sk_rect(256,192,768,576)); */
    /* sk_draw_paint(sk_surface, sk_paint); */

    sk_image = sk_new_image_snapshot(sk_surface);
    if (!sk_image) {
        goto done;
    }
    sk_data = sk_new_encoded_png(sk_image);
    if (!sk_data) {
        goto done;
    }

    sk_data_get_data(sk_data, &data, &size);

    out_file = fopen("test_libskia_out.png", "w");
    if (!out_file) {
        goto done;
    }
    if (1 != fwrite(data, size, 1, out_file)) {
        goto done;
    }
    return_value = 0;
done:
    if (out_file)
        fclose(out_file);
    sk_paint_unref(sk_paint);
    sk_data_unref(sk_data);
    sk_image_unref(sk_image);
    sk_surface_unref(sk_surface);
    return return_value;
}
