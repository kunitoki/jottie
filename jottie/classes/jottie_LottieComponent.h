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
#include "jottie_LottieFile.h"

namespace jottie {

//==============================================================================
/**
 * @brief A custom JUCE Component for rendering Lottie animations.
 *
 * This class extends `juce::Component` and privately implements `juce::Timer`. It allows you to load
 * and display Lottie animations, control playback, and receive notifications about animation events. It only allows
 * to play a single lottie animation, if more animations are to be played, consider using the other class
 * `LottieMultiComponent`.
 *
 * @see LottieMultiComponent
 */
class LottieComponent : public juce::Component, private juce::Timer
{
public:
    //==============================================================================
    /**
     * @brief Default constructor.
     */
    LottieComponent();

    /**
     * @brief Constructor with a specified component name.
     *
     * @param componentName The name for the component.
     */
    explicit LottieComponent (juce::StringRef componentName);

    //==============================================================================
    /**
     * @brief Load a Lottie animation from JSON string.
     *
     * @param jsonString The JSON string representing the animation.
     * @param scaleFactor The scale factor for the animation (default: 1.0f).
     *
     * @return The result of the loading operation.
     */
    juce::Result loadAnimationJson (const juce::String& jsonString, float scaleFactor = 1.0f);

    /**
     * @brief Load a Lottie animation from JSON stream.
     *
     * @param jsonFileStream The JSON stream representing the animation.
     * @param scaleFactor The scale factor for the animation (default: 1.0f).
     *
     * @return The result of the loading operation.
     */
    juce::Result loadAnimationJson (std::unique_ptr<juce::InputStream> jsonFileStream, float scaleFactor = 1.0f);

    /**
     * @brief Load a Lottie animation from JSON file.
     *
     * @param jsonFile The JSON file representing the animation.
     * @param scaleFactor The scale factor for the animation (default: 1.0f).
     *
     * @return The result of the loading operation.
     */
    juce::Result loadAnimationJson (const juce::File& jsonFile, float scaleFactor = 1.0f);

    //==============================================================================
    /**
     * @brief Load a Lottie animation from dotLottie file.
     *
     * Only the first animation found in the dot lottie file will be used.
     *
     * @param lottieFile The dotLottie file representing the animation.
     * @param scaleFactor The scale factor for the animation (default: 1.0f).
     *
     * @return The result of the loading operation.
     */
    juce::Result loadAnimationLottie (const juce::File& lottieFile, float scaleFactor = 1.0f);

    /**
     * @brief Load a Lottie animation from dotLottie stream.
     *
     * Only the first animation found in the dot lottie file will be used.
     *
     * @param lottieFileStream The dotLottie stream representing the animation.
     * @param scaleFactor The scale factor for the animation (default: 1.0f).
     *
     * @return The result of the loading operation.
     */
    juce::Result loadAnimationLottie (std::unique_ptr<juce::InputStream> lottieFileStream, float scaleFactor = 1.0f);

    /**
     * @brief Load a Lottie animation from dotLottie file.
     *
     * Only the first animation found in the dot lottie file will be used.
     *
     * @param lottieFile The dotLottie file representing the animation.
     * @param animationId The animation id to load from the dotLottie file.
     * @param scaleFactor The scale factor for the animation (default: 1.0f).
     *
     * @return The result of the loading operation.
     */
    juce::Result loadAnimationLottie (const juce::File& lottieFile, juce::StringRef animationId, float scaleFactor = 1.0f);

    /**
     * @brief Load a Lottie animation from dotLottie file.
     *
     * Only the first animation found in the dot lottie file will be used.
     *
     * @param lottieFile The dotLottie file representing the animation.
     * @param animationId The animation id to load from the dotLottie file.
     * @param scaleFactor The scale factor for the animation (default: 1.0f).
     *
     * @return The result of the loading operation.
     */
    juce::Result loadAnimationLottie (std::unique_ptr<juce::InputStream> lottieFileStream, juce::StringRef animationId, float scaleFactor = 1.0f);

    //==============================================================================
    /**
     * @brief Set the current frame position normalized [0.0, 1.0].
     *
     * @param normalisedPosition The normalized frame position to set.
     */
    void setFrameNormalised (double normalisedPosition);

    /**
     * @brief Get the current frame position normalized [0.0, 1.0].
     *
     * @return The normalized frame position.
     */
    double getCurrentFrameNormalised() const;

