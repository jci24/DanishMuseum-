/*
  ==============================================================================

    OneDWave.cpp
    Created: 24 Mar 2022 8:38:24am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OneDWave.h"





//DO WE NEED THIS??? WHEN IS IT CALLED??
//==============================================================================
OneDWave::OneDWave(double kIn) : k (kIn) // <- This is an initialiser list. It initialises the member variable 'k' (in the "private" section in OneDWave.h), using the argument of the constructor 'kIn'. Basically, k = kIn;
{
    
    
     c = 300; // Wave speed (in m/s)
     L = 3; // Length (in m) // Time step (in s)

     // Stiff String
     sigma0 = 1;
     sigma1 = 0.01;
     T = 300;
     rho = 7850;
     r = 0.0005;
     A = r * r * double_Pi;
     I = r * r * r * r * double_Pi * 0.25;
     E = 2e11;
     cSq = T / (rho * A);
     kappaSq = E * I / (rho * A);
     stabilityTerm = cSq * k * k + 4.0 * sigma1 * k;
     hStiff = sqrt(0.5 * (stabilityTerm + sqrt((stabilityTerm * stabilityTerm) + 16.0 * kappaSq * k * k)));
     NStiff = floor(L / hStiff);
     hStiff = L / NStiff;
     


     
     // Coefficients used for damping
     S0 = sigma0 * k;
     S1 = (2.0 * sigma1 * k) / (hStiff * hStiff);

     // Scheme coefficients
     B0 = 2.0 - 2.0 * lambdaSq - 6.0 * muSq - 2.0 * S1; // u_l^n
     B1 = lambdaSq + 4.0 * muSq + S1;                   // u_{l+-1}^n
     B2 = -muSq;                                        // u_{l+-2}^n
     C0 = -1.0 + S0 + 2.0 * S1;                         // u_l^{n-1}
     C1 = -S1;                                          // u_{l+-1}^{n-1}

     Adiv = 1.0 / (1.0 + S0);                           // u_l^{n+1}

     // Divide by u_l^{n+1} term
     B0 *= Adiv;
     B1 *= Adiv;
     B2 *= Adiv;
     C0 *= Adiv;
     C1 *= Adiv;


     
     
    //Ideal String
     h = c * k; // Grid spacing (in m)
     N = floor(L / h);
     h = L / N;

     //Bar
     kappa1 = 0.006;
     hBar = sqrt(2 * kappa1 * k);
     NBar = floor(L / hBar);
     hBar = L / NBar;

     

     lambdaSq = c * c * k * k / (h * h);
     
     
     //B0 = (2 - 2) * lambdaSq;
     
     //DBG (lambdaSq);
     //StabilityCheck
     if (lambdaSq > 0.99)   // lambda =< 1 for stability
     lambdaSq = 0.99;
    
    //******
    
        
    // Initialise vectors containing the state of the system
    uStates = std::vector<std::vector<double>> (3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(N+1, 0)); // initializing a vector, with length, content
    wStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(N+1, 0)); // initializing a vector, with length, content
    yStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(N + 1, 0)); // initializing a vector, with length, content
    fStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(N + 1, 0)); // initializing a vector, with length, content
    mStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(N + 1, 0)); // initializing a vector, with length, content
    bStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(N + 1, 0)); // initializing a vector, with length, content
    
    // Initialise vector of pointers to the states
    u.resize (3, nullptr);
    w.resize (3, nullptr);
    y.resize (3, nullptr);
    f.resize(3, nullptr);
    m.resize(3, nullptr);
    b.resize(3, nullptr);

    
    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];
    for (int i = 0; i < 3; ++i)
        w[i] = &wStates[i][0];
    for (int i = 0; i < 3; ++i)
        y[i] = &yStates[i][0];
    for (int i = 0; i < 3; ++i)
        f[i] = &fStates[i][0];
    for (int i = 0; i < 3; ++i)
        m[i] = &mStates[i][0];
    for (int i = 0; i < 3; ++i)
        b[i] = &bStates[i][0];

    

    
    // Excite at the start halfway along the system.
    exciteStiffstringWithGuitar(0.5); // excite function

}






OneDWave::~OneDWave()
{
}



void OneDWave::paint (juce::Graphics& g)
{
    // Set the colour of the path
    g.setColour(Colours::cyan);
    
    // Generate the path that visualises the state of the system.
    Path visualStatePath = visualiseState (g);
    
    // Draw the path using a stroke (thickness) of 2 pixels.
    g.strokePath (visualStatePath, PathStrokeType(2.0f));
}


void OneDWave::resized()
{
}


Path OneDWave::visualiseState (Graphics& g)
{
    double visualScaling = 200; // we have to scale up the state of the system from 'transverse displacement' to 'pixels'
    
    // String-boundaries are in the vertical middle of the component
    double stringBoundaries = getHeight() / 2.0;
    
    // Initialise path
    Path stringPath;
    
    // Start path
    stringPath.startNewSubPath (0, -u[1][0] * visualScaling + stringBoundaries);
    stringPath.startNewSubPath (0, -w[1][0] * visualScaling + stringBoundaries);
    stringPath.startNewSubPath (0, -y[1][0] * visualScaling + stringBoundaries);
    stringPath.startNewSubPath(0, -f[1][0] * visualScaling + stringBoundaries);
    stringPath.startNewSubPath(0, -m[1][0] * visualScaling + stringBoundaries);
    stringPath.startNewSubPath(0, -b[1][0] * visualScaling + stringBoundaries);

    
    // Visual spacing between two grid points
    double spacing = getWidth() / static_cast<double>(N);
    double x = spacing;
    
    for (int l = 1; l <= N; l++)
    {
        // Needs to be -u, because a positive u would visually go down
        float newY = -u[1][l] * visualScaling + stringBoundaries;
        float newX = -w[1][l] * visualScaling + stringBoundaries;
        float newZ = -y[1][l] * visualScaling + stringBoundaries;
        float newF = -f[1][l] * visualScaling + stringBoundaries;
        float newM = -m[1][l] * visualScaling + stringBoundaries;
        float newB = -b[1][l] * visualScaling + stringBoundaries;

        
        // if we get NAN values, make sure that we don't get an exception
        if (isnan(newY))
            newY = 0;
        if (isnan(newX))
            newX = 0;
        if (isnan(newZ))
            newZ = 0;
        if (isnan(newF))
            newF = 0;
        if (isnan(newM))
            newM = 0;
        if (isnan(newB))
            newB = 0;
        
        stringPath.lineTo (x, newY);
        stringPath.lineTo (x, newX);
        stringPath.lineTo (x, newZ);
        stringPath.lineTo (x, newF);
        stringPath.lineTo (x, newM);
        stringPath.lineTo (x, newB);
        x += spacing;
    }
    
    return stringPath;
}



















//==============Define all different Types of Excitation==============



void OneDWave::exciteNone (double excitationLoc)
{
    /*
     u[0][all] = 0;
     u[1][all] = 0;
     u[2][all] = 0;
     */
}
void OneDWave::exciteExciteBody (double excitationLoc)
{
    // width (in grid points) of the excitation
    double width = 2;
    
    //to do: implement sensor or some dynamics
    float a = 0.5;
            
            // make sure we're not going out of bounds at the left boundary
            int start = std::max (floor((N+1) * excitationLoc) - floor(width * 0.5), 1.0);
            
            for (int l = 0; l < width; ++l)
            {
                // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
                if (l+start > N - 1)
                    break;
                
                u[1][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                u[2][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                // initializing uPrev = u so that the difference between the two won't be so huge
            }
}



void OneDWave::exciteStiffstringWithGuitar (double excitationLoc)
{
    // width (in grid points) of the excitation
    double width = 4;
    
    //to do: implement sensor or some dynamics
    float a = 0.5;
            
            // make sure we're not going out of bounds at the left boundary
            int start = std::max (floor((NStiff+1) * excitationLoc) - floor(width * 0.5), 1.0);
            
            for (int l = 0; l < width; ++l)
            {
                // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
                if (l+start > NStiff - 1)
                    break;
                
                u[1][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                u[2][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                // initializing uPrev = u so that the difference between the two won't be so huge
            }
}


void OneDWave::exciteStiffstringWithVioline (double excitationLoc)
{
    //to do: fill
}





void OneDWave::exciteIdealStringWithGuitar (double excitationLoc)
{
    // width (in grid points) of the excitation
    double width = 40;
    
    //to do: implement sensor or some dynamics
    float a = 0.5;
            
            // make sure we're not going out of bounds at the left boundary
            int start = std::max (floor((N+1) * excitationLoc) - floor(width * 0.5), 1.0);
            
            for (int l = 0; l < width; ++l)
            {
                // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
                if (l+start > N - 1)
                    break;
                
                u[1][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                u[2][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));

                w[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                w[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                y[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                y[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                f[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                f[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                m[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                m[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                b[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                b[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                // initializing uPrev = u so that the difference between the two won't be so huge
            }
}



void OneDWave::exciteIdealStringWithVioline (double excitationLoc)
{
     //to do: fill
}



void OneDWave::exciteMetalBarWithXylophon (double excitationLoc)
{
    // width (in grid points) of the excitation
    double width = 5;
    
    //to do: implement sensor or some dynamics
    float a = 0.5;
            
            // make sure we're not going out of bounds at the left boundary
            int start = std::max (floor((N+1) * excitationLoc) - floor(width * 0.5), 1.0);
            
            for (int l = 0; l < width; ++l)
            {
                // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
                if (l+start > N - 1)
                    break;
                
                u[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                u[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                w[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                w[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                y[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                y[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                f[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                f[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                m[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                m[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                b[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                b[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                // initializing uPrev = u so that the difference between the two won't be so huge
            }
}



void OneDWave::exciteWoodBarWithXylophon (double excitationLoc)
{
    // width (in grid points) of the excitation
    double width = 20;
    
    //to do: implement sensor or some dynamics
    float a = 0.5;
            
            // make sure we're not going out of bounds at the left boundary
            int start = std::max (floor((N+1) * excitationLoc) - floor(width * 0.5), 1.0);
            
            for (int l = 0; l < width; ++l)
            {
                // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
                if (l+start > N - 1)
                    break;
                
                u[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                u[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                w[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                w[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                y[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                y[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                f[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                f[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                m[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                m[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

                b[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                b[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
                // initializing uPrev = u so that the difference between the two won't be so huge
            }
}













//  ====================================


void OneDWave::setParamtersStiffStringGuitar(float frequencyMultiplikator)
{

    c = 1000; // Wave speed (in m/s)
    L = 1; // Length (in m) // Time step (in s)
    //    a = amplitude;

    //Stiff String
    sigma0 = 1;
    sigma1 = 0.01;
    T = 300;
    rho = 7850;
    r = 0.0005;
    A = r * r * double_Pi;
    I = r * r * r * r * double_Pi * 0.25;
    E = 2e11;
    cSq = T / (rho * A);
    kappaSq = E * I / (rho * A);
    stabilityTerm = cSq * k * k + 4.0 * sigma1 * k;
    hStiff = sqrt(0.5 * (stabilityTerm + sqrt((stabilityTerm * stabilityTerm) + 16.0 * kappaSq * k * k)));
    NStiff = floor(L / hStiff);
    hStiff = L / NStiff;

    

    lambdaSq = c*c * k*k / (hStiff*hStiff) ;
   
    // Coefficients used for damping
    S0 = sigma0 * k;
    S1 = (2.0 * sigma1 * k) / (hStiff * hStiff);

    // Scheme coefficients
    B0 = 2.0 - 2.0 * lambdaSq - 6.0 * muSq - 2.0 * S1; // u_l^n
    B1 = lambdaSq + 4.0 * muSq + S1;                   // u_{l+-1}^n
    B2 = -muSq;                                        // u_{l+-2}^n
    C0 = -1.0 + S0 + 2.0 * S1;                         // u_l^{n-1}
    C1 = -S1;                                          // u_{l+-1}^{n-1}

    Adiv = 1.0 / (1.0 + S0);                           // u_l^{n+1}

    // Divide by u_l^{n+1} term
    B0 *= Adiv;
    B1 *= Adiv;
    B2 *= Adiv;
    C0 *= Adiv;
    C1 *= Adiv;

    

    //DBG (lambdaSq);
    //StabilityCheck
    if (lambdaSq > 0.99)   // lambda =< 1 for stability
    lambdaSq = 0.99;

}

void OneDWave::setParamtersIdealStringGuitar(float frequencyMultiplikator)
{
    c = 300; // Wave speed (in m/s)
    L = 1; // Length (in m) // Time step (in s)
    //    a = amplitude;
    
    h = c * k; // Grid spacing (in m)
    N = floor(L / h);
    h = L / N;
    
    lambdaSq = c*c * k*k / (h*h) ;
    
    sigma1 = 0.001;
    
    B0 = (2 - 2) * lambdaSq;
    
    //DBG (lambdaSq);
    //StabilityCheck
    if (lambdaSq > 0.99)   // lambda =< 1 for stability
        lambdaSq = 0.99;
}

void OneDWave::setParamtersStiffStringVioline(float frequencyMultiplikator)
{
    c = 1400; // Wave speed (in m/s)
    L = 0.5; // Length (in m) // Time step (in s)
    //    a = amplitude;
    
    h = c * k; // Grid spacing (in m)
    N = floor(L / h);
    h = L / N;
    
    lambdaSq = c*c * k*k / (h*h) ;
    
    sigma1 = 0.01;
    
    B0 = (2 - 2) * lambdaSq;
    
    //DBG (lambdaSq);
    //StabilityCheck
    if (lambdaSq > 0.99)   // lambda =< 1 for stability
        lambdaSq = 0.99;
}

void OneDWave::setParamtersIdealStringVioline(float frequencyMultiplikator)
{
    c = 400; // Wave speed (in m/s)
    L = 0.5; // Length (in m) // Time step (in s)
    //    a = amplitude;
    
    h = c * k; // Grid spacing (in m)
    N = floor(L / h);
    h = L / N;
    
    lambdaSq = c*c * k*k / (h*h) ;
    
    sigma1 = 0.01;
    
    B0 = (2 - 2) * lambdaSq;
    
    //DBG (lambdaSq);
    //StabilityCheck
    if (lambdaSq > 0.99)   // lambda =< 1 for stability
        lambdaSq = 0.99;
}

void OneDWave::setParamtersMetalBar(float frequencyMultiplikator)
{
    c = 2000; // Wave speed (in m/s)
    L = 0.5; // Length (in m) // Time step (in s)
    //    a = amplitude;

    //h = c * k; // Grid spacing (in m)
    
    
    hBar = sqrt(2 * kappa1 * k);
    NBar = floor(L / hBar);
    hBar = L / NBar;

    
    lambdaSq = c*c * k*k / (h*h);
   
    sigma1 = 0.01;
    
    //B0 = (2 - 2) * lambdaSq;
    
    //DBG (lambdaSq);
    //StabilityCheck
    if (lambdaSq > 0.99)   // lambda =< 1 for stability
        lambdaSq = 0.99;
    if (hBar>0.99)
    {
        hBar = 0.49;
    }
}

void OneDWave::setParamtersWoodenBar(float frequencyMultiplikator)
{
    c = 400; // Wave speed (in m/s)
    L = 0.3; // Length (in m) // Time step (in s)
    //    a = amplitude;
    
    hBar = sqrt(2 * kappa1 * k);
    NBar = floor(L / hBar);
    hBar = L / NBar;
    
    lambdaSq = c*c * k*k / (h*h) ;
    
    sigma1 = 0.05;
    
    //B0 = (2 - 2) * lambdaSq;
    
    //DBG (lambdaSq);
    //StabilityCheck
    if (lambdaSq > 0.99)   // lambda =< 1 for stability
        lambdaSq = 0.99;
    if (hBar > 0.99)
    {
        hBar = 0.49;
    }
}











//==============Define all different Types of Calculation==============


void OneDWave::calculateSchemeNone()
{
    for(int l = 1; l < N; l++)
    {
        
        
        u[0][l] = 0;
        
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeExciteBody()
{
    for(int l = 1; l < N; l++)
    {
        if (l == 1)
        {
            //shouldExcite =  false; //Prevents excitation while calculation
            u[0][l] = 1;            //Only a short Sirac Impulse
        }
        
        u[0][l] = 0;
        
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeStiffstringWithGuitar()
{

    for(int l = 2; l < NStiff-1; l++)
    {
        if (l == 1)
        {
            //shouldExcite =  false; //Prevents excitation while calculation
        }
       
        u[0][l] = B0 * u[1][l] + B1 * (u[1][l + 1] + u[1][l - 1]) + B2 * (u[1][l + 2] + u[1][l - 2])
                + C0 * u[2][l] + C1 * (u[2][l + 1] + u[2][l - 1]);
        w[0][l] = B0 * w[1][l] + B1 * (w[1][l + 1] + w[1][l - 1]) + B2 * (w[1][l + 2] + w[1][l - 2])
                + C0 * w[2][l] + C1 * (w[2][l + 1] + w[2][l - 1]);
        y[0][l] = B0 * y[1][l] + B1 * (y[1][l + 1] + y[1][l - 1]) + B2 * (y[1][l + 2] + y[1][l - 2])
                + C0 * y[2][l] + C1 * (y[2][l + 1] + y[2][l - 1]);
        f[0][l] = B0 * f[1][l] + B1 * (f[1][l + 1] + f[1][l - 1]) + B2 * (f[1][l + 2] + f[1][l - 2])
                + C0 * f[2][l] + C1 * (f[2][l + 1] + f[2][l - 1]);
        m[0][l] = B0 * m[1][l] + B1 * (m[1][l + 1] + m[1][l - 1]) + B2 * (m[1][l + 2] + u[1][l - 2])
                + C0 * m[2][l] + C1 * (m[2][l + 1] + m[2][l - 1]);
        b[0][l] = B0 * b[1][l] + B1 * (b[1][l + 1] + b[1][l - 1]) + B2 * (b[1][l + 2] + b[1][l - 2])
                + C0 * b[2][l] + C1 * (b[2][l + 1] + b[2][l - 1]);



        //u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    shouldExcite =  true;
    
}




void OneDWave::calculateSchemeStiffstringWithVioline()
{
    for(int l = 1; l < N; l++)
    {
        if (l == 1)
        {
            //shouldExcite =  false; //Prevents excitation while calculation
        }
        
        //u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeIdealStringWithGuitar()
{
    for(int l = 2; l < N-3; l++)
    {
        if (l == 1)
        {
            //shouldExcite =  false; //Prevents excitation while calculation
        }
        
        u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * (u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        w[0][l] = (2 * w[1][l] - w[2][l] + lambdaSq * (w[1][l + 1] - 2 * w[1][l] + w[1][l - 1]) + sigma0 * k * w[2][l]) / (1 + sigma0 * k);
        y[0][l] = (2 * y[1][l] - y[2][l] + lambdaSq * (y[1][l + 1] - 2 * y[1][l] + y[1][l - 1]) + sigma0 * k * y[2][l]) / (1 + sigma0 * k);
        f[0][l] = (2 * f[1][l] - f[2][l] + lambdaSq * (f[1][l + 1] - 2 * f[1][l] + f[1][l - 1]) + sigma0 * k * f[2][l]) / (1 + sigma0 * k);
        m[0][l] = (2 * m[1][l] - m[2][l] + lambdaSq * (m[1][l + 1] - 2 * m[1][l] + m[1][l - 1]) + sigma0 * k * m[2][l]) / (1 + sigma0 * k);
        b[0][l] = (2 * b[1][l] - b[2][l] + lambdaSq * (b[1][l + 1] - 2 * b[1][l] + b[1][l - 1]) + sigma0 * k * b[2][l]) / (1 + sigma0 * k);

        // To do: Add frquency depended damping!
        
    }
    
    shouldExcite =  true;
}
void OneDWave::calculateSchemeIdealStringWithVioline()
{
    for(int l = 1; l < N; l++)
    {
        if (l == 1)
        {
            //shouldExcite =  false; //Prevents excitation while calculation
        }
        
        u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeMetalBarWithXylophon()
{
    for(int l = 2; l < N-3; l++)
    {
        if (l == 1)
        {
            //shouldExcite =  false; //Prevents excitation while calculation
        }
        
        u[0][l] = (2 * u[1][l] - u[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (u[1][l + 2] - 4 * u[1][l + 1] + 6 * u[1][l] - 4 * u[1][l - 1] + u[1][l - 2]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        w[0][l] = (2 * w[1][l] - w[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (w[1][l + 2] - 4 * w[1][l + 1] + 6 * w[1][l] - 4 * w[1][l - 1] + w[1][l - 2]) + sigma0 * k * w[2][l]) / (1 + sigma0 * k);
        y[0][l] = (2 * y[1][l] - y[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (y[1][l + 2] - 4 * y[1][l + 1] + 6 * y[1][l] - 4 * y[1][l - 1] + y[1][l - 2]) + sigma0 * k * y[2][l]) / (1 + sigma0 * k);
        f[0][l] = (2 * f[1][l] - f[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (f[1][l + 2] - 4 * f[1][l + 1] + 6 * f[1][l] - 4 * f[1][l - 1] + f[1][l - 2]) + sigma0 * k * f[2][l]) / (1 + sigma0 * k);
        m[0][l] = (2 * m[1][l] - m[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (m[1][l + 2] - 4 * m[1][l + 1] + 6 * m[1][l] - 4 * m[1][l - 1] + m[1][l - 2]) + sigma0 * k * m[2][l]) / (1 + sigma0 * k);
        b[0][l] = (2 * b[1][l] - b[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (b[1][l + 2] - 4 * b[1][l + 1] + 6 * b[1][l] - 4 * b[1][l - 1] + b[1][l - 2]) + sigma0 * k * b[2][l]) / (1 + sigma0 * k);



        u[0][1] = (2 * u[1][4] - u[2][3] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (u[1][6] - 4 * u[1][5] + 6 * u[1][4] - 4 * u[1][3] + u[1][2]) + sigma0 * k * u[2][3]) / (1 + sigma0 * k);
        w[0][1] = (2 * w[1][4] - w[2][3] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (w[1][6] - 4 * w[1][5] + 6 * w[1][4] - 4 * w[1][3] + w[1][2]) + sigma0 * k * w[2][3]) / (1 + sigma0 * k);
        
        
        /*u[0][1] = (2 * u[1][3] - u[2][3] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (u[1][5] - 4 * u[1][4] + 6 * u[1][3] - 4 * u[1][2] + u[1][1]) + sigma0 * k * u[2][3]) / (1 + sigma0 * k);
        u[0][1] = (2 * u[1][3] - u[2][3] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (u[1][5] - 4 * u[1][4] + 6 * u[1][3] - 4 * u[1][2] + u[1][1]) + sigma0 * k * u[2][3]) / (1 + sigma0 * k);
        u[0][1] = (2 * u[1][3] - u[2][3] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (u[1][5] - 4 * u[1][4] + 6 * u[1][3] - 4 * u[1][2] + u[1][1]) + sigma0 * k * u[2][3]) / (1 + sigma0 * k);
        u[0][1] = (2 * u[1][3] - u[2][3] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (u[1][5] - 4 * u[1][4] + 6 * u[1][3] - 4 * u[1][2] + u[1][1]) + sigma0 * k * u[2][3]) / (1 + sigma0 * k);
        //u[0][1] = (2 * u[1][3] - u[2][3) - k ^ 2kappa1 ^ 2 / h1 ^ 4 (u(5) - 4 * u(4) + 5 * u(3) - 4 * u(2)) + sigma0 * k * uPrev(3)) / (1 + sigma0 * k);
        //uNext(N1) = (2 * u(N1 - 2) - uPrev(N1 - 2) - k ^ 2kappa1 ^ 2 / h1 ^ 4 (-4 * u(N1 - 1) + 5 * u(N1 - 2) - 4 * u(N1 - 3) + u(N1 - 4)) + sigma0 * k * uPrev(N1 - 2)) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!*/
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeWoodBarWithXylophon()
{
    for(int l = 2; l < N-3; l++)
    {
        if (l == 1)
        {
            //shouldExcite =  false; //Prevents excitation while calculation
        }
        
        u[0][l] = (2 * u[1][l] - u[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (u[1][l + 2] - 4 * u[1][l + 1] + 6 * u[1][l] - 4 * u[1][l - 1] + u[1][l - 2]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        w[0][l] = (2 * w[1][l] - w[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (w[1][l + 2] - 4 * w[1][l + 1] + 6 * w[1][l] - 4 * w[1][l - 1] + w[1][l - 2]) + sigma0 * k * w[2][l]) / (1 + sigma0 * k);
        y[0][l] = (2 * y[1][l] - y[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (y[1][l + 2] - 4 * y[1][l + 1] + 6 * y[1][l] - 4 * y[1][l - 1] + y[1][l - 2]) + sigma0 * k * y[2][l]) / (1 + sigma0 * k);
        f[0][l] = (2 * f[1][l] - f[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (f[1][l + 2] - 4 * f[1][l + 1] + 6 * f[1][l] - 4 * f[1][l - 1] + f[1][l - 2]) + sigma0 * k * f[2][l]) / (1 + sigma0 * k);
        m[0][l] = (2 * m[1][l] - m[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (m[1][l + 2] - 4 * m[1][l + 1] + 6 * m[1][l] - 4 * m[1][l - 1] + m[1][l - 2]) + sigma0 * k * m[2][l]) / (1 + sigma0 * k);
        b[0][l] = (2 * b[1][l] - b[2][l] - (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4)) * (b[1][l + 2] - 4 * b[1][l + 1] + 6 * b[1][l] - 4 * b[1][l - 1] + b[1][l - 2]) + sigma0 * k * b[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!

       
        
        
    }
    
    shouldExcite =  true;
}











//===============================================



void OneDWave::updateStates()
{
    // Here is where you'll have to implement the pointer switch.
    
  
    double* uTmp = u[2]; // saving that location in memory for uPrev, /we need to store the pointer adress. even if we don't need the data anymore
    u[2] = u[1];            //read it like u1 is going to be u2
    u[1] = u[0];            //read it like u0 is going to be u1
    u[0] = uTmp;            // now get back the adress of the vector that can be overwritten
    
    double* wTmp = w[2];
    w[2] = w[1];            //read it like u1 is going to be u2
    w[1] = w[0];            //read it like u0 is going to be u1
    w[0] = wTmp;            // now get back the adress of the vector that can be overwritten

    double* yTmp = y[2];
    y[2] = y[1];            //read it like u1 is going to be u2
    y[1] = y[0];            //read it like u0 is going to be u1
    y[0] = yTmp;            // now get back the adress of the vector that can be overwritten

    double* fTmp = f[2]; // saving that location in memory for uPrev, /we need to store the pointer adress. even if we don't need the data anymore
    f[2] = f[1];            //read it like u1 is going to be u2
    f[1] = f[0];            //read it like u0 is going to be u1
    f[0] = fTmp;            // now get back the adress of the vector that can be overwritten

    double* mTmp = m[2];
    m[2] = m[1];            //read it like u1 is going to be u2
    m[1] = m[0];            //read it like u0 is going to be u1
    m[0] = mTmp;            // now get back the adress of the vector that can be overwritten

    double* bTmp = b[2];
    b[2] = b[1];            //read it like u1 is going to be u2
    b[1] = b[0];            //read it like u0 is going to be u1
    b[0] = bTmp;            // now get back the adress of the vector that can be overwritten
    //******
}

















