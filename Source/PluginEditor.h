/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "vector"
#include <cmath>
#include <algorithm>
#include <string>
#include "EventTimer.h"

#define DEBUG true

//==============================================================================
/**
*/
class USpectrumAnalyzerAudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    USpectrumAnalyzerAudioProcessorEditor (USpectrumAnalyzerAudioProcessor&);
    ~USpectrumAnalyzerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void timerCallback() override;
		
	void processFFT();
	void updateSpectrumGraphics(float* buffer, int bufferLen, float scale = 10.0, float step = 1.0);
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    USpectrumAnalyzerAudioProcessor& processor;
	EventTimer mTimer;
	juce::AudioSampleBuffer mAudioSampleBuffer;

	int mWidth;
	int mHeight;

	float* fftBuffer;
	// power of 2
	int fftsize = 10;
	int fftbuflen = pow(2, fftsize);
	int ffthalflen = floor(fftbuflen / 2);

	// prevents drawing when buffer is updating with new data
	bool mUILock = false;
	bool mProcessingLock = false;

	FFT fft;
	std::vector< Line<float> > lines;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (USpectrumAnalyzerAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
