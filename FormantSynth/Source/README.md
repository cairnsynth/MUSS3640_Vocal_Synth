# Faust API

This API allows to interact with a Faust object and its associated audio engine at a high level. The idea is that all the audio part of the app is implemented in Faust allowing developers to focus on the design of the application itself. 

### Application Set-Up

Import `DspFaust.h` and `DspFaust.cpp` in your project (this can be done simply by dragging these files in your project tree). Then, import `DspFaust.h` (`#include "DspFaust.h"`) in the file where you want to create/control the Faust object. 

### Using the C++ API

The current Faust API is designed to seamlessly integrate to the life cycle of an app. It is accessible through a single `DspFaust` object. The constructor of that object is used to set the sampling rate and the buffer size:

	DspFaust* dspFaust = new DspFaust(SR, BS);

The `start()` method is used to start the audio computing. Similarly, `stop()` can be called to stop the audio computing.

It is possible to interact with the different parameters of the Faust object by using the `setParamValue` method. Two versions of this method exist: one where the parameter can be selected by its address and one where it can be selected using its ID. The [Parameters List](#parameters-list) section gives a list of the addresses and corresponding IDs of the current Faust object.

If your Faust object is polyphonic (e.g. if you used the `-nvoices` option when generating this API), then you can use the MIDI polyphony methods like `keyOn`, `keyOff`, etc.

It is possible to change the parameters of polyphonic voices independently using the `setVoiceParamValue` method. This method takes as one of its arguments the address to the voice returned by `keyOn` or `newVoice` when it is called. E.g:

	uintptr_t voiceAddress = dspFaust->keyOn(70, 100);
	dspFaust->setVoiceParamValue(1, voiceAddress, 214);
	dspFaust->keyOff(70);
	
In the example above, a new note is created and its parameter ID 1 is modified. This note is then terminated. Note that parameters addresses (path) are different for independent voices than when using `setParamValue`. The list of these addresses is provided in a separate sub-section of the [Parameters List](#parameters-list) section.

Finally, note that new voices don't necessarily have to be created using `keyOn`. Indeed, you might choose to just use the `newVoice` method for that:

	uintptr_t voiceAddress = dspFaust->newVoice();
	dspFaust->setVoiceParamValue(1, voiceAddress, 214);
	dspFaust->deleteVoice(voiceAddress);

This is particularly useful when making applications where each finger of the user is an independent sound that doesn't necessarily has a pitch.

In case you would like to use the built-in accelerometer or gyroscope of your device to control some of the parameters of your Faust object, all you have to do is to send the raw accelerometer data to it by using the `propagateAcc` or `propagateGyr` for the gyroscope. After that, mappings can be configured directly from the Faust code using [this technique](#using-built-in-sensors-to-control-parameters) or using the `setAccConverter` and `setGyrConverter` method.

## Parameters List

### Main Parameters

* **0**: `/Polyphonic/Voices/Panic`
* **1**: `/Polyphonic/Voices/FormantSynth/f1BW`
* **2**: `/Polyphonic/Voices/FormantSynth/f1Freq`
* **3**: `/Polyphonic/Voices/FormantSynth/f1Gain`
* **4**: `/Polyphonic/Voices/FormantSynth/f2BW`
* **5**: `/Polyphonic/Voices/FormantSynth/f2Freq`
* **6**: `/Polyphonic/Voices/FormantSynth/f2Gain`
* **7**: `/Polyphonic/Voices/FormantSynth/f3BW`
* **8**: `/Polyphonic/Voices/FormantSynth/f3Freq`
* **9**: `/Polyphonic/Voices/FormantSynth/f3Gain`
* **10**: `/Polyphonic/Voices/FormantSynth/f4BW`
* **11**: `/Polyphonic/Voices/FormantSynth/f4Freq`
* **12**: `/Polyphonic/Voices/FormantSynth/f4Gain`
* **13**: `/Polyphonic/Voices/FormantSynth/f5BW`
* **14**: `/Polyphonic/Voices/FormantSynth/f5Freq`
* **15**: `/Polyphonic/Voices/FormantSynth/f5Gain`
* **16**: `/Polyphonic/Voices/FormantSynth/freq`
* **17**: `/Polyphonic/Voices/FormantSynth/gain`
* **18**: `/Polyphonic/Voices/FormantSynth/gate`
* **19**: `/Polyphonic/Voices/FormantSynth/voice/envelope/voiceAttack`
* **20**: `/Polyphonic/Voices/FormantSynth/voice/envelope/voiceDecay`
* **21**: `/Polyphonic/Voices/FormantSynth/voice/envelope/voiceRelease`
* **22**: `/Polyphonic/Voices/FormantSynth/voice/envelope/voiceSustain`
* **23**: `/Polyphonic/Voices/FormantSynth/voice/fricative/noiseAttack`
* **24**: `/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourHigh`
* **25**: `/Polyphonic/Voices/FormantSynth/voice/fricative/noiseColourLow`
* **26**: `/Polyphonic/Voices/FormantSynth/voice/fricative/noiseDecay`
* **27**: `/Polyphonic/Voices/FormantSynth/voice/fricative/noiseRelease`
* **28**: `/Polyphonic/Voices/FormantSynth/voice/fricative/noiseSustain`
* **29**: `/Polyphonic/Voices/FormantSynth/voice/mixer/bpGain`
* **30**: `/Polyphonic/Voices/FormantSynth/voice/mixer/fofGain`
* **31**: `/Polyphonic/Voices/FormantSynth/voice/mixer/fricativeGain`
* **32**: `/Polyphonic/Voices/FormantSynth/voice/source/Pressure`
* **33**: `/Polyphonic/Voices/FormantSynth/voice/source/T0`
* **34**: `/Polyphonic/Voices/FormantSynth/voice/source/Te`
* **35**: `/Polyphonic/Voices/FormantSynth/voice/source/bpSourcePW`
* **36**: `/Polyphonic/Voices/FormantSynth/voice/source/bpSourceSelect`
* **37**: `/Polyphonic/Voices/FormantSynth/voice/source/glideTime`
* **38**: `/Polyphonic/Voices/FormantSynth/voice/source/noise`
* **39**: `/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoAttack`
* **40**: `/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoFreq`
* **41**: `/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoRelease`
* **42**: `/Polyphonic/Voices/FormantSynth/voice/vibrato/vibratoSustain`

### Independent Voice

* **0**: `/FormantSynth/f1BW`
* **1**: `/FormantSynth/f1Freq`
* **2**: `/FormantSynth/f1Gain`
* **3**: `/FormantSynth/f2BW`
* **4**: `/FormantSynth/f2Freq`
* **5**: `/FormantSynth/f2Gain`
* **6**: `/FormantSynth/f3BW`
* **7**: `/FormantSynth/f3Freq`
* **8**: `/FormantSynth/f3Gain`
* **9**: `/FormantSynth/f4BW`
* **10**: `/FormantSynth/f4Freq`
* **11**: `/FormantSynth/f4Gain`
* **12**: `/FormantSynth/f5BW`
* **13**: `/FormantSynth/f5Freq`
* **14**: `/FormantSynth/f5Gain`
* **15**: `/FormantSynth/freq`
* **16**: `/FormantSynth/gain`
* **17**: `/FormantSynth/gate`
* **18**: `/FormantSynth/voice/envelope/voiceAttack`
* **19**: `/FormantSynth/voice/envelope/voiceDecay`
* **20**: `/FormantSynth/voice/envelope/voiceRelease`
* **21**: `/FormantSynth/voice/envelope/voiceSustain`
* **22**: `/FormantSynth/voice/fricative/noiseAttack`
* **23**: `/FormantSynth/voice/fricative/noiseColourHigh`
* **24**: `/FormantSynth/voice/fricative/noiseColourLow`
* **25**: `/FormantSynth/voice/fricative/noiseDecay`
* **26**: `/FormantSynth/voice/fricative/noiseRelease`
* **27**: `/FormantSynth/voice/fricative/noiseSustain`
* **28**: `/FormantSynth/voice/mixer/bpGain`
* **29**: `/FormantSynth/voice/mixer/fofGain`
* **30**: `/FormantSynth/voice/mixer/fricativeGain`
* **31**: `/FormantSynth/voice/source/Pressure`
* **32**: `/FormantSynth/voice/source/T0`
* **33**: `/FormantSynth/voice/source/Te`
* **34**: `/FormantSynth/voice/source/bpSourcePW`
* **35**: `/FormantSynth/voice/source/bpSourceSelect`
* **36**: `/FormantSynth/voice/source/glideTime`
* **37**: `/FormantSynth/voice/source/noise`
* **38**: `/FormantSynth/voice/vibrato/vibratoAttack`
* **39**: `/FormantSynth/voice/vibrato/vibratoFreq`
* **40**: `/FormantSynth/voice/vibrato/vibratoRelease`
* **41**: `/FormantSynth/voice/vibrato/vibratoSustain`

## API Reference

### `DspFaust(bool auto_connect = true)`
Default constructor for the static model, to be used wih audio drivers that impose their sample rate and buffer size (like JACK and JUCE).



#### Arguments



* `auto_connect`: whether to automatically connect audio outputs to the hardware (usable with JACK)


---


### `DspFaust(int SR, int BS, bool auto_connect = true)`
Constructor for the static model.



#### Arguments



* `SR`: sampling rate

* `BS`: buffer size

* `auto_connect`: whether to automatically connect audio outputs to the hardware (usable with JACK)


---


### `DspFaust(const string& dsp_content, int SR, int BS, bool auto_connect = true)`
Constructor for the dynamic model, using LLVM + JIT compilation.



#### Arguments



* `dsp_content`: the DSP as a file or string

* `SR`: sampling rate

* `BS`: buffer size

* `auto_connect`: whether to automatically connect audio outputs to the hardware (usable with JACK)


---


### `bool start()`
Start the audio processing.



Returns `true` if successful and `false` if not.


---


### `void stop()`
Stop the audio processing.


---


### `bool isRunning()`
Returns `true` if audio is running.


---


### `uintptr_t keyOn(int pitch, int velocity)`
Instantiate a new polyphonic voice. This method can

only be used if the `-nvoices` flag has been

provided before compilation.



`keyOn` will return 0 if the Faust object is not

polyphonic or the address to the allocated voice as

an `uintptr_t` otherwise. This value can be used later with

[`setVoiceParamValue`](#setvoiceparamvalue) or

[`getVoiceParamValue`](#getvoiceparamvalue) to access

the parameters of a specific voice.



#### Arguments



* `pitch`: MIDI note number (0-127)

* `velocity`: MIDI velocity (0-127)


---


### `int keyOff(int pitch)`
De-instantiate a polyphonic voice. This method can

only be used if the `-nvoices` flag has been

provided before compilation.



`keyOff` will return 0 if the object is not polyphonic

and 1 otherwise.



#### Arguments



* `pitch`: MIDI note number (0-127), should be the same

as the one used for `keyOn`


---


### `uintptr_t newVoice()`
Instantiate a new polyphonic voice. This method can

only be used if `-nvoices` flag has been

provided before compilation.



`newVoice` will return 0 if the Faust object is not

polyphonic or the address to the allocated voice as

a `uintptr_t` otherwise. This value can be used later with

`setVoiceParamValue`, `getVoiceParamValue` or

`deleteVoice` to access the parameters of a specific

voice.


---


### `int deleteVoice(uintptr_t voice)`
De-instantiate a polyphonic voice. This method can

only be used if `-nvoices` flag has been

provided before compilation.



`deleteVoice` will return 0 if the object is not polyphonic

and 1 otherwise.



#### Arguments



* `voice`: the address of the voice given by `newVoice` or `keyOn`


---


### `void allNotesOff(bool hard = false)`
Terminates all the active voices, gently (with release when hard = false or immediately when hard = true).


---


### `void propagateMidi(int count, double time, int type, int channel, int data1, int data2)`
Take a raw MIDI message and propagate it to the Faust

DSP object. This method can be used concurrently with

[`keyOn`](#keyOn) and [`keyOff`](#keyOff).



`propagateMidi` can only be used if `-nvoices` flag has been

provided before compilation.



#### Arguments



* `count`: size of the message (1-3)

* `time`: time stamp in usec

* `type`: message type (byte)

* `channel`: channel number

* `data1`: first data byte (should be `null` if `count<2`)

* `data2`: second data byte (should be `null` if `count<3`)


---


### `const char* getJSONUI()`
Returns the JSON description of the UI of the Faust object.


---


### `const char* getJSONMeta()`
Returns the JSON description of the metadata of the Faust object.


---


### `void buildUserInterface(UI* ui_interface)`
Calls the polyphonic or monophonic buildUserInterface with the ui_interface parameter.



#### Arguments



* `ui_interface`: an UI* object


---


### `int getParamsCount()`
Returns the number of parameters of the Faust object.


---


### `void setParamValue(const char* address, float value)`
Set the value of one of the parameters of the Faust

object in function of its address (label/shortname/path).



#### Arguments



* `address`: address (label/shortname/path) of the parameter

* `value`: value of the parameter


---


### `void setParamValue(int id, float value)`
Set the value of one of the parameters of the Faust

object in function of its id.



#### Arguments



* `id`: id of the parameter

* `value`: value of the parameter


---


### `float getParamValue(const char* address)`
Returns the value of a parameter in function of its

address (label/shortname/path).



#### Arguments



* `address`: address (label/shortname/path) of the parameter


---


### `float getParamValue(int id)`
Returns the value of a parameter in function of its id.



#### Arguments



* `id`: id of the parameter


---


### `void setVoiceParamValue(const char* address, uintptr_t voice, float value)`
Set the value of one of the parameters of the Faust

object in function of its address (label/shortname/path) for a specific voice.



#### Arguments



* `address`: address (label/shortname/path) of the parameter

* `voice`: address of the polyphonic voice (retrieved from `keyOn`)

* `value`: value of the parameter


---


### `void setVoiceParamValue(int id, uintptr_t voice, float value)`
Set the value of one of the parameters of the Faust

object in function of its id for a specific voice.



#### Arguments



* `id`: id of the parameter

* `voice`: address of the polyphonic voice (retrieved from `keyOn`)

* `value`: value of the parameter


---


### `float getVoiceParamValue(const char* address, uintptr_t voice)`
Returns the value of a parameter in function of its

address (label/shortname/path) for a specific voice.



#### Arguments



* `address`: address (label/shortname/path) of the parameter

* `voice`: address of the polyphonic voice (retrieved from `keyOn`)


---


### `float getVoiceParamValue(int id, uintptr_t voice)`
Returns the value of a parameter in function of its

id for a specific voice.



#### Arguments



* `id`: id of the parameter

* `voice`: address of the polyphonic voice (retrieved from `keyOn`)


---


### `const char* getParamAddress(int id)`
Returns the address (label/shortname/path) of a parameter in function of its ID.



#### Arguments



* `id`: id of the parameter


---


### `const char* getVoiceParamAddress(int id, uintptr_t voice)`
Returns the address (label/shortname/path) of a parameter in function of its ID.



#### Arguments



* `id`: id of the parameter

* `voice`: address of the polyphonic voice (retrieved from `keyOn`)


---


### `float getParamMin(const char* address)`
Returns the minimum value of a parameter in function of

its address (label/shortname/path).



#### Arguments



* `address`: address (label/shortname/path) of the parameter


---


### `float getParamMin(int id)`
Returns the minimum value of a parameter in function

of its ID.



#### Arguments



* `id`: id of the parameter


---


### `float getParamMax(const char* address)`
Returns the maximum value of a parameter in function of

its address (label/shortname/path).



#### Arguments



* `address`: address (label/shortname/path) of the parameter


---


### `float getParamMax(int id)`
Returns the maximum value of a parameter in function

of its ID.



#### Arguments



* `id`: id of the parameter


---


### `float getParamInit(const char* address)`
Returns the default value of a parameter in function of

its address (label/shortname/path).



#### Arguments



* `address`: address (label/shortname/path) of the parameter


---


### `float getParamInit(int id)`
Returns the default value of a parameter in function

of its ID.



#### Arguments



* `id`: id of the parameter


---


### `const char* getMetadata(const char* address, const char* key)`
Returns the metadataof a parameter in function of

its address (label/shortname/path) and the metadata key.



#### Arguments



* `address`: address (label/shortname/path) of the parameter

* `key`: the metadata key


---


### `const char* getMetadata(int id, const char* key)`
Returns the metadataof a parameter in function of

its iD and the metadata key.



#### Arguments



* `id`: id of the parameter

* `key`: the metadata key


---


### `void propagateAcc(int acc, float v)`
Propagate the RAW value of a specific accelerometer

axis to the Faust object.



#### Arguments



* `acc`: the accelerometer axis (**0**: x, **1**: y, **2**: z)

* `v`: the RAW accelerometer value in m/s


---


### `void setAccConverter(int id, int acc, int curve, float amin, float amid, float amax)`
Set the conversion curve for the accelerometer.



#### Arguments



* `id`: the UI parameter id

* `acc`: the accelerometer axis (**0**: x, **1**: y, **2**: z)

* `curve`: the curve (**0**: up, **1**: down, **2**: up and down, **2**: down and up)

* `amin`: mapping min point

* `amid`: mapping middle point

* `amax`: mapping max point


---


### `void propagateGyr(int gyr, float v)`
Propagate the RAW value of a specific gyroscope

axis to the Faust object.



#### Arguments



* `gyr`: the gyroscope axis (**0**: x, **1**: y, **2**: z)

* `v`: the RAW gyroscope value in m/s


---


### `void setGyrConverter(int id, int gyr, int curve, float amin, float amid, float amax)`
Set the conversion curve for the gyroscope.



#### Arguments



* `id`: the UI parameter id

* `gyr`: the gyroscope axis (**0**: x, **1**: y, **2**: z)

* `curve`: the curve (**0**: up, **1**: down, **2**: up and down, **2**: down and up)

* `amin`: mapping min point

* `amid`: mapping middle point

* `amax`: mapping max point


---


### `float getCPULoad()`
Returns the CPU load (between 0 and 1.0).


---


### `void configureOSC(int xmit, int inport, int outport, int errport, const char* address)`
Change the OSC configuration.



#### Arguments



* `xmit`: the xmit state (off-0 all-1 alias-2)

* `inport`: the input port number

* `outport`: the output port number

* `errport`: the error port number

* `address`: the destination IP address


---


### `bool isOSCOn()`
Return OSC Status.


---


### `int getScreenColor()`
Get the requested screen color.

-1 means no screen color control (no screencolor metadata found)

otherwise return 0x00RRGGBB a ready to use color


---

