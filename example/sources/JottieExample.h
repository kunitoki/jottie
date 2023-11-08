
#pragma once

#include <JuceHeader.h>
#include <BinaryData.h>

class JottieExampleComponent : public juce::Component
{
public:
    JottieExampleComponent()
    {
        //openGLContext.attachTo (*getTopLevelComponent());
        
        addAndMakeVisible (lottieComponent);
        lottieComponent.setBackgroundColour (juce::Colours::transparentBlack);
        
        lottieComponent.loadAnimationLottie (
            std::make_unique<juce::MemoryInputStream> (BinaryData::xmas_lottie, BinaryData::xmas_lottieSize, false), 1.0f);

        lottieComponent.setFrameRate (lottieComponent.getFrameRate() * 1.5);
        
        setSize (600, 600);
    }

    ~JottieExampleComponent() override
    {
        //openGLContext.detach();
    }

    void visibilityChanged() override
    {
        if (isVisible())
            lottieComponent.play();
        else
            lottieComponent.stop();
    }

    void paint (juce::Graphics& g) override
    {
        juce::ColourGradient gradient
        {
            juce::Colour (100, 210, 220), 0.0f, static_cast<float> (getHeight()),
            juce::Colour (240, 55, 250), static_cast<float> (getWidth()), 0.0f,
            false
        };

        g.setGradientFill (gradient);
        g.fillAll();
    }

    void resized() override
    {
        lottieComponent.setBounds (getLocalBounds());
    }

private:
    juce::OpenGLContext openGLContext;
    jottie::LottieComponent lottieComponent; 
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JottieExampleComponent)
};

