
# Overview
The MUSS3640 Vocal Synthesiser is a standalone application/VST3 plugin developed to allow exploratory synthesis of the singing voice. It implements a combination of formant-wave-function and bandpass synthesis techniques.

This documentation provides information on the installation and usage of the plugin.


# Installation
The standalone executable and VST3 files are contained within the `/MUSS3640 Vocal Synthesiser/Builds/` folder.


### Standalone Application
To run the standalone application, run `/MUSS3640 Vocal Synthesiser/Builds/FormantSynth.exe`.


### VST3
To install the VST3 plugin, copy `FormantSynth.vst3` from the `/MUSS3640 Vocal Synthesiser/Builds/` folder to your system's VST3 install location.

For Windows, the VST3 install location is often located at:
- `C:\Program Files\Common Files\VST3\` (64-bit architectures)
- `C:\Program Files (x86)\Common Files\VST3` (32-bit architectures)

For Macintosh, the VST3 install location is often located at:
- `Macintosh HD:/Library/Audio/Plug-Ins/VST3/`

The VST3 install location may need to be re-scanned from within your DAW for it to become available in the DAW.

Note that your DAW's settings may affect the folder it searches for VST3 plugins. Ensure that you check the configured search path within the DAW.

#### VST Plugin Issues
While the VST3 plugin can be run using a DAW host, there are some issues with it in its current form:
- Audio is output from the VST directly to the systems audio device, and not passed through the DAW
	- This issue is believed to be caused by the use of the Faust generated DSP code, which handles the output of audio
	- Resampling the output audio can act as a work around to this issue
- There are stability issues when using external MIDI input from the DAW into the plugin, resulting in crashes

<div style="page-break-after: always;"></div> 

# Using The Application/Plugin
In this section the usage of the application will be explained in detail.

### User Interface
A guide to the user interface is shown in _Figure 1_.

![ui diagrams drawio](https://github.com/luke1241/MUSS3640_Vocal_Synth/assets/95569413/2db11bb3-1144-4f72-be48-c0ce2fdf6661)

_Figure 1: Vocal synthesiser UI guide_

#### 1-Source Control
The `Bandpass Source` drop-down box at the top of the Source Control section allows the user to select from the three source waveshapes:
- Square wave
- Sawtooth wave
- Glottal pulse model

The sliders located below the drop-down box provide control over source-specific parameters. Grey sliders are deactivated for the currently selected source waveshape.

##### Square Wave
The Square source option selects a basic square wave, with pulse width modulation.

- `Pulsewidth` controls the pulse width of the square wave, ranging from 0.0 to 1.0.

##### Sawtooth Wave
The Sawtooth source option selects a basic sawtooth, or down-ramping saw wave. This waveshape has no controllable parameters.

##### Glottal Pulse Model
The Glottal Pulse source option selects the glottal pulse model as the source waveshape. This waveshape is an implementation of the Rosenberg glottal model.

- `Pressure` controls the range of the time value input into the model, from 0.0 to 1.0.
	- This varies the proportion of the wave which is output (_Figure 2_)
- `T0` controls the $T_0$ parameter of the model.
	- $T_0$ defines the glottal time period, or the period of the oscillation.
- `Te` controls the $T_e$ parameter of the model.
	- $T_e$ defines the glottal closed time, or the period of the oscillation in which the glottis is fully closed.
- `Noise` controls the level of white noise added to the time value of the model
	- This varies the harmonic content of the output, by adding random variation to the time value. More variation is added to areas of the wave with a higher change in amplitude (_Figure 3_).


![gpm pressure drawio](https://github.com/luke1241/MUSS3640_Vocal_Synth/assets/95569413/d2984bab-f369-4b2a-a5fe-500b8ce9e56b)


_Figure 2: Effect of glottal pulse model `Pressure` control_

![gpm noise drawio (1)](https://github.com/luke1241/MUSS3640_Vocal_Synth/assets/95569413/89c4167e-8de3-438d-83f8-d372c5c25037)

_Figure 3: Effect of glottal pulse model `Noise` control_

##### Fricative Source
In the Fricative Source area the user can set the high and low cut-off frequencies of the fricative bandpass filter.

#### 2-Formant Parameters
The Formant Parameters area contains a table of number boxes displaying each formant's frequency, bandwidth, and gain. The columns represent formants 1 through 5. Rows 1, 2, and 3 display the formant frequency, bandwidth, and gain respectively.

These number boxes can be used to quickly set formant parameters from within the plugin. It is worth noting that any values set in the table will be overwritten by values stored in the loaded XML file when the `Phoneme` slider is moved.

#### 3-Filter Control
The Filter Control area contains the controls for the filter stage of each synthesis model.

- `Load XML File` opens a file browser dialogue from which the user can select an XML file containing phoneme data (see [[#XML Preset Creation]]). The phonemes stored in the file will be loaded into the synthesiser, allowing the user to select between them using the `Phoneme` slider.
- `Use Skirt Width` toggles whether the FOF model makes use of the skirt width multiplier. 
  The skirt width multiplier is applied to the bandwidth of the formant to achieve a more realistic output sound. When deactivated, the output of the FOF model will be lower in amplitude, and have far more pronounced resonances at the frequency of each formant.
  It makes use of a look-up table of multiplication values corresponding to each of the five vowel sounds. As a result, this parameter may cause inaccuracies in the model if used in conjunction with an XML file containing additional phonemes.
  
	- `Use Skirt Width` enabled: The skirt width multiplication will be performed and the result will be used as the FOF skirt width of each formant.
	- `Use Skirt Width` disabled: The skirt width multiplication will be bypassed, and the bandwidth of each formant will be used as the FOF skirt width.
- `Formant Shift` shifts the frequency of each formant by the same amount.
	- This allows loaded phonemes to be better tuned to certain vocal ranges.
- `Phoneme` slider determines from which phoneme stored in the loaded XML file the data is taken.
	- The slider label will update to display the name of the currently selected phoneme. Transitions between phonemes are displayed using an arrow.

#### 5-Vibrato
The Vibrato area contains controls for the vibrato of the source frequency. The vibrato has a dedicated ASR envelope to control it's amplitude independently of the voiced sound. 
- `Freq` controls the frequency of the vibrato, ranging from 0.0Hz to 8.0Hz
	- This is the standard range of vibrato frequency found in the human voice.
- `A` controls the attack time of the vibrato envelope.
- `S` controls the sustain level of the vibrato envelope, and doubles as a gain setting.
- `R` controls the release time of the vibrato envelope. For best results set the release of the vibrato to at least that of the voiced envelope.

#### 6-Voiced Envelope
The Voiced Envelope consists of a standard ADSR envelope, as found in many synthesisers. 
- `A` controls the attack time of the envelope.
- `D` controls the decay time of the envelope.
- `S` controls the sustain level of the envelope.
- `R` controls the release time of the envelope.

#### 7-Fricative Envelope
As with the Voiced Envelope, the Fricative Envelope is implemented as an ADSR envelope.
- `A` controls the attack time of the envelope.
- `D` controls the decay time of the envelope.
- `S` controls the sustain level of the envelope.
- `R` controls the release time of the envelope.

#### 8-Mixer
The Mixer area contains gain controls for each synthesis model and the fricative noise output.
- `FOF Gain` sets the gain of the FOF model's output signal
- `BP Gain` sets the gain of the bandpass model's output signal
- `Fricative Gain` sets the gain of the fricative noise output signal

Each of the gain parameters may be set by the currently loaded XML file. The checkboxes beside each gain slider may be used to prevent the data stored in the XML file updating the gain controls. 

<div style="page-break-after: always;"></div> 

### XML Preset Creation
Packaged with the Vocal Synthesiser are several "Voice Presets", containing phoneme data. These files are located in `/MUSS3640 Vocal Synthesiser/Voice Presets/` by default.
These files can be loaded into the synthesiser using the `Load XML File` button. It is possible for the user to create their own XML files. This section will give details on this process.

#### XML File Structure
_Figure 4_ shows an excerpt from the XML file template provided with the plugin, demonstrating the phoneme data structure.

All phonemes in the file must be contained withing the `<PHONEME_DATA>` tag. The name attribute may be changed to the users liking.

The `<_PHONEME_>` tag contains the data for a single phoneme. This may be duplicated as many times as necessary to add phonemes to the file. The `<_PHONEME_>` tag name (`_PHONEME_`) can be changed to set the phoneme's display name within the plugin. It is essential that the name of the closing tag `</_PHONEME_>` matches that of the opening tag.

Elements for each formant, synthesis model gains, and the fricative parameters are defined as child elements of the phoneme. It is important that the names of these elements (`F1`, `F2`, etc) are not changed, as they are used by the application to parse the XML file. 

```xml
<PHONEME_DATA Name="Phoneme Preset">
  <_Phoneme_>
    <F1 Freq="600" Bandwidth="60" Gain="1.0" />
    <F2 Freq="1040" Bandwidth="70" Gain="0.1995" />
    <F3 Freq="2250" Bandwidth="110" Gain="0.1259" />
    <F4 Freq="2450" Bandwidth="120" Gain="0.1259" />
    <F5 Freq="2750" Bandwidth="130" Gain="0.01" />
    <VOICED FOF_gain="1.0" BP_gain="1.0">
    <FRICATIVE Gain="0" Lowcut="20.0" Highcut="20000.0" Attack="0.1" Decay="0.5" Sustain="0.5" Release="1.0" />
  </_Phoneme_>
