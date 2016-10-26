/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
#include "Smooth.h"



//==============================================================================
MarkRau_hw3_polyphonicSynthAudioProcessor::MarkRau_hw3_polyphonicSynthAudioProcessor()
{
	int nVoices = 20;
	for (int i = nVoices; --i >= 0;)    
	{
		synth.addVoice(new MarkVoice());
	}

	synth.clearSounds();
	synth.addSound(new MarkSound());
}

MarkRau_hw3_polyphonicSynthAudioProcessor::~MarkRau_hw3_polyphonicSynthAudioProcessor()
{
}

//==============================================================================
const String MarkRau_hw3_polyphonicSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MarkRau_hw3_polyphonicSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MarkRau_hw3_polyphonicSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double MarkRau_hw3_polyphonicSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MarkRau_hw3_polyphonicSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MarkRau_hw3_polyphonicSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MarkRau_hw3_polyphonicSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String MarkRau_hw3_polyphonicSynthAudioProcessor::getProgramName (int index)
{
    return String();
}

void MarkRau_hw3_polyphonicSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MarkRau_hw3_polyphonicSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	synth.setCurrentPlaybackSampleRate(sampleRate);
}

void MarkRau_hw3_polyphonicSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MarkRau_hw3_polyphonicSynthAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void MarkRau_hw3_polyphonicSynthAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	int bufSize = buffer.getNumSamples();
	buffer.clear();
	synth.renderNextBlock(buffer, midiMessages, 0, bufSize);
	//reverb.renderNextBlock(buffer, 0, bufsize);
}

//==============================================================================
bool MarkRau_hw3_polyphonicSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MarkRau_hw3_polyphonicSynthAudioProcessor::createEditor()
{
    return new MarkRau_hw3_polyphonicSynthAudioProcessorEditor (*this);
}

//==============================================================================
void MarkRau_hw3_polyphonicSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MarkRau_hw3_polyphonicSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MarkRau_hw3_polyphonicSynthAudioProcessor();
}
