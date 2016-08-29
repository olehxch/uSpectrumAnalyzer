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
        mAudioSampleBuffer.makeCopyOf(processor.getAudioSampleBuffer());
        processFFT();

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

    g.setColour(Colours::darkblue);
    std::vector<Line<float> > copyLines(lines);
    for (auto line : copyLines) {
        g.drawLine(line, 1.0);
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

    float scale = mHeight;
    float fftfreqs = fft.getSize() / 2;
    float step = (float)mWidth / (float)mAudioSampleBuffer.getNumSamples();

    fft.performFrequencyOnlyForwardTransform(fftBuffer);
    updateSpectrumGraphics(fftBuffer, mAudioSampleBuffer.getNumSamples(), scale, step);
}

void USpectrumAnalyzerAudioProcessorEditor::updateSpectrumGraphics(float* buffer, int len, float scale, float step)
{
    lines.clear();

    

    float pos = 0;
    for (int i = 0; i < len; i++) {
        float cur = buffer[i] * scale;

        // check if cur is inf or NaN 
        //if (std::isinf(cur) || cur != cur) cur = 0.0;

        Line<float> l;
        l.setStart(pos, mHeight);

        float maxh = (mHeight - cur);
        if (maxh > mHeight) maxh = mHeight - 5.0;

        l.setEnd(pos, maxh);

        lines.push_back(l);

        pos += step;
    }
}