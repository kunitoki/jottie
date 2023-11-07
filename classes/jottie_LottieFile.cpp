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

#include "jottie_LottieFile.h"

namespace jottie {
namespace {

//==============================================================================
juce::Result parseAnimationIds (juce::ZipFile& zipFile, juce::StringArray& animationIds)
{
    animationIds.clear();

    auto manifestEntry = zipFile.getEntry ("manifest.json");
    if (manifestEntry == nullptr)
        return juce::Result::fail ("Unable to find lottie manifest.json file in lottie file");

    std::unique_ptr<juce::InputStream> manifestStream (zipFile.createStreamForEntry (*manifestEntry));
    if (manifestStream == nullptr)
        return juce::Result::fail ("Unable to open lottie manifest.json file for reading");

    juce::var manifest;
    if (! juce::JSON::parse (manifestStream->readEntireStreamAsString(), manifest))
        return juce::Result::fail ("Unable to parse lottie manifest.json file");

    juce::DynamicObject::Ptr rootObject = manifest.getDynamicObject();

    const auto& animations = rootObject->getProperty ("animations");
    if (! animations.isArray())
        return juce::Result::fail ("Invalid animations property in lottie manifest.json file");

    for (const auto& entry : *animations.getArray())
    {
        if (! entry.isObject())
            continue;

        juce::DynamicObject::Ptr animationObject = entry.getDynamicObject();
        animationIds.add (animationObject->getProperty ("id").toString());
    }

    return juce::Result::ok();
}

 } // namespace

//==============================================================================
LottieFile::LottieFile (const juce::File& lottieFile,
                        std::unique_ptr<juce::ZipFile> lottieZip,
                        juce::StringArray animationIds)
    : lottieFile (lottieFile)
    , lottieZip (std::move (lottieZip))
    , animationIds (std::move (animationIds))
{
}

LottieFile::LottieFile (std::unique_ptr<juce::InputStream> lottieStream,
                        std::unique_ptr<juce::ZipFile> lottieZip,
                        juce::StringArray animationIds)
    : lottieStream (std::move (lottieStream))
    , lottieZip (std::move (lottieZip))
    , animationIds (std::move (animationIds))
{
}

//==============================================================================
LottieFile::Ptr LottieFile::open (const juce::File& lottieFile)
{
    auto inputStream = lottieFile.createInputStream();
    if (inputStream == nullptr)
        return {};

    auto lottieZip = std::make_unique<juce::ZipFile> (inputStream.release(), true);

    juce::StringArray animationIds;
    if (auto result = parseAnimationIds (*lottieZip, animationIds); result.failed())
        return {};

    return new LottieFile (lottieFile, std::move (lottieZip), std::move (animationIds));
}

LottieFile::Ptr LottieFile::open (std::unique_ptr<juce::InputStream> inputStream)
{
    jassert (inputStream != nullptr);

    auto lottieZip = std::make_unique<juce::ZipFile> (inputStream.get(), false);

    juce::StringArray animationIds;
    if (auto result = parseAnimationIds (*lottieZip, animationIds); result.failed())
        return {};

    return new LottieFile (std::move (inputStream), std::move (lottieZip), std::move (animationIds));
}

//==============================================================================
void LottieFile::close()
{
    lottieZip.reset();
}

//==============================================================================
int LottieFile::getNumAnimations() const
{
    return animationIds.size();
}

//==============================================================================
juce::String LottieFile::getAnimationId (int index) const
{
    if (! juce::isPositiveAndBelow (index, animationIds.size()))
        return {};

    return animationIds.getReference (index);
}

//==============================================================================
LottieAnimation::Ptr LottieFile::loadAnimation (int index)
{
    if (! juce::isPositiveAndBelow (index, animationIds.size()))
        return {};

    return loadAnimation (juce::StringRef (animationIds.getReference (index)));
}

LottieAnimation::Ptr LottieFile::loadAnimation (juce::StringRef animationId)
{
    auto it = cachedAnimations.find (animationId);
    if (it != cachedAnimations.end())
        return it->second;

    if (lottieZip == nullptr)
        lottieZip = openZipFile();

    if (lottieZip == nullptr || ! animationIds.contains (animationId))
        return {};

    juce::String animationPath;
    animationPath << "animations/" << animationId << ".json";

    auto animationEntry = lottieZip->getEntry (animationPath);
    if (animationEntry == nullptr)
        return {};

    std::unique_ptr<juce::InputStream> animationStream (lottieZip->createStreamForEntry (*animationEntry));
    if (animationStream == nullptr)
        return {};

    LottieAnimation::Ptr animation = new LottieAnimation (animationStream->readEntireStreamAsString());

    cachedAnimations[animationId] = animation;

    return animation;
}

void LottieFile::loadAllAnimations()
{
    for (const auto& animationId : animationIds)
        loadAnimation (juce::StringRef (animationId));
}

//==============================================================================
void LottieFile::clearAnimation (juce::StringRef animationId)
{
    cachedAnimations.erase (animationId);
}

void LottieFile::clearAllAnimations()
{
    cachedAnimations.clear();
}

//==============================================================================
std::unique_ptr<juce::ZipFile> LottieFile::openZipFile()
{
    if (lottieStream == nullptr)
    {
        auto inputStream = lottieFile.createInputStream();
        if (inputStream == nullptr)
            return {};

        return std::make_unique<juce::ZipFile> (inputStream.release(), true);
    }
    else
    {
        lottieStream->setPosition(0);
        return std::make_unique<juce::ZipFile> (lottieStream.get(), false);
    }
}

} // namespace jottie
