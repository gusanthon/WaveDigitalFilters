#pragma once

#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <juce_dsp/juce_dsp.h>
#include "RCA_MK2_SEF.h"

class RCA_MK2_SEFAudioProcessor : public chowdsp::PluginBase<RCA_MK2_SEFAudioProcessor>
{
public:
    RCA_MK2_SEFAudioProcessor();

    static void addParameters (Parameters& params);

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processAudioBlock (juce::AudioBuffer<float>&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    std::atomic<float>* highPassCutoffParam = nullptr;
    std::atomic<float>* lowPassCutoffParam = nullptr;

//    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> hpfSmooth[2];



    RCA_MK2_SEF mk2Sef[2];

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RCA_MK2_SEFAudioProcessor)
};
