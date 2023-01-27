#ifndef RCA_MK2_SEF_H_INCLUDED
#define RCA_MK2_SEF_H_INCLUDED

#include <chowdsp_wdf/chowdsp_wdf.h>
#include <juce_dsp/juce_dsp.h>

using namespace chowdsp::wdft;

class RCA_MK2_SEF
{
public:
    RCA_MK2_SEF() = default;

    void prepare (float sampleRate)
    {
        C_HP1.prepare(sampleRate);
        C_HP2.prepare(sampleRate);
        C_HPm1.prepare(sampleRate);
        C_HPm2.prepare(sampleRate);
        C_LP1.prepare(sampleRate);
        C_LPm1.prepare(sampleRate);

        L_HP1.prepare(sampleRate);
        L_HPm.prepare(sampleRate);
        L_LP1.prepare(sampleRate);
        L_LP2.prepare(sampleRate);
        L_LPm1.prepare(sampleRate);
        L_LPm2.prepare(sampleRate);
    }

    void reset()
    {

        C_HP1.reset();
        C_HP2.reset();
        C_HPm1.reset();
        C_HPm2.reset();
        C_LP1.reset();
        C_LPm1.reset();

        L_HP1.reset();
        L_HPm.reset();
        L_LP1.reset();
        L_LP2.reset();
        L_LPm1.reset();
        L_LPm2.reset();
    }

    void setOutputImpedance(float newZ) {
        if (outputImpedance != newZ) {
            outputImpedance = newZ;
        }
    }

    void setInputImpedance(float newZ) {
        if (inputImpedance != newZ) {
            inputImpedance = newZ;
        }
    }


    void setHighPassCutoff(float highPassCutoff) {
        float C = root2 / (k * highPassCutoff * juce::MathConstants<float>::twoPi);
        float L = k / (2.0f * root2 * highPassCutoff * juce::MathConstants<float>::twoPi);

        C_HPm1.setCapacitanceValue(C);
        C_HPm2.setCapacitanceValue(C);
        L_HPm.setInductanceValue(L);

        C_HP1.setCapacitanceValue(C);
        C_HP2.setCapacitanceValue(C);
        L_HP1.setInductanceValue(L);

    }

    void setLowPassCutoff(float lowPassCutoff) {
        float C = (2.0f * root2) / (k * lowPassCutoff * juce::MathConstants<float>::twoPi);
        float L = (root2 * k) / (lowPassCutoff * juce::MathConstants<float>::twoPi);

        C_LPm1.setCapacitanceValue(C);
        L_LPm1.setInductanceValue(L);
        L_LPm2.setInductanceValue(L);

        C_LP1.setCapacitanceValue(C);
        L_LP1.setInductanceValue(L);
        L_LP2.setInductanceValue(L);

    }

    inline float processSample (float x) {
        Vs.setVoltage(x);
        Vs.incident(S0.reflected());
        S0.incident(Vs.reflected());
        return voltage<float>(Rt);
    }

private:

    float inputImpedance = 560;
    float outputImpedance = 560;

    ResistorT<float> Rt {outputImpedance};
    InductorT<float> L_LPm2 {1.0e-3f, 48000};

    WDFSeriesT<float, decltype(L_LPm2), decltype(Rt)> S8 {L_LPm2, Rt};
    CapacitorT<float> C_LPm1 {1.0e-8f, 48000};

    WDFParallelT<float, decltype(C_LPm1), decltype(S8)> P4 {C_LPm1, S8};
    InductorT<float> L_LPm1 {1.0e-3f, 48000};

    WDFSeriesT<float, decltype(L_LPm1), decltype(P4)> S7 {L_LPm1, P4};
    InductorT<float> L_LP2 {1.0e-3f, 48000};

    WDFSeriesT<float, decltype(L_LP2), decltype(S7)> S6 {L_LP2, S7};
    CapacitorT<float> C_LP1 {1.0e-8f, 48000};

    WDFParallelT<float, decltype(C_LP1), decltype(S6)> P3 {C_LP1, S6};
    InductorT<float> L_LP1 {1.0e-3f, 48000};

    WDFSeriesT<float, decltype(L_LP1), decltype(P3)> S5 {L_LP1, P3};
    CapacitorT<float> C_HP2 {1.0e-8f, 48000};

    WDFSeriesT<float, decltype(C_HP2), decltype(S5)> S4 {C_HP2, S5};
    InductorT<float> L_HP1 {1.0e-3f, 48000};

    WDFParallelT<float, decltype(L_HP1), decltype(S4)> P2 {L_HP1, S4};
    CapacitorT<float> C_HP1 {5.0e-8f, 48000};

    WDFSeriesT<float, decltype(C_HP1), decltype(P2)> S3 {C_HP1, P2};
    CapacitorT<float> C_HPm2 {5.0e-8f, 48000};

    WDFSeriesT<float, decltype(C_HPm2), decltype(S3)> S2 {C_HPm2, S3};
    InductorT<float> L_HPm {1.0e-3f, 48000.f};

    WDFParallelT<float, decltype(L_HPm), decltype(S2)> P1 {L_HPm, S2};
    CapacitorT<float> C_HPm1 {5.0e-8f, 48000.f};

    WDFSeriesT<float, decltype(C_HPm1), decltype(P1)> S1 {C_HPm1, P1};
    ResistorT<float> Rin {inputImpedance};

    WDFSeriesT<float, decltype(Rin), decltype(S1)> S0 {Rin, S1};
    IdealVoltageSourceT<float, decltype(S0)> Vs {S0};

    const float root2 = sqrtf(2);
    float k = 560.0f;


};

#endif //RCA_MK2_SEF_H_INCLUDED
