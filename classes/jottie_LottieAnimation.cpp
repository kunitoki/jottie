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

#include "jottie_LottieAnimation.h"

#include <cstdint>

namespace jottie {
namespace {

//==============================================================================
Lottie_Animation* createAnimation (const juce::String& jsonData)
{
    const auto hash = juce::String (jsonData.hashCode());

    return lottie_animation_from_data (jsonData.getCharPointer(), hash.getCharPointer(), "/");
}

void destroyAnimation (Lottie_Animation* animation)
{
    if (animation != nullptr)
        lottie_animation_destroy (animation);
}

int getAnimationNumFrames (Lottie_Animation* animation)
{
    return (animation != nullptr) ? static_cast<int> (lottie_animation_get_totalframe (animation)) : 0;
}

double getAnimationFrameRate(Lottie_Animation* animation)
{
    return (animation != nullptr) ? lottie_animation_get_framerate (animation) : 0.0;
}

juce::Rectangle<int> getAnimationSize (Lottie_Animation* animation)
{
    std::size_t width = 0;
    std::size_t height = 0;

    if (animation != nullptr)
        lottie_animation_get_size (animation, std::addressof (width), std::addressof (height));

    return juce::Rectangle<int>{ 0, 0, static_cast<int> (width), static_cast<int> (height) };
}

void renderAnimationToImage (Lottie_Animation* animation, juce::Image& image, int currentFrame)
{
    if (animation == nullptr || ! image.isValid())
        return;

    juce::Image::BitmapData bitmapData (image, juce::Image::BitmapData::ReadWriteMode::writeOnly);

    lottie_animation_render (animation,
                             static_cast<std::size_t> (currentFrame),
                             reinterpret_cast<uint32_t*> (bitmapData.data),
                             static_cast<std::size_t> (bitmapData.width),
                             static_cast<std::size_t> (bitmapData.height),
                             static_cast<std::size_t> (bitmapData.lineStride));
}

juce::Result setAnimationPropertyOverride (Lottie_Animation* animation, LottieAnimation::Property property, const juce::String& keyPath, const juce::Colour& color)
{
    if (animation == nullptr)
        return juce::Result::fail ("Invalid animation");

    // TODO - test properties

    lottie_animation_property_override(animation,
                                       static_cast<Lottie_Animation_Property> (property),
                                       keyPath.toRawUTF8(),
                                       color.getFloatRed(),
                                       color.getFloatGreen(),
                                       color.getFloatBlue());

    return juce::Result::ok();
}

juce::Result setAnimationPropertyOverride (Lottie_Animation* animation, LottieAnimation::Property property, const juce::String& keyPath, float value)
{
    if (animation == nullptr)
        return juce::Result::fail ("Invalid animation");

    // TODO - test properties

    lottie_animation_property_override(animation,
                                       static_cast<Lottie_Animation_Property> (property),
                                       keyPath.toRawUTF8(),
                                       value);

    return juce::Result::ok();
}

juce::Result setAnimationPropertyOverride (Lottie_Animation* animation, LottieAnimation::Property property, const juce::String& keyPath, const juce::Range<int>& range)
{
    if (animation == nullptr)
        return juce::Result::fail ("Invalid animation");

    // TODO - test properties

    lottie_animation_property_override(animation,
                                       static_cast<Lottie_Animation_Property> (property),
                                       keyPath.toRawUTF8(),
                                       range.getStart(),
                                       range.getEnd());

    return juce::Result::ok();
}

juce::Result setAnimationPropertyOverride (Lottie_Animation* animation, LottieAnimation::Property property, const juce::String& keyPath, const juce::Point<float>& point)
{
    if (animation == nullptr)
        return juce::Result::fail ("Invalid animation");

    // TODO - test properties

    lottie_animation_property_override(animation,
                                       static_cast<Lottie_Animation_Property> (property),
                                       keyPath.toRawUTF8(),
                                       point.getX(),
                                       point.getY());

    return juce::Result::ok();
}
} // namespace

//==============================================================================
LottieAnimation::LottieAnimation (const juce::String& data)
    : animation (createAnimation (data))
    , numFrames (getAnimationNumFrames (animation))
    , frameRate (getAnimationFrameRate (animation))
{
}

LottieAnimation::~LottieAnimation()
{
    destroyAnimation (animation);
}

//==============================================================================
bool LottieAnimation::isValid() const
{
    return animation != nullptr && canvas.isValid();
}

//==============================================================================
void LottieAnimation::setSize (int width, int height)
{
    originalWidth = width;
    originalHeight = height;

    const auto newSize = getScaledSize();
    if (newSize.getWidth() != canvas.getWidth() || newSize.getHeight() != canvas.getHeight())
    {
        canvas = juce::Image (juce::Image::ARGB, newSize.getWidth(), newSize.getHeight(), true);

        if (canRenderCurrentFrame())
            renderCurrentFrame();
    }
}

void LottieAnimation::setSize (const juce::Rectangle<int>& size)
{
    setSize (size.getWidth(), size.getHeight());
}

juce::Rectangle<int> LottieAnimation::getSize() const
{
    return juce::Rectangle<int>(0, 0, originalWidth, originalHeight);
}

juce::Rectangle<int> LottieAnimation::getScaledSize() const
{
    auto newWidth = juce::roundToInt (static_cast<float> (originalWidth) * scaleFactor);
    auto newHeight = juce::roundToInt (static_cast<float> (originalHeight) * scaleFactor);

    return juce::Rectangle<int>(0, 0, newWidth, newHeight);
}

juce::Rectangle<int> LottieAnimation::getOriginalSize() const
{
    return getAnimationSize (animation);
}

//==============================================================================
void LottieAnimation::setScaleFactor (float newScaleFactor)
{
    newScaleFactor = juce::jmax (newScaleFactor, 0.0001f);

    if (! juce::approximatelyEqual (scaleFactor, newScaleFactor))
    {
        scaleFactor = newScaleFactor;

        setSize (originalWidth, originalHeight);
    }
}

float LottieAnimation::getScaleFactor() const
{
    return scaleFactor;
}

//==============================================================================
int LottieAnimation::getNumFrames() const
{
    return numFrames;
}

double LottieAnimation::getFrameRate() const
{
    return frameRate;
}

void LottieAnimation::setFrame (int frameNumber)
{
    currentFrame = juce::jlimit (0, numFrames - 1, frameNumber);
}

int LottieAnimation::getCurrentFrame() const
{
    return currentFrame;
}

//==============================================================================
juce::Result LottieAnimation::setPropertyOverride (Property property, const juce::String& keyPath, const juce::Colour& color)
{
    return setAnimationPropertyOverride (animation, property, keyPath, color);
}

juce::Result LottieAnimation::setPropertyOverride (Property property, const juce::String& keyPath, float value)
{
    return setAnimationPropertyOverride (animation, property, keyPath, value);
}

juce::Result LottieAnimation::setPropertyOverride (Property property, const juce::String& keyPath, const juce::Range<int>& range)
{
    return setAnimationPropertyOverride (animation, property, keyPath, range);
}

juce::Result LottieAnimation::setPropertyOverride (Property property, const juce::String& keyPath, const juce::Point<float>& point)
{
    return setAnimationPropertyOverride (animation, property, keyPath, point);
}

//==============================================================================
void LottieAnimation::render (juce::Graphics& g, juce::Point<int> topLeft)
{
    renderCurrentFrame();

    if (juce::approximatelyEqual (scaleFactor, 1.0f))
        g.drawImageAt (canvas, topLeft.x, topLeft.y);
    else
        g.drawImageTransformed (canvas, juce::AffineTransform::scale (1.0f / scaleFactor));
}

void LottieAnimation::render (juce::Graphics& g, const juce::AffineTransform& transform)
{
    renderCurrentFrame();

    if (juce::approximatelyEqual (scaleFactor, 1.0f))
        g.drawImageTransformed (canvas, transform);
    else
        g.drawImageTransformed (canvas, transform.scaled (1.0f / scaleFactor));
}

//==============================================================================
void LottieAnimation::renderCurrentFrame()
{
    if (canRenderCurrentFrame() && lastFrame != currentFrame)
    {
        renderAnimationToImage (animation, canvas, currentFrame);

        lastFrame = currentFrame;
    }
}

bool LottieAnimation::canRenderCurrentFrame() const
{
    return isValid() && juce::isPositiveAndBelow (currentFrame, numFrames);
}

} // namespace jottie
