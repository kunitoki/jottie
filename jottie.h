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

/*******************************************************************************

 BEGIN_JUCE_MODULE_DECLARATION

  ID:               jottie
  vendor:           kunitoki
  version:          1.0.0
  name:             jottie
  description:      jottie is a juce port of rLottie.
  website:          https://github.com/Samsung/rlottie
  license:          MIT

  dependencies:     juce_core juce_graphics juce_events juce_gui_basics
  OSXFrameworks:    Accelerate
  iOSFrameworks:    Accelerate

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once
#define JOTTIE_H_INCLUDED

//==============================================================================
/** Config: JOTTIE_ENABLE_MODEL_CACHE
    If this option is turned on, the low level rLottie library will cache loaded animation models.
*/
#if !defined (JOTTIE_ENABLE_MODEL_CACHE)
 #define JOTTIE_ENABLE_MODEL_CACHE 1
#endif

//==============================================================================
/** Config: JOTTIE_ENABLE_LOGGING
    If this option is turned on, the low level rLottie library will produce logging output, useful for debugging.
*/
#if !defined (JOTTIE_ENABLE_LOGGING)
 #define JOTTIE_ENABLE_LOGGING 0
#endif

//==============================================================================
/** Config: JOTTIE_ENABLE_DUMP_TREE
    If this option is turned on, the low level rLottie library will dump animation tree information, useful for debugging.
*/
#if !defined (JOTTIE_ENABLE_DUMP_TREE)
 #define JOTTIE_ENABLE_DUMP_TREE 0
#endif

//==============================================================================

#if !defined (JOTTIE_MODULE_SKIP_BUILD)

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "classes/jottie_LottieComponent.h"
#include "classes/jottie_LottieAnimation.h"
#include "classes/jottie_LottieFile.h"

#endif
