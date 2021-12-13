#include "BaxandallWDF.h"

void BaxandallWDF::prepare (double fs)
{
    Ca.prepare ((float) fs);
    Cb.prepare ((float) fs);
    Cc.prepare ((float) fs);
    Cd.prepare ((float) fs);
    Ce.prepare ((float) fs);
}

void BaxandallWDF::setParams (float bassParam, float trebleParam)
{
    Pb_plus.setResistanceValue (Pb * bassParam);
    Pb_minus.setResistanceValue (Pb * (1.0f - bassParam));

    Pt_plus.setResistanceValue (Pt * trebleParam);
    Pt_minus.setResistanceValue (Pt * (1.0f - trebleParam));

    const auto Ra = S4.R;
    const auto Rb = P1.R;
    const auto Rc = Resc.R;
    const auto Rd = S3.R;
    const auto Re = S2.R;
    const auto Rf = S1.R;
    const auto Ga = 1.0f / Ra;
    const auto Gb = 1.0f / Rb;
    const auto Gc = 1.0f / Rc;
    const auto Gd = 1.0f / Rd;
    const auto Ge = 1.0f / Re;
    const auto Gf = 1.0f / Rf;

    // This scattering matrix was derived using the R-Solver python script (https://github.com/jatinchowdhury18/R-Solver),
    // with netlist input: netlists/baxandall.txt
    R.setSMatrixData ({{ -2*((Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd)*Ge + (Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Ga*Gc)*Ge)*Gf)*Ra/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf) + 1,                                                 -2*(Ga*Gb*Gd*Ge + (Ga*Gb*Gc + Ga*Gb*Gd + Ga*Gb*Ge)*Gf)*Ra/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                2*((Ga*Gb*Gc + Ga*Gc*Gd)*Ge + (Ga*Gc*Gd + Ga*Gc*Ge)*Gf)*Ra/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                                          2*(Ga*Gb*Gd*Ge - Ga*Gc*Gd*Gf)*Ra/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                   2*(Ga*Gc*Ge*Gf + (Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd)*Ge)*Ra/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                       -2*(Ga*Gb*Gc + Ga*Gb*Ge + (Ga*Gb + Ga*Gc)*Gd)*Gf*Ra/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf)},
                        {                                                -2*(Ga*Gb*Gd*Ge + (Ga*Gb*Gc + Ga*Gb*Gd + Ga*Gb*Ge)*Gf)*Rb/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf), -2*(Ga*Gb*Gc*Gd + (Ga*Gb + Gb*Gc)*Gd*Ge + (Ga*Gb*Gc + (Ga*Gb + Gb*Gc)*Gd + (Ga*Gb + Gb*Gc)*Ge)*Gf)*Rb/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf) + 1,                                              -2*(Ga*Gb*Gc*Gd + Gb*Gc*Gd*Ge + (Gb*Gc*Gd + Gb*Gc*Ge)*Gf)*Rb/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                  2*(Ga*Gb*Gc*Gd + Gb*Gc*Gd*Gf + (Ga*Gb + Gb*Gc)*Gd*Ge)*Rb/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                                          2*(Ga*Gb*Gd*Ge - Gb*Gc*Ge*Gf)*Rb/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                       -2*(Ga*Gb*Gc + Ga*Gb*Gd + (Ga*Gb + Gb*Gc)*Ge)*Gf*Rb/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf)},
                        {                                               2*((Ga*Gb*Gc + Ga*Gc*Gd)*Ge + (Ga*Gc*Gd + Ga*Gc*Ge)*Gf)*Rc/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                              -2*(Ga*Gb*Gc*Gd + Gb*Gc*Gd*Ge + (Gb*Gc*Gd + Gb*Gc*Ge)*Gf)*Rc/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),        -2*(Ga*Gb*Gc*Gd + (Ga*Gb*Gc + (Ga + Gb)*Gc*Gd)*Ge + ((Ga + Gb)*Gc*Gd + (Ga + Gb)*Gc*Ge)*Gf)*Rc/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf) + 1,                                                     2*(Ga*Gb*Gc*Gd + Gb*Gc*Gd*Ge + (Ga + Gb)*Gc*Gd*Gf)*Rc/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                     -2*((Ga + Gb)*Gc*Ge*Gf + (Ga*Gb*Gc + Ga*Gc*Gd)*Ge)*Rc/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                                             2*(Ga*Gc*Gd - Gb*Gc*Ge)*Gf*Rc/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf)},
                        {                                                                         2*(Ga*Gb*Gd*Ge - Ga*Gc*Gd*Gf)*Rd/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                  2*(Ga*Gb*Gc*Gd + Gb*Gc*Gd*Gf + (Ga*Gb + Gb*Gc)*Gd*Ge)*Rd/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                     2*(Ga*Gb*Gc*Gd + Gb*Gc*Gd*Ge + (Ga + Gb)*Gc*Gd*Gf)*Rd/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),             -2*(Ga*Gb*Gc*Gd + (Ga*Gb + Gb*Gc)*Gd*Ge + ((Ga + Gb)*Gc*Gd + (Ga + Gb + Gc)*Gd*Ge)*Gf)*Rd/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf) + 1,                                                             -2*(Ga*Gb*Gd*Ge + (Ga + Gb + Gc)*Gd*Ge*Gf)*Rd/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                                -2*(Ga*Gc*Gd + (Ga + Gb + Gc)*Gd*Ge)*Gf*Rd/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf)},
                        {                                                  2*(Ga*Gc*Ge*Gf + (Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd)*Ge)*Re/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                                          2*(Ga*Gb*Gd*Ge - Gb*Gc*Ge*Gf)*Re/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                     -2*((Ga + Gb)*Gc*Ge*Gf + (Ga*Gb*Gc + Ga*Gc*Gd)*Ge)*Re/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                             -2*(Ga*Gb*Gd*Ge + (Ga + Gb + Gc)*Gd*Ge*Gf)*Re/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                 -2*(((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd)*Ge*Gf + (Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd)*Ge)*Re/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf) + 1,                                                                   -2*(Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge*Gf*Re/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf)},
                        {                                                      -2*(Ga*Gb*Gc + Ga*Gb*Ge + (Ga*Gb + Ga*Gc)*Gd)*Gf*Rf/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                       -2*(Ga*Gb*Gc + Ga*Gb*Gd + (Ga*Gb + Gb*Gc)*Ge)*Gf*Rf/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                                             2*(Ga*Gc*Gd - Gb*Gc*Ge)*Gf*Rf/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                                -2*(Ga*Gc*Gd + (Ga + Gb + Gc)*Gd*Ge)*Gf*Rf/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                                                                   -2*(Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge*Gf*Rf/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf),                     -2*(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge)*Gf*Rf/(Ga*Gb*Gc + (Ga*Gb + Ga*Gc)*Gd + (Ga*Gb + Gb*Gc + (Ga + Gb + Gc)*Gd)*Ge + ((Ga + Gb)*Gc + (Ga + Gb + Gc)*Gd + (Ga + Gb + Gc)*Ge)*Gf) + 1}});
}
