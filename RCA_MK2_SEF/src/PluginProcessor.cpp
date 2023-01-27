#include "PluginProcessor.h"

RCA_MK2_SEFAudioProcessor::RCA_MK2_SEFAudioProcessor()
{
    highPassCutoffParam = vts.getRawParameterValue ("hpfc");
    lowPassCutoffParam = vts.getRawParameterValue("lpfc");
}

void RCA_MK2_SEFAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    juce::NormalisableRange<float> fcRange (20.0f, 20000.0f);
    //    fcRange.setSkewForCentre (1000.0f);

    params.push_back (std::make_unique<VTSParam> ("hpfc", "!HP Cutoff [Hz]", juce::String(), fcRange, 20.0f, &freqValToString, &stringToFreqVal));
    params.push_back (std::make_unique<VTSParam> ("lpfc", "!LP Cutoff [Hz]", juce::String(), fcRange, 20000.0f, &freqValToString, &stringToFreqVal));
    //    params.push_back (std::make_unique<VTSParam> ("zIn", "input impedance [ohms]", juce::String(), fcRange, 20000.0f, &freqValToString, &stringToFreqVal));
}

void RCA_MK2_SEFAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);
    for (auto & ch : mk2Sef)
    {
        ch.prepare(sampleRate);
    }
}

void RCA_MK2_SEFAudioProcessor::releaseResources()
{
    oversampling.reset();

    for (auto & ch : mk2Sef)
        ch.reset();
}

void RCA_MK2_SEFAudioProcessor::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);

    auto&& osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < (int) osBlock.getNumChannels(); ++ch)
    {
        //        mk2Sef->setInputImpedance();
        mk2Sef->setHighPassCutoff(*highPassCutoffParam);
        mk2Sef->setLowPassCutoff(*lowPassCutoffParam);

        auto* x = osBlock.getChannelPointer ((size_t) ch);

        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            x[n] = mk2Sef[ch].processSample (x[n]);
    }

    oversampling.processSamplesDown (block);
}

juce::AudioProcessorEditor* RCA_MK2_SEFAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

// This creates new instances of the plugin...
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RCA_MK2_SEFAudioProcessor();
}
