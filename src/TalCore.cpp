/*
==============================================================================

This file is part of the JUCE library - "Jules' Utility Class Extensions"
Copyright 2004-7 by Raw Material Software ltd.

------------------------------------------------------------------------------

JUCE can be redistributed and/or modified under the terms of the
GNU General Public License, as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.

JUCE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JUCE; if not, visit www.gnu.org/licenses or write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330,
Boston, MA 02111-1307 USA

------------------------------------------------------------------------------

If you'd like to release a closed-source product which uses JUCE, commercial
licenses are also available: visit www.rawmaterialsoftware.com/juce for
more information.

==============================================================================
*/

#include "includes.h"
#include "TalComponent.h"
#include "ProgramChunk.h"

/**
This function must be implemented to create a new instance of your
plugin object.
*/
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TalCore();
}

TalCore::TalCore()
{
    // init engine
    if (this->getSampleRate() > 0)
        sampleRate = (float)this->getSampleRate();
    else
        sampleRate = 44100.0f;

    engine = new SynthEngine(sampleRate);
    params = engine->param;

    talPresets = new TalPreset*[NUMPROGRAMS];

    for (int i = 0; i < NUMPROGRAMS; i++) talPresets[i] = new TalPreset(); 
    curProgram = 0;

    // load factory presets
    ProgramChunk *chunk = new ProgramChunk();
    XmlDocument myDocument (chunk->getXmlChunk());
    XmlElement* mainElement = myDocument.getDocumentElement();

    MemoryBlock* destData = new MemoryBlock();
    copyXmlToBinary(*mainElement, *destData);
    setStateInformation(destData->getData(), destData->getSize());
    setCurrentProgram(curProgram);

    // used for midi learn
    lastMovedController = 0;
    currentNumberOfVoices = 0;
    valueChangedMidiController = 0;

    this->bpm = 120.0f;
}

TalCore::~TalCore()
{
    if (talPresets) delete[] talPresets;
    if (engine) delete engine;
}

const String TalCore::getName() const
{
    return "Noize Mak3r";
}

int TalCore::getNumParameters()
{
    return (int)NUMPARAM;
}

float TalCore::getParameter (int index)
{
    if (index < NUMPARAM)
        return talPresets[curProgram]->programData[index];
    else 
        return 0;
}

int TalCore::getValueChangedMidiController()
{
    return valueChangedMidiController;
}

