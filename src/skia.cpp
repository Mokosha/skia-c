/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkSurface.h"
#include "SkImage.h"
#include "SkData.h"
#include "SkCanvas.h"
#include "SkColor.h"
#include "SkPaint.h"

class sk_paint_t : public SkRefCnt, public SkPaint {
public:
    sk_paint_t() { }
    explicit sk_paint_t(const sk_paint_t& paint) : SkPaint(paint) { }
};

typedef SkSurface sk_surface_t;
typedef SkImage sk_image_t;
typedef SkData sk_data_t;
typedef GrRenderTarget sk_render_target_t;

struct sk_path_t; // TODO

#define SKIA_IMPLEMENTATION 1
#include "../include/skia.h"

sk_error_t sk_surface_ref(sk_surface_t* sk_surface) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    sk_surface->ref();
    return sk_no_error;
}

sk_error_t sk_surface_unref(sk_surface_t* sk_surface) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    sk_surface->unref();
    return sk_no_error;
}

sk_error_t sk_image_ref(sk_image_t* sk_image) {
    if (!sk_image) {
        return sk_null_pointer_error;
    }
    sk_image->ref();
    return sk_no_error;

}

sk_error_t  sk_image_unref(sk_image_t* sk_image) {
    if (!sk_image) {
        return sk_null_pointer_error;
    }
    sk_image->unref();
    return sk_no_error;
}

sk_error_t  sk_data_ref(sk_data_t* sk_data) {
    if (!sk_data) {
        return sk_null_pointer_error;
    }
    sk_data->ref();
    return sk_no_error;
}

sk_error_t sk_data_unref(sk_data_t* sk_data) {
    if (!sk_data) {
        return sk_null_pointer_error;
    }
    sk_data->unref();
    return sk_no_error;
}

sk_error_t sk_paint_ref(sk_paint_t* sk_paint) {
    if (!sk_paint) {
        return sk_null_pointer_error;
    }
    sk_paint->ref();
    return sk_no_error;
}

sk_error_t sk_paint_unref(sk_paint_t* sk_paint) {
    if (!sk_paint) {
        return sk_null_pointer_error;
    }
    sk_paint->unref();
    return sk_no_error;
}

sk_surface_t* sk_new_raster_surface(sk_isize_t size) {
    SkImageInfo info = SkImageInfo::MakeN32Premul(size.width, size.height);
    return SkSurface::NewRaster(info);
}

sk_surface_t* sk_new_raster_direct_surface(sk_isize_t size,
                                           sk_color_type_t colorType,
                                           void* pixelAddress,
                                           size_t rowBytes,
                                           sk_release_procedure_t proc,
                                           void* context) {
    return NULL;  // not implemented yet.
}

sk_surface_t* sk_new_picture_surface(sk_isize_t size) {
    return NULL;  // not implemented yet.
}

sk_surface_t* sk_new_render_target_direct_surface(
    sk_render_target_t* sk_render_target) {
    return SkSurface::NewRenderTargetDirect(sk_render_target);
}

sk_image_t* sk_new_image_snapshot(sk_surface_t* sk_surface) {
    if (!sk_surface) {
        return NULL;
    }
    return sk_surface->newImageSnapshot();
}

sk_data_t* sk_new_data(const void* data,
                       size_t size,
                       sk_length_release_procedure_t proc,
                       void* context) {
    if (!data) {
        return NULL;
    }
    return SkData::NewWithProc(data, size, proc, context);
}

sk_paint_t* sk_new_paint()  {
    return SkNEW(sk_paint_t);
}

sk_paint_t* sk_copy_paint(sk_paint_t* sk_paint)  {
    if (sk_paint) {
        return SkNEW_ARGS(sk_paint_t, (*sk_paint));
    }
    return SkNEW(sk_paint_t);
}


sk_data_t* sk_new_encoded_bmp(const sk_image_t* sk_image)  {
    return NULL;
}

sk_data_t* sk_new_encoded_gif(const sk_image_t* sk_image)  {
    if (sk_image) {
        return sk_image->encode(SkImageEncoder::kGIF_Type, 100);
    }
    return NULL;
}

sk_data_t* sk_new_encoded_jpeg(const sk_image_t* sk_image, int quality)  {
    if (sk_image) {
        return sk_image->encode(SkImageEncoder::kJPEG_Type, quality);
    }
    return NULL;
}

sk_data_t* sk_new_encoded_png(const sk_image_t* sk_image)  {
    if (sk_image) {
        return sk_image->encode(SkImageEncoder::kPNG_Type, 100);
    }
    return NULL;
}

sk_data_t* sk_new_encoded_webp(const sk_image_t* sk_image, int quality)  {
    if (sk_image) {
        return sk_image->encode(SkImageEncoder::kWEBP_Type, quality);
    }
    return NULL;
}

