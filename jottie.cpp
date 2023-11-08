/**
 * ==============================================================================
 *
 * This file is part of the `jottie` library.
 *
 * Standalone JUCE module https://github.com/kunitoki/jottie
 * Copyright (c) 2023 Lucio Asnaghi
 *
 * Originally created in HISE https://github.com/christophhart/HISE/tree/master/hi_rlottie
 * Copyright (c) 2019 Christoph Hart
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ==============================================================================
 */

#ifdef JOTTIE_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JOTTIE cpp file"
#endif

#if !defined (JOTTIE_MODULE_SKIP_BUILD)

#include "jottie.h"

#if (defined (_WIN32) || defined (_WIN64))
#pragma warning (push)
#pragma warning (disable : 4245)
#pragma warning (disable : 4251)
#pragma warning (disable : 4267)
#pragma warning (disable : 4334)
#pragma warning (disable : 4456)
#pragma warning (disable : 4458)
#pragma warning (disable : 4505)
#pragma warning (disable : 4706)
#pragma warning (disable : 4702)
#pragma warning (disable : 4701)
#pragma warning (disable : 4611)
#pragma warning (disable : 4838)
#pragma warning (disable : 4996)
#elif defined (__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#pragma clang diagnostic ignored "-Wfloat-conversion"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wextra-semi"
#pragma clang diagnostic ignored "-Wcomma"
#endif

#include "rlottie/src/lottie/zip/zip.cpp"
#include "rlottie/src/lottie/lottieitem.cpp"
#include "rlottie/src/lottie/lottieitem_capi.cpp"
#include "rlottie/src/lottie/lottieloader.cpp"
#include "rlottie/src/lottie/lottiemodel.cpp"
#include "rlottie/src/lottie/lottieproxymodel.cpp"
#include "rlottie/src/lottie/lottieparser.cpp"
#include "rlottie/src/lottie/lottieanimation.cpp"
#include "rlottie/src/lottie/lottiekeypath.cpp"
#include "rlottie/src/vector/vrect.cpp"
#include "rlottie/src/vector/vdasher.cpp"
#include "rlottie/src/vector/vbrush.cpp"
#include "rlottie/src/vector/vbitmap.cpp"
#include "rlottie/src/vector/vpainter.cpp"
#include "rlottie/src/vector/vdrawhelper_common.cpp"
#include "rlottie/src/vector/vdrawhelper.cpp"
#include "rlottie/src/vector/vdrawhelper_sse2.cpp"
#include "rlottie/src/vector/vdrawhelper_neon.cpp"
#include "rlottie/src/vector/vrle.cpp"
#include "rlottie/src/vector/vpath.cpp"
#include "rlottie/src/vector/vpathmesure.cpp"
#include "rlottie/src/vector/vmatrix.cpp"
#include "rlottie/src/vector/velapsedtimer.cpp"
#include "rlottie/src/vector/vdebug.cpp"
#include "rlottie/src/vector/vinterpolator.cpp"
#include "rlottie/src/vector/vbezier.cpp"
#include "rlottie/src/vector/vraster.cpp"
#include "rlottie/src/vector/vdrawable.cpp"
#include "rlottie/src/vector/vimageloader.cpp"
#include "rlottie/src/vector/varenaalloc.cpp"
#include "rlottie/src/vector/stb/stb_image.cpp"
#include "rlottie/src/vector/freetype/v_ft_math.cpp"
#include "rlottie/src/vector/freetype/v_ft_raster.cpp"
#include "rlottie/src/vector/freetype/v_ft_stroker.cpp"
#include "rlottie/src/binding/c/lottieanimation_capi.cpp"

#if (defined (_WIN32) || defined (_WIN64))
#pragma warning (pop)
#elif defined (__clang__)
#pragma clang diagnostic pop
#endif

#include "classes/jottie_LottieComponent.cpp"
#include "classes/jottie_LottieAnimation.cpp"
#include "classes/jottie_LottieFile.cpp"

#endif
