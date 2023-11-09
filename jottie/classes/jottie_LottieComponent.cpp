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

#include "jottie_LottieComponent.h"
#include "jottie_LottieAnimation.h"
#include "jottie_LottieFile.h"

namespace jottie {

//==============================================================================
LottieComponent::LottieComponent()
{
    setOpaque (true);
}

LottieComponent::LottieComponent (juce::StringRef componentName)
    : juce::Component (componentName)
{
    setOpaque (true);
}

//==============================================================================
juce::Result LottieComponent::loadAnimationJson (const juce::String& jsonString, float scaleFactor)
{
    currentAnimation = new LottieAnimation (jsonString);

    initialiseAnimation (currentAnimation, scaleFactor);
    
    currentScaleFactor = scaleFactor;

    return currentAnimation->isValid()
        ? juce::Result::ok()
        : juce::Result::fail("Error loading animation");
}

juce::Result LottieComponent::loadAnimationJson (std::unique_ptr<juce::InputStream> jsonFileStream, float scaleFactor)
{
    if (jsonFileStream == nullptr)
        return juce::Result::fail("Unable to read from a non existing json file stream");

    return loadAnimationJson (jsonFileStream->readEntireStreamAsString(), scaleFactor);
}

juce::Result LottieComponent::loadAnimationJson (const juce::File& jsonFile, float scaleFactor)
{
    auto jsonFileStream = jsonFile.createInputStream();
    if (jsonFileStream == nullptr)
        return juce::Result::fail("Unable to open json file for reading");

    return loadAnimationJson (jsonFileStream->readEntireStreamAsString(), scaleFactor);
}

//==============================================================================
juce::Result LottieComponent::loadAnimationLottie (const juce::File& lottieFile, float scaleFactor)
{
    auto lottie = LottieFile::open (lottieFile);
    if (lottie == nullptr)
        return juce::Result::fail("Unable to open lottie file for reading");

    if (lottie->getNumAnimations() == 0)
        return juce::Result::fail("Unable to find animation in lottie file");

    auto animation = lottie->loadAnimation (0);

    initialiseAnimation (animation, scaleFactor);

    currentLottieFile = std::move (lottie);
    currentAnimation = std::move (animation);
    currentScaleFactor = scaleFactor;

    return juce::Result::ok();
}

juce::Result LottieComponent::loadAnimationLottie (std::unique_ptr<juce::InputStream> lottieFileStream, float scaleFactor)
{
    if (lottieFileStream == nullptr)
        return juce::Result::fail("Unable to read from a non existing json file stream");

    auto lottie = LottieFile::open (std::move (lottieFileStream));
    if (lottie == nullptr)
        return juce::Result::fail("Unable to open lottie file for reading");

    if (lottie->getNumAnimations() == 0)
        return juce::Result::fail("Unable to find animation in lottie file");

    auto animation = lottie->loadAnimation (0);

    initialiseAnimation (animation, scaleFactor);

    currentLottieFile = std::move (lottie);
    currentAnimation = std::move (animation);
    currentScaleFactor = scaleFactor;

    return juce::Result::ok();
}

juce::Result LottieComponent::loadAnimationLottie (const juce::File& lottieFile, juce::StringRef animationId, float scaleFactor)
{
    auto lottie = LottieFile::open (lottieFile);
    if (lottie == nullptr)
        return juce::Result::fail("Unable to open lottie file for reading");

    auto animation = lottie->loadAnimation (animationId);
    if (animation == nullptr)
        return juce::Result::fail("Unable to find animation in lottie file");

    initialiseAnimation (animation, scaleFactor);

    currentLottieFile = std::move (lottie);
    currentAnimation = std::move (animation);
    currentScaleFactor = scaleFactor;

    return juce::Result::ok();
}

juce::Result LottieComponent::loadAnimationLottie (std::unique_ptr<juce::InputStream> lottieFileStream, juce::StringRef animationId, float scaleFactor)
{
    if (lottieFileStream == nullptr)
        return juce::Result::fail("Unable to read from a non existing json file stream");

    auto lottie = LottieFile::open (std::move (lottieFileStream));
    if (lottie == nullptr)
        return juce::Result::fail("Unable to open lottie file for reading");

    auto animation = lottie->loadAnimation (animationId);
    if (animation == nullptr)
        return juce::Result::fail("Unable to find animation in lottie file");

    initialiseAnimation (animation, scaleFactor);

    currentLottieFile = std::move (lottie);
    currentAnimation = std::move (animation);
    currentScaleFactor = scaleFactor;

    return juce::Result::ok();
}

//==============================================================================
LottieAnimation::Ptr LottieComponent::getCurrentAnimation() const
{
    return currentAnimation;
}

//==============================================================================
void LottieComponent::setFrameNormalised (double normalisedPosition)
{
    if (currentAnimation == nullptr)
        return;

    normalisedPosition = juce::jlimit (0.0, 1.0, normalisedPosition);

    const auto frameIndex = juce::roundToInt (normalisedPosition * static_cast<double> (currentAnimation->getNumFrames() - 1));

    currentFrame = frameIndex;

    repaint();
}

double LottieComponent::getCurrentFrameNormalised() const
{
    if (currentAnimation == nullptr)
        return 0.0;

    const auto nf = static_cast<double> (currentAnimation->getNumFrames() - 1);

    return (nf > 0.0) ? static_cast<double> (currentFrame) / nf : 0.0f;
}

//==============================================================================
juce::RelativeTime LottieComponent::getTotalDurationTime() const
{
    if (currentAnimation == nullptr)
        return juce::RelativeTime::seconds (0.0);

    return juce::RelativeTime::seconds (static_cast<double> (currentAnimation->getNumFrames()) * currentAnimation->getFrameRate());
}

//==============================================================================
void LottieComponent::setFrameRate(double newFrameRate)
{
    currentFrameRate = juce::jlimit (0.0, 120.0, newFrameRate);

    if (isTimerRunning())
    {
        if (currentFrameRate > 0.0)
            startTimerHz (static_cast<int> (currentFrameRate));
        else
            stopTimer();
    }
}

double LottieComponent::getFrameRate() const
{
    return currentFrameRate;
}

void LottieComponent::resetFrameRate()
{
    currentFrameRate = currentAnimation != nullptr ? currentAnimation->getFrameRate() : 0.0;
}

//==============================================================================
int LottieComponent::getDirection() const
{
    return currentDirection;
}

void LottieComponent::setDirection (int newDirection)
{
    currentDirection = newDirection < 0 ? -1 : 1;
}

//==============================================================================
juce::Result LottieComponent::play()
{
    if (currentAnimation == nullptr)
        return juce::Result::fail ("Invalid or not loaded animation");
    
    if (currentFrameRate > 0.0)
        startTimerHz (static_cast<int> (currentFrameRate));
    
    listeners.call (&Listener::animationStarted, this, currentAnimation, currentFrameRate);
    
    return juce::Result::ok();
}

juce::Result LottieComponent::play (juce::StringRef animationId)
{
    if (currentLottieFile == nullptr)
        return juce::Result::fail ("Invalid or not available lottie file");
    
    auto animation = currentLottieFile->loadAnimation (animationId);
    if (animation == nullptr)
        return juce::Result::fail("Unable to find animation in lottie file");

    initialiseAnimation (animation, currentScaleFactor);

    currentAnimation = std::move (animation);
    
    return play();
}

juce::Result LottieComponent::stop()
{
    if (currentAnimation == nullptr)
        return juce::Result::fail ("Invalid or not loaded animation");
    
    listeners.call (&Listener::animationStopped, this, currentAnimation, getCurrentFrameNormalised());
    
    stopTimer();
    
    return juce::Result::ok();
}

juce::Result LottieComponent::reset()
{
    if (currentAnimation == nullptr)
        return juce::Result::fail ("Invalid or not loaded animation");

    currentFrame = 0;

    listeners.call (&Listener::animationReset, this, currentAnimation);

    repaint();

    return juce::Result::ok();
}

//==============================================================================
void LottieComponent::setBackgroundColour (const juce::Colour& newBackgroundColour)
{
    backgroundColour = newBackgroundColour;

    setOpaque (backgroundColour.getAlpha() == 0xff);
}

juce::Colour LottieComponent::getBackgroundColour() const
{
    return backgroundColour;
}

//==============================================================================
void LottieComponent::addListener (Listener* listener)
{
    listeners.add (listener);
}

void LottieComponent::removeListener (Listener* listener)
{
    listeners.remove (listener);
}

void LottieComponent::removeAllListeners()
{
    listeners.clear();
}

//==============================================================================
void LottieComponent::paint (juce::Graphics& g)
{
    if (isOpaque() || backgroundColour.getAlpha() != 0x0)
        g.fillAll (backgroundColour);

    if (currentAnimation != nullptr)
    {
        currentAnimation->setFrame (currentFrame);
        currentAnimation->render (g, { 0, 0 });
    }
}

//==============================================================================
void LottieComponent::resized()
{
    if (currentAnimation != nullptr)
        currentAnimation->setSize (getWidth(), getHeight());
}

//==============================================================================
void LottieComponent::timerCallback()
{
    if (currentAnimation == nullptr || currentAnimation->getNumFrames() == 0)
        return;

    currentFrame += currentDirection;

    if (currentFrame < 0)
    {
        currentFrame = currentAnimation->getNumFrames() - 1;

        listeners.call (&Listener::animationCompleted, this, currentAnimation);
    }
    else if (currentFrame >= currentAnimation->getNumFrames())
    {
        currentFrame = 0;

        listeners.call (&Listener::animationCompleted, this, currentAnimation);
    }

    repaint();
}

//==============================================================================
void LottieComponent::initialiseAnimation (LottieAnimation::Ptr animation, float scaleFactor)
{
    jassert (animation != nullptr);

    if (scaleFactor > 0.0f)
        animation->setScaleFactor (scaleFactor);

    currentFrameRate = animation->getFrameRate();
    currentFrame = 0;

    animation->setSize (getWidth(), getHeight());
}

} // namespace jottie
