/*
  ==============================================================================

    SynthVoice.h
    Created: 23 Oct 2016 9:58:15pm
    Author:  Mark Rau

  ==============================================================================
*/

#ifndef SYNTHVOICE_H_INCLUDED
#define SYNTHVOICE_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"
#include "Smooth.h"
#define FAUSTFLOAT float
#include "mydsp.h"

// Not really using this here, but we need it for things to work
struct MarkSound : public SynthesiserSound
{
    MarkSound() {}


    bool appliesToNote (int /*midiNoteNumber*/) override        { return true; }
    bool appliesToChannel (int /*midiChannel*/) override        { return true; }
};

// The FM synth voice. The FM synth is hardcoded here but ideally it should be in its own class
// to have a clear hierarchy (Sine -> FMSynth -> FMVoice)

struct MarkVoice : public SynthesiserVoice
{
	MarkVoice():
		tailOff(true)
	{		
		//build buffer for Faust sound
		audioBuffer = new float*[1];
		audioBuffer[0] = new float[1024];

		sineMod.init(getSampleRate());
		sineMod.buildUserInterface(&sineModControl);

		for (int i = 0; i<sineModControl.getParamsCount(); i++) {
			std::cout << sineModControl.getParamAdress(i) << "\n";
		}

		
	};


	~MarkVoice() {
		delete[] audioBuffer;
	};

    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<MarkSound*> (sound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
		level = velocity;
		freq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);

		//smooth[0].setSmooth(0.01);
		//smooth[0].tick(freq);
		//smooth[1].setSmooth(0.9999);

		// we don't want an ugly sweep when the note begins...
		smooth[0].setSmooth(0);
		smooth[0].tick(freq);

		smooth[1].setSmooth(0.999);
		

		//Set parameters of Faust object
		sineModControl.setParamValue("/SModulation1/NLFM/ModulationType1", (float) 1);
		sineModControl.setParamValue("/SModulation1/NLFM/Frequency", freq);
		sineModControl.setParamValue("/SModulation1/NLFM/Volume", (float) 1);

     
        onOff = true;
    }

    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        onOff = false; // end the note
        level = 0; // ramp envelope to 0 if tail off is allowed


        tailOff = allowTailOff;
    }

    void pitchWheelMoved (int /*newValue*/) override
    {
        // Pitch wheel is an important standard MIDI feature that should be implemented
    }

    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override
    {
        // Thats where you would change the value of the modulator index and frequency
        // if you wanted to control them with MIDI controllers
    }

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {


		//Make buffer of Faust sound
		sineMod.compute(numSamples, NULL, audioBuffer);

        // only compute block if note is on!
        if(envelope != 0 || onOff){
            while (--numSamples >= 0){
               
				envelope = smooth[1].tick(level);
                //const float currentSample = (float) 0;
                for (int i = outputBuffer.getNumChannels(); --i >= 0;){
                    outputBuffer.addSample (i, startSample, audioBuffer[0][startSample]*envelope);
					//float test = audioBuffer[0][startSample] * envelope;
               }
                ++startSample;

                // if tail off is disabled, we end the note right away, otherwise, we wait for envelope
                // to reach a safe value
				if (!onOff && (envelope < 0.001 || !tailOff)) {
					envelope = 0;
					clearCurrentNote();
				}
            }
        }
    }

private:
    bool onOff, tailOff;

	//define faust sine modulator
	mydsp sineMod; // the Faust module (mydsp.h)
	MapUI sineModControl; // used to easily control the Faust module (mydsp.h)


	float** audioBuffer; // multichannel audio buffer used both for input and output

	float level;
	float freq;
	int modType;
	Smooth smooth[3];
	double envelope;


};







#endif  // SYNTHVOICE_H_INCLUDED
