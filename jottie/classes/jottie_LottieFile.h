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

#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "jottie_LottieAnimation.h"

#include <unordered_map>
#include <memory>

namespace jottie {

//==============================================================================
/**
 * @brief A class representing a Lottie animation file with multiple animations.
 *
 * The LottieFile class is used to manage and manipulate Lottie animation files. It provides functionality to load,
 * clear, and close animations within the file.
 *
 * To easily create a dotLottie file from a Lottie animation json, it's possible to use this web service:
 *   https://lottiefiles.com/tools/lottie-to-dotlottie?utm_source=dotlottieio
 *
 * @see LottieAnimation
 */
class LottieFile : public juce::ReferenceCountedObject
{
public:
    //==============================================================================
    /**
     * @brief A smart pointer to a LottieFile object.
     */
    using Ptr = juce::ReferenceCountedObjectPtr<LottieFile>;

    //==============================================================================
    /**
     * @brief Open a Lottie file from a given file path.
     *
     * @param lottieFile The file path to the Lottie file.
     * @return A smart pointer to the opened LottieFile, or nullptr if the file cannot be opened.
     */
    static LottieFile::Ptr open (const juce::File& lottieFile);

    /**
     * @brief Open a Lottie file from a given input stream.
     *
     * @param inputStream A unique pointer to the input stream containing the Lottie data.
     * @return A smart pointer to the opened LottieFile, or nullptr if the input stream is invalid.
     */
    static LottieFile::Ptr open (std::unique_ptr<juce::InputStream> inputStream);

    //==============================================================================
    /**
     * @brief Close the Lottie file.
     */
    void close();

    //==============================================================================
    /**
     * @brief Get the number of animations in the Lottie file.
     *
     * @return The number of animations.
     */
    int getNumAnimations() const;

    /**
     * @brief Get the id of animations in the Lottie file.
     *
     * @param index The index of the animation to get the name of.
     *
     * @return The id of the animation requested at the specific index.
     */
    juce::String getAnimationId (int index) const;

    //==============================================================================
    /**
     * @brief Load a specific animation from the file by index.
     *
     * If the animation was previously loaded and it's currently in the internal cache, it will be returned instead of
     * being loaded again from the Lottie file.
     *
     * @param index The index of the animation to load.
     *
     * @return A smart pointer to the loaded LottieAnimation.
     */
    LottieAnimation::Ptr loadAnimation(int index);

    /**
     * @brief Load a specific animation from the file by animation ID.
     *
     * If the animation was previously loaded and it's currently in the internal cache, it will be returned instead of
     * being loaded again from the Lottie file.
     *
     * @param animationId The ID of the animation to load.
     *
     * @return A smart pointer to the loaded LottieAnimation, or nullptr if not found.
     */
    LottieAnimation::Ptr loadAnimation(juce::StringRef animationId);

    /**
     * @brief Load all animations from and put them in the cache.
     */
    void loadAllAnimations();

    //==============================================================================
    /**
     * @brief Clear a specific cached animation by animation ID.
     *
     * @param animationId The ID of the animation to clear from the cache.
     */
    void clearAnimation(juce::StringRef animationId);

    /**
     * @brief Clear all animations from the internal cache.
     */
    void clearAllAnimations();

private:
    LottieFile (const juce::File& lottieFile,
                std::unique_ptr<juce::ZipFile> lottieZip,
                juce::StringArray animationIds);

    LottieFile (std::unique_ptr<juce::InputStream> lottieStream,
                std::unique_ptr<juce::ZipFile> lottieZip,
                juce::StringArray animationIds);

    std::unique_ptr<juce::ZipFile> openZipFile();

    juce::File lottieFile;
    std::unique_ptr<juce::InputStream> lottieStream;
    std::unique_ptr<juce::ZipFile> lottieZip;
    juce::StringArray animationIds;
    std::unordered_map<juce::String, LottieAnimation::Ptr> cachedAnimations;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LottieFile)
};

} // namespace jottie
