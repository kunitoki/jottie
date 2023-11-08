
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
        lottieComponent.setInterceptsMouseClicks (false, false);
        lottieComponent.setBackgroundColour (juce::Colours::transparentBlack);
        
        lottieComponent.loadAnimationLottie (
            std::make_unique<juce::MemoryInputStream> (BinaryData::cook_lottie, BinaryData::cook_lottieSize, false), "cook1", 1.0f);

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
        g.fillAll(juce::Colours::white);
    }

    void resized() override
    {
        lottieComponent.setBounds (getLocalBounds());
    }

    void mouseDown (const juce::MouseEvent& ev) override
    {
        juce::ignoreUnused (ev);
        
        currentAnimation = (currentAnimation + 1) % 4;
        
        const juce::StringRef animations[] = { "cook1", "cook2", "cook3", "cook4" };
        
        lottieComponent.play (animations [currentAnimation]);
    }

private:
    juce::OpenGLContext openGLContext;
    jottie::LottieComponent lottieComponent;
    int currentAnimation = 1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JottieExampleComponent)
};