    //==============================================================================
    /**
     * @brief Get the total duration of the animation.
     *
     * @return The total duration as a RelativeTime.
     */
    juce::RelativeTime getTotalDurationTime() const;

    //==============================================================================
    /**
     * @brief Set the frame rate for the animation.
     *
     * @param newFrameRate The new frame rate in frames per second.
     */
    void setFrameRate(double newFrameRate);

    /**
     * @brief Get the current frame rate of the animation.
     *
     * @return The current frame rate in frames per second.
     */
    double getFrameRate() const;

    /**
     * @brief Reset the frame rate to its default value (0.0).
     */
    void resetFrameRate();

    //==============================================================================
    /**
     * @brief Get the current direction of the animation playback.
     *
     * @return The current direction, +1 if moving forward or -1 if moving backwards.
     */
    int getDirection() const;

    /**
     * @brief Set the current direction of the animation playback.
     *
     * @param newDirection The new animation direction, +1 when it should move forward or -1 when backwards.
     */
    void setDirection (int newDirection);

    //==============================================================================
    /**
     * @brief Start playing the animation.
     */
    juce::Result play();
    juce::Result play (juce::StringRef animationName);

    /**
     * @brief Stop the animation.
     */
    juce::Result stop();

    /**
     * @brief Reset the animation to its initial state.
     */
    juce::Result reset();

    //==============================================================================
    /**
     * @brief Set the background color of the component.
     *
     * @param newBackgroundColour The new background color.
     */
    void setBackgroundColour (const juce::Colour& newBackgroundColour);

    /**
     * @brief Get the current background color of the component.
     *
     * @return The current background color.
     */
    juce::Colour getBackgroundColour() const;

    //==============================================================================
    /**
     * @brief An abstract class for receiving animation-related events.
     */
    class Listener
    {
    public:
        virtual ~Listener() = default;

        /**
         * @brief Called when the animation starts playing.
         *
         * @param source The source LottieComponent.
         * @param frameRate The frame rate at which the animation is playing.
         */
        virtual void animationStarted (LottieComponent* source, LottieAnimation::Ptr animation, double frameRate)
        {
            juce::ignoreUnused (source, animation, frameRate);
        }

        /**
         * @brief Called when the animation is stopped.
         *
         * @param source The source LottieComponent.
         * @param normalisedPosition The normalized position where the animation stopped.
         */
        virtual void animationStopped (LottieComponent* source, LottieAnimation::Ptr animation, double normalisedPosition)
        {
            juce::ignoreUnused (source, animation, normalisedPosition);
        }

        /**
         * @brief Called when the animation is reset to its initial state.
         *
         * @param source The source LottieComponent.
         */
        virtual void animationReset (LottieComponent* source, LottieAnimation::Ptr animation)
        {
            juce::ignoreUnused (source, animation);
        }

        /**
         * @brief Called when the animation is completed.
         *
         * @param source The source LottieComponent.
         */
        virtual void animationCompleted (LottieComponent* source, LottieAnimation::Ptr animation)
        {
            juce::ignoreUnused (source, animation);
        }
    };

    /**
     * @brief Add a listener to receive animation-related events.
     *
     * @param listener A pointer to the listener object.
     */
    void addListener (Listener* listener);

    /**
     * @brief Remove a listener to stop receiving animation-related events.
     *
     * @param listener A pointer to the listener object.
     */
    void removeListener (Listener* listener);

    /**
     * @brief Remove all registered listeners.
     */
    void removeAllListeners();

    //==============================================================================
    /**
     * @brief Return the current animation being loaded into the component, can be a nullptr.
     */
    LottieAnimation::Ptr getCurrentAnimation() const;

    //==============================================================================
    /** @internal */
    void paint (juce::Graphics& g) override;
    /** @internal */
    void resized() override;

private:
    void timerCallback() override;

    void initialiseAnimation (LottieAnimation::Ptr animation, float scaleFactor);

    LottieAnimation::Ptr currentAnimation;
    LottieFile::Ptr currentLottieFile;
    juce::ListenerList<Listener> listeners;
    juce::Colour backgroundColour = juce::Colours::black;
    float currentScaleFactor = 1.0f;
    int currentFrame = 0;
    double currentFrameRate = 0.0;
    int currentDirection = 1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LottieComponent)
};

} // namespace jottie
