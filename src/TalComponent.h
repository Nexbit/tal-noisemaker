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

#ifndef TALCOMPONENTEDITOR_H
#define TALCOMPONENTEDITOR_H

#include "TalCore.h"
#include "FilmStripKnob.h"
#include "ImageSlider.h"
#include "ImageToggleButton.h"

#include "./Engine/AudioUtils.h"


//==============================================================================
/**
    This is the Component that our filter will use as its UI.

    One or more of these is created by the DemoJuceFilter::createEditor() method,
    and they will be deleted at some later time by the wrapper code.

    To demonstrate the correct way of connecting a filter to its UI, this
    class is a ChangeListener, and our demo filter is a ChangeBroadcaster. The
    editor component registers with the filter when it's created and deregisters
    when it's destroyed. When the filter's parameters are changed, it broadcasts
    a message and this editor responds by updating its display.
*/
class TalComponent   : public AudioProcessorEditor,
                          public ChangeListener,
                          public SliderListener,
						  public ButtonListener,
						  public ComboBoxListener
{
public:
    /** Constructor.

        When created, this will register itself with the filter for changes. It's
        safe to assume that the filter won't be deleted before this object is.
    */
    TalComponent(TalCore* const ownerFilter);



    /** Destructor. */
    ~TalComponent();

    //==============================================================================
    /** Our demo filter is a ChangeBroadcaster, and will call us back when one of
        its parameters changes.
    */
    void changeListenerCallback (void* source);
    void sliderValueChanged (Slider*);
	void buttonClicked (Button *);
    void comboBoxChanged (ComboBox*);

    //==============================================================================
    /** Standard Juce paint callback. */
    void paint (Graphics& g);

    /** Standard Juce resize callback. */
    //void resized();

    static const char* bmp00128_png;
    static const int bmp00128_pngSize;

    static const char* bmp00129_png;
    static const int bmp00129_pngSize;

    static const char* bmp00130_png;
    static const int bmp00130_pngSize;

    static const char* bmp00131_png;
    static const int bmp00131_pngSize;

    static const char* bmp00132_png;
    static const int bmp00132_pngSize;

    static const char* bmp00133_png;
    static const int bmp00133_pngSize;

    static const char* bmp00134_png;
    static const int bmp00134_pngSize;

    static const char* bmp00135_png;
    static const int bmp00135_pngSize;

private:
    //==============================================================================
    //const Image* internalCachedBackgroundImage;

	FilmStripKnob *volumeKnob;

    ComboBox *filtertypeComboBox;
	FilmStripKnob *cutoffKnob;
	FilmStripKnob *resonanceKnob; 
	FilmStripKnob *filterContourKnob; 
	FilmStripKnob *keyfollowKnob; 

	FilmStripKnob *osc1VolumeKnob; 
	FilmStripKnob *osc2VolumeKnob; 
	FilmStripKnob *osc3VolumeKnob; 
	FilmStripKnob *portamentoKnob; 

	FilmStripKnob *osc1WaveformKnob;
	FilmStripKnob *osc2WaveformKnob;

	FilmStripKnob *oscMasterTuneKnob;
	FilmStripKnob *osc1TuneKnob;
	FilmStripKnob *osc2TuneKnob;
	FilmStripKnob *osc1FineTuneKnob;
	FilmStripKnob *osc2FineTuneKnob;

	ImageSlider *filterAttackKnob; 
	ImageSlider *filterDecayKnob; 
	ImageSlider *filterSustainKnob; 
	ImageSlider *filterReleaseKnob;

	ImageSlider *ampAttackKnob; 
	ImageSlider *ampDecayKnob; 
	ImageSlider *ampSustainKnob; 
	ImageSlider *ampReleaseKnob; 

    ImageSlider *velocityVolumeKnob;
    ImageSlider *velocityContourKnob;
    ImageSlider *velocityCutoffKnob;
    ImageSlider *pitchwheelCutoffKnob;
    ImageSlider *pitchwheelPitchKnob;
    ImageSlider *highpassKnob;
    ImageSlider *detuneKnob;

	ImageToggleButton *oscSyncButton;
	ImageToggleButton *panicButton;
	ImageToggleButton *midiLearnButton;
	ImageToggleButton *chorus1Button;
	ImageToggleButton *chorus2Button;

	ComboBox *voicesComboBox;
	ComboBox *portamentoModeComboBox;

	FilmStripKnob *lfo1WaveformKnob;
	FilmStripKnob *lfo2WaveformKnob;

	FilmStripKnob *lfo1RateKnob;
	FilmStripKnob *lfo2RateKnob;
	
	FilmStripKnob *lfo1AmountKnob;
	FilmStripKnob *lfo2AmountKnob;

	ComboBox *lfo1DestinationComboBox;
	ComboBox *lfo2DestinationComboBox;

	FilmStripKnob *lfo1PhaseKnob;
	FilmStripKnob *lfo2PhaseKnob;

	FilmStripKnob *osc1PwKnob;
	FilmStripKnob *osc2FmKnob;
	FilmStripKnob *osc1PhaseKnob;
	FilmStripKnob *osc2PhaseKnob;
	FilmStripKnob *transposeKnob;

	FilmStripKnob *reverbWetKnob;
	FilmStripKnob *reverbDecayKnob;
	FilmStripKnob *reverbPreDelayKnob;
	FilmStripKnob *reverbHighCutKnob;
	FilmStripKnob *reverbLowCutKnob;

	FilmStripKnob *oscBitcrusherKnob;

	FilmStripKnob *ringmodulationKnob;

	FilmStripKnob *freeAdAttackKnob;
	FilmStripKnob *freeAdDecayKnob;
	FilmStripKnob *freeAdAmountKnob;
	ComboBox *freeAdDestinationComboBox;

	ImageToggleButton *lfo1SyncButton;
	ImageToggleButton *lfo1KeyTriggerButton;
	ImageToggleButton *lfo2SyncButton;
	ImageToggleButton *lfo2KeyTriggerButton;

	Label *versionLabel;
    Label *infoText;

	AudioUtils audioUtils;
    TooltipWindow tooltipWindow;

    HyperlinkButton *hyperlinkButtoon;

    void updateParametersFromFilter();
	FilmStripKnob* addNormalKnob(int x, int y, TalCore* const ownerFilter, const Image knobImage, int numOfFrames, const int parameter);
	ImageToggleButton* addNormalButton(int x, int y, TalCore* const ownerFilter, const Image buttonImage, bool isKickButton, int parameter);	
	ImageSlider* addSlider(int x, int y, TalCore* const ownerFilter, const Image sliderImage, int height, int parameter);
	ComboBox* addComboBox(int x, int y, int width, TalCore* const ownerFilter, int parameter);
    void updateInfo(Slider* caller);

	void setTooltip(Slider* slider);

    // handy wrapper method to avoid having to cast the filter to a DemoJuceFilter
    // every time we need it..
    TalCore* getFilter() const throw()       { return (TalCore*) getAudioProcessor(); }
};


#endif
