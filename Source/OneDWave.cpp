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
     
    
    sigma0 = 1;
    sigma1 = 0.01;
    kappa1 = 0.001;

    //Stiff String
    
    stabilityTerm = pow(c, 2) * pow(k, 2) + 4 * sigma1 * k;
    hStiff = sqrt(stabilityTerm + sqrt(pow(stabilityTerm, 2) + 16 * pow(kappa1, 2) * pow(k, 2)) / 2);
    NStiff = floor(L / hStiff);
    hStiff = L / NStiff;

    lambda = c * k / hStiff;
    mu = kappa1 * k / pow(hStiff, 2);

    //IdealString 
    hIdeal = c * k;
    NIdeal = floor(L / hIdeal);
    hIdeal = L / NIdeal;

    lambdaSqIdeal = c * c * k * k / (hIdeal * hIdeal);

    //Bar
    hBar = sqrt(2 * kappa1 * k);
    NBar = floor(L / hBar);
    hBar = L / NBar;

    lambdaSqBar = c * c * k * k / (hBar * hBar);
    muSqBar = (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4));

    
        
    // Initialise vectors containing the state of the system
    uStates = std::vector<std::vector<double>> (3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(NIdeal+1, 0)); // initializing a vector, with length, content
    vStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(NIdeal + 1, 0)); // initializing a vector, with length, content
    wStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(NIdeal + 1, 0)); // initializing a vector, with length, content
    mStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(NIdeal + 1, 0)); // initializing a vector, with length, content
    
    // Initialise vector of pointers to the states
    u.resize (3, nullptr);
    v.resize (3, nullptr);
    w.resize (3, nullptr);
    m.resize(3, nullptr);

    
    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
    {
        u[i] = &uStates[i][0];
        v[i] = &vStates[i][0];
        w[i] = &wStates[i][0];
        m[i] = &mStates[i][0];

    }
        
    
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
    
    // Visual spacing between two grid points
    double spacing = getWidth() / static_cast<double>(N);
    double x = spacing;
    
    for (int l = 1; l <= NBar; l++)
    {
        // Needs to be -u, because a positive u would visually go down
        float newY = -u[1][l] * visualScaling + stringBoundaries;
        
        // if we get NAN values, make sure that we don't get an exception
        if (isnan(newY))
            newY = 0;
        
        stringPath.lineTo (x, newY);
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
            int start = std::max (floor((NIdeal+1) * excitationLoc) - floor(width * 0.5), 1.0);
            
            for (int l = 0; l < width; ++l)
            {
                // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
                if (l+start > NIdeal - 1)
                    break;
                
                u[1][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                u[2][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                // initializing uPrev = u so that the difference between the two won't be so huge
            }
}



void OneDWave::exciteStiffstringWithGuitar (double excitationLoc)
{
    // width (in grid points) of the excitation
    double width = 10;
    
    //to do: implement sensor or some dynamics
    float a = 0.5;
            
            // make sure we're not going out of bounds at the left boundary
            int start = std::max (floor((NStiff +1) * excitationLoc) - floor(width * 0.5), 1.0);
            
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
            int start = std::max (floor((NIdeal +1) * excitationLoc) - floor(width * 0.5), 1.0);
            
            for (int l = 0; l < width; ++l)
            {
                // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
                if (l+start > NIdeal - 1)
                    break;
                
                u[1][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                u[2][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
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
            int start = std::max (floor((NBar +1) * excitationLoc) - floor(width * 0.5), 1.0);
            
            for (int l = 0; l < width; ++l)
            {
                // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
                if (l+start > NBar - 1)
                    break;
                
                u[1][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                u[2][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
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
            int start = std::max (floor((NBar + 1) * excitationLoc) - floor(width * 0.5), 1.0);
            
            for (int l = 0; l < width; ++l)
            {
                // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
                if (l+start > NBar - 1)
                    break;
                
                u[1][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                u[2][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
                // initializing uPrev = u so that the difference between the two won't be so huge
            }
}













//  ====================================


void OneDWave::setParamtersStiffStringGuitar(float frequencyMultiplikator)
{
    c = 300;
    L = 0.5;

    kappa1 = 0.001;
    sigma1 = 0.005;

    stabilityTerm = pow(c, 2) * pow(k, 2) + 4 * sigma1 * k;
    hStiff = sqrt(stabilityTerm + sqrt(pow(stabilityTerm, 2) + 16 * pow(kappa1, 2) * pow(k, 2)) / 2);
    NStiff = floor(L / hStiff);
    hStiff = L / NStiff;

    lambda = c * k / hStiff;
    mu = kappa1 * k / pow(hStiff, 2);

    if (stabilityTerm > 0.99)   // lambda =< 1 for stability
        stabilityTerm = 0.99;
    if (mu > 0.99)
        mu = 0.99;
    if (lambda > 0.99)
        lambda = 0.99;

}

void OneDWave::setParamtersIdealStringGuitar(float frequencyMultiplikator)
{
    c = 300;
    L = 1;

    hIdeal = c * k;
    NIdeal = floor(L / hIdeal);
    hIdeal = L / NIdeal;

    sigma0 = 0.5;
    sigma1 = 0.002;
    lambdaSqIdeal = c * c * k * k / (hIdeal * hIdeal);

    if (lambdaSqIdeal > 0.99)   // lambda =< 1 for stability
        lambdaSqIdeal = 0.99;

}

void OneDWave::setParamtersStiffStringVioline(float frequencyMultiplikator)
{

}

void OneDWave::setParamtersIdealStringVioline(float frequencyMultiplikator)
{

}

void OneDWave::setParamtersMetalBar(float frequencyMultiplikator)
{
    c = 400;
    L = 0.5;

    hBar = sqrt(2 * kappa1 * k);
    NBar = floor(L / hBar);
    hBar = L / NBar;

    lambdaSqBar = c * c * k * k / (hBar * hBar);
    muSqBar = (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4));

    if (lambdaSqBar > 0.99)   // lambda =< 1 for stability
        lambdaSqBar = 0.99;
    if (muSqBar > 0.99)   // lambda =< 1 for stability
        muSqBar = 0.5;
}

void OneDWave::setParamtersWoodenBar(float frequencyMultiplikator)
{
    c = 60;
    L = 0.1;
    kappa1 = 0.02;

    hBar = sqrt(2 * kappa1 * k);
    NBar = floor(L / hBar);
    hBar = L / NBar;

    lambdaSqBar = c * c * k * k / (hBar * hBar);
    muSqBar = (pow(k, 2) * pow(kappa1, 2) / pow(hBar, 4));

    if (lambdaSqBar > 0.99)   // lambda =< 1 for stability
        lambdaSqBar = 0.99;
    if (muSqBar > 0.99)   // lambda =< 1 for stability
        muSqBar = 0.5;
}











//==============Define all different Types of Calculation==============


void OneDWave::calculateSchemeNone()
{
    for(int l = 1; l < NIdeal; l++)
    {
        
        
        u[0][l] = 0;
        
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeExciteBody()
{
    for(int l = 1; l < NIdeal; l++)
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

    for(int l = 2; l < NStiff-1; l++) //clamped boundaries
    {
        

        u[0][l] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * u[1][l] 
                + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * u[1][l + 1] 
                + u[1][l - 1] - pow(mu, 2) * (u[1][l + 2] + u[1][l - 2]) 
                + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * u[2][l] - 2 * sigma1 * k / pow(hStiff, 2) * (u[2][l + 1] 
                + u[2][l - 1])) / (1 + sigma0 * k);

        v[0][l] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * v[1][l]
                + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * v[1][l + 1]
                + v[1][l - 1] - pow(mu, 2) * (v[1][l + 2] + v[1][l - 2])
                + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * v[2][l] - 2 * sigma1 * k / pow(hStiff, 2) * (v[2][l + 1]
                + v[2][l - 1])) / (1 + sigma0 * k);

        w[0][l] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * w[1][l]
                + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * w[1][l + 1]
                + w[1][l - 1] - pow(mu, 2) * (w[1][l + 2] + w[1][l - 2])
                + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * w[2][l] - 2 * sigma1 * k / pow(hStiff, 2) * (w[2][l + 1]
                + w[2][l - 1])) / (1 + sigma0 * k);

        m[0][l] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * m[1][l]
                + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * m[1][l + 1]
                + m[1][l - 1] - pow(mu, 2) * (m[1][l + 2] + m[1][l - 2])
                + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * m[2][l] - 2 * sigma1 * k / pow(hStiff, 2) * (m[2][l + 1]
                + m[2][l - 1])) / (1 + sigma0 * k);


        
    }
    
    //Boundary Conditions Stiff String
    u[0][1] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * u[1][1]
            + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * u[1][2]
            + u[1][0] - pow(mu, 2) * (u[1][3])
            + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * u[2][1] - 2 * sigma1 * k / pow(hStiff, 2) * (u[2][2]
            + u[2][0])) / (1 + sigma0 * k);
    u[0][NStiff - 1] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * u[1][NStiff - 1]
            + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * u[1][NStiff]
            + u[1][NStiff-2] - pow(mu, 2) * (u[1][NStiff - 3])
            + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * u[2][NStiff - 1] - 2 * sigma1 * k / pow(hStiff, 2) * (u[2][NStiff]
            + u[2][NStiff - 2])) / (1 + sigma0 * k);

    v[0][1] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * v[1][1]
                + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * v[1][2]
                + v[1][0] - pow(mu, 2) * (v[1][3])
                + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * v[2][1] - 2 * sigma1 * k / pow(hStiff, 2) * (v[2][2]
                + v[2][0])) / (1 + sigma0 * k);
    v[0][NStiff - 1] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * v[1][NStiff - 1]
                        + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * v[1][NStiff]
                        + v[1][NStiff - 2] - pow(mu, 2) * (v[1][NStiff - 3])
                        + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * v[2][NStiff - 1] - 2 * sigma1 * k / pow(hStiff, 2) * (v[2][NStiff]
                        + v[2][NStiff - 2])) / (1 + sigma0 * k);

    w[0][1] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * w[1][1]
            + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * w[1][2]
            + w[1][0] - pow(mu, 2) * (w[1][3])
            + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * w[2][1] - 2 * sigma1 * k / pow(hStiff, 2) * (w[2][2]
            + w[2][0])) / (1 + sigma0 * k);

    w[0][NStiff - 1] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * w[1][NStiff - 1]
            + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * w[1][NStiff]
            + w[1][NStiff - 2] - pow(mu, 2) * (w[1][NStiff - 3])
            + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * w[2][NStiff - 1] - 2 * sigma1 * k / pow(hStiff, 2) * (w[2][NStiff]
            + w[2][NStiff - 2])) / (1 + sigma0 * k);

    m[0][1] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * m[1][1]
            + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * m[1][2]
            + m[1][0] - pow(mu, 2) * (m[1][3])
            + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * m[2][1] - 2 * sigma1 * k / pow(hStiff, 2) * (m[2][2]
            + m[2][0])) / (1 + sigma0 * k);

    m[0][NStiff - 1] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(hStiff, 2)) * m[1][NStiff - 1]
            + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(hStiff, 2)) * m[1][NStiff]
            + m[1][NStiff - 2] - pow(mu, 2) * (m[1][NStiff - 3])
            + (-1 + sigma0 * k + 4 * sigma1 * k / pow(hStiff, 2)) * m[2][NStiff - 1] - 2 * sigma1 * k / pow(hStiff, 2) * (m[2][NStiff]
            + m[2][NStiff - 2])) / (1 + sigma0 * k);


    shouldExcite =  true;
    
}