void TalCore::setParameter (int index, float newValue)
{
    if (index < NUMPARAM)
    {
        params[index] = newValue;
        talPresets[curProgram]->programData[index] = newValue;

        switch(index)
        {
        case VOLUME:
            engine->setVolume(newValue);
            break;
        case CUTOFF:
            engine->setCutoff(newValue);
            break;
        case RESONANCE:
            engine->setResonance(newValue);
            break;
        case FILTERCONTOUR:
            engine->setFilterContour(newValue);
            break;
        case KEYFOLLOW:
            engine->setKeyfollow(newValue);
            break;
        case FILTERATTACK:
            engine->setFilterAttack(newValue);
            break;
        case FILTERDECAY:
            engine->setFilterDecay(newValue);
            break;
        case FILTERSUSTAIN:
            engine->setFilterSustain(newValue);
            break;
        case FILTERRELEASE:
            engine->setFilterRelease(newValue);
            break;
        case AMPATTACK:
            engine->setAmpAttack(newValue);
            break;
        case AMPDECAY:
            engine->setAmpDecay(newValue);
            break;
        case AMPSUSTAIN:
            engine->setAmpSustain(newValue);
            break;
        case AMPRELEASE:
            engine->setAmpRelease(newValue);
            break;
        case OSC1VOLUME:
            engine->setOsc1Volume(newValue);
            break;
        case OSC2VOLUME:
            engine->setOsc2Volume(newValue);
            break;
        case OSC3VOLUME:
            engine->setOsc3Volume(newValue);
            break;
        case OSC1WAVEFORM:
            engine->setOsc1Waveform(newValue);
            break;
        case OSC2WAVEFORM:
            engine->setOsc2Waveform(newValue);
            break;
        case OSC1TUNE:
            engine->setOsc1Tune(newValue);
            break;
        case OSC2TUNE:
            engine->setOsc2Tune(newValue);
            break;
        case OSC1FINETUNE:
            engine->setOsc1FineTune(newValue);
            break;
        case OSC2FINETUNE:
            engine->setOsc2FineTune(newValue);
            break;
        case OSCSYNC:
            engine->setOscSync(newValue > 0.0f);
            break;
        case PANIC:
            engine->setPanic();
            break;
        case PORTAMENTO:
            engine->setPortamento(newValue);
            break;
        case PORTAMENTOMODE:
            engine->setPortamentoMode(newValue);
            break;
        case LFO1RATE:
            engine->setLfo1Rate(newValue, this->bpm);
            break;
        case LFO2RATE:
            engine->setLfo2Rate(newValue, this->bpm);
            break;
        case LFO1AMOUNT:
            engine->setLfo1Amount(newValue);
            break;
        case LFO2AMOUNT:
            engine->setLfo2Amount(newValue);
            break;
        case LFO1WAVEFORM:
            engine->setLfo1Waveform(newValue);
            break;
        case LFO2WAVEFORM:
            engine->setLfo2Waveform(newValue);
            break;
        case LFO1DESTINATION:
            engine->setLfo1Destination(newValue);
            break;
        case LFO2DESTINATION:
            engine->setLfo2Destination(newValue);
            break;
        case OSC1PW:
            engine->setOsc1Pw(newValue);
            break;
        case OSC1PHASE:
            engine->setOsc1Phase(newValue);
            break;
        case OSC2FM:
            engine->setOsc1Fm(newValue);
            break;
        case OSC2PHASE:
            engine->setOsc2Phase(newValue);
            break;
        case FREEADATTACK:
            engine->setFreeAdAttack(newValue);
            break;
        case FREEADDECAY:
            engine->setFreeAdDecay(newValue);
            break;
        case FREEADAMOUNT:
            engine->setFreeAdAmount(newValue);
            break;
        case FREEADDESTINATION:
            engine->setFreeAdDestination(newValue);
            break;
        case LFO1SYNC:
            engine->setLfo1Sync(newValue, getParameter(LFO1RATE), this->bpm);
            break;
        case LFO1KEYTRIGGER:
            engine->setLfo1KeyTrigger(newValue);
            break;
        case LFO1PHASE:
            engine->setLfo1Phase(newValue);
            break;
        case LFO2SYNC:
            engine->setLfo2Sync(newValue, getParameter(LFO2RATE), this->bpm);
            break;
        case LFO2KEYTRIGGER:
            engine->setLfo2KeyTrigger(newValue);
            break;
        case LFO2PHASE:
            engine->setLfo2Phase(newValue);
            break;
        case VELOCITYVOLUME:
            engine->setVelocityVolume(newValue);
            break;
        case VELOCITYCONTOUR:
            engine->setVelocityContour(newValue);
            break;
        case VELOCITYCUTOFF:
            engine->setVelocityCutoff(newValue);
            break;
        case PITCHWHEELCUTOFF:
            engine->setPitchwheelCutoff(newValue);
            break;
        case PITCHWHEELPITCH:
            engine->setPitchwheelPitch(newValue);
            break;
        case HIGHPASS:
            engine->setHighPass(newValue);
            break;
        case DETUNE:
            engine->setDetune(newValue);
            break;
        case OSCMASTERTUNE:
            engine->setMastertune(newValue);
            break;
        case TRANSPOSE:
            engine->setTranspose(newValue);
            break;
        case RINGMODULATION:
            engine->setRingmodulation(newValue);
            break;
        case CHORUS1ENABLE:
            engine->setChorus(newValue > 0.0f, getParameter(CHORUS2ENABLE) > 0.0f);
            break;
        case CHORUS2ENABLE:
            engine->setChorus(getParameter(CHORUS1ENABLE) > 0.0f, newValue > 0.0f);
            break;
        case REVERBWET:
            engine->setReverbWet(newValue);
            break;
        case REVERBDECAY:
            engine->setReverbDecay(newValue);
            break;
        case REVERBPREDELAY:
            engine->setReverbPreDelay(newValue);
            break;
        case REVERBHIGHCUT:
            engine->setReverbHighCut(newValue);
            break;
        case REVERBLOWCUT:
            engine->setReverbLowCut(newValue);
            break;
        case OSCBITCRUSHER:
            engine->setOscBitcrusher(newValue);
            break;
        case FILTERTYPE:
            engine->setFiltertype(newValue);
            break;
        }
        sendChangeMessage(this);

        // for midi learn
        lastMovedController = index;
    }
}

