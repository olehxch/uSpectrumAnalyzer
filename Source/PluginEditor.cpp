/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
USpectrumAnalyzerAudioProcessorEditor::USpectrumAnalyzerAudioProcessorEditor(USpectrumAnalyzerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p),
    fft(fftsize - 1, false),
    mAudioSampleBuffer(2, 440)
{
    setSize (600, 300);

    mWidth = this->getWidth();
    mHeight = this->getHeight();

    fftBuffer = new float[fftbuflen] { 0 };

    // update GUI at 30fps
    //this->startTimer(16);
    
    mTimer.setCallback([this]() {
        mUILock = true;
        //if (mUILock.try_lock()) {

            mAudioSampleBuffer.makeCopyOf(processor.getAudioSampleBuffer());
            processFFT();

            //mUILock.unlock();
        //}
        mUILock = false;

        repaint();
    });
    mTimer.startTimer(30);
}

USpectrumAnalyzerAudioProcessorEditor::~USpectrumAnalyzerAudioProcessorEditor()
{
    mTimer.stopTimer();
    delete fftBuffer;
}

//==============================================================================
void USpectrumAnalyzerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::white);

    if (!mUILock) {
    //if(mUILock.try_lock()) {
        g.setColour(Colour::fromRGB(0x2B, 0x2C, 0x43)); // set line color
        std::vector<Line<float> > copyLines(lines);
        for (auto line : copyLines) {
            g.drawLine(line, 2.0);
        }
        //mUILock.unlock();
    }

    g.setColour(Colours::darkgreen);
    g.drawText("Spectrum Analyzer", 0, 5, mWidth, 10, Justification::centred);

    g.drawText("Samples: " + std::to_string(mAudioSampleBuffer.getNumSamples()), 0, 5, 150, 10, Justification::left, false);
    g.drawText("Channels: " + std::to_string(mAudioSampleBuffer.getNumChannels()), 0, 20, 150, 10, Justification::left, false);
    g.drawText("Magnitude: " + std::to_string(mAudioSampleBuffer.getMagnitude(0, mAudioSampleBuffer.getNumSamples())), 0, 35, 150, 10, Justification::left, false);
}

void USpectrumAnalyzerAudioProcessorEditor::resized()
{
}

void USpectrumAnalyzerAudioProcessorEditor::timerCallback()
{
    repaint();
}

void USpectrumAnalyzerAudioProcessorEditor::processFFT()
{
    const float* readLeft = mAudioSampleBuffer.getReadPointer(0);
    const float* readRight = mAudioSampleBuffer.getReadPointer(1);
    int len = mAudioSampleBuffer.getNumSamples();

    juce::Range<float> minmax = mAudioSampleBuffer.findMinMax(0, 0, mAudioSampleBuffer.getNumSamples());

    if (len > fftbuflen) len = fftbuflen;

    // mix left and right channels to mono and copy them to fft buffer with zero-paddings
    for (int i = 0; i < fftbuflen; i++) {
        if (i < len) {
            fftBuffer[i] = (readLeft[i] + readRight[i]) / 2.0;
        } else {
            fftBuffer[i] = 0.0;
        }
    }

    float scale = (float)mHeight / minmax.getEnd();
    int step = mWidth / ffthalflen;

    fft.performFrequencyOnlyForwardTransform(fftBuffer);
    updateSpectrumGraphics(fftBuffer, ffthalflen, scale, step);
}

void USpectrumAnalyzerAudioProcessorEditor::updateSpectrumGraphics(float* buffer, int bufferHalfLen, float scale, int step)
{
    lines.clear();

    float pos = 0;
    for (int i = 0; i < bufferHalfLen; i++) {
        float cur = buffer[i] * scale;

        // check if cur is inf or NaN 
        if (std::isinf(cur) || cur != cur) cur = 0.0;

        Line<float> l;
        l.setStart(pos, mHeight);
        l.setEnd(pos, mHeight - cur);

        lines.push_back(l);

        pos += step;
    }
}