void OneDWave::calculateSchemeStiffstringWithVioline()
{
    for(int l = 2; l < NStiff-1; l++)
    {            
        u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSqIdeal * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeIdealStringWithGuitar()
{
    for(int l = 1; l < NIdeal; l++)
    {
        
        
        u[0][l] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * u[1][l] - (1 - sigma0 / k) * u[2][l] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * u[1][l + 1]
                   + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * u[1][l - 1] - 2 * sigma1 / k * hIdeal * hIdeal * u[2][l + 1] + 4 * sigma1 / k * hIdeal * hIdeal * u[2][l]
                   - 2 * sigma1 / k * hIdeal * hIdeal * u[2][l - 1]) / (1 + sigma0 / k);

        v[0][l] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * v[1][l] - (1 - sigma0 / k) * v[2][l] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * v[1][l + 1]
                + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * v[1][l - 1] - 2 * sigma1 / k * hIdeal * hIdeal * v[2][l + 1] + 4 * sigma1 / k * hIdeal * hIdeal * v[2][l]
                - 2 * sigma1 / k * hIdeal * hIdeal * v[2][l - 1]) / (1 + sigma0 / k);

        w[0][l] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * w[1][l] - (1 - sigma0 / k) * w[2][l] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * w[1][l + 1]
                + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * w[1][l - 1] - 2 * sigma1 / k * hIdeal * hIdeal * w[2][l + 1] + 4 * sigma1 / k * hIdeal * hIdeal * w[2][l]
                - 2 * sigma1 / k * hIdeal * hIdeal * w[2][l - 1]) / (1 + sigma0 / k);

        m[0][l] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * m[1][l] - (1 - sigma0 / k) * m[2][l] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * m[1][l + 1]
                + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * m[1][l - 1] - 2 * sigma1 / k * hIdeal * hIdeal * m[2][l + 1] + 4 * sigma1 / k * hIdeal * hIdeal * m[2][l]
                - 2 * sigma1 / k * hIdeal * hIdeal * m[2][l - 1]) / (1 + sigma0 / k);
        
        //u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSqIdeal * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }

    //Boundaries Ideal String//

    u[0][1] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * u[1][1] - (1 - sigma0 / k) * u[2][1] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * u[1][2]
                + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * u[1][0] - 2 * sigma1 / k * hIdeal * hIdeal * u[2][2] + 4 * sigma1 / k * hIdeal * hIdeal * u[2][1]
                - 2 * sigma1 / k * hIdeal * hIdeal * u[2][0]) / (1 + sigma0 / k);
    u[0][NIdeal - 2] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * u[1][NIdeal - 2] - (1 - sigma0 / k) * u[2][NIdeal - 2] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * u[1][NIdeal - 1]
                        + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * u[1][NIdeal - 3] - 2 * sigma1 / k * hIdeal * hIdeal * u[2][NIdeal - 1] + 4 * sigma1 / k * hIdeal * hIdeal * u[2][NIdeal - 2]
                        - 2 * sigma1 / k * hIdeal * hIdeal * u[2][NIdeal - 3]) / (1 + sigma0 / k);

    v[0][1] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * v[1][1] - (1 - sigma0 / k) * v[2][1] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * v[1][2]
                + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * v[1][0] - 2 * sigma1 / k * hIdeal * hIdeal * v[2][2] + 4 * sigma1 / k * hIdeal * hIdeal * v[2][1]
                - 2 * sigma1 / k * hIdeal * hIdeal * v[2][0]) / (1 + sigma0 / k);
    v[0][NIdeal - 2] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * v[1][NIdeal - 2] - (1 - sigma0 / k) * v[2][NIdeal - 2] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * v[1][NIdeal - 1]
                + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * v[1][NIdeal - 3] - 2 * sigma1 / k * hIdeal * hIdeal * v[2][NIdeal - 1] + 4 * sigma1 / k * hIdeal * hIdeal * v[2][NIdeal - 2]
                - 2 * sigma1 / k * hIdeal * hIdeal * v[2][NIdeal - 3]) / (1 + sigma0 / k);

    w[0][1] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * w[1][1] - (1 - sigma0 / k) * w[2][1] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * w[1][2]
            + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * w[1][0] - 2 * sigma1 / k * hIdeal * hIdeal * w[2][2] + 4 * sigma1 / k * hIdeal * hIdeal * w[2][1]
            - 2 * sigma1 / k * hIdeal * hIdeal * w[2][0]) / (1 + sigma0 / k);
    w[0][NIdeal - 2] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * w[1][NIdeal - 2] - (1 - sigma0 / k) * w[2][NIdeal - 2] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * w[1][NIdeal - 1]
            + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * w[1][NIdeal - 3] - 2 * sigma1 / k * hIdeal * hIdeal * w[2][NIdeal - 1] + 4 * sigma1 / k * hIdeal * hIdeal * w[2][NIdeal - 2]
            - 2 * sigma1 / k * hIdeal * hIdeal * w[2][NIdeal - 3]) / (1 + sigma0 / k);

    m[0][1] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * m[1][1] - (1 - sigma0 / k) * m[2][1] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * m[1][2]
            + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * m[1][0] - 2 * sigma1 / k * hIdeal * hIdeal * m[2][2] + 4 * sigma1 / k * hIdeal * hIdeal * m[2][1]
            - 2 * sigma1 / k * hIdeal * hIdeal * m[2][0]) / (1 + sigma0 / k);
    m[0][NIdeal - 2] = ((2 - 2 * lambdaSqIdeal - 4 * sigma1 / k * pow(hIdeal, 2)) * m[1][NIdeal - 2] - (1 - sigma0 / k) * m[2][NIdeal - 2] + (lambdaSqIdeal + 2 * sigma1 / k * hIdeal * hIdeal) * m[1][NIdeal - 1]
                    + (lambdaSqIdeal - 2 * sigma1 / k * hIdeal * hIdeal) * m[1][NIdeal - 3] - 2 * sigma1 / k * hIdeal * hIdeal * m[2][NIdeal - 1] + 4 * sigma1 / k * hIdeal * hIdeal * m[2][NIdeal - 2]
                    - 2 * sigma1 / k * hIdeal * hIdeal * m[2][NIdeal - 3]) / (1 + sigma0 / k);
    
    shouldExcite =  true;
}
void OneDWave::calculateSchemeIdealStringWithVioline()
{
    for(int l = 1; l < NIdeal; l++)
    {
        
        u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSqIdeal * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeMetalBarWithXylophon()
{
    for (int l = 3; l < NBar - 2; l++)
    {

        u[0][l] = (2 * u[1][l] - u[2][l] - muSqBar * (u[1][l + 2] - 4 * u[1][l + 1] 
                   + 6 * u[1][l] - 4 * u[1][l - 1] + u[1][l - 2]) 
                   + sigma0 * k * u[2][l]) / (1 + sigma0 * k);

        v[0][l] = (2 * v[1][l] - v[2][l] - muSqBar * (v[1][l + 2] - 4 * v[1][l + 1]
                    + 6 * v[1][l] - 4 * v[1][l - 1] + v[1][l - 2])
                    + sigma0 * k * v[2][l]) / (1 + sigma0 * k);

        w[0][l] = (2 * v[1][l] - v[2][l] - muSqBar * (w[1][l + 2] - 4 * w[1][l + 1]
                + 6 * w[1][l] - 4 * w[1][l - 1] + w[1][l - 2])
                + sigma0 * k * w[2][l]) / (1 + sigma0 * k);

        m[0][l] = (2 * m[1][l] - m[2][l] - muSqBar * (m[1][l + 2] - 4 * m[1][l + 1]
                + 6 * m[1][l] - 4 * m[1][l - 1] + m[1][l - 2])
                + sigma0 * k * m[2][l]) / (1 + sigma0 * k);
    }

    //Boundary Conditions
    u[0][1] = (2 * u[1][3] - u[2][3] - muSqBar * (u[1][5] - 4 * u[1][4]
            + 5 * u[1][3] - 4 * u[1][2])
            + sigma0 * k * u[2][3]) / (1 + sigma0 * k);
    u[0][NBar] = (2 * u[1][NBar-2] - u[2][NBar-2] - muSqBar * (- 4 * u[1][NBar - 1]
            + 5 * u[1][NBar - 2] - 4 * u[1][NBar - 3] + u[1][NBar - 4])
            + sigma0 * k * u[2][NBar-2]) / (1 + sigma0 * k);

    v[0][1] = (2 * v[1][3] - v[2][3] - muSqBar * (v[1][5] - 4 * v[1][4]
            + 5 * v[1][3] - 4 * v[1][2])
            + sigma0 * k * v[2][3]) / (1 + sigma0 * k);

    v[0][NBar] = (2 * v[1][NBar - 2] - v[2][NBar - 2] - muSqBar * (-4 * v[1][NBar - 1]
            + 5 * v[1][NBar - 2] - 4 * v[1][NBar - 3] + v[1][NBar - 4])
            + sigma0 * k * v[2][NBar - 2]) / (1 + sigma0 * k);

    w[0][1] = (2 * w[1][3] - w[2][3] - muSqBar * (w[1][5] - 4 * w[1][4]
            + 5 * w[1][3] - 4 * w[1][2])
            + sigma0 * k * w[2][3]) / (1 + sigma0 * k);

    w[0][NBar] = (2 * w[1][NBar - 2] - w[2][NBar - 2] - muSqBar * (-4 * w[1][NBar - 1]
            + 5 * w[1][NBar - 2] - 4 * w[1][NBar - 3] + w[1][NBar - 4])
            + sigma0 * k * w[2][NBar - 2]) / (1 + sigma0 * k);

    m[0][1] = (2 * m[1][3] - m[2][3] - muSqBar * (m[1][5] - 4 * m[1][4]
            + 5 * m[1][3] - 4 * m[1][2])
            + sigma0 * k * m[2][3]) / (1 + sigma0 * k);

    m[0][NBar] = (2 * m[1][NBar - 2] - m[2][NBar - 2] - muSqBar * (-4 * m[1][NBar - 1]
               + 5 * m[1][NBar - 2] - 4 * m[1][NBar - 3] + m[1][NBar - 4])
               + sigma0 * k * m[2][NBar - 2]) / (1 + sigma0 * k);

        shouldExcite =  true;
}

void OneDWave::calculateSchemeWoodBarWithXylophon()
{
    for(int l = 3; l < NBar-2; l++)
    {
        
        u[0][l] = (2 * u[1][l] - u[2][l] - muSqBar * (u[1][l + 2] - 4 * u[1][l + 1] + 6 * u[1][l] 
                - 4 * u[1][l - 1] + u[1][l - 2]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);

        v[0][l] = (2 * v[1][l] - v[2][l] - muSqBar * (v[1][l + 2] - 4 * v[1][l + 1] + 6 * v[1][l]
                - 4 * v[1][l - 1] + v[1][l - 2]) + sigma0 * k * v[2][l]) / (1 + sigma0 * k);

        w[0][l] = (2 * w[1][l] - w[2][l] - muSqBar * (w[1][l + 2] - 4 * w[1][l + 1] + 6 * w[1][l]
                - 4 * w[1][l - 1] + w[1][l - 2]) + sigma0 * k * w[2][l]) / (1 + sigma0 * k);

        m[0][l] = (2 * m[1][l] - m[2][l] - muSqBar * (m[1][l + 2] - 4 * m[1][l + 1]
                + 6 * m[1][l] - 4 * m[1][l - 1] + m[1][l - 2])
                + sigma0 * k * m[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
        
    }
    
    //Boundary Conditions
    u[0][1] = (2 * u[1][3] - u[2][3] - muSqBar * (u[1][5] - 4 * u[1][4]
            + 5 * u[1][3] - 4 * u[1][2])
            + sigma0 * k * u[2][3]) / (1 + sigma0 * k);
    u[0][NBar] = (2 * u[1][NBar - 2] - u[2][NBar - 2] - muSqBar * (-4 * u[1][NBar - 1]
               + 5 * u[1][NBar - 2] - 4 * u[1][NBar - 3] + u[1][NBar - 4])
               + sigma0 * k * u[2][NBar - 2]) / (1 + sigma0 * k);

    v[0][1] = (2 * v[1][3] - v[2][3] - muSqBar * (v[1][5] - 4 * v[1][4]
              + 5 * v[1][3] - 4 * v[1][2])
              + sigma0 * k * v[2][3]) / (1 + sigma0 * k);
    v[0][NBar] = (2 * v[1][NBar - 2] - v[2][NBar - 2] - muSqBar * (-4 * v[1][NBar - 1]
              + 5 * v[1][NBar - 2] - 4 * v[1][NBar - 3] + v[1][NBar - 4])
              + sigma0 * k * v[2][NBar - 2]) / (1 + sigma0 * k);

    w[0][1] = (2 * w[1][3] - w[2][3] - muSqBar * (w[1][5] - 4 * w[1][4]
            + 5 * w[1][3] - 4 * w[1][2])
            + sigma0 * k * w[2][3]) / (1 + sigma0 * k);
    w[0][NBar] = (2 * w[1][NBar - 2] - w[2][NBar - 2] - muSqBar * (-4 * w[1][NBar - 1]
            + 5 * w[1][NBar - 2] - 4 * w[1][NBar - 3] + w[1][NBar - 4])
            + sigma0 * k * w[2][NBar - 2]) / (1 + sigma0 * k);

    m[0][1] = (2 * m[1][3] - m[2][3] - muSqBar * (m[1][5] - 4 * m[1][4]
            + 5 * m[1][3] - 4 * m[1][2])
            + sigma0 * k * m[2][3]) / (1 + sigma0 * k);

    m[0][NBar] = (2 * m[1][NBar - 2] - m[2][NBar - 2] - muSqBar * (-4 * m[1][NBar - 1]
            + 5 * m[1][NBar - 2] - 4 * m[1][NBar - 3] + m[1][NBar - 4])
            + sigma0 * k * m[2][NBar - 2]) / (1 + sigma0 * k);

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
    
    double* vTmp = v[2];
    v[2] = v[1];            //read it like u1 is going to be u2
    v[1] = v[0];            //read it like u0 is going to be u1
    v[0] = vTmp;            // now get back the adress of the vector that can be overwritten

    double* wTmp = w[2];
    w[2] = w[1];            //read it like u1 is going to be u2
    w[1] = w[0];            //read it like u0 is going to be u1
    w[0] = wTmp;            // now get back the adress of the vector that can be overwritten

    double* mTmp = m[2];
    m[2] = m[1];            //read it like u1 is going to be u2
    m[1] = m[0];            //read it like u0 is going to be u1
    m[0] = mTmp;            // now get back the adress of the vector that can be overwritten

    
    //******
}

