const String TalCore::getParameterName (int index)
{
    switch(index)
    {
    case VOLUME: return T("volume");
    case OSC1VOLUME: return T("osc1volume");
    case OSC2VOLUME: return T("osc2volume");
    case OSC3VOLUME: return T("osc3volume");

    case OSC1WAVEFORM: return T("osc1waveform");
    case OSC2WAVEFORM: return T("osc2waveform");
    case OSCSYNC: return T("oscsync");

    case OSCMASTERTUNE: return T("oscmastertune");
    case OSC1TUNE: return T("osc1tune");
    case OSC2TUNE: return T("osc2tune");
    case OSC1FINETUNE: return T("osc1finetune");
    case OSC2FINETUNE: return T("osc2finetune");

    case FILTERTYPE: return T("filtertype");
    case CUTOFF: return T("cutoff");
    case RESONANCE: return T("resonance");
    case KEYFOLLOW: return T("keyfollow");
    case FILTERCONTOUR: return T("filtercontour");
    case FILTERATTACK: return T("filterattack");
    case FILTERDECAY: return T("filterdecay");
    case FILTERSUSTAIN: return T("filtersustain");
    case FILTERRELEASE: return T("filterrelease");

    case AMPATTACK: return T("ampattack");
    case AMPDECAY: return T("ampdecay");
    case AMPSUSTAIN: return T("ampsustain");
    case AMPRELEASE: return T("amprelease");

    case VOICES: return T("voices");
    case PORTAMENTO: return T("portamento");
    case PORTAMENTOMODE: return T("portamentomode");

    case LFO1WAVEFORM: return T("lfo1waveform");
    case LFO2WAVEFORM: return T("lfo2waveform");
    case LFO1RATE: return T("lfo1rate");
    case LFO2RATE: return T("lfo2rate");
    case LFO1AMOUNT: return T("lfo1amount");
    case LFO2AMOUNT: return T("lfo2amount");
    case LFO1DESTINATION: return T("lfo1destination");
    case LFO2DESTINATION: return T("lfo2destination");
    case LFO1PHASE: return T("lfo1phase");
    case LFO2PHASE: return T("lfo2phase");

    case OSC1PW: return T("osc1pw");
    case OSC2FM: return T("osc2fm");
    case OSC1PHASE: return T("osc1phase");
    case OSC2PHASE: return T("osc2phase");
    case TRANSPOSE: return T("transpose");

    case FREEADATTACK: return T("freeadattack");
    case FREEADDECAY: return T("freeaddecay");
    case FREEADAMOUNT: return T("freeadamount");
    case FREEADDESTINATION: return T("freeaddestination");

    case LFO1SYNC: return T("lfo1sync");
    case LFO1KEYTRIGGER: return T("lfo1keytrigger");
    case LFO2SYNC: return T("lfo2sync");
    case LFO2KEYTRIGGER: return T("lfo2keytrigger");

    case VELOCITYVOLUME: return T("velocityvolume");
    case VELOCITYCONTOUR: return T("velocitycontour");
    case VELOCITYCUTOFF: return T("velocitycutoff");
    case PITCHWHEELCUTOFF: return T("pitchwheelcutoff");
    case PITCHWHEELPITCH: return T("pitchwheelpitch");
    case HIGHPASS: return T("highpass");
    case DETUNE: return T("detune");
    case RINGMODULATION: return T("ringmodulation");

    case CHORUS1ENABLE: return T("chorus1enable");
    case CHORUS2ENABLE: return T("chorus2enable");

    case REVERBWET: return T("reverbwet");
    case REVERBDECAY: return T("reverbdecay");
    case REVERBPREDELAY: return T("reverbpredelay");
    case REVERBHIGHCUT: return T("reverbhighcut");
    case REVERBLOWCUT: return T("reverblowcut");

    case OSCBITCRUSHER: return T("oscbitcrusher");
    }
    return String::empty;
}

const String TalCore::getParameterText (int index)
{
    if (index < NUMPARAM)
    {
        return String(talPresets[curProgram]->programData[index], 2);
    }
    return String::empty;
}

const String TalCore::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String TalCore::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool TalCore::isInputChannelStereoPair (int index) const
{
    return true;
}

bool TalCore::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool TalCore::acceptsMidi() const
{
    return true;
}

bool TalCore::producesMidi() const
{
    return false;
}

void TalCore::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void TalCore::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void TalCore::processBlock (AudioSampleBuffer& buffer,
                            MidiBuffer& midiMessages)
{
    // Handle sample rate changes
    if (sampleRate != this->getSampleRate())
    {
        sampleRate = this->getSampleRate();
        engine->setSampleRate(sampleRate);
        setCurrentProgram(curProgram);
    }

    // Number of voices
    if (currentNumberOfVoices != params[VOICES])
    {
        currentNumberOfVoices = params[VOICES];
        engine->setNumberOfVoices((int)params[VOICES]);
        setCurrentProgram(curProgram);
    }

    // bpm
    AudioPlayHead::CurrentPositionInfo pos;
    if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition (pos))
    {
        this->bpm = pos.bpm;
        this->engine->triggerLfoToHost(this->bpm, pos.ppqPosition);
    }
    else
    {
        this->bpm = 120.0f;
    }

    const ScopedLock sl (this->getCallbackLock());

    nextMidiMessage = new MidiMessage(0xF0);
    midiMessage = new MidiMessage(0xF0);

    int numberOfChannels = getNumOutputChannels();
    int bufferSize = buffer.getNumSamples();

    // midi messages
    MidiBuffer::Iterator midiIterator(midiMessages);
    nextMidiMessage = new MidiMessage(0xF0);
    midiMessage = new MidiMessage(0xF0);
    hasMidiMessage = midiIterator.getNextEvent(*nextMidiMessage, midiEventPos);

    if (numberOfChannels == 2)
    {
        float *samples0 = buffer.getSampleData(0, 0);
        float *samples1 = buffer.getSampleData(1, 0);

        int samplePos = 0;
        int numSamples = buffer.getNumSamples();
        while (numSamples > 0)
        {
            processMidiPerSample(&midiIterator, samplePos);
            engine->process(samples0++, samples1++);

            numSamples--;
            samplePos++;
        }
    }
}

