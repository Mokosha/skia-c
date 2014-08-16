/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef skia_DEFINED
#define skia_DEFINED

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdint.h>  /* int32_t */
#include <stdlib.h>  /* size_t */

#ifndef SKIA_IMPLEMENTATION
/* the following are opaque types. */
typedef struct sk_surface_struct sk_surface_t;
typedef struct sk_image_struct sk_image_t;
typedef struct sk_data_struct sk_data_t;
typedef struct sk_render_target_struct sk_render_target_t;
typedef struct sk_path_struct sk_path_t;
typedef struct sk_paint_struct sk_paint_t;

#define SK_API
#endif  /* SKIA_IMPLEMENTATION */

/* Simple structs. */
typedef struct { float left, top, right, bottom; } sk_rect_t;
typedef struct { int32_t left, top, right, bottom; } sk_irect_t;
typedef struct { float width, height; } sk_size_t;
typedef struct { int32_t width, height; } sk_isize_t;

typedef void (*sk_release_procedure_t)(void* address, void* context);
typedef void (*sk_length_release_procedure_t)(const void* address,
                                              size_t length,
                                              void* context);

typedef uint32_t sk_color_t;

typedef enum {
    sk_native32_color_type = 1
} sk_color_type_t;

typedef enum {
    sk_no_error = 0,
    sk_other_error = 1,
    sk_null_pointer_error = 2,
} sk_error_t;

SK_API sk_error_t sk_surface_ref(sk_surface_t*);
SK_API sk_error_t sk_surface_unref(sk_surface_t*);

SK_API sk_error_t sk_image_ref(sk_image_t*);
SK_API sk_error_t sk_image_unref(sk_image_t*);

SK_API sk_error_t sk_data_ref(sk_data_t*);
SK_API sk_error_t sk_data_unref(sk_data_t*);

SK_API sk_error_t sk_paint_ref(sk_paint_t*);
SK_API sk_error_t sk_paint_unref(sk_paint_t*);

/*
  In general, any function that returns a pointer should have the
  corresponding TYPE_unref() function called to release memory.  If
  TYPE_ref() is called N times, TYPE_unref() should be called N+1
  times.
*/

/* return NULL on error */
SK_API sk_surface_t* sk_new_raster_surface(sk_isize_t size);

SK_API sk_surface_t* sk_new_raster_direct_surface(sk_isize_t size,
                                                  sk_color_type_t colorType,
                                                  void* pixelAddress,
                                                  size_t rowBytes,
                                                  sk_release_procedure_t proc,
                                                  void* context);

SK_API sk_surface_t* sk_new_picture_surface(sk_isize_t size);

SK_API sk_surface_t* sk_new_render_target_direct_surface(sk_render_target_t*);

SK_API sk_image_t* sk_new_image_snapshot(sk_surface_t*);

SK_API sk_data_t* sk_new_data(const void* data,
                              size_t size,
                              sk_length_release_procedure_t proc,
                              void* context);

SK_API sk_paint_t* sk_new_paint();
SK_API sk_paint_t* sk_new_paint_copy(sk_paint_t*);

/* These functions will fail (and return NULL) if the apropriate */
/* libraries aren't linked to libskia. */
SK_API sk_data_t* sk_new_encoded_bmp(const sk_image_t*);
SK_API sk_data_t* sk_new_encoded_gif(const sk_image_t*);
SK_API sk_data_t* sk_new_encoded_jpeg(const sk_image_t*, int quality);
SK_API sk_data_t* sk_new_encoded_png(const sk_image_t*);
SK_API sk_data_t* sk_new_encoded_webp(const sk_image_t*, int quality);

SK_API sk_error_t sk_image_peek_pixels(const sk_image_t*,
                                       sk_isize_t *sizeOut,
                                       sk_color_type_t* colorTypeOut,
                                       size_t* rowBytesOut,
                                       const void** pixelsOut);

SK_API sk_error_t sk_surface_get_size(const sk_surface_t*, sk_isize_t* sizeOut);

SK_API sk_error_t sk_image_get_size(const sk_image_t*, sk_isize_t* sizeOut);

SK_API sk_error_t sk_data_get_data(sk_data_t*, const void**, size_t*);

/* skpaint */

SK_API sk_error_t sk_paint_reset(sk_paint_t*);
SK_API sk_error_t sk_paint_get_color(const sk_paint_t*, sk_color_t* colorOut);
SK_API sk_error_t sk_paint_set_color(sk_paint_t*, sk_color_t);

/* TODO:  all other paint gettrs and setters. */



/* The following are canvas draw commands  */

SK_API sk_error_t sk_flush(sk_surface_t* sk_surface);

SK_API sk_error_t sk_save_matrix_and_clip(sk_surface_t* sk_surface,
                                          int* saveCount);
SK_API sk_error_t sk_save_matrix(sk_surface_t* sk_surface, int* saveCount);
SK_API sk_error_t sk_save_clip(sk_surface_t* sk_surface, int* saveCount);
SK_API sk_error_t sk_restore(sk_surface_t* sk_surface);
SK_API sk_error_t sk_restore_to_count(sk_surface_t* sk_surface, int saveCount);

SK_API sk_error_t sk_translate(sk_surface_t* sk_surface, float dx, float dy);
SK_API sk_error_t sk_scale(sk_surface_t* sk_surface, float sx, float sy);
SK_API sk_error_t sk_rotate(sk_surface_t* sk_surface, float degrees);

SK_API sk_error_t sk_clip_rect(sk_surface_t* sk_surface, sk_rect_t rect);
SK_API sk_error_t sk_draw_paint(sk_surface_t* sk_surface, sk_paint_t* sk_paint);

/* all the other canvas operations */

/* SkPaint?  SkPath? */

SK_API sk_color_t sk_color_from_argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
SK_API uint8_t sk_color_get_a(sk_color_t color);
SK_API uint8_t sk_color_get_r(sk_color_t color);
SK_API uint8_t sk_color_get_g(sk_color_t color);
SK_API uint8_t sk_color_get_b(sk_color_t color);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif  /* skia_DEFINED */


