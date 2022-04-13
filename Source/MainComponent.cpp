#include "MainComponent.h"




//initialise classes from MainComponent.h
Soundmodel soundmodel;
ImpulseResponse impulseResponse;




//==============================================================================
MainComponent::MainComponent()
{
    // MainWindow Size
    setSize (600, 400);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }


    
    
    
    
    
    
    //======================addListeners
    
    
    
    /* Add the Editor as a listener to the slider
     
     Once the slider is moved, it will send a message to all listeners, which will call their implementation of the "sliderValueChanged()" function.
     */
    
        //Sliders
    amplitudeSlider.addListener (this);
    stringLengthSlider.addListener (this);
    dampingSlider.addListener (this);
    materialSlider.addListener (this);
    
    //Buttons
    //buttonInstrument1.addListener(this);
    //buttonInstrument2.addListener(this);
    //buttonInstrument3.addListener(this);
    //buttonInstrument4.addListener(this);
    //startButton.addListener(this);
    
    //Comboboxes
    body.addListener(this);
    stringOrBarTyp.addListener(this);
    instrument.addListener(this);
    
    
    
    
    
    
    
    //======================addAndMakeVidible Sliders, Buttons and Comboboxes
    
    
    
    // Make all components (slider, buttons...) visible
    
    addAndMakeVisible (amplitudeSlider);
    addAndMakeVisible (stringLengthSlider);
    addAndMakeVisible (dampingSlider);
    addAndMakeVisible (materialSlider);
    
    
    //addAndMakeVisible (labelPosition);
    addAndMakeVisible (labelAmplitude);
    addAndMakeVisible (labelStringLength);
    addAndMakeVisible (labelDamping);
    addAndMakeVisible (labelMaterial);
    
    
    //addAndMakeVisible (startButton);
    //addAndMakeVisible (buttonInstrument1);
    //addAndMakeVisible (buttonInstrument2);
    //addAndMakeVisible (buttonInstrument3);
    //addAndMakeVisible (buttonInstrument4);
    
    
    //Comboboxes
    
    addAndMakeVisible (instrument);
    addAndMakeVisible (body);
    addAndMakeVisible (stringOrBarTyp);
    
    
   
    

    //================Display initial/overall conditions for all objects
    
    
    
    // Here inital grafics and states like font-size, inital values are defined
     
    //png
    auto Image1 = ImageCache::getFromMemory(BinaryData::Westerngitarre_jpg, BinaryData::Westerngitarre_jpgSize);
    auto Image2 = ImageCache::getFromMemory(BinaryData::Alurohr_jpg, BinaryData::Alurohr_jpgSize);
    auto Image3 = ImageCache::getFromMemory(BinaryData::Geige_jpg, BinaryData::Geige_jpgSize);
    auto Image4 = ImageCache::getFromMemory(BinaryData::MetalBar_jpg, BinaryData::MetalBar_jpgSize);
    auto Image5 = ImageCache::getFromMemory(BinaryData::guitarwithoutstrings_jpg, BinaryData::guitarwithoutstrings_jpgSize);
    auto Image6 = ImageCache::getFromMemory(BinaryData::Holzbox_jpg, BinaryData::Holzbox_jpgSize);
    auto Image7 = ImageCache::getFromMemory(BinaryData::Metallbox_jpg, BinaryData::Metallbox_jpgSize);
    auto Image8 = ImageCache::getFromMemory(BinaryData::Pappkarton_jpeg, BinaryData::Pappkarton_jpegSize);
    auto Image9 = ImageCache::getFromMemory(BinaryData::violineWithoutStrings_jpg, BinaryData::violineWithoutStrings_jpgSize);
    auto Image10 = ImageCache::getFromMemory(BinaryData::WoodenBar_jpg, BinaryData::WoodenBar_jpgSize);
    auto Image11 = ImageCache::getFromMemory(BinaryData::violinNylonStrings_jpg, BinaryData::violinNylonStrings_jpgSize);
    auto Image12 = ImageCache::getFromMemory(BinaryData::GuitarNylonStrings_jpg, BinaryData::GuitarNylonStrings_jpgSize);
    auto Image13 = ImageCache::getFromMemory(BinaryData::ViolinOnlyStrings_jpg, BinaryData::ViolinOnlyStrings_jpgSize);
    auto Image14 = ImageCache::getFromMemory(BinaryData::GuitarOnlyStrings_jpg, BinaryData::GuitarOnlyStrings_jpgSize);
    
    
    
    
    
    
    //png -> Security, if file is missing
    if (! Image1.isNull())
        mImageWesternStringGuitar.setImage(Image1, RectanglePlacement::centred);
    else
        jassert(! Image1.isNull());
    
    if (! Image2.isNull())
        mImageBodyOther.setImage(Image2, RectanglePlacement::centred);
    else
        jassert(! Image2.isNull());
    
    if (! Image3.isNull())
        mImageWesternStringVioline.setImage(Image3, RectanglePlacement::centred);
    else
        jassert(! Image3.isNull());
    
    if (! Image4.isNull())
        mImageMetalBar.setImage(Image4, RectanglePlacement::centred);
    else
        jassert(! Image4.isNull());
    
    if (! Image5.isNull())
        mImageBodyGuitar.setImage(Image5, RectanglePlacement::centred);
    else
        jassert(! Image5.isNull());
    
    if (! Image6.isNull())
        mImageBodyWoodBox.setImage(Image6, RectanglePlacement::centred);
    else
        jassert(! Image6.isNull());
    
    if (! Image7.isNull())
        mImageBodyMetalBox.setImage(Image7, RectanglePlacement::centred);
    else
        jassert(! Image7.isNull());
    
    if (! Image8.isNull())
        mImageBodyPaperBox.setImage(Image8, RectanglePlacement::centred);
    else
        jassert(! Image8.isNull());
    
    if (! Image9.isNull())
        mImageBodyVioline.setImage(Image9, RectanglePlacement::centred);
    else
        jassert(! Image9.isNull());
    
    if (! Image10.isNull())
        mImageWoodBar.setImage(Image10, RectanglePlacement::centred);
    else
        jassert(! Image10.isNull());
    
    if (! Image11.isNull())
        mImageNylonStringVioline.setImage(Image11, RectanglePlacement::centred);
    else
        jassert(! Image11.isNull());
    
    if (! Image12.isNull())
        mImageNylonStringGuitar.setImage(Image12, RectanglePlacement::centred);
    else
        jassert(! Image12.isNull());
    
    if (! Image13.isNull())
        mImageOnlyNylonString.setImage(Image13, RectanglePlacement::centred);
    else
        jassert(! Image13.isNull());
    
    if (! Image14.isNull())
        mImageOnlyWesternString.setImage(Image14, RectanglePlacement::centred);
    else
        jassert(! Image14.isNull());
    
   
    
    
    
    
    //SLIDERS
    /*
     - setRange (min, max, stepSize)
     - setValue (defaultValue)
     - setStyle, Labels, attached Components...
     */
    amplitudeSlider.setRange (0, 1, 0.1);
    amplitudeSlider.setValue (0.5);
    amplitudeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    amplitudeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    labelAmplitude.setText("Amplitude", dontSendNotification);
    labelAmplitude.attachToComponent(&amplitudeSlider, false);
    labelAmplitude.setJustificationType(juce::Justification::horizontallyCentred);
    
    stringLengthSlider.setRange (1, 3, 0.1);
    stringLengthSlider.setValue (50);
    stringLengthSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    stringLengthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    //stringLengthSlider.setTextValueSuffix(" Hz ");
    labelStringLength.setText("Length", dontSendNotification);
    labelStringLength.attachToComponent(&stringLengthSlider, false);
    labelStringLength.setJustificationType(juce::Justification::horizontallyCentred);
    
    dampingSlider.setRange (0, 10, 0.1);
    dampingSlider.setValue (1);
    dampingSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    labelDamping.setText("Damping", dontSendNotification);
    labelDamping.attachToComponent(&dampingSlider, false);
    labelDamping.setJustificationType(juce::Justification::horizontallyCentred);
    
    materialSlider.setRange (300, 2000, 10);
    materialSlider.setValue (1000);
    materialSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    materialSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    labelMaterial.setText("Material", dontSendNotification);
    labelMaterial.attachToComponent(&materialSlider, false);
    labelMaterial.setJustificationType(juce::Justification::horizontallyCentred);
    
    
    
    
    //ComboBoxes
    
    instrument.setText("Instrument");
    instrument.setJustificationType(4);
    instrument.addItem("Stringinstrument",1);
    instrument.addItem("Xylophone",2);
    
    
    //Two Comboboxes are not visible at the start. Anyhow their justification is defined here
    stringOrBarTyp.setJustificationType(4);
    
    body.setJustificationType(4);
    
    

}










MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    ///__________________________
    
    // Retrieve sample rate
    fs = sampleRate;
    
    // Initialise the instance of the OneDWave class
    oneDWave = std::make_unique<OneDWave> (1.0 / fs); // our k value
    //this is a pointer, that does indential with: oneDWave oneDWave
    // better to have a pointer here, because it is initilized at another part in the code as a null-pointer. It would complain, because it is not defined earlier, but here, if not a pointer.
    
    
    // Add it to the application and make it visible (a must-do in JUCE)
    addAndMakeVisible(oneDWave.get());
    
    
    // Call the resized function now that the instance of the OneDWave class is initialised
    resized();
    //connected to the void below, that comes with a if-cause, "if oneDwave is still a nullpointer"
    //also somehow gets the setSize
    
    // Start the graphics timer (refresh 15 times per second)
    startTimerHz (15);
    
    ///__________________________
}






















//============================MAIN AUDIO CORE==========================




void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) //gets called from the JUCE back end with information about the buffer
{
    ///__________________________

    // Prevent noise in the buffer.
    bufferToFill.clearActiveBufferRegion();

    // Get the number of channels.
    int numChannels = bufferToFill.buffer->getNumChannels();
    
    // Get pointers to output locations.
    float* const channelData1 = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    float* const channelData2 = numChannels > 1 ? bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample) : nullptr;

    // filling two vectors with outputs
    
    float outputSoundModel = 0.0;
    float output = 0.0;

    std::vector<float* const*> curChannel {&channelData1, &channelData2};
    
    
    
    
    
    
    // For all samples in the buffer.. // do things here // take in the
    for (int i = 0; i < bufferToFill.numSamples; ++i) // loop over all samples in the buffer
    {
        
        
        
        
            if (soundmodel.getSoundTyp() == Soundmodel::SoundTyp::None)
            {
                oneDWave->calculateSchemeNone();
                oneDWave->updateStates();
                outputSoundModel = oneDWave->getOutput (0.8);
                //DBG("None");
            }
            else if (soundmodel.getSoundTyp() == Soundmodel::SoundTyp::ExciteBody)
            {
                oneDWave->calculateSchemeExciteBody();
                oneDWave->updateStates();
                outputSoundModel = oneDWave->getOutput (0.8);
               // DBG("ExciteBody");
            }
            else if (soundmodel.getSoundTyp() == Soundmodel::SoundTyp::StiffstringWithGuitar)
            {
                oneDWave->calculateSchemeStiffstringWithGuitar();
                oneDWave->updateStates();
                outputSoundModel = oneDWave->getOutput (0.8);
                //DBG("StiffstringWithGuitar");
            }
            else if (soundmodel.getSoundTyp() == Soundmodel::SoundTyp::StiffstringWithVioline)
            {
                oneDWave->calculateSchemeStiffstringWithVioline();
                oneDWave->updateStates();
                outputSoundModel = oneDWave->getOutput (0.8);
                //DBG("StiffstringWithVioline");
            }
            else if (soundmodel.getSoundTyp() == Soundmodel::SoundTyp::IdealStringWithGuitar)
            {
                oneDWave->calculateSchemeIdealStringWithGuitar();
                oneDWave->updateStates();
                outputSoundModel = oneDWave->getOutput (0.8);
                //DBG("IdealStringWithGuitar");
            }
            else if (soundmodel.getSoundTyp() == Soundmodel::SoundTyp::IdealStringWithVioline)
            {
                oneDWave->calculateSchemeIdealStringWithVioline();
                oneDWave->updateStates();
                outputSoundModel = oneDWave->getOutput (0.8);
                //DBG("IdealStringWithVioline");
            }
            else if (soundmodel.getSoundTyp() == Soundmodel::SoundTyp::MetalBarWithXylophon)
            {
                oneDWave->calculateSchemeMetalBarWithXylophon();
                oneDWave->updateStates();
                outputSoundModel = oneDWave->getOutput (0.8);
                //DBG("MetalBarWithXylophon");
            }
            else if (soundmodel.getSoundTyp() == Soundmodel::SoundTyp::WoodBarWithXylophon)
            {
                oneDWave->calculateSchemeWoodBarWithXylophon();
                oneDWave->updateStates();
                outputSoundModel = oneDWave->getOutput (0.8);
                //DBG("WoodBarWithXylophon");
            }
        
        
        
        
        
        //==========Calculate all ImpulseResponses
        
        //To Do:: Security, that Body/ImpulseResponse isn't switched while calculation is done
        
        
        if (impulseResponse.getImpulseResponseTyp() == ImpulseResponse::ImpulseResponseTyp::Dirac)
        {
            //Do load Audio "Dirac", make sure it is not loaded every n or buffer
            //Do convolution with Dirac
            output = outputSoundModel;
            //DBG("Dirac");
        }
        
        else if (impulseResponse.getImpulseResponseTyp() == ImpulseResponse::ImpulseResponseTyp::GuitarBody)
        
        {
            //Do load Audio "GuitarBody", make sure it is not loaded every n or buffer
            //Do convultion with Guitar Body
            output = outputSoundModel;
            //DBG("Guitar Body");
        }
        
        else if (impulseResponse.getImpulseResponseTyp() == ImpulseResponse::ImpulseResponseTyp::ViolinBody)
            
        {
            //Do load Audio , make sure it is not loaded every n or buffer
            //Do convolutiion
            output = outputSoundModel;
            //DBG("Violin Body");
        }
            
        else if (impulseResponse.getImpulseResponseTyp() == ImpulseResponse::ImpulseResponseTyp::MetalBox)
            
        {
            //Do load Audio , make sure it is not loaded every n or buffer
            //Do convolutiion
            output = outputSoundModel;
            //DBG("Metal Box");
        }
            
        else if (impulseResponse.getImpulseResponseTyp() == ImpulseResponse::ImpulseResponseTyp::WoodBox)
            
        {
            //Do load Audio , make sure it is not loaded every n or buffer
            //Do convolutiion
            output = outputSoundModel;
            //DBG("Wood Box");
        }
        
        else if (impulseResponse.getImpulseResponseTyp() == ImpulseResponse::ImpulseResponseTyp::WoodBox)
            
        {
            //Do load Audio , make sure it is not loaded every n or buffer
            //Do convolutiion
            output = outputSoundModel;
            //DBG("Wood Box");
        }
            
        else if (impulseResponse.getImpulseResponseTyp() == ImpulseResponse::ImpulseResponseTyp::PaperBox)
            
        {
            //Do load Audio , make sure it is not loaded every n or buffer
            //Do convolutiion
            output = outputSoundModel;
            //DBG("Paper Box");
        }
            
        else if (impulseResponse.getImpulseResponseTyp() == ImpulseResponse::ImpulseResponseTyp::OtherBox)
            
        {
            //Do load Audio , make sure it is not loaded every n or buffer
            //Do convolutiion
            output = outputSoundModel;
            //DBG("Other Box");
        }
            
      
        
        // Send the output to both channels.
        for (int channel = 0; channel < numChannels; ++channel)
            curChannel[channel][0][i] = limit(output, -1.0, 1.0);
    
   
    
    
    
    }
    
    ///__________________________

} // sends it to speakers after that