float TalCore::getBpm()
{
    return this->bpm;
}

inline void TalCore::processMidiPerSample(MidiBuffer::Iterator *midiIterator, const int samplePos)
{
    // There can be more than one event at the same position
    while (this->getNextEvent(midiIterator, samplePos))
    {
        if (midiMessage->isController())
        {
            handleController (midiMessage->getControllerNumber(),
                midiMessage->getControllerValue());
        }
        else if (midiMessage->isNoteOn())
        {
            engine->setNoteOn(midiMessage->getNoteNumber(), midiMessage->getFloatVelocity());
        }
        else if (midiMessage->isNoteOff())
        {
            engine->setNoteOff(midiMessage->getNoteNumber());
        }
        else if (midiMessage->isPitchWheel())
        {
            // [0..16383] center = 8192;
            engine->setPitchwheelAmount((midiMessage->getPitchWheelValue() - 8192.0f) / (16383.0f * 0.5f));
        }
    }
}

inline bool TalCore::getNextEvent(MidiBuffer::Iterator *midiIterator, const int samplePos)
{
    if (hasMidiMessage && midiEventPos <= samplePos)
    {
        *midiMessage = *nextMidiMessage;
        hasMidiMessage = midiIterator->getNextEvent(*nextMidiMessage, midiEventPos);
        return true;
    } 
    return false;
}

void TalCore::handleController (const int controllerNumber,
                                const int controllerValue)
{
    if (params[MIDILEARN] > 0.0f)
    {
        for (int i = 0; i < NUMPROGRAMS; i++) 
        {
            talPresets[i]->midiMap[controllerNumber] = lastMovedController; 
        }
    }
    int midiControlNumber = talPresets[curProgram]->midiMap[controllerNumber];
    if (midiControlNumber > 0)
    {
        setParameter(midiControlNumber, controllerValue / 127.0f);
        valueChangedMidiController = midiControlNumber;
    }
}

AudioProcessorEditor* TalCore::createEditor()
{
    return new TalComponent (this);
}

void TalCore::getStateInformation (MemoryBlock& destData)
{
    // header
    XmlElement tal("tal");
    tal.setAttribute (T("curprogram"), curProgram);
    tal.setAttribute (T("version"), 1);

    // programs
    XmlElement *programList = new XmlElement ("programs");
    for (int i = 0; i < NUMPROGRAMS; i++)
    {
        getXmlPrograms(programList, i);
    }
    tal.addChildElement(programList);

    storeMidiMapping(tal);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (tal, destData);

    // use this for new factory presets
    //#ifdef _DEBUG && WIN32
    //File *file = new File("presets.txt");
    //String myXmlDoc = tal.createDocument ("presets.txt");
    //file->replaceWithText(myXmlDoc);
    //#endif
}

void TalCore::setStateInformation (const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);

    curProgram = 0;
    if (xmlState != 0 && xmlState->hasTagName(T("tal")))
    {
        curProgram = (int)xmlState->getIntAttribute (T("curprogram"), 1);
        XmlElement* programs = xmlState->getFirstChildElement();
        if (programs->hasTagName(T("programs")))
        {
            int i = 0;
            forEachXmlChildElement (*programs, e)
            {
                setXmlPrograms(e, i);
                i++;
            }
        }

        restoreMidiMapping(xmlState);

        delete xmlState;
        setCurrentProgram(curProgram);
        sendChangeMessage (this);
    }
}

