#pragma once

#include <JuceHeader.h>

///__________________________

#include "OneDWave.h"

///__________________________

class MainComponent  : public juce::AudioAppComponent,
///__________________________
                       public Timer, // timer to update the graphics
///__________________________
                        public Slider::Listener,
///__________________________
                        //public Button::Listener,
///__________________________
                        public ComboBox::Listener

{
public:
    //==============================================================================
    MainComponent(); // Constructor 1st
    ~MainComponent() override; // Destructor

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override; // Juce Audio Functions; same as processBlock
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override; // those are called from the back end of JUCE; always before processing
    void releaseResources() override; // release resources when the plugin gets closed
    
    //======== Mouse Trigger ==============
    //void mouseDown (const MouseEvent &e) override;
    // Called when a mouse button is pressed. 
    

    //==============================================================================
    void paint (juce::Graphics& g) override; // JUCE GUI functions
    void resized() override; // mandatory to give the 1DWave the bounds in the GUI

    ///__________________________
    
    
    // Sliders and Buttons and Comboboxes
    
    void sliderValueChanged (Slider* slider) override;
    //void buttonClicked (Button*) override;
    void comboBoxChanged (ComboBox *comboBoxThatHasChanged) override;
    
    // The timer callback function that updates Silvins string graphic.
    void timerCallback() override;
    
    // The setPicture function that updates the images.
    void clearImages();
    
    // Limiter to keep the output signal between -1 and 1. Arguments are: input value, minimum value and maximum value
    float limit (float val, float min, float max);

    ///__________________________

    
    //png
    ImageComponent mImageBodyGuitar;
    ImageComponent mImageBodyVioline;
    ImageComponent mImageNylonStringGuitar;
    ImageComponent mImageNylonStringVioline;
    ImageComponent mImageMetalBar;
    ImageComponent mImageWoodBar;
    ImageComponent mImageBodyMetalBox;
    ImageComponent mImageBodyWoodBox;
    ImageComponent mImageBodyPaperBox;
    ImageComponent mImageBodyOther;
    ImageComponent mImageWesternStringGuitar;
    ImageComponent mImageWesternStringVioline;
    ImageComponent mImageOnlyWesternString;
    ImageComponent mImageOnlyNylonString;
    
  
    
    

    
    
    
private: // Member variables
    ///__________________________
    
    double fs; // Sample rate which we can retrieve from the prepareToPlay function
    
    std::unique_ptr<OneDWave> oneDWave; // using a smart pointer for 1D wave class; initializing the pointer without knowing the size 

    
    bool showImage;
   

   //Initialise all objects
    Slider amplitudeSlider;
    Slider stringLengthSlider;
    Slider dampingSlider;
    Slider materialSlider;
    
    Label labelAmplitude;
    Label labelStringLength;
    Label labelDamping;
    Label labelMaterial;
    
    //Buttons
    //TextButton buttonInstrument1 {"String"};
    //TextButton buttonInstrument2 {"Bar"};
    //TextButton buttonInstrument3 {"Plate"};
    //TextButton buttonInstrument4 {"Stiffstring"};
    
    
    // Comboboxes
    ComboBox body;
    ComboBox stringOrBarTyp;
    ComboBox instrument;

    
    
    
    
    
    
    ///__________________________
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};




class Soundmodel
{
public:
    enum class SoundTyp
    {
        None, //without Body, without BarOrString
        ExciteBody,  //Only Body, no BarOrString
        StiffstringWithGuitar,
        StiffstringWithVioline, //Bowing
        IdealStringWithGuitar,
        IdealStringWithVioline, //Bowing
        MetalBarWithXylophon,
        WoodBarWithXylophon
    };
    
    void setSoundModel(SoundTyp t)
    {
        type = t;
        
    }
    SoundTyp getSoundTyp()
    {
        return type;
    }
    
private:
    SoundTyp type {SoundTyp::None};
    
};



class ImpulseResponse
{
    public:
        enum class ImpulseResponseTyp
        {
            Dirac,
            GuitarBody,
            ViolinBody,
            MetalBox,
            WoodBox,
            PaperBox,
            OtherBox
        };

    
    
    void setImpulseResponseTyp(ImpulseResponseTyp IR)
    {
        impulseResponsetyp = IR;
    }
    
    ImpulseResponseTyp getImpulseResponseTyp()
    {
        return impulseResponsetyp;
    }
    
    private:
    ImpulseResponseTyp impulseResponsetyp {ImpulseResponseTyp::Dirac};
    
};
