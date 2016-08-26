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
#include "EventTimer.h"

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
	void fft(float* buffer, int len);
	float findmax(float * buf, int len);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    USpectrumAnalyzerAudioProcessor& processor;

	EventTimer mTimer;

	int mWidth;
	int mHeight;

	//float* mBuffer;
	//int mBufferLen;

	float* fftBuffer;
	// power of 2
	int fftsize = 10;
	int fftbuflen = pow(2, fftsize);
	int ffthalflen = floor(fftbuflen / 2);

	// prevents drawing when buffer is updating with new data
	bool m_locked = false;

	std::vector< Line<float> > lines;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (USpectrumAnalyzerAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
