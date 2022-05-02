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
     sigma1 = 0.005;
     kappa1 = 0.001;
     //stabilityTerm = pow(c, 2) * pow(k, 2) + 4 * sigma1 * k;
     h = c * k; // Grid spacing (in m)
     N = floor(L / h);
     h = L / N;
     
     lambdaSq = c*c * k*k / (h*h) ;
     sigma0 = 1;
     
     
     
     
     //DBG (lambdaSq);
     //StabilityCheck
     if (lambdaSq > 0.99)   // lambda =< 1 for stability
     lambdaSq = 0.99;
    
    //******
    
        
    // Initialise vectors containing the state of the system
    uStates = std::vector<std::vector<double>> (3, // initializing a vector of vectors (a matrix) or 3xN+1
                                                std::vector<double>(N+1, 0)); // initializing a vector, with length, content
    
    // Initialise vector of pointers to the states
    u.resize (3, nullptr);
    
    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];
    
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













//  ====================================


void OneDWave::setParamtersStiffStringGuitar(float frequencyMultiplikator)
{

    c = 300; // Wave speed (in m/s)
    L = 0.5; // Length (in m) // Time step (in s)
    //    a = amplitude;
    kappa1 = 0.001;
    sigma1 = 0.005;
    stabilityTerm = pow(c, 2) * pow(k, 2) + 4 * sigma1 * k;
    h = sqrt(stabilityTerm + sqrt(pow(stabilityTerm, 2) + 16 * pow(kappa1, 2) * pow(k, 2)) / 2);
    N = floor(L / h);
    h = L / N;

    
    lambda = c * k / h;
    mu = kappa1 * k / pow(h, 2);
    

    

    //DBG (lambdaSq);
    //StabilityCheck
    if (stabilityTerm > 0.99)   // lambda =< 1 for stability
        stabilityTerm = 0.99;
    if (mu > 0.99)
        mu = 0.99;
    if (lambda > 0.99)
        lambda = 0.99;

    uStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content

// Initialise vector of pointers to the states
    u.resize(3, nullptr);

    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];

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

    uStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content

// Initialise vector of pointers to the states
    u.resize(3, nullptr);

    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];
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

    uStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content

// Initialise vector of pointers to the states
    u.resize(3, nullptr);

    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];
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

    uStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content

// Initialise vector of pointers to the states
    u.resize(3, nullptr);

    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];
}

void OneDWave::setParamtersMetalBar(float frequencyMultiplikator)
{
    c = 400; // Wave speed (in m/s)
    L = 0.01; // Length (in m) // Time step (in s)
    kappa1 = 0.001;
    h = sqrt(2 * kappa1 * k); // Grid spacing (in m)
    N = floor(L / h);
    h = L / N;
    lambdaSq = c * c * k * k / (h * h);
    
    
    //sigma1 = 0.01;
    muSq = (pow(k, 2) * pow(kappa1, 2) / pow(h, 4));

    //StabilityCheck
    if (lambdaSq > 0.99)   // lambda =< 1 for stability
        lambdaSq = 0.99;
    if (muSq > 0.99)   // lambda =< 1 for stability
        muSq = 0.5;

    uStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content

// Initialise vector of pointers to the states
    u.resize(3, nullptr);

    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];
}

void OneDWave::setParamtersWoodenBar(float frequencyMultiplikator)
{
    c = 400; // Wave speed (in m/s)
    L = 0.3; // Length (in m) // Time step (in s)
    kappa1 = 0.001;
    h = sqrt(2 * kappa1 * k); // Grid spacing (in m)
    N = floor(L / h);
    h = L / N;
    lambdaSq = c * c * k * k / (h * h);


    //sigma1 = 0.01;
    muSq = (pow(k, 2) * pow(kappa1, 2) / pow(h, 4));

    //StabilityCheck
    if (lambdaSq > 0.99)   // lambda =< 1 for stability
        lambdaSq = 0.99;
    if (muSq > 0.99)   // lambda =< 1 for stability
        muSq = 0.5;

    uStates = std::vector<std::vector<double>>(3, // initializing a vector of vectors (a matrix) or 3xN+1
        std::vector<double>(N + 1, 0)); // initializing a vector, with length, content

// Initialise vector of pointers to the states
    u.resize(3, nullptr);

    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];
}











//==============Define all different Types of Calculation==============


void OneDWave::calculateSchemeNone()
{
    for(int l = 2; l < N-1; l++)
    {
        
        
        u[0][l] = 0;
        
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeExciteBody()
{
    for(int l = 2; l < N-1; l++)
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

    for(int l = 3; l < N-2; l++)
    {
        if (l == 1)
        {
            //shouldExcite =  false; //Prevents excitation while calculation
        }
       


        u[0][l] = ((2 - 2 * pow(lambda, 2) - 6 * pow(mu, 2) - 4 * sigma1 * k / pow(h, 2)) * u[1][l] + (pow(lambda, 2) + 4 * pow(mu, 2) + 2 * sigma1 * k / pow(h, 2)) * u[1][l + 1] + u[1][l - 1] - pow(mu, 2) * (u[1][l + 2] + u[1][l - 2]) + (-1 + sigma0 * k + 4 * sigma1 * k / pow(h, 2)) * u[2][l] - 2 * sigma1 * k / pow(h, 2) * (u[2][l + 1] + u[2][l - 1])) / (1 + sigma0 * k);

        //u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    shouldExcite =  true;
    
}




void OneDWave::calculateSchemeStiffstringWithVioline()
{
    for(int l = 2; l < N-1; l++)
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

void OneDWave::calculateSchemeIdealStringWithGuitar()
{
    for(int l = 2; l < N-1; l++)
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
void OneDWave::calculateSchemeIdealStringWithVioline()
{
    for(int l = 2; l < N-1; l++)
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
    for(int l = 3; l < N-2; l++)
    {
        
        
        
        u[0][l] = (2 * u[1][l] - u[2][l] - muSq * (u[1][l + 2] - 4 * u[1][l + 1] + 6 * u[1][l] - 4 * u[1][l - 1] + u[1][l - 2]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        
        //u[0][l] = (2 * u[1][l] - u[2][l] + lambdaSq * ( u[1][l+1] - 2 * u[1][l] + u[1][l-1]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
        // To do: Add frquency depended damping!
        
    }
    
    shouldExcite =  true;
}

void OneDWave::calculateSchemeWoodBarWithXylophon()
{
    for(int l = 3; l < N-2; l++)
    {
        if (l == 1)
        {
            //shouldExcite =  false; //Prevents excitation while calculation
        }
        
        u[0][l] = (2 * u[1][l] - u[2][l] - muSq * (u[1][l + 2] - 4 * u[1][l + 1] + 6 * u[1][l] - 4 * u[1][l - 1] + u[1][l - 2]) + sigma0 * k * u[2][l]) / (1 + sigma0 * k);
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
    
    
    //******
}

















