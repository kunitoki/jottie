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

#include "../rlottie/inc/rlottie_capi.h"

namespace jottie {

//==============================================================================
/**
 * @brief Represents a Lottie animation for rendering and customization.
 *
 * The `LottieAnimation` class provides functionality to load and render Lottie animations, as well as the
 * ability to customize various animation properties, like fill color, stroke color, opacity, etc.
 */
class LottieAnimation : public juce::ReferenceCountedObject
{
public:
    //==============================================================================
    /**
     * @brief A reference counted pointer to a `LottieAnimation` instance.
     */
    using Ptr = juce::ReferenceCountedObjectPtr<LottieAnimation>;

    //==============================================================================
    /**
     * @brief Enumerates the properties that can be customized in the Lottie animation.
     */
    enum class Property
    {
        FillColor           = LOTTIE_ANIMATION_PROPERTY_FILLCOLOR,
        FillOpacity         = LOTTIE_ANIMATION_PROPERTY_FILLOPACITY,
        StrokeColor         = LOTTIE_ANIMATION_PROPERTY_STROKECOLOR,
        StrokeOpacity       = LOTTIE_ANIMATION_PROPERTY_STROKEOPACITY,
        StrokeWidth         = LOTTIE_ANIMATION_PROPERTY_STROKEWIDTH,
        TransformAnchor     = LOTTIE_ANIMATION_PROPERTY_TR_ANCHOR,
        TransformPosition   = LOTTIE_ANIMATION_PROPERTY_TR_POSITION,
        TransformScale      = LOTTIE_ANIMATION_PROPERTY_TR_SCALE,
        TransformRotation   = LOTTIE_ANIMATION_PROPERTY_TR_ROTATION,
        TransformOpacity    = LOTTIE_ANIMATION_PROPERTY_TR_OPACITY
    };

    //==============================================================================
    /**
     * @brief Constructs a `LottieAnimation` from Lottie animation data.
     *
     * @param data A string containing Lottie animation data.
     */
    LottieAnimation (const juce::String& data);

    /**
     * @brief Destroys the `LottieAnimation` instance.
     */
    ~LottieAnimation();

    //==============================================================================
    /**
     * @brief Checks if the Lottie animation is valid and properly loaded.
     *
     * @return True if the animation is valid, false otherwise.
     */
    bool isValid() const;

    //==============================================================================
    /**
     * @brief Sets the size of the animation in pixels.
     *
     * @param width  The width of the animation.
     * @param height The height of the animation.
     */
    void setSize (int width, int height);

    /**
     * @brief Sets the size of the animation using a `juce::Rectangle`.
     *
     * @param size A `juce::Rectangle` specifying the width and height of the animation.
     */
    void setSize (const juce::Rectangle<int>& size);

    /**
     * @brief Gets the current size of the animation.
     *
     * @return A `juce::Rectangle` representing the size of the animation.
     */
    juce::Rectangle<int> getSize() const;

    /**
     * @brief Gets the scaled size of the animation, considering the scale factor.
     *
     * @return A `juce::Rectangle` representing the scaled size of the animation.
     */
    juce::Rectangle<int> getScaledSize() const;

    /**
     * @brief Gets the original size of the animation (before any scaling).
     *
     * @return A `juce::Rectangle` representing the original size of the animation.
     */
    juce::Rectangle<int> getOriginalSize() const;

    //==============================================================================
    /**
     * @brief Sets the scale factor for rendering the animation.
     *
     * @param newScaleFactor The new scale factor.
     */
    void setScaleFactor (float newScaleFactor);

    /**
     * @brief Gets the current scale factor.
     *
     * @return The scale factor for rendering the animation.
     */
    float getScaleFactor() const;

    //==============================================================================
    /**
     * @brief Gets the total number of frames in the animation.
     *
     * @return The number of frames in the animation.
     */
    int getNumFrames() const;

    /**
     * @brief Gets the frame rate of the animation.
     *
     * @return The frame rate of the animation in frames per second.
     */
    double getFrameRate() const;

    /**
     * @brief Sets the current frame to be displayed.
     *
     * @param frameNumber The frame number to set.
     */
    void setFrame (int frameNumber);

    /**
     * @brief Gets the current frame number being displayed.
     *
     * @return The current frame number.
     */
    int getCurrentFrame() const;

    //==============================================================================
    /**
     * @brief Overrides a property of the Lottie animation for a specific key path with a color value.
     *
     * @param property The property to override.
     * @param keyPath  The key path specifying which part of the animation to override.
     * @param color    The color value to set.
     *
     * @return A `juce::Result` indicating the success of the operation.
     */
    juce::Result setPropertyOverride (Property property, const juce::String& keyPath, const juce::Colour& color);

    /**
     * @brief Overrides a property of the Lottie animation for a specific key path with a float value.
     *
     * @param property The property to override.
     * @param keyPath  The key path specifying which part of the animation to override.
     * @param value    The float value to set.
     *
     * @return A `juce::Result` indicating the success of the operation.
     */
    juce::Result setPropertyOverride (Property property, const juce::String& keyPath, float value);

    /**
     * @brief Overrides a property of the Lottie animation for a specific key path with a range of integers.
     *
     * @param property The property to override.
     * @param keyPath  The key path specifying which part of the animation to override.
     * @param range    The range of integers to set.
     *
     * @return A `juce::Result` indicating the success of the operation.
     */
    juce::Result setPropertyOverride (Property property, const juce::String& keyPath, const juce::Range<int>& range);

    /**
     * @brief Overrides a property of the Lottie animation for a specific key path with a 2D point.
     *
     * @param property The property to override.
     * @param keyPath  The key path specifying which part of the animation to override.
     * @param point    The 2D point to set.
     *
     * @return A `juce::Result` indicating the success of the operation.
     */
    juce::Result setPropertyOverride (Property property, const juce::String& keyPath, const juce::Point<float>& point);

    //==============================================================================
    /**
     * @brief Renders the Lottie animation on a `juce::Graphics` context at the specified position.
     *
     * @param g        The `juce::Graphics` context to render the animation on.
     * @param topLeft  The top-left position at which to render the animation.
     */
    void render (juce::Graphics& g, juce::Point<int> topLeft);

    /**
     * @brief Renders the Lottie animation on a `juce::Graphics` context with a specific transformation.
     *
     * @param g        The `juce::Graphics` context to render the animation on.
     * @param transform  The transformation to apply the animation rendering.
     */
    void render (juce::Graphics& g, const juce::AffineTransform& transform);

private:
    bool canRenderCurrentFrame() const;
    void renderCurrentFrame();

    Lottie_Animation* animation = nullptr;

    int originalWidth = 0;
    int originalHeight = 0;
    float scaleFactor = 1.0f;
    int lastFrame = -1;
    int currentFrame = 0;
    int numFrames = 0;
    double frameRate = 0.0;

    juce::Image canvas;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LottieAnimation)
};

} // namespace jottie