</PHONEME_DATA>
```
_Figure 4: Excerpt from `templatePhonemeFile.xml`_

While it is possible to create phoneme preset files using a basic text editor or IDE, it is recommended to use a dedicated XML file editor, such as XML Notepad, to edit the files. This will help prevent formatting errors in the file.

XML notepad can be downloaded here:

	https://microsoft.github.io/XmlNotepad/#install/


<div style="page-break-after: always;"></div> 

### Praat Script
The `get_formants_from_audio.praat` script located in the `/MUSS3640 Vocal Synthesiser/Source/` folder can be used to extract formant data from recorded audio of a human voice.

The script is able to process multiple `.wav` files at once, outputting a `.csv` file for each audio file.

#### Running The Script
In order to run the script, Praat must be installed on the system. Praat can be downloaded here:

	https://www.fon.hum.uva.nl/praat/

##### Running From Command Line
To run the script from the command line, set the command line directory to the folder containing the audio files you wish to analyse. Run the following command (Windows):

	your/Praat/installation/folder/Praat.exe --run script/location/get_formants_from_audio.praat [output style]

Where `[output style]` =
- `Full` : outputs all data samples, last row is column average
- `Averages` : outputs just the average values

The output `.csv` files will be created in the directory containing the audio files.

##### Running Within Praat
Open Praat and navigate to

	Praat -> Open Praat Script

Select the `get_formants_from_audio.praat` as the script to open. Run the script (CTRL+R).

When prompted, select the output style:
- `Full` : outputs all data samples, last row is column average
- `Averages` : outputs just the average values

From the file browser dialogue box, select the folder containing the audio files you wish to analyse.

The output `.csv` files will be created in the directory containing the audio samples.

#### Using The Output Data
The script creates a `.csv` file for each audio file it processes. The `.csv` file contains the following data columns:
- `time(s)` : time of sample
- `intensity` : overall intensity of voicing
- `nformants` : number of formants detected
- `Fn(Hz)` : $n^{th}$ formant frequency
- `In` : $n^{th}$ formant intensity
- `Bn(Hz)` : $n^{th}$ formant bandwidth
- `Pitch` : fundamental frequency of voicing

This data may be input into the synthesiser using either the [[#2-Formant Parameters|Formant Parameters]] area, or stored in a custom preset file.

The Praat analysis produces very low intensity values. As a result, it is recommended that formant gains are copied from one of the voice presets.

<div style="page-break-after: always;"></div> 
<div style="page-break-after: always;"></div> 

## Source Code Overview
The source code is located in the `/MUSS3640 Vocal Synthesiser/Source/` folder.
- `FormantSynth.dsp` : The Faust DSP code
- `get_formants_from_audio.praat` : The Praat script for audio analysis

The `JUCE` subfolder contains the source code for the application/plugin:
- `DspFaust.cpp` : Class file containing the exported Faust code
- `DspFaust.h` : DspFaust header file
- `ExternalMidi.h` : Classes used for handling external MIDI input into the plugin
- `Phoneme.cpp` : Class used for the storage of phoneme data in the plugin
- `Phoneme.h` : Phoneme class header file
- `PluginEditor.cpp` : Class for handling the plugin GUI
- `PluginEditor.h` : Plugin GUI header file
- `PluginProcessor.cpp` : Class for handling the plugin audio processing
- `PluginProcessor.h` : Plugin audio processing header file