void MainComponent::releaseResources()
{
}












//============ MAIN GRAFICS ==================================================================





void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    
    
    
    
    //CREATING EMPTY AREAS, THAT ARE LATER FILLED WITH THE SLIDERS
    
    //Organize in areas
    int upperFrameforSliderLabel = 50;
    int AmountOfSlider = 4;
    int AmountOfButtons = 3;
    int SlidersHeight = 5; // 4 = fourth of the whole space height, five = fifth of the space
    int InstrumentButtonWidth = 4; // 4 = fourth of the whole space width, five = fifth of the space
    int FrameForPNG = 80; // in Pixel
    
    
    
    // All Sliders (Top from left to right)
    Rectangle<int> sliderAreaAll;
    sliderAreaAll.setBounds (0, upperFrameforSliderLabel, getWidth()-getWidth()/InstrumentButtonWidth,getHeight()/SlidersHeight); //gives back the upper part of the plugin
    
    
    // 4 Sliders
    // 1st Slider
    Rectangle<int> sliderArea1;
    sliderArea1.setBounds(sliderAreaAll.getX(), sliderAreaAll.getY(), sliderAreaAll.getWidth()/AmountOfSlider, sliderAreaAll.getHeight());
    // 2nd Slider
    Rectangle<int> sliderArea2;
    sliderArea2.setBounds(sliderAreaAll.getWidth()/AmountOfSlider, sliderAreaAll.getY(), sliderAreaAll.getWidth()/AmountOfSlider, sliderAreaAll.getHeight());
    // 3rd Slider
    Rectangle<int> sliderArea3;
    sliderArea3.setBounds(sliderAreaAll.getWidth()/AmountOfSlider*2, sliderAreaAll.getY(), sliderAreaAll.getWidth()/AmountOfSlider, sliderAreaAll.getHeight());
    // 4th Slider
    Rectangle<int> sliderArea4;
    sliderArea4.setBounds(sliderAreaAll.getWidth()/AmountOfSlider*3, sliderAreaAll.getY(), sliderAreaAll.getWidth()/AmountOfSlider, sliderAreaAll.getHeight());
    
  
    
    
    
    
    // Space that isn't taken by the sliders (bottom from left to right)
    Rectangle<int> notSliderArea;
    notSliderArea.setBounds(0,sliderAreaAll.getHeight()+sliderAreaAll.getY(), getWidth(), getHeight()-sliderAreaAll.getHeight());
    
    
    // Space for the display of the Instrument buttons (right part of the bottom)
    Rectangle<int> InstrumentButtonAreaAll;
    InstrumentButtonAreaAll.setBounds(getWidth()-getWidth()/InstrumentButtonWidth,sliderAreaAll.getHeight()+sliderAreaAll.getY(), getWidth()/InstrumentButtonWidth, getHeight()-(sliderAreaAll.getHeight()+sliderAreaAll.getY()));
    
    // 4 InstrumentButtons
    // 1st Button
    Rectangle<int> buttonArea1;
    buttonArea1.setBounds(InstrumentButtonAreaAll.getX(),InstrumentButtonAreaAll.getY(),InstrumentButtonAreaAll.getWidth(),InstrumentButtonAreaAll.getHeight()/AmountOfButtons);
    // 2nd Button
    Rectangle<int> buttonArea2;
    buttonArea2.setBounds(InstrumentButtonAreaAll.getX(),InstrumentButtonAreaAll.getY()+1*InstrumentButtonAreaAll.getHeight()/AmountOfButtons,InstrumentButtonAreaAll.getWidth(),InstrumentButtonAreaAll.getHeight()/AmountOfButtons);
    // 3rd Button
    Rectangle<int> buttonArea3;
    buttonArea3.setBounds(InstrumentButtonAreaAll.getX(),InstrumentButtonAreaAll.getY()+2*InstrumentButtonAreaAll.getHeight()/AmountOfButtons,InstrumentButtonAreaAll.getWidth(),InstrumentButtonAreaAll.getHeight()/AmountOfButtons);
    // 4th Button
    Rectangle<int> buttonArea4;
    buttonArea4.setBounds(InstrumentButtonAreaAll.getX(),InstrumentButtonAreaAll.getY()+3*InstrumentButtonAreaAll.getHeight()/AmountOfButtons,InstrumentButtonAreaAll.getWidth(),InstrumentButtonAreaAll.getHeight()/AmountOfButtons);
    
    
    
    // Space for the display of the Instrument graphical representation (png) (left part of the button)
    Rectangle<int> InstrumentPictureArea;
    InstrumentPictureArea.setBounds(0 + FrameForPNG, sliderAreaAll.getHeight() + FrameForPNG, getWidth() - (getWidth()/InstrumentButtonWidth) - 2*FrameForPNG, getHeight() - sliderAreaAll.getHeight() - 2*FrameForPNG);
    
    
    
    
    
    //SLIDERS
    
    amplitudeSlider.setBounds(sliderArea1);
    stringLengthSlider.setBounds (sliderArea2);
    dampingSlider.setBounds (sliderArea3);
    materialSlider.setBounds (sliderArea4);
    
    //startButton.setBounds(StartButtonAreaAll);
    //buttonInstrument1.setBounds(buttonArea1);
    //buttonInstrument2.setBounds(buttonArea2);
    //buttonInstrument3.setBounds(buttonArea3);
    //buttonInstrument4.setBounds(buttonArea4);
    
    
    //ComboBoxes
    body.setBounds(buttonArea1);
    stringOrBarTyp.setBounds(buttonArea2);
    instrument.setBounds(buttonArea3);
    
    
    //png
    //std::cout << showImage;
    mImageBodyGuitar.setBounds(InstrumentPictureArea);
    mImageBodyVioline.setBounds(InstrumentPictureArea);
    mImageWesternStringGuitar.setBounds(InstrumentPictureArea);
    mImageWesternStringVioline.setBounds(InstrumentPictureArea);
    mImageNylonStringGuitar.setBounds(InstrumentPictureArea);
    mImageNylonStringVioline.setBounds(InstrumentPictureArea);
    mImageBodyMetalBox.setBounds(InstrumentPictureArea);
    mImageBodyWoodBox.setBounds(InstrumentPictureArea);
    mImageBodyPaperBox.setBounds(InstrumentPictureArea);
    mImageBodyOther.setBounds(InstrumentPictureArea);
    mImageOnlyWesternString.setBounds(InstrumentPictureArea);
    mImageOnlyNylonString.setBounds(InstrumentPictureArea);
    mImageMetalBar.setBounds(InstrumentPictureArea);
    mImageWoodBar.setBounds(InstrumentPictureArea);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /// SILVINS VISUALIZATION__________________________
    
    // Check if the instance of the OneDWave class is initialised
    if (oneDWave != nullptr)
    {
        // Use the entire application window to visualise the state of the system
        oneDWave->setBounds (0,upperFrameforSliderLabel, getWidth(), getHeight() - upperFrameforSliderLabel);
    }
    
    ///__________________________
    
    
    
    
    
    
    
    
    
    
    
    
}

///__________________________

void MainComponent::timerCallback()
{
    // Repaint the application. This function gets called 15x per second (as defined in prepareToPlay)
    repaint();
}

// Implementation of the limiter
float MainComponent::limit (float val, float min, float max)
{
    if (val < min)
        return min;
    else if (val > max)
        return max;
    else
        return val;
}

///__________________________


/*
void MainComponent::buttonClicked(Button* button)

//TO DO
// If we want to make the buttons change the audio (Switch statement)
//this could be integrated here with a bool variable?
{
 
    if (button == &buttonInstrument1)
    {
        buttonInstrument1.setButtonText("listen");
        buttonInstrument2.setButtonText("Bar");
        buttonInstrument3.setButtonText("Plate");
        buttonInstrument4.setButtonText("Stiffstring");
        
        
        addAndMakeVisible(mImageComponent1);
        mImageComponent1.setVisible(true);
        mImageComponent2.setVisible(false);
        mImageComponent3.setVisible(false);
        mImageComponent4.setVisible(false);
 

        
        
    }
    
    else if (button == &buttonInstrument2)
    {
        buttonInstrument1.setButtonText("String");
        buttonInstrument2.setButtonText("listen");
        buttonInstrument3.setButtonText("Plate");
        buttonInstrument4.setButtonText("Stiffstring");
       
        
        addAndMakeVisible(mImageComponent2);
        mImageComponent1.setVisible(false);
        mImageComponent2.setVisible(true);
        mImageComponent3.setVisible(false);
        mImageComponent4.setVisible(false);
        
    }
    
    else if (button == &buttonInstrument3)
    {
        buttonInstrument1.setButtonText("String");
        buttonInstrument2.setButtonText("Bar");
        buttonInstrument3.setButtonText("listen");
        buttonInstrument4.setButtonText("Stiffstring");
        
        addAndMakeVisible(mImageComponent3);
        mImageComponent1.setVisible(false);
        mImageComponent2.setVisible(false);
        mImageComponent3.setVisible(true);
        mImageComponent4.setVisible(false);
        //std::cout << showImage;
    }
 
    if (button == &buttonInstrument4)
    {
        
    
        buttonInstrument4.setButtonText("Instrument");
   
        addAndMakeVisible(mImageComponent4);
        mImageComponent1.setVisible(false);
        mImageComponent2.setVisible(false);
        mImageComponent3.setVisible(false);
        mImageComponent4.setVisible(true);
        
    }
    
 
}
*/