void TalCore::getXmlPrograms(XmlElement *programList, int programNumber)
{
        XmlElement* program = new XmlElement ("program");
        program->setAttribute (T("programname"), talPresets[programNumber]->name);
        program->setAttribute (T("volume"), talPresets[programNumber]->programData[VOLUME]);

        program->setAttribute (T("filtertype"), talPresets[programNumber]->programData[FILTERTYPE]);
        program->setAttribute (T("cutoff"), talPresets[programNumber]->programData[CUTOFF]);
        program->setAttribute (T("resonance"), talPresets[programNumber]->programData[RESONANCE]);
        program->setAttribute (T("osc1volume"), talPresets[programNumber]->programData[OSC1VOLUME]);
        program->setAttribute (T("osc2volume"), talPresets[programNumber]->programData[OSC2VOLUME]);
        program->setAttribute (T("osc3volume"), talPresets[programNumber]->programData[OSC3VOLUME]);
        program->setAttribute (T("osc1waveform"), talPresets[programNumber]->programData[OSC1WAVEFORM]);
        program->setAttribute (T("osc2waveform"), talPresets[programNumber]->programData[OSC2WAVEFORM]);
        program->setAttribute (T("oscsync"), talPresets[programNumber]->programData[OSCSYNC]);

        program->setAttribute (T("oscmastertune"), talPresets[programNumber]->programData[OSCMASTERTUNE]);
        program->setAttribute (T("osc1tune"), talPresets[programNumber]->programData[OSC1TUNE]);
        program->setAttribute (T("osc2tune"), talPresets[programNumber]->programData[OSC2TUNE]);
        program->setAttribute (T("osc1finetune"), talPresets[programNumber]->programData[OSC1FINETUNE]);
        program->setAttribute (T("osc2finetune"), talPresets[programNumber]->programData[OSC2FINETUNE]);
        program->setAttribute (T("portamento"), talPresets[programNumber]->programData[PORTAMENTO]);

        program->setAttribute (T("keyfollow"), talPresets[programNumber]->programData[KEYFOLLOW]);
        program->setAttribute (T("filtercontour"), talPresets[programNumber]->programData[FILTERCONTOUR]);
        program->setAttribute (T("filterattack"), talPresets[programNumber]->programData[FILTERATTACK]);
        program->setAttribute (T("filterdecay"), talPresets[programNumber]->programData[FILTERDECAY]);
        program->setAttribute (T("filtersustain"), talPresets[programNumber]->programData[FILTERSUSTAIN]);
        program->setAttribute (T("filterrelease"), talPresets[programNumber]->programData[FILTERRELEASE]);

        program->setAttribute (T("ampattack"), talPresets[programNumber]->programData[AMPATTACK]);
        program->setAttribute (T("ampdecay"), talPresets[programNumber]->programData[AMPDECAY]);
        program->setAttribute (T("ampsustain"), talPresets[programNumber]->programData[AMPSUSTAIN]);
        program->setAttribute (T("amprelease"), talPresets[programNumber]->programData[AMPRELEASE]);

        program->setAttribute (T("voices"), talPresets[programNumber]->programData[VOICES]);
        program->setAttribute (T("portamentomode"), talPresets[programNumber]->programData[PORTAMENTOMODE]);

        program->setAttribute (T("lfo1waveform"), talPresets[programNumber]->programData[LFO1WAVEFORM]);
        program->setAttribute (T("lfo2waveform"), talPresets[programNumber]->programData[LFO2WAVEFORM]);
        program->setAttribute (T("lfo1rate"), talPresets[programNumber]->programData[LFO1RATE]);
        program->setAttribute (T("lfo2rate"), talPresets[programNumber]->programData[LFO2RATE]);
        program->setAttribute (T("lfo1amount"), talPresets[programNumber]->programData[LFO1AMOUNT]);
        program->setAttribute (T("lfo2amount"), talPresets[programNumber]->programData[LFO2AMOUNT]);
        program->setAttribute (T("lfo1destination"), talPresets[programNumber]->programData[LFO1DESTINATION]);
        program->setAttribute (T("lfo2destination"), talPresets[programNumber]->programData[LFO2DESTINATION]);
        program->setAttribute (T("lfo1phase"), talPresets[programNumber]->programData[LFO1PHASE]);
        program->setAttribute (T("lfo2phase"), talPresets[programNumber]->programData[LFO2PHASE]);

        program->setAttribute (T("osc1pw"), talPresets[programNumber]->programData[OSC1PW]);
        program->setAttribute (T("osc2fm"), talPresets[programNumber]->programData[OSC2FM]);
        program->setAttribute (T("osc1phase"), talPresets[programNumber]->programData[OSC1PHASE]);
        program->setAttribute (T("osc2phase"), talPresets[programNumber]->programData[OSC2PHASE]);
        program->setAttribute (T("transpose"), talPresets[programNumber]->programData[TRANSPOSE]);

        program->setAttribute (T("freeadattack"), talPresets[programNumber]->programData[FREEADATTACK]);
        program->setAttribute (T("freeaddecay"), talPresets[programNumber]->programData[FREEADDECAY]);
        program->setAttribute (T("freeadamount"), talPresets[programNumber]->programData[FREEADAMOUNT]);
        program->setAttribute (T("freeaddestination"), talPresets[programNumber]->programData[FREEADDESTINATION]);

        program->setAttribute (T("lfo1sync"), talPresets[programNumber]->programData[LFO1SYNC]);
        program->setAttribute (T("lfo1keytrigger"), talPresets[programNumber]->programData[LFO1KEYTRIGGER]);
        program->setAttribute (T("lfo2sync"), talPresets[programNumber]->programData[LFO2SYNC]);
        program->setAttribute (T("lfo2keytrigger"), talPresets[programNumber]->programData[LFO2KEYTRIGGER]);

        program->setAttribute (T("velocityvolume"), talPresets[programNumber]->programData[VELOCITYVOLUME]);
        program->setAttribute (T("velocitycontour"), talPresets[programNumber]->programData[VELOCITYCONTOUR]);
        program->setAttribute (T("velocitycutoff"), talPresets[programNumber]->programData[VELOCITYCUTOFF]);
        program->setAttribute (T("pitchwheelcutoff"), talPresets[programNumber]->programData[PITCHWHEELCUTOFF]);
        program->setAttribute (T("pitchwheelpitch"), talPresets[programNumber]->programData[PITCHWHEELPITCH]);
        program->setAttribute (T("highpass"), talPresets[programNumber]->programData[HIGHPASS]);
        program->setAttribute (T("detune"), talPresets[programNumber]->programData[DETUNE]);
        program->setAttribute (T("ringmodulation"), talPresets[programNumber]->programData[RINGMODULATION]);

        program->setAttribute (T("chorus1enable"), talPresets[programNumber]->programData[CHORUS1ENABLE]);
        program->setAttribute (T("chorus2enable"), talPresets[programNumber]->programData[CHORUS2ENABLE]);

        program->setAttribute (T("reverbwet"), talPresets[programNumber]->programData[REVERBWET]);
        program->setAttribute (T("reverbdecay"), talPresets[programNumber]->programData[REVERBDECAY]);
        program->setAttribute (T("reverbpredelay"), talPresets[programNumber]->programData[REVERBPREDELAY]);
        program->setAttribute (T("reverbhighcut"), talPresets[programNumber]->programData[REVERBHIGHCUT]);
        program->setAttribute (T("reverblowcut"), talPresets[programNumber]->programData[REVERBLOWCUT]);
        program->setAttribute (T("oscbitcrusher"), talPresets[programNumber]->programData[OSCBITCRUSHER]);
        programList->addChildElement(program);
}

