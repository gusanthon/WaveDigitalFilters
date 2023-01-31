#include "PluginProcessor.h"


juce::String resValToString (float resVal)
{
    if (resVal <= 1000.0f)
        return juce::String (resVal, 2, false) + " Ohm";

    return juce::String (resVal / 1000.0f, 2, false) + " kOhm";
}


RCA_MK2_SEFAudioProcessor::RCA_MK2_SEFAudioProcessor()
{
    highPassCutoffParam = vts.getRawParameterValue ("hpfc");
    lowPassCutoffParam = vts.getRawParameterValue("lpfc");

    highPassModParam = vts.getRawParameterValue("hpfMod");
    lowPassModParam = vts.getRawParameterValue("lpfMod");

    zInputParam = vts.getRawParameterValue("zInput");
    zOutputParam = vts.getRawParameterValue("zOutput");

    kValParam = vts.getRawParameterValue("kVal");

    gainParam = vts.getRawParameterValue("gain");
}

void RCA_MK2_SEFAudioProcessor::addParameters (Parameters& params)
{
    using namespace chowdsp::ParamUtils;

    juce::NormalisableRange<float> hpfcRange (20.0f, 8000.0f);
    juce::NormalisableRange<float> lpfcRange (20.0f, 20000.0f);
    juce::NormalisableRange<float> gainRange (0.0f, 20.0f);

    hpfcRange.setSkewForCentre (200.0f);
    lpfcRange.setSkewForCentre(2000);
    gainRange.setSkewForCentre(6);

    params.push_back (std::make_unique<VTSParam> ("hpfc", "HP Cutoff [Hz]", juce::String(), hpfcRange, 20.0f, &freqValToString, &stringToFreqVal));
    params.push_back (std::make_unique<VTSParam> ("lpfc", "LP Cutoff [Hz]", juce::String(), lpfcRange, 20000.0f, &freqValToString, &stringToFreqVal));

    params.push_back (std::make_unique<juce::AudioParameterChoice> ("hpfMod", "high pass mod", juce::StringArray { "Off", "On" }, 1));
    params.push_back (std::make_unique<juce::AudioParameterChoice> ("lpfMod", "low pass mod", juce::StringArray { "Off", "On" }, 1));

    params.push_back (std::make_unique<VTSParam> ("zInput", "input Z [ohm]", juce::String(), lpfcRange, 560.0f, &resValToString, &stringToFreqVal));
    params.push_back (std::make_unique<VTSParam> ("zOutput", "output Z [ohm]", juce::String(), lpfcRange, 560.0f, &resValToString, &stringToFreqVal));

    params.push_back (std::make_unique<VTSParam> ("kVal", "constant k [ohm]", juce::String(), lpfcRange, 560.0f, &resValToString, &stringToFreqVal));

    params.push_back (std::make_unique<VTSParam> ("gain", "gain [dB]", juce::String(), gainRange, 5.0f, &gainValToString, &stringToFreqVal));
}

void RCA_MK2_SEFAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
    {
        mk2Sef[ch].prepare(sampleRate);
        hpfSmooth[ch].reset(sampleRate, .005f);
        lpfSmooth[ch].reset(sampleRate, .005f);
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

    int lowPassMod = (int) *lowPassModParam;
    int highPassMod = (int) *highPassModParam;

    float hpfParam = *highPassCutoffParam;
    float lpfParam = *lowPassCutoffParam;

    float gain = vts.getRawParameterValue ("gain")->load();
    gain = juce::Decibels::decibelsToGain(gain);

    if (lowPassMod != prevLowPassMod)
    {
        for (auto & ch : mk2Sef)
        {
            ch.reset();
        }

        prevLowPassMod = lowPassMod;
    }
    if (highPassMod != prevHighPassMod) {
        for (auto & ch : mk2Sef)
        {
            ch.reset();
        }

        prevHighPassMod = highPassMod;
    }

    juce::dsp::AudioBlock<float> block (buffer);

    auto&& osBlock = oversampling.processSamplesUp (block);

    for (int ch = 0; ch < (int) osBlock.getNumChannels(); ++ch)
    {

        hpfSmooth[ch].setTargetValue((hpfParam));
        lpfSmooth[ch].setTargetValue((lpfParam) );

        mk2Sef[ch].setKVal(*kValParam);

        mk2Sef[ch].setLowPassMod(lowPassMod);
        mk2Sef[ch].setHighPassMod(highPassMod);

        mk2Sef[ch].setInputImpedance(*zInputParam);
        mk2Sef[ch].setOutputImpedance(*zOutputParam);

        auto* x = osBlock.getChannelPointer ((size_t) ch);

        for (int n = 0; n < (int) osBlock.getNumSamples(); ++n)
            {
                mk2Sef[ch].setHighPassCutoff(hpfSmooth[ch].getNextValue());
                mk2Sef[ch].setLowPassCutoff(lpfSmooth[ch].getNextValue());
                x[n] = gain * mk2Sef[ch].processSample (x[n]);
            }


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
