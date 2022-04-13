/*
  ==============================================================================

    OneDWave.cpp
    Created: 24 Mar 2022 8:38:24am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OneDWave.h"

//==============================================================================
OneDWave::OneDWave (double kIn) : k (kIn) // <- This is an initialiser list. It initialises the member variable 'k' (in the "private" section in OneDWave.h), using the argument of the constructor 'kIn'. Basically, k = kIn;
{
    
    
    
    
    
    //TO DO
    //Does it make sense to check the state of MainComponent::
    //Evertime the ComboBox is changed, select new values here for the calculation!!
    //But make sure it is only done once in a while and not every sample step!!
    
    
    
    
    
    
    
    
    
    
     c = 300; // Wave speed (in m/s)
     L = 3; // Length (in m) // Time step (in s)
     
     
     h = c * k; // Grid spacing (in m)
     N = floor(L / h);
     h = L / N;
     
     lambdaSq = c*c * k*k / (h*h) ;
     sigma0 = 1;
     sigma1 = 0.01;
     kappa = 0.006;
     hBar = sqrt(2 * kappa * k);
     
     B0 = (2 - 2) * lambdaSq;
     
     //DBG (lambdaSq);
     //StabilityCheck
     if (lambdaSq > 0.99)   // lambda =< 1 for stability
     lambdaSq = 0.99;
    
    //******
    
        
    // Initialise vectors containing the state of the system
    uStates = std::vector<std::vector<double>> (3, // initializing a vector of vectors (a matrix) or 3xN+1
                                        std::vector<double>(N+1, 0)); // initializing a vector, with length, content
    wStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content
    xStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content
    yStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content
    zStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content
    nStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
                                        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content
    
    // Initialise vector of pointers to the states
    u.resize (3, nullptr);
    w.resize (3, nullptr);
    x.resize (3, nullptr);
    y.resize (3, nullptr);
    z.resize (3, nullptr);
    n.resize (3, nullptr);
    
    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];
    for (int i = 0; i < 3; ++i)
        w[i] = &wStates[i][0];
    for (int i = 0; i < 3; ++i)
        x[i] = &xStates[i][0];
    for (int i = 0; i < 3; ++i)
        y[i] = &yStates[i][0];
    for (int i = 0; i < 3; ++i)
        z[i] = &zStates[i][0];
    for (int i = 0; i < 3; ++i)
        n[i] = &nStates[i][0];
    
        
    
    // Excite at the start halfway along the system.
    //excite (0.5); // excite function 

}








//What is that for?
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
    
    for (int l = 1; l <= N; l++)
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











// mouse Click
void OneDWave::mouseDown (const MouseEvent &e)
{
    c = wavespeed; // Wave speed (in m/s)
    L = stringLength; // Length (in m) // Time step (in s)
//    a = amplitude;
    
    h = c * k; // Grid spacing (in m)
    N = floor(L / h);
    h = L / N;
    
    lambdaSq = c*c * k*k / (h*h) ;
    sigma0 = damping;
    sigma1 = 0.01;
    
    B0 = (2 - 2) * lambdaSq;
    
    //DBG (lambdaSq);
    //StabilityCheck
    if (lambdaSq > 0.99)   // lambda =< 1 for stability
        lambdaSq = 0.99;
    excitationLoc = e.x / static_cast<double> (getWidth());

    // Activate the excitation flag to be used by the MainComponent to excite the string
    excitationFlag = true;
    
   
}






//==============Define all different Types of Excitation==============
/*
 TO DO
 
void exciteNone (double excitationLoc);
void exciteExciteBody (double excitationLoc);
void exciteStiffstringWithGuitar (double excitationLoc);
void exciteStiffstringWithVioline (double excitationLoc);
void exciteIdealStringWithGuitar (double excitationLoc);
void exciteIdealStringWithVioline (double excitationLoc);
void exciteMetalBarWithXylophon (double excitationLoc);
void exciteWoodBarWithXylophon (double excitationLoc);
*/




