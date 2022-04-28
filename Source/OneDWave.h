/*
  ==============================================================================
    OneDWave.h
    Created: 24 Mar 2022 8:38:24am
    Author:  Silvin Willemsen
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>




//==============================================================================
/*
*/
class OneDWave: public juce::Component
{
public:
    OneDWave (double k); // initialise the model with the time step // constructor / destructor
    ~OneDWave() override;

    void paint (juce::Graphics&) override; // JUCE GUI Functions
    void resized() override;
    
   
    
    
    //======== Mouse Trigger ==============
    //void mouseDown (const MouseEvent &e) override;
    
    // Called when a mouse button is pressed.
    
    // Function to visualise the state of the system.
    Path visualiseState (Graphics& g);
    
    
    
    // Function to excite the system.
     
    //==============Define all different Types of Excitation==============
    void exciteNone (double excitationLoc);
    void exciteExciteBody (double excitationLoc);
    void exciteStiffstringWithGuitar (double excitationLoc);
    void exciteStiffstringWithVioline (double excitationLoc);
    void exciteIdealStringWithGuitar (double excitationLoc);
    void exciteIdealStringWithVioline (double excitationLoc);
    void exciteMetalBarWithXylophon (double excitationLoc);
    void exciteWoodBarWithXylophon (double excitationLoc);
    
    
    //==============Define all different ParameterSets for different Strings, ==============
    void setParamtersStiffStringGuitar(float frequencyMultiplikator);
    void setParamtersIdealStringGuitar(float frequencyMultiplikator);
    void setParamtersStiffStringVioline(float frequencyMultiplikator);
    void setParamtersIdealStringVioline(float frequencyMultiplikator);
    void setParamtersMetalBar(float frequencyMultiplikator);
    void setParamtersWoodenBar(float frequencyMultiplikator);
    
   
    
    
    
    //Custom Functions
    // Function containing the update equation.
    void calculateSchemeNone();
    void calculateSchemeExciteBody();
    void calculateSchemeStiffstringWithGuitar();
    void calculateSchemeStiffstringWithVioline();
    void calculateSchemeIdealStringWithGuitar();
    void calculateSchemeIdealStringWithVioline();
    void calculateSchemeMetalBarWithXylophon();
    void calculateSchemeWoodBarWithXylophon();
    
    
    // Update the states by performing a pointer switch.
    void updateStates();
    
    /* To Do: Which updateStates make sense? Only Bar, String, Plate?
        
    void updateStatesNone();
    void updateStatesExciteBody();
    void updateStatesStiffstringWithGuitar();
    void updateStatesStiffstringWithVioline();
    void updateStatesIdealStringWithGuitar();
    void updateStatesIdealStringWithVioline();
    void updateStatesMetalBarWithXylophon();
    void updateStatesWoodBarWithXylophon();
    */
    
    
    
    // Get the output at a specified ratio of the length of the system
    float getOutput (float outRatio) { return u[1][(int)floor(outRatio * N)]; };
    float getOutputw(float outRatio) { return w[1][(int)floor(outRatio * N)]; };
    float getOutputy(float outRatio) { return y[1][(int)floor(outRatio * N)]; };
    float getOutputf(float outRatio) { return f[1][(int)floor(outRatio * N)]; };
    float getOutputm(float outRatio) { return m[1][(int)floor(outRatio * N)]; };
    float getOutputb(float outRatio) { return b[1][(int)floor(outRatio * N)]; };
    
    void setDamping (double dampingToSet)
    {
        sigma0 = dampingToSet; //Here Sigma0 gets defined and updated, everytime a slider is changed.
    }
    
 
    
    double sigma0;
    
private:
    
    // Member Variables
    double k; // Time step (in s)
    double c; // Wave speed (in m/s)
    double h; // Grid spacing (in m)
    double L; // Length (in m)
    
    double hBar;
    double kappa1;

    double rho, A, T, E, I, cSq, kappaSq,muSq, hStiff, NStiff;
    double r;
    double stabilityTerm;
    
    double sigma1;
    
    double lambdaSq; // Courant number squared to be used in the update equation
    double lambdaSqBar;
    double N; // number of intervals (number of grid points is N+1)
    double NBar;
    
    bool shouldExcite;

    // A vector of 3 vectors saving the states of the system at n+1, n and n-1
    std::vector<std::vector<double>> uStates;
    std::vector<std::vector<double>> wStates;
    std::vector<std::vector<double>> yStates;
    std::vector<std::vector<double>> fStates;
    std::vector<std::vector<double>> mStates;
    std::vector<std::vector<double>> bStates;
    
    // Pointers to the 3 vectors in uStates. The first index indicates the time index (0 -> n+1, 1 -> n, 2 -> n-1) and the second index indicates the spatial index.
    /*
     Examples:
         u[0][3] -> u_{3}^{n+1}
         u[1][7] -> u_{7}^{n}
         u[2][50] -> u_{50}^{n-1}
     */
    std::vector<double*> u;
    std::vector<double*> w;
    std::vector<double*> y;
    std::vector<double*> f;
    std::vector<double*> m;
    std::vector<double*> b;
    
    
    double Adiv, B0, B1, B2, C0, C1, S0, S1;
    
    
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneDWave)
};