sk_error_t sk_image_peek_pixels(const sk_image_t* sk_image,
                                sk_isize_t *sizeOut,
                                sk_color_type_t* colorTypeOut,
                                size_t* rowBytesOut,
                                const void** pixelsOut) {
    if (!(sk_image && sizeOut && colorTypeOut && rowBytesOut && pixelsOut)) {
        return sk_null_pointer_error;
    }
    SkImageInfo info;
    size_t rowBytes;
    const void* pixels = sk_image->peekPixels(&info, &rowBytes);
    if ((info.fColorType == kN32_SkColorType)
        && (info.fAlphaType == kPremul_SkAlphaType)) {
        *sizeOut = (sk_isize_t){info.fWidth, info.fHeight};
        *pixelsOut = pixels;
        *colorTypeOut = sk_native32_color_type;
        *rowBytesOut = rowBytes;
        return sk_no_error;
    }
    return sk_other_error;
}


sk_error_t sk_surface_get_size(const sk_surface_t* sk_surface,
                               sk_isize_t* sizeOut) {
    if (!(sk_surface && sizeOut)) {
        return sk_null_pointer_error;
    }
    *sizeOut = (sk_isize_t){sk_surface->width(), sk_surface->height()};
    return sk_no_error;
}

sk_error_t sk_image_get_size(const sk_image_t* sk_image, sk_isize_t* sizeOut) {
    if (!(sk_image && sizeOut)) {
        return sk_null_pointer_error;
    }
    *sizeOut = (sk_isize_t){sk_image->width(), sk_image->height()};
    return sk_no_error;
}

////////////////////////////////////////////////////////////////////////////////

sk_error_t sk_data_get_data(sk_data_t* sk_data, const void** data, size_t* size) {
    if (!sk_data) {
        return sk_null_pointer_error;
    }
    if (data) {
        *data = sk_data->data();
    }
    if (size) {
        *size = sk_data->size();
    }
    return sk_no_error;
}

sk_error_t sk_paint_reset(sk_paint_t* sk_paint)  {
    if (!sk_paint) {
        return sk_null_pointer_error;
    }
    sk_paint->reset();
    return sk_no_error;
}

sk_color_t sk_paint_get_color(sk_paint_t* sk_paint, sk_color_t* colorOut) {
    if (!(sk_paint && colorOut)) {
        return sk_null_pointer_error;
    }
    *colorOut = sk_paint->getColor();
    return sk_no_error;
}

sk_error_t sk_paint_set_color(sk_paint_t* sk_paint, sk_color_t color)  {
    if (!sk_paint) {
        return sk_null_pointer_error;
    }
    sk_paint->setColor(color);
    return sk_no_error;
}

// The following are canvas draw commands

sk_error_t sk_flush(sk_surface_t* sk_surface) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->flush();
    return sk_no_error;
}

sk_error_t sk_save_matrix_and_clip(sk_surface_t* sk_surface,
                                   int* saveCount) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    int count = sk_surface->getCanvas()->save();
    if (saveCount) {
        *saveCount = count;
    }
    return sk_no_error;
}

sk_error_t sk_restore(sk_surface_t* sk_surface) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->restore();
    return sk_no_error;
}
sk_error_t sk_restore_to_count(sk_surface_t* sk_surface, int saveCount) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->restoreToCount(saveCount);
    return sk_no_error;
}

sk_error_t sk_translate(sk_surface_t* sk_surface, float dx, float dy) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->translate(dx, dy);
    return sk_no_error;
}

sk_error_t sk_scale(sk_surface_t* sk_surface, float sx, float sy) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->scale(sx, sy);
    return sk_no_error;
}

sk_error_t sk_rotate(sk_surface_t* sk_surface, float degrees) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->rotate(degrees);
    return sk_no_error;
}

static inline SkRect to_rect(sk_rect_t rect) {
    return SkRect::MakeLTRB(rect.left, rect.top, rect.right, rect.bottom);
}
sk_error_t sk_clip_rect(sk_surface_t* sk_surface, sk_rect_t rect) {
    if (!sk_surface) {
        return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->clipRect(to_rect(rect));
    return sk_no_error;
}
sk_error_t sk_draw_paint(sk_surface_t* sk_surface, sk_paint_t* sk_paint) {
    if (!(sk_surface && sk_paint)) {
        return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->drawPaint(*sk_paint);
    return sk_no_error;
}

////////////////////////////////////////////////////////////////////////////////

// all the other canvas operations

// SkPaint?  SkPath?

sk_color_t sk_color_from_argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b)  {
    return SkColorSetARGBMacro(a, r, g, b);
}

uint8_t sk_color_get_a(sk_color_t color)  {
    return SkColorGetA(color);
}

uint8_t sk_color_get_r(sk_color_t color)  {
    return SkColorGetR(color);
}

uint8_t sk_color_get_g(sk_color_t color)  {
    return SkColorGetG(color);
}

uint8_t sk_color_get_b(sk_color_t color)  {
    return SkColorGetB(color);
}
