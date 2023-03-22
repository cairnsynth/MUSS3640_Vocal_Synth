clearinfo

inDir$ = chooseDirectory$: "Select folder of .wav files"
inDirWav$ = inDir$ + "\*.wav"
wavList = Create Strings as file list: "wavList", inDirWav$
numFiles = Get number of strings
if numFiles == 0
	exitScript: "No .wav files found in selected folder"
endif

form Output Style
	choice Output_style 2
		button Averages
		button Full
endform

writeInfoLine: output_style$

appendInfoLine: "Working directory: ", inDir$

number_of_formants = 5

for fileNum from 1 to numFiles

	selectObject: wavList
	wavFile$ = Get string: fileNum
	this_sound = Read from file: inDir$ + "\" + wavFile$
	Play
	sound_name$ = selected$("Sound")
	appendInfoLine: "Current File: ", sound_name$

	#Create pitch object
	pitch = To Pitch... 0.0 75 600
	pitchValue = Get mean... 0.0 0.0 Hertz
	appendInfoLine: "Pitch (F0): ", 'pitchValue'
	appendInfoLine: ""

	#Create spectrogram object
	select 'this_sound'
	spectrogram = To Spectrogram... 0.005 5000.0 0.002 20.0 Gaussian

	#Create formant object
	select 'this_sound'
	formant = To Formant (burg)... 0 number_of_formants 5000 0.025 50
	select formant

	table = Down to Table: "no", "yes", 6, "yes", 3, "yes", 3, "yes"
	Append column: "Pitch"
	nRows = Get number of rows

	for f to number_of_formants
		index = Get column index: "F" + string$(f) + "(Hz)"
		Insert column: index + 1, "I" + string$(f)
	endfor

	for row to nRows
		select table
		time = Object_'table'[row, "time(s)"]

		for f to number_of_formants
			frequency = Object_'table'[row, "F" + string$(f) + "(Hz)"]

			select spectrogram
			if frequency != undefined
				intensity = Get power at: time, frequency
			else
				intensity = undefined
			endif
			select table
			Set string value: row, "I" + string$(f), fixed$(intensity, 3)
		endfor

		select pitch
		pitchValue = Get value at time: time, "Hertz", "Nearest"
		select table
		Set string value: row, "Pitch", fixed$(pitchValue, 3)
	endfor

	select table
	#Append row
	#Append row
	nRows = Get number of rows
	nColumns = Get number of columns

	avgTable = Create Table with column names: "table", 1, "Averages:"
	
	for column to nColumns
		select table
		total_val = 0.0
		val_count = 1
		column_label$ = Get column label... column
		for row to nRows
			current_val = Object_'table'[row, column_label$]
			if current_val != undefined
				total_val = total_val + current_val
				val_count = val_count + 1
			endif
		
		endfor

		average_val = total_val / val_count
		Set string value: nRows, column_label$, fixed$(average_val, 3)
		appendInfoLine: column_label$, " Average: ", average_val
		
		select avgTable
		Append column: column_label$
		Set string value: 1, column_label$, fixed$(average_val, 3)

	endfor

	outFile$ = sound_name$ + ".csv"
	writeInfoLine: outFile$
	if output_style$ == "Full"
		select table
		
	else
		select avgTable
	endif

	Save as comma-separated file: inDir$ + "/" + outFile$

	select table
	Remove
	select avgTable
	Remove
	select formant
	Remove
	select pitch
	Remove
	select spectrogram
	Remove
	select Sound 'sound_name$'
	Remove

	

endfor

select wavList
Remove