void MainComponent::sliderValueChanged (Slider* slider)
{
    /*
     
     Now that we created a function to set the position of the Processor, let's apply the slider value to the position.
     Obtaining the slider value happens through the Slider class' "getValue()" function.
     
     */
    if (slider == &amplitudeSlider) //HERE IT IS DEFINED, WHAT THE SLIDER ACTUALLY DOES, or in which value the change is written
        oneDWave->setAmplitude(amplitudeSlider.getValue());
    else if (slider == &stringLengthSlider)
        oneDWave->setStringLength (stringLengthSlider.getValue());
    else if (slider == &dampingSlider)
        oneDWave->setDamping (dampingSlider.getValue());
    else if (slider == &materialSlider)
        oneDWave->setWavespeed (materialSlider.getValue());
}


void MainComponent::clearImages()
{
    mImageBodyGuitar.setVisible(false);
    mImageBodyVioline.setVisible(false);
    mImageWesternStringGuitar.setVisible(false);
    mImageWesternStringVioline.setVisible(false);
    mImageNylonStringGuitar.setVisible(false);
    mImageNylonStringVioline.setVisible(false);
    mImageMetalBar.setVisible(false);
    mImageWoodBar.setVisible(false);
    mImageBodyMetalBox.setVisible(false);
    mImageBodyWoodBox.setVisible(false);
    mImageBodyPaperBox.setVisible(false);
    mImageBodyOther.setVisible(false);
    mImageOnlyWesternString.setVisible(false);
    mImageOnlyNylonString.setVisible(false);
}