void TalCore::setXmlPrograms(XmlElement* e, int programNumber)
{
    if (e->hasTagName(T("program")) && programNumber < NUMPROGRAMS)
    {
        talPresets[programNumber]->name = e->getStringAttribute (T("programname"), T("Not Saved") + programNumber);
        talPresets[programNumber]->programData[VOLUME] = (float) e->getDoubleAttribute (T("volume"), 0.5f);

        talPresets[programNumber]->programData[FILTERTYPE] = (float) e->getDoubleAttribute (T("filtertype"), 1.0f);
        talPresets[programNumber]->programData[CUTOFF] = (float) e->getDoubleAttribute (T("cutoff"), 1.0f);
        talPresets[programNumber]->programData[RESONANCE] = (float) e->getDoubleAttribute (T("resonance"), 0.0f);
        talPresets[programNumber]->programData[OSC1VOLUME] = (float) e->getDoubleAttribute (T("osc1volume"), 0.8f);
        talPresets[programNumber]->programData[OSC2VOLUME] = (float) e->getDoubleAttribute (T("osc2volume"), 0.0f);
        talPresets[programNumber]->programData[OSC3VOLUME] = (float) e->getDoubleAttribute (T("osc3volume"), 0.8f);
        talPresets[programNumber]->programData[OSC1WAVEFORM] = (float) e->getDoubleAttribute (T("osc1waveform"), 0.0f);
        talPresets[programNumber]->programData[OSC2WAVEFORM] = (float) e->getDoubleAttribute (T("osc2waveform"), 0.0f);
        talPresets[programNumber]->programData[OSCSYNC] = (float) e->getDoubleAttribute (T("oscsync"), 0.0f);

        talPresets[programNumber]->programData[OSCMASTERTUNE] = (float) e->getDoubleAttribute (T("oscmastertune"), 0.5f);
        talPresets[programNumber]->programData[OSC1TUNE] = (float) e->getDoubleAttribute (T("osc1tune"), 0.25f);
        talPresets[programNumber]->programData[OSC2TUNE] = (float) e->getDoubleAttribute (T("osc2tune"), 0.5f);
        talPresets[programNumber]->programData[OSC1FINETUNE] = (float) e->getDoubleAttribute (T("osc1finetune"), 0.5f);
        talPresets[programNumber]->programData[OSC2FINETUNE] = (float) e->getDoubleAttribute (T("osc2finetune"), 0.5f);

        talPresets[programNumber]->programData[KEYFOLLOW] = (float) e->getDoubleAttribute (T("keyfollow"), 0.0f);
        talPresets[programNumber]->programData[FILTERCONTOUR] = (float) e->getDoubleAttribute (T("filtercontour"), 0.5f);
        talPresets[programNumber]->programData[FILTERATTACK] = (float) e->getDoubleAttribute (T("filterattack"), 0.0f);
        talPresets[programNumber]->programData[FILTERDECAY] = (float) e->getDoubleAttribute (T("filterdecay"), 0.0f);
        talPresets[programNumber]->programData[FILTERSUSTAIN] = (float) e->getDoubleAttribute (T("filtersustain"), 1.0f);
        talPresets[programNumber]->programData[FILTERRELEASE] = (float) e->getDoubleAttribute (T("filterrelease"), 0.0f);

        talPresets[programNumber]->programData[AMPATTACK] = (float) e->getDoubleAttribute (T("ampattack"), 0.0f);
        talPresets[programNumber]->programData[AMPDECAY] = (float) e->getDoubleAttribute (T("ampdecay"), 0.0f);
        talPresets[programNumber]->programData[AMPSUSTAIN] = (float) e->getDoubleAttribute (T("ampsustain"), 1.0f);
        talPresets[programNumber]->programData[AMPRELEASE] = (float) e->getDoubleAttribute (T("amprelease"), 0.0f);

        talPresets[programNumber]->programData[VOICES] = (float) e->getDoubleAttribute (T("voices"), 1.0f);
        talPresets[programNumber]->programData[PORTAMENTOMODE] = (float) e->getDoubleAttribute (T("portamentomode"), 1.0f);
        talPresets[programNumber]->programData[PORTAMENTO] = (float) e->getDoubleAttribute (T("portamento"), 0.0f);

        talPresets[programNumber]->programData[LFO1WAVEFORM] = (float) e->getDoubleAttribute (T("lfo1waveform"), 0.0f);
        talPresets[programNumber]->programData[LFO2WAVEFORM] = (float) e->getDoubleAttribute (T("lfo2waveform"), 0.0f);
        talPresets[programNumber]->programData[LFO1RATE] = (float) e->getDoubleAttribute (T("lfo1rate"), 0.0f);
        talPresets[programNumber]->programData[LFO2RATE] = (float) e->getDoubleAttribute (T("lfo2rate"), 0.0f);
        talPresets[programNumber]->programData[LFO1AMOUNT] = (float) e->getDoubleAttribute (T("lfo1amount"), 0.5f);
        talPresets[programNumber]->programData[LFO2AMOUNT] = (float) e->getDoubleAttribute (T("lfo2amount"), 0.5f);
        talPresets[programNumber]->programData[LFO1DESTINATION] = (float) e->getDoubleAttribute (T("lfo1destination"), 1.0f);
        talPresets[programNumber]->programData[LFO2DESTINATION] = (float) e->getDoubleAttribute (T("lfo2destination"), 1.0f);
        talPresets[programNumber]->programData[LFO1PHASE] = (float) e->getDoubleAttribute (T("lfo1phase"), 0.0f);
        talPresets[programNumber]->programData[LFO2PHASE] = (float) e->getDoubleAttribute (T("lfo2phase"), 0.0f);

        talPresets[programNumber]->programData[OSC1PW] = (float) e->getDoubleAttribute (T("osc1pw"), 0.5f);
        talPresets[programNumber]->programData[OSC2FM] = (float) e->getDoubleAttribute (T("osc2fm"), 0.0f);
        talPresets[programNumber]->programData[OSC1PHASE] = (float) e->getDoubleAttribute (T("osc1phase"), 0.5f);
        talPresets[programNumber]->programData[OSC2PHASE] = (float) e->getDoubleAttribute (T("osc2phase"), 0.0f);
        talPresets[programNumber]->programData[TRANSPOSE] = (float) e->getDoubleAttribute (T("transpose"), 0.5f);

        talPresets[programNumber]->programData[FREEADATTACK] = (float) e->getDoubleAttribute (T("freeadattack"), 0.0f);
        talPresets[programNumber]->programData[FREEADDECAY] = (float) e->getDoubleAttribute (T("freeaddecay"), 0.0f);
        talPresets[programNumber]->programData[FREEADAMOUNT] = (float) e->getDoubleAttribute (T("freeadamount"), 0.0f);
        talPresets[programNumber]->programData[FREEADDESTINATION] = (float) e->getDoubleAttribute (T("freeaddestination"), 1.0f);

        talPresets[programNumber]->programData[LFO1SYNC] = (float) e->getDoubleAttribute (T("lfo1sync"), 0.0f);
        talPresets[programNumber]->programData[LFO1KEYTRIGGER] = (float) e->getDoubleAttribute (T("lfo1keytrigger"), 0.0f);
        talPresets[programNumber]->programData[LFO2SYNC] = (float) e->getDoubleAttribute (T("lfo2sync"), 0.0f);
        talPresets[programNumber]->programData[LFO2KEYTRIGGER] = (float) e->getDoubleAttribute (T("lfo2keytrigger"), 0.0f);

        talPresets[programNumber]->programData[VELOCITYVOLUME] = (float) e->getDoubleAttribute (T("velocityvolume"), 0.0f);
        talPresets[programNumber]->programData[VELOCITYCONTOUR] = (float) e->getDoubleAttribute (T("velocitycontour"), 0.0f);
        talPresets[programNumber]->programData[VELOCITYCUTOFF] = (float) e->getDoubleAttribute (T("velocitycutoff"), 0.0f);
        talPresets[programNumber]->programData[PITCHWHEELCUTOFF] = (float) e->getDoubleAttribute (T("pitchwheelcutoff"), 0.0f);
        talPresets[programNumber]->programData[PITCHWHEELPITCH] = (float) e->getDoubleAttribute (T("pitchwheelpitch"), 0.0f);
        talPresets[programNumber]->programData[HIGHPASS] = (float) e->getDoubleAttribute (T("highpass"), 0.0f);
        talPresets[programNumber]->programData[DETUNE] = (float) e->getDoubleAttribute (T("detune"), 0.0f);
        talPresets[programNumber]->programData[RINGMODULATION] = (float) e->getDoubleAttribute (T("ringmodulation"), 0.0f);
        talPresets[programNumber]->programData[CHORUS1ENABLE] = (float) e->getDoubleAttribute (T("chorus1enable"), 0.0f);
        talPresets[programNumber]->programData[CHORUS2ENABLE] = (float) e->getDoubleAttribute (T("chorus2enable"), 0.0f);

        talPresets[programNumber]->programData[REVERBWET] = (float) e->getDoubleAttribute (T("reverbwet"), 0.0f);
        talPresets[programNumber]->programData[REVERBDECAY] = (float) e->getDoubleAttribute (T("reverbdecay"), 0.5f);
        talPresets[programNumber]->programData[REVERBPREDELAY] = (float) e->getDoubleAttribute (T("reverbpredelay"), 0.0f);
        talPresets[programNumber]->programData[REVERBHIGHCUT] = (float) e->getDoubleAttribute (T("reverbhighcut"), 0.0f);
        talPresets[programNumber]->programData[REVERBLOWCUT] = (float) e->getDoubleAttribute (T("reverblowcut"), 1.0f);

        talPresets[programNumber]->programData[OSCBITCRUSHER] = (float) e->getDoubleAttribute (T("oscbitcrusher"), 1.0f);
    }
}

void TalCore::storeMidiMapping(XmlElement tal)
{
    XmlElement *midiMapList = new XmlElement ("midimap");
    for (int i = 0; i < 256; i++)
    {
        if (talPresets[0]->midiMap[i] != 0)
        {
            XmlElement* map = new XmlElement ("map");
            map->setAttribute (T("param"), talPresets[0]->midiMap[i]);
            map->setAttribute (T("controllernumber"), i);
            midiMapList->addChildElement(map);
        }
    }
    tal.addChildElement(midiMapList);
}

void TalCore::restoreMidiMapping(XmlElement* xmlState)
{
    XmlElement* midiMap = xmlState->getChildByName(T("midimap"));
    if (midiMap != 0 && midiMap->hasTagName(T("midimap")))
    {
        forEachXmlChildElement (*midiMap, e)
        {
            for (int j = 0; j < NUMPROGRAMS; j++) 
            {
                int controller = e->getIntAttribute(T("controllernumber"), 0);
                if (controller < 256 && controller > 0)
                {
                    talPresets[j]->midiMap[controller] = e->getIntAttribute(T("param"), 0);
                }
            }
        }
    }
}

int TalCore::getNumPrograms ()
{
    return NUMPROGRAMS;
}

int TalCore::getCurrentProgram ()
{
    return curProgram;
}

void TalCore::setCurrentProgram (int index)
{
    if (index < NUMPROGRAMS)
    {
        curProgram = index;
        for (int i = 0; i < NUMPARAM; i++) 
        {
            setParameter(i, talPresets[index]->programData[i]);
        }
        this->engine->reset();
        sendChangeMessage (this);
    }
}

const String TalCore::getProgramName (int index)
{
    if (index < NUMPROGRAMS)
        return talPresets[index]->name;
    else
        return T("Invalid");
}

void TalCore::changeProgramName (int index, const String& newName)
{
    if (index < NUMPROGRAMS)
        talPresets[index]->name = newName;
}
