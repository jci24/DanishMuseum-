/*
  ==============================================================================

   This file is part of the JUCE examples.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             ConvolutionDemo
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Convolution demo using the DSP module.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_dsp, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        ConvolutionDemo

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once



using namespace dsp;



//==============================================================================


struct ConvolutionDemoDSP
{

    double sampleRate = 0.0;
    bool bypass = false;


    Convolution convolution;


    void prepareConv(const ProcessSpec& spec) //prepareConv is only called with PrepareToPlay(), so here it is done for Dirac
    {

        sampleRate = spec.sampleRate;
        convolution.prepare(spec);
        updateParametersConvDirac();

    }


    void processConv(ProcessContextReplacing<float> context)
    {
        // context.isBypassed = bypass;


         // Load a new IR if there's one available. Note that this doesn't lock or allocate!
        bufferTransfer.get([this](BufferWithSampleRate& buf)
            {
                //File irAcGuitarHammer = File("/Users/stx/0000_Austausch/0005_Aalborg university/2022_SS/Semester Project/ImpulseResponses/IRs_Hammers_Sweeps_Rev/01AcGuitar_HammerRev.wav");
                //const File &irAcGuitarHammer2 = irAcGuitarHammer;

                convolution.loadImpulseResponse(std::move(buf.buffer),
                    buf.sampleRate,
                    Convolution::Stereo::yes,
                    Convolution::Trim::no,
                    Convolution::Normalise::no);


                /*convolution.loadImpulseResponse     (&irAcGuitarHammer2,
                                                     Convolution::Stereo::yes,
                                                     Convolution::Trim::no,
                                                     Convolution::Normalise::no);
                */
                // DBG(convolution.getCurrentIRSize());
                 /*
                  void loadImpulseResponse (const File& fileImpulseResponse,
                             Stereo isStereo, Trim requiresTrimming, size_t size,
                             Normalise requiresNormalisation = Normalise::yes);

                  This function loads an impulse response from an audio buffer.
                  To avoid memory allocation on the audio thread, this function takes
                  ownership of the buffer passed in.

                  If calling this function during processing, make sure that the buffer is
                  not allocated on the audio thread (be careful of accidental copies!).
                  If you need to pass arbitrary/generated buffers it's recommended to
                  create these buffers on a separate thread and to use some wait-free
                  construct (a lock-free queue or a SpinLock/GenericScopedTryLock combination)
                  to transfer ownership to the audio thread without allocating.

                  @param buffer                   the AudioBuffer to use
                  @param bufferSampleRate         the sampleRate of the data in the AudioBuffer
                  @param isStereo                 selects either stereo or mono
                  @param requiresTrimming         optionally trim the start and the end of the impulse response
                  @param requiresNormalisation    optionally normalise the impulse response amplitude
                 */
                DBG("process");
                DBG(convolution.getCurrentIRSize());
            });

        convolution.process(context);


    }

    void resetConv()
    {
        convolution.reset();
    }



    //To Do:
    //now get's updated every buffer. Should we change that, that it is only updatet if we have changes in the ComboBoxes?
    void updateParametersConvDirac()
    {
        File irDirac = File("C:\\Users\\Jaime\\Documents\\IR_SemesterProject\\00DiracImpuls.wav");

        auto assetInputStream = irDirac.createInputStream();
        if (assetInputStream == nullptr)
        {
            jassertfalse;
            return;
        }
        AudioFormatManager manager;
        manager.registerBasicFormats();
        std::unique_ptr<AudioFormatReader> reader{ manager.createReaderFor(std::move(assetInputStream)) };

        if (reader == nullptr)
        {
            jassertfalse;
            return;
        }

        AudioBuffer<float> buffer(static_cast<int> (reader->numChannels),
            static_cast<int> (reader->lengthInSamples));
        reader->read(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), 0, buffer.getNumSamples());

        bufferTransfer.set(BufferWithSampleRate{ std::move(buffer), reader->sampleRate });

        DBG(convolution.getCurrentIRSize());
        DBG(irDirac.getSize());
    }

    void updateParametersConvGuitar()
    {
        File irAcGuitar = File("C:\\Users\\Jaime\\Documents\\IR_SemesterProject\\01AcGuitar_SweepRev.wav");
        auto assetInputStream = irAcGuitar.createInputStream();
        if (assetInputStream == nullptr)
        {
            jassertfalse;
            return;
        }
        AudioFormatManager manager;
        manager.registerBasicFormats();
        std::unique_ptr<AudioFormatReader> reader{ manager.createReaderFor(std::move(assetInputStream)) };

        if (reader == nullptr)
        {
            jassertfalse;
            return;
        }

        AudioBuffer<float> buffer2(static_cast<int> (reader->numChannels),
            static_cast<int> (reader->lengthInSamples));
        reader->read(buffer2.getArrayOfWritePointers(), buffer2.getNumChannels(), 0, buffer2.getNumSamples());

        bufferTransfer.set(BufferWithSampleRate{ std::move(buffer2), reader->sampleRate });

        DBG(convolution.getCurrentIRSize());

        DBG(irAcGuitar.getSize());
    }

    void updateParametersConvVioline()
    {
        File irViolineSweep = File("C:\\Users\\Jaime\\Documents\\IR_SemesterProject\\07Violin_SweepRev.wav");

        auto assetInputStream = irViolineSweep.createInputStream();
        if (assetInputStream == nullptr)
        {
            jassertfalse;
            return;
        }
        AudioFormatManager manager;
        manager.registerBasicFormats();
        std::unique_ptr<AudioFormatReader> reader{ manager.createReaderFor(std::move(assetInputStream)) };

        if (reader == nullptr)
        {
            jassertfalse;
            return;
        }

        AudioBuffer<float> buffer(static_cast<int> (reader->numChannels),
            static_cast<int> (reader->lengthInSamples));
        reader->read(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), 0, buffer.getNumSamples());

        bufferTransfer.set(BufferWithSampleRate{ std::move(buffer), reader->sampleRate });

        DBG(convolution.getCurrentIRSize());
        DBG(irViolineSweep.getSize());
    }

    void updateParametersConvMetalBox()
    {
        File irMetalBoxSweep = File("C:\\Users\\Jaime\\Documents\\IR_SemesterProject\\03MetalBox_SweepRev.wav");

        auto assetInputStream = irMetalBoxSweep.createInputStream();
        if (assetInputStream == nullptr)
        {
            jassertfalse;
            return;
        }
        AudioFormatManager manager;
        manager.registerBasicFormats();
        std::unique_ptr<AudioFormatReader> reader{ manager.createReaderFor(std::move(assetInputStream)) };

        if (reader == nullptr)
        {
            jassertfalse;
            return;
        }

        AudioBuffer<float> buffer(static_cast<int> (reader->numChannels),
            static_cast<int> (reader->lengthInSamples));
        reader->read(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), 0, buffer.getNumSamples());

        bufferTransfer.set(BufferWithSampleRate{ std::move(buffer), reader->sampleRate });

        DBG(convolution.getCurrentIRSize());
        DBG(irMetalBoxSweep.getSize());
    }

    void updateParametersConvWoodBox()
    {
        File irWoodBoxSweep = File("C:\\Users\\Jaime\\Documents\\IR_SemesterProject\\04Wood_SweepRev.wav");

        auto assetInputStream = irWoodBoxSweep.createInputStream();
        if (assetInputStream == nullptr)
        {
            jassertfalse;
            return;
        }

        AudioFormatManager manager;
        manager.registerBasicFormats();
        std::unique_ptr<AudioFormatReader> reader{ manager.createReaderFor(std::move(assetInputStream)) };

        if (reader == nullptr)
        {
            jassertfalse;
            return;
        }

        AudioBuffer<float> buffer(static_cast<int> (reader->numChannels),
            static_cast<int> (reader->lengthInSamples));
        reader->read(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), 0, buffer.getNumSamples());

        bufferTransfer.set(BufferWithSampleRate{ std::move(buffer), reader->sampleRate });

        DBG(convolution.getCurrentIRSize());
        DBG(irWoodBoxSweep.getSize());
    }

    void updateParametersConvPaperBox()
    {
        File irPaperBoxSweep = File("C:\\Users\\Jaime\\Documents\\IR_SemesterProject\\05PaperBox_SweepRev.wav");

        auto assetInputStream = irPaperBoxSweep.createInputStream();
        if (assetInputStream == nullptr)
        {
            jassertfalse;
            return;
        }

        AudioFormatManager manager;
        manager.registerBasicFormats();
        std::unique_ptr<AudioFormatReader> reader{ manager.createReaderFor(std::move(assetInputStream)) };

        if (reader == nullptr)
        {
            jassertfalse;
            return;
        }

        AudioBuffer<float> buffer(static_cast<int> (reader->numChannels),
            static_cast<int> (reader->lengthInSamples));
        reader->read(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), 0, buffer.getNumSamples());

        bufferTransfer.set(BufferWithSampleRate{ std::move(buffer), reader->sampleRate });

        DBG(convolution.getCurrentIRSize());
        DBG(irPaperBoxSweep.getSize());
    }

    void updateParametersConvOtherBox()
    {
        File irOtherBoxSweep = File("C:\\Users\\Jaime\\Documents\\IR_SemesterProject\\11CeramicDrum_SweepRev.wav");

        auto assetInputStream = irOtherBoxSweep.createInputStream();
        if (assetInputStream == nullptr)
        {
            jassertfalse;
            return;
        }
        AudioFormatManager manager;
        manager.registerBasicFormats();
        std::unique_ptr<AudioFormatReader> reader{ manager.createReaderFor(std::move(assetInputStream)) };

        if (reader == nullptr)
        {
            jassertfalse;
            return;
        }

        AudioBuffer<float> buffer(static_cast<int> (reader->numChannels),
            static_cast<int> (reader->lengthInSamples));
        reader->read(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), 0, buffer.getNumSamples());

        bufferTransfer.set(BufferWithSampleRate{ std::move(buffer), reader->sampleRate });

        DBG(convolution.getCurrentIRSize());
        DBG(irOtherBoxSweep.getSize());
    }

















    //==============================================================================
    struct BufferWithSampleRate
    {
        BufferWithSampleRate() = default;

        BufferWithSampleRate(AudioBuffer<float>&& bufferIn, double sampleRateIn)
            : buffer(std::move(bufferIn)), sampleRate(sampleRateIn) {}

        AudioBuffer<float> buffer;
        double sampleRate = 0.0;
    };



    class BufferTransfer
    {
    public:
        void set(BufferWithSampleRate&& p)
        {
            const SpinLock::ScopedLockType lock(mutex);
            buffer = std::move(p);
            newBuffer = true;
        }

        // Call `fn` passing the new buffer, if there's one available
        template <typename Fn>
        void get(Fn&& fn)
        {
            const SpinLock::ScopedTryLockType lock(mutex);

            if (lock.isLocked() && newBuffer)
            {
                fn(buffer);
                newBuffer = false;
            }
        }

    private:
        BufferWithSampleRate buffer;
        bool newBuffer = false;
        SpinLock mutex;
    };

    BufferTransfer bufferTransfer;

};

