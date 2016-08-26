/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
USpectrumAnalyzerAudioProcessorEditor::USpectrumAnalyzerAudioProcessorEditor (USpectrumAnalyzerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

	mWidth = this->getWidth();
	mHeight = this->getHeight();

	fftBuffer = new float[fftbuflen] { 0 };

	mTimer.setCallback([this]() {
		//this->timerCallback();
		fft(processor.getBuffer(), processor.getBufferLen());
		repaint();
	});
	mTimer.startTimer(50);
}

USpectrumAnalyzerAudioProcessorEditor::~USpectrumAnalyzerAudioProcessorEditor()
{
	//if(mBuffer != nullptr) delete mBuffer;
	mTimer.stopTimer();
}

//==============================================================================
void USpectrumAnalyzerAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colour::fromRGB(0xF1, 0xF0, 0xFE)); // #F1F0FE

	g.drawLine(0, 0, mWidth, 0);
	g.drawText("Spectrum Analyzer", 0, 5, mWidth, 10, Justification::centred);

	if (!m_locked) {
		g.setColour(Colour::fromRGB(0x2B, 0x2C, 0x43)); // set line color
		std::vector<Line<float> > copyLines(lines);
		for (auto line : copyLines) {
			g.drawLine(line, 2.0);
		}
	}
}

void USpectrumAnalyzerAudioProcessorEditor::resized()
{
	//setBounds(getBounds());
}

void USpectrumAnalyzerAudioProcessorEditor::timerCallback()
{
	//if (m_locked) return;
	
	m_locked = true;
	fft(processor.getBuffer(), processor.getBufferLen());
	m_locked = false;

	repaint();
}

void USpectrumAnalyzerAudioProcessorEditor::fft(float* buffer, int len) 
{
	if (buffer == nullptr) return;

	//const int fftsize = 10;
	//const int fftbuflen = pow(2, fftsize);
	//const int halfbuflen = floor(fftbuflen / 2);

	// do this to prevent over array copying
	if (len > fftbuflen) len = fftbuflen;

	//float* fftBuffer = new float[fftbuflen] { 0 };
	memcpy(fftBuffer, buffer, len);

	FFT fft(fftsize - 1, false);

	//const int fftBufferSize = juce::nextPowerOfTwo(len);
	//int powerOfTwo = log(fftBufferSize) / log(2);
	//int halfBuffer = fftBufferSize / 2;

	//float* fftBuffer = new float[fftBufferSize] { 0 };
	//memcpy(fftBuffer, buffer, len);

	//FFT fft(powerOfTwo - 1, false);
	fft.performFrequencyOnlyForwardTransform(fftBuffer);
	lines.clear();

	float pos = 0;
	float scale = (float)mHeight / findmax(fftBuffer, len);
	float step = (float)ffthalflen / (float)mWidth;

	//if (std::isinf(scale) || scale != scale) scale = 1.0;

	for (int i = 0; i < ffthalflen; i++) {
		float cur = fftBuffer[i] * scale;

		// check if cur is NaN
		if (cur != cur) cur = 0.0;

		Line<float> l;
		l.setStart(pos, mHeight);
		l.setEnd(pos, mHeight - cur);

		lines.push_back(l);

		pos += step;
	}
}

float USpectrumAnalyzerAudioProcessorEditor::findmax(float * buf, int len)
{
	int i = len;
	float max = 0.0;
	while (i--) {
		if (buf[i] > max) max = buf[i];
	}

	return max;
}