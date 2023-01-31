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

    std::atomic<float>* highPassModParam = nullptr;
    std::atomic<float>* lowPassModParam = nullptr;

    juce::SmoothedValue<float> hpfSmooth[2];
    juce::SmoothedValue<float> lpfSmooth[2];

    int prevHighPassMod = 1;
    int prevLowPassMod = 1;

    std::atomic<float>* zInputParam = nullptr;
    std::atomic<float>* zOutputParam = nullptr;

    std::atomic<float>* kValParam = nullptr;

    std::atomic<float>* gainParam = nullptr;


    RCA_MK2_SEF mk2Sef[2];

    juce::dsp::Oversampling<float> oversampling { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RCA_MK2_SEFAudioProcessor)
};
