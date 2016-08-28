/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
USpectrumAnalyzerAudioProcessor::USpectrumAnalyzerAudioProcessor()
{
}

USpectrumAnalyzerAudioProcessor::~USpectrumAnalyzerAudioProcessor()
{
}

//==============================================================================
const String USpectrumAnalyzerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool USpectrumAnalyzerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool USpectrumAnalyzerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double USpectrumAnalyzerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int USpectrumAnalyzerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int USpectrumAnalyzerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void USpectrumAnalyzerAudioProcessor::setCurrentProgram (int index)
{
}

const String USpectrumAnalyzerAudioProcessor::getProgramName (int index)
{
    return String();
}

void USpectrumAnalyzerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void USpectrumAnalyzerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mAudioSampleBuffer.setSize(2, samplesPerBlock);
}

void USpectrumAnalyzerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool USpectrumAnalyzerAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
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

void USpectrumAnalyzerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    mAudioSampleBuffer.makeCopyOf(buffer);
}

//==============================================================================
bool USpectrumAnalyzerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* USpectrumAnalyzerAudioProcessor::createEditor()
{
    return new USpectrumAnalyzerAudioProcessorEditor (*this);
}

//==============================================================================
void USpectrumAnalyzerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void USpectrumAnalyzerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new USpectrumAnalyzerAudioProcessor();
}

AudioSampleBuffer USpectrumAnalyzerAudioProcessor::getAudioSampleBuffer()
{
    return mAudioSampleBuffer;
}