void OneDWave::excite (double excitationLoc)
{
    float a;
    // width (in grid points) of the excitation
    double width = 10;
    if (amplitude >= 0)
        a = amplitude;
    else a = 0.1;
    DBG(a);
    // make sure we're not going out of bounds at the left boundary
    int start = std::max (floor((N+1) * excitationLoc) - floor(width * 0.5), 1.0);
    
    for (int l = 0; l < width; ++l)
    {
        // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
        if (l + start > (clamped ? N - 2 : N - 1))
            break;
        
        u[1][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
        u[2][l+start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));

        w[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
        w[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

        x[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
        x[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

        y[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
        y[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

        z[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
        z[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));

        n[1][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));
        n[2][l + start] += a * (1 - cos(2.0 * MathConstants<double>::pi * l / (width - 1.0)));


        // initializing uPrev = u so that the difference between the two won't be so huge
    }
    excitationFlag = false;
}










//==============Define all different Types of Calculation==============


void OneDWave::calculateSchemeNone()
{
    for(int l = 1; l < N; l++)
    {
        
        
        u[0][l] = 0;
        
        
    }
    
    //shouldExcite =  true;
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
    
    //shouldExcite =  true;
}

void OneDWave::calculateSchemeStiffstringWithGuitar()
{

    for(int l = 2; l < N-3; l++)
    {
       
       
        u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    
    
}




void OneDWave::calculateSchemeStiffstringWithVioline()
{
    for(int l = 2; l < N-3; l++)
    {
       
        
        u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);

        // To do: Add frquency depended damping!
        
    }
    
    
}

void OneDWave::calculateSchemeIdealStringWithGuitar()
{
    for(int l = 2; l < N-3; l++)
    {
        
        
        u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * (u[1][l + 1] - 2 * u[1][l] + u[1][l - 1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        w[0][l] = (2 * w[1][l] - w[2][l] + lambdaSq * (w[1][l + 1] - 2 * w[1][l] + w[1][l - 1]) + sigma0 * k * w[2][l]) / (1 + sigma0 * k);
        x[0][l] = (2 * x[1][l] - x[2][l] + lambdaSq * (x[1][l + 1] - 2 * x[1][l] + x[1][l - 1]) + sigma0 * k * x[2][l]) / (1 + sigma0 * k);
        y[0][l] = (2 * y[1][l] - y[2][l] + lambdaSq * (y[1][l + 1] - 2 * y[1][l] + y[1][l - 1]) + sigma0 * k * y[2][l]) / (1 + sigma0 * k);
        z[0][l] = (2 * z[1][l] - z[2][l] + lambdaSq * (z[1][l + 1] - 2 * z[1][l] + z[1][l - 1]) + sigma0 * k * z[2][l]) / (1 + sigma0 * k);
        n[0][l] = (2 * n[1][l] - n[2][l] + lambdaSq * (n[1][l + 1] - 2 * n[1][l] + n[1][l - 1]) + sigma0 * k * n[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    
}
void OneDWave::calculateSchemeIdealStringWithVioline()
{
    for(int l = 2; l < N-3; l++)
    {
        
        
        u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * (u[1][l + 1] - 2 * u[1][l] + u[1][l - 1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        w[0][l] = (2 * w[1][l] - w[2][l] + lambdaSq * (w[1][l + 1] - 2 * w[1][l] + w[1][l - 1]) + sigma0 * k * w[2][l]) / (1 + sigma0 * k);
        x[0][l] = (2 * x[1][l] - x[2][l] + lambdaSq * (x[1][l + 1] - 2 * x[1][l] + x[1][l - 1]) + sigma0 * k * x[2][l]) / (1 + sigma0 * k);
        y[0][l] = (2 * y[1][l] - y[2][l] + lambdaSq * (y[1][l + 1] - 2 * y[1][l] + y[1][l - 1]) + sigma0 * k * y[2][l]) / (1 + sigma0 * k);

        // To do: Add frquency depended damping!
        
    }
    
    
}
void OneDWave::calculateSchemeMetalBarWithXylophon()
{
    for(int l = 2; l < N-3; l++)
    {
       
        
        u[0][l] = (2 * u[1][l] - u[2][l] - (pow(k, 2) * pow(kappa, 2) / pow(hBar, 4)) * (u[1][l + 2] - 4 * u[1][l + 1] + 6 * u[1][l] - 4 * u[1][l - 1] + u[1][l - 2]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        w[0][l] = (2 * w[1][l] - w[2][l] - (pow(k, 2) * pow(kappa, 2) / pow(hBar, 4)) * (w[1][l + 2] - 4 * w[1][l + 1] + 6 * w[1][l] - 4 * w[1][l - 1] + w[1][l - 2]) + sigma0 * k * w[2][l]) / (1 + sigma0 * k);
        x[0][l] = (2 * x[1][l] - x[2][l] - (pow(k, 2) * pow(kappa, 2) / pow(hBar, 4)) * (x[1][l + 2] - 4 * x[1][l + 1] + 6 * x[1][l] - 4 * x[1][l - 1] + x[1][l - 2]) + sigma0 * k * x[2][l]) / (1 + sigma0 * k);
        y[0][l] = (2 * y[1][l] - y[2][l] - (pow(k, 2) * pow(kappa, 2) / pow(hBar, 4)) * (y[1][l + 2] - 4 * y[1][l + 1] + 6 * y[1][l] - 4 * y[1][l - 1] + y[1][l - 2]) + sigma0 * k * y[2][l]) / (1 + sigma0 * k);
        z[0][l] = (2 * z[1][l] - z[2][l] - (pow(k, 2) * pow(kappa, 2) / pow(hBar, 4)) * (z[1][l + 2] - 4 * z[1][l + 1] + 6 * z[1][l] - 4 * z[1][l - 1] + z[1][l - 2]) + sigma0 * k * z[2][l]) / (1 + sigma0 * k);
        n[0][l] = (2 * n[1][l] - n[2][l] - (pow(k, 2) * pow(kappa, 2) / pow(hBar, 4)) * (n[1][l + 2] - 4 * n[1][l + 1] + 6 * n[1][l] - 4 * n[1][l - 1] + n[1][l - 2]) + sigma0 * k * n[2][l]) / (1 + sigma0 * k);

        // To do: Add frquency depended damping!
        
    }
    
    
}
void OneDWave::calculateSchemeWoodBarWithXylophon()
{
    for(int l = 2; l < N-3; l++)
    {
       
        
        u[0][l] = (2 * u[1][l] - u[2][l] - (pow(k, 2) * pow(kappa, 2) / pow(hBar, 4)) * (u[1][l + 2] - 4 * u[1][l + 1] + 6 * u[1][l] - 4 * u[1][l - 1] + u[1][l - 2]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    
}









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
    w[0] = wTmp;

    double* xTmp = x[2]; // saving that location in memory for uPrev, /we need to store the pointer adress. even if we don't need the data anymore
    x[2] = x[1];            //read it like u1 is going to be u2
    x[1] = x[0];            //read it like u0 is going to be u1
    x[0] = xTmp;            // now get back the adress of the vector that can be overwritten
    double* yTmp = y[2];
    y[2] = y[1];            //read it like u1 is going to be u2
    y[1] = y[0];            //read it like u0 is going to be u1
    y[0] = yTmp;

    double* zTmp = z[2]; // saving that location in memory for uPrev, /we need to store the pointer adress. even if we don't need the data anymore
    z[2] = z[1];            //read it like u1 is going to be u2
    z[1] = z[0];            //read it like u0 is going to be u1
    z[0] = zTmp;            // now get back the adress of the vector that can be overwritten
    double* nTmp = n[2];
    n[2] = n[1];            //read it like u1 is going to be u2
    n[1] = n[0];            //read it like u0 is going to be u1
    n[0] = nTmp;

    
    
    //******
}

