void MainComponent::comboBoxChanged (ComboBox *comboBoxThatHasChanged)
{
    
    
    
    
    //First The selection of the instrument through the instrument ComboBox is necessary
    //There will still be no sound until body or strings/Bars are choosen
    
    
    if (comboBoxThatHasChanged == &instrument)
    {
        //Here it is decided what happens if the instrument is coosen.
        //Pictures are cleared
        //The other comboboxes are reset and get new values
        
        if (instrument.getSelectedId() == 1)
        {
            DBG("String");
            stringOrBarTyp.clear();
            stringOrBarTyp.setText("String Typ");
            stringOrBarTyp.addItem("None",11);
            stringOrBarTyp.addItem("Western Strings",12);
            stringOrBarTyp.addItem("Nylon Strings",13);
            
            body.clear();
            body.setText("Body");
            body.addItem("None",111);
            body.addItem("Guitar",112);
            body.addItem("Violine",113);
            
            //Initial
            body.setSelectedId(111);
            stringOrBarTyp.setSelectedId(11);
            
            //Set Soundmodell to None
            soundmodel.setSoundModel(Soundmodel::SoundTyp::None);
            //Choose Impulse response = DiracImpluse
            impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::Dirac);
            
            //To Do: changeColor(red)
            
            
        }
        
        if (instrument.getSelectedId() == 2)
        {
            DBG("Xylophon");
            stringOrBarTyp.clear();
            stringOrBarTyp.setText("Bar Type");
            stringOrBarTyp.addItem("None",11);
            stringOrBarTyp.addItem("Metal Bar",22);
            stringOrBarTyp.addItem("Wooden Bar",23);
            
            
            body.clear();
            body.setText("Body");
            body.addItem("None",111);
            body.addItem("Metal Box",212);
            body.addItem("Wooden Box",213);
            body.addItem("Paper Box",214);
            body.addItem("Other Box",215);
            
            
            //Initial
            body.setSelectedId(111);
            stringOrBarTyp.setSelectedId(11);
            
            //Set Soundmodell to None
            soundmodel.setSoundModel(Soundmodel::SoundTyp::None);
            //Choose Impulse response = DiracImpluse
            impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::Dirac);
            
            //To Do: changeColor(blue)
            
            
        }
        
    }
    
    
    
    
  
    
    //The selection of the body or string/bar follow after in no particular order
    //That's why there are a lot of "if"-cases, always checking what the other Combobox does.
    
    if (comboBoxThatHasChanged == &body)
    {
        clearImages(); //reset all
        
        if (instrument.getSelectedId() == 1) //Stringinstrument
        {
 
            if (body.getSelectedId() == 111) //No Body
            {
                    // no picture if no strings selected. Otherwise strings
                    if (stringOrBarTyp.getSelectedId() == 11) //No Strings
                    {
                            //show none
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::None);
                    }
                
                    else if (stringOrBarTyp.getSelectedId() == 12) // WesternGuitarStrings
                    {
                            addAndMakeVisible(mImageOnlyWesternString);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::StiffstringWithGuitar);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 13) // NylonStrings
                    {
                            addAndMakeVisible(mImageOnlyNylonString);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::IdealStringWithGuitar);
                    }
                
                    else DBG("Problem with Picture1");
                
                
                //Choose Impulse response = DiracImpluse
            impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::Dirac);
                
                
            }
        
            else if (body.getSelectedId() == 112) // GuitarBody
            {
                
                    if (stringOrBarTyp.getSelectedId() == 11) //None
                    {
                            addAndMakeVisible(mImageBodyGuitar); //Show picture Guitar Body without Strings
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 12) // WesternGuitarStrings
                    {
                            addAndMakeVisible(mImageWesternStringGuitar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::StiffstringWithGuitar);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 13) // NylonStrings
                    {
                            addAndMakeVisible(mImageNylonStringGuitar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::IdealStringWithGuitar);
                    }
                    else DBG("Problem with Picture2");
                
                    //Choose Impulse response = GuitarBody
                    impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::GuitarBody);
            }
            
            
            else if (body.getSelectedId() == 113) //ViolineBody
            {
                    if (stringOrBarTyp.getSelectedId() == 11) //None
                    {
                            addAndMakeVisible(mImageBodyVioline); //Show picture Violine Body without Strings
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 12) // WesternViolineStrings
                    {
                            addAndMakeVisible(mImageWesternStringVioline);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::StiffstringWithVioline);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 13) // NylonStrings
                    {
                            addAndMakeVisible(mImageNylonStringVioline);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::IdealStringWithVioline);
                    }
                    else DBG("Problem with Picture3");
                
                //Choose Impulse response = ViolineBody
                impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::ViolinBody);
            }
        
        }//End String instrument
        
        //Start Xylophon
            else if (instrument.getSelectedId() == 2) //xylophon
            {
                
                
                if (body.getSelectedId() == 111) //no body
                {
                    if (stringOrBarTyp.getSelectedId() == 11) //None
                    {
                            //show none
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::None);
                    }
                    
                    else if (stringOrBarTyp.getSelectedId() == 22) // MetalBar
                    {
                            addAndMakeVisible(mImageMetalBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::MetalBarWithXylophon);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 23) // WoodenBAr
                    {
                            addAndMakeVisible(mImageWoodBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::WoodBarWithXylophon);
                    }
                    else DBG("Problem with Picture30");
                   
                    //Choose Impulse response = Dirac
                    impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::Dirac);
                }
                
            
            if (body.getSelectedId() == 212) //Metal Box
            {
                    if (stringOrBarTyp.getSelectedId() == 11) //None
                    {
                            addAndMakeVisible(mImageBodyMetalBox); //Show picture Metal Box without Bars
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 22) // MetalBar
                    {
                            addAndMakeVisible(mImageBodyMetalBox);
                            addAndMakeVisible(mImageMetalBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::MetalBarWithXylophon);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 23) // WoodenBAr
                    {
                            addAndMakeVisible(mImageBodyMetalBox);
                            addAndMakeVisible(mImageWoodBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::WoodBarWithXylophon);
                    }
                    else DBG("Problem with Picture30");
                
                
                
                //Choose Impulse response = MetalBox
                impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::MetalBox);
            
            }
            
            else if (body.getSelectedId() == 213) //Wooden Box
            {
                    if (stringOrBarTyp.getSelectedId() == 11) //None
                    {
                            addAndMakeVisible(mImageBodyWoodBox); //Show picture Metal Box without Bars
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 22) // MetalBar
                    {
                            addAndMakeVisible(mImageBodyWoodBox);
                            addAndMakeVisible(mImageMetalBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::MetalBarWithXylophon);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 23) // WoodenBAr
                    {
                            addAndMakeVisible(mImageBodyWoodBox);
                            addAndMakeVisible(mImageWoodBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::WoodBarWithXylophon);
                    }
                    else DBG("Problem with Picture31");
                
                //Choose Impulse response = WoodBox
                impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::WoodBox);
            }
            
            else if (body.getSelectedId() == 214)  //Paper Box
            {
                    if (stringOrBarTyp.getSelectedId() == 11) //None
                    {
                            addAndMakeVisible(mImageBodyPaperBox); //Show picture Metal Box without Bars
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 22) // MetalBar
                    {
                            addAndMakeVisible(mImageBodyPaperBox);
                            addAndMakeVisible(mImageMetalBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::MetalBarWithXylophon);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 23) // WoodenBAr
                    {
                            addAndMakeVisible(mImageBodyPaperBox);
                            addAndMakeVisible(mImageWoodBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::WoodBarWithXylophon);
                    }
                    else DBG("Problem with Picture32");
                
                //Choose Impulse response = PaperBox
                impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::PaperBox);
                
            }
            
            else if (body.getSelectedId() == 215)   //Other Box
            {
                    if (stringOrBarTyp.getSelectedId() == 11) //None
                    {
                            addAndMakeVisible(mImageBodyOther); //Show picture Metal Box without Bars
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 22) // MetalBar
                    {
                            addAndMakeVisible(mImageBodyOther);
                            addAndMakeVisible(mImageMetalBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::MetalBarWithXylophon);
                    }
                    else if (stringOrBarTyp.getSelectedId() == 23) // WoodenBAr
                    {
                            addAndMakeVisible(mImageBodyOther);
                            addAndMakeVisible(mImageWoodBar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::WoodBarWithXylophon);
                    }
                    else DBG("Problem with Picture33");
                
                //Choose Impulse response = OtherBox
               impulseResponse.setImpulseResponseTyp(ImpulseResponse::ImpulseResponseTyp::OtherBox);
            }
        
        
       
        } //End of Xylophon
        
    } //ComboBox "Body"
    
    
    
   
    
    
    
    
    //================================================String//Bar
    
    if (comboBoxThatHasChanged == &stringOrBarTyp)
    {
        clearImages(); //clear all
        
        if (instrument.getSelectedId() == 1) //Stringinstrument
        {
            
                if (stringOrBarTyp.getSelectedId() == 11) //None
                {
                    // no picture
                    
                        if (body.getSelectedId() == 111) // None
                        {
                            //show none
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::None);
                        }
                        else if (body.getSelectedId() == 112) // GuitarBody
                        {
                            addAndMakeVisible(mImageBodyGuitar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                        }
                    
                        else if (body.getSelectedId() == 113) // ViolinBody
                        {
                            addAndMakeVisible(mImageBodyVioline);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                        }
                        else DBG("Problem with Picture10");
                    
                    
                    
                }
            
            
                else if (stringOrBarTyp.getSelectedId() == 12) //Western Strings
                {
                        //Show picture WesternStrings
                        if (body.getSelectedId() == 111) // None
                        {
                            addAndMakeVisible(mImageOnlyWesternString);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::StiffstringWithGuitar);
                        }
                        else if (body.getSelectedId() == 112) // GuitarBody
                        {
                            addAndMakeVisible(mImageWesternStringGuitar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::StiffstringWithGuitar);
                        }
                        else if (body.getSelectedId() == 113) // ViolinBody
                        {
                            addAndMakeVisible(mImageWesternStringVioline);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::StiffstringWithVioline);
                        }
                        else DBG("Problem with Picture11");
                    
                    
                }
            
                else if (stringOrBarTyp.getSelectedId() == 13) //Nylon Strings
                {
                    
                    
                        if (body.getSelectedId() == 111) // None
                        {
                            addAndMakeVisible(mImageOnlyNylonString);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::IdealStringWithGuitar);
                        }
                        if (body.getSelectedId() == 112) // GuitarBody
                        {
                            addAndMakeVisible(mImageNylonStringGuitar);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::IdealStringWithGuitar);
                        }
                        else if (body.getSelectedId() == 113) // ViolinBody
                        {
                            addAndMakeVisible(mImageNylonStringVioline);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::StiffstringWithVioline);
                        }
                        else DBG("Problem with Picture3");
                        //Choose Parameters for c, kappa, T, Sigma, lambda etc.
                }
        } //End String Instrument
        
        //Start Xylophon
        else if (instrument.getSelectedId() == 2) //Stringinstrument
        {
            
            
            
                if (stringOrBarTyp.getSelectedId() == 11) // None
                {
                        if (body.getSelectedId() == 111) // None
                        {
                            //none
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::None);
                        }
                    
                        else if (body.getSelectedId() == 212) // Metal Box
                        {
                            addAndMakeVisible(mImageBodyMetalBox);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                        }
                        else if (body.getSelectedId() == 213) // WoodenBox
                        {
                            addAndMakeVisible(mImageBodyWoodBox);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                        }
                        else if (body.getSelectedId() == 214) // PaperBox
                        {
                            addAndMakeVisible(mImageBodyPaperBox);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                        }
                        else if (body.getSelectedId() == 215) // OtherBox
                        {
                            addAndMakeVisible(mImageBodyOther);
                            //Choose SoundModel Excitation
                            soundmodel.setSoundModel(Soundmodel::SoundTyp::ExciteBody);
                        }
                    
                        else DBG("Problem with Picture40");
            
                }
                    
            
                if (stringOrBarTyp.getSelectedId() == 22) //Metal BAr
                {
                        if (body.getSelectedId() == 111) // None
                            addAndMakeVisible(mImageMetalBar);
                    
                        else if (body.getSelectedId() == 212) // Metal Box
                        {
                             addAndMakeVisible(mImageBodyMetalBox);
                            addAndMakeVisible(mImageMetalBar);
                        }
                        else if (body.getSelectedId() == 213) // WoodenBox
                        {
                            addAndMakeVisible(mImageBodyWoodBox);
                            addAndMakeVisible(mImageMetalBar);
                        }
                        else if (body.getSelectedId() == 214) // PaperBox
                        {
                            addAndMakeVisible(mImageBodyPaperBox);
                            addAndMakeVisible(mImageMetalBar);
                        }
                        else if (body.getSelectedId() == 215) // OtherBox
                        {
                            addAndMakeVisible(mImageBodyOther);
                            addAndMakeVisible(mImageMetalBar);
                        }
                    
                        else DBG("Problem with Picture41");
                    
                        //Choose SoundModel Excitation
                        soundmodel.setSoundModel(Soundmodel::SoundTyp::MetalBarWithXylophon);
                    
                   
                }
                
                else if (stringOrBarTyp.getSelectedId() == 23) //Wood Bar
                {
                    if (body.getSelectedId() == 111) // None
                        addAndMakeVisible(mImageWoodBar);
                    
                    else if (body.getSelectedId() == 212) // Metal Box
                    {
                        addAndMakeVisible(mImageBodyMetalBox);
                        addAndMakeVisible(mImageWoodBar);
                    }
                    else if (body.getSelectedId() == 213) // WoodenBox
                    {
                        addAndMakeVisible(mImageBodyWoodBox);
                        addAndMakeVisible(mImageWoodBar);
                    }
                    else if (body.getSelectedId() == 214) // PaperBox
                    {
                        addAndMakeVisible(mImageBodyPaperBox);
                        addAndMakeVisible(mImageWoodBar);
                    }
                    else if (body.getSelectedId() == 215) // OtherBox
                    {
                        addAndMakeVisible(mImageBodyOther);
                        addAndMakeVisible(mImageWoodBar);
                    }
                    
                    else DBG("Problem with Picture42");
                    
                    //Choose SoundModel Excitation
                    soundmodel.setSoundModel(Soundmodel::SoundTyp::WoodBarWithXylophon);
                }
        } //End Xylophon
        
    } // End ComboBox "StringOrBar"
    
    
    
    
}

