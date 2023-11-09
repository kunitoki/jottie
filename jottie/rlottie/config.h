/**
 * ==============================================================================
 *
 * This file is part of the `jottie` library.
 *
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

#pragma once

#include "../jottie.h"

// rLottie options
#if JOTTIE_ENABLE_THREAD_SUPPORT
  #define LOTTIE_THREAD_SUPPORT 1
#endif

#if JOTTIE_ENABLE_MODEL_CACHE
  #define LOTTIE_CACHE_SUPPORT 1
#endif

#if JOTTIE_ENABLE_LOGGING_SUPPORT
  #define LOTTIE_LOGGING_SUPPORT 1
#endif

#if JOTTIE_ENABLE_DUMP_TREE
  #define LOTTIE_DUMP_TREE_SUPPORT 1
#endif

