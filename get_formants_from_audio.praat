#
# Praat script to enable the extraction of formant data from given .wav        
# files                                                                        
#-------------------------------------------------------------------------------
# To run from command line:                                                    
# your/Praat/installation/folder/Praat.exe --run script/location/              
#   get_formants_from_audio.praat [output style]                               
#                                                                              
# output style =                                                               
# "Averages" : outputs just the average values                                 
# "Full"     : outputs all data samples, last row is column average            
#-------------------------------------------------------------------------------
# References:
# [1] NCSU Phonetics Lab, get_formants.praat, 
#     <https://phon.wordpress.ncsu.edu/lab-manual/scripts/praat-scripting/#:~:text=get_formants.praat%20follows%20a%20typical%20structure%20for%20a%20Praat,screen%20them%206%20make%20the%20measurements%20More%20items>
#     [accessed 22/03/2023]
# [2] Joey Stanley, A Tutorial on Extracting Formants in Praat,
#     <https://joeystanley.com/blog/a-tutorial-on-extracting-formants-in-praat#:~:text=A%20Tutorial%20on%20Extracting%20Formants%20in%20Praat%201,8%20Determining%20the%20extraction%20time%20...%20More%20items>
#     [accessed 22/03/2023]
# [3] Eryk Walczak, Extracting pitch track from audio files into a data frame,
#     <https://walczak.org/tag/cli/#:~:text=To%20invoke%20the%20R%20script%2C%20run%20in%20the,best%20candidate%20pitch%20above%20a%20certain%20confidence%20threshold.>
#     [accessed 22/03/2023]
# [4] Praat Scripting Tutorial, Dealing with file names programmatically,
#     <https://praatscriptingtutorial.com/filePaths>
#     [accessed 22/03/2023]
# [5] Wouter van Nifterick, Praat: Get formant intensity (Stack Overflow),
#     <https://stackoverflow.com/questions/40025339/praat-get-formant-intensity>
#     [accessed 22/03/2023]
# [6] Jörg Mayer, Simple Input Forms,
#     <https://praatscripting.lingphon.net/simpleinput-3.html>
#     [accessed 22/03/2023]
#

# Clear praat output window
clearinfo  
# Open file browser dialog
inDir$ = chooseDirectory$: "Select folder of .wav files"
# Get all .wav files in directory as string list
inDirWav$ = inDir$ + "/*.wav" 
wavList = Create Strings as file list: "wavList", inDirWav$
numFiles = Get number of strings
# Error if no .wav files
if numFiles == 0
	exitScript: "No .wav files found in selected folder"
endif
# Form popup for output style
form Output Style 
	choice Output_style 2
		button Averages
		button Full
endform

# Print directory
appendInfoLine: "Working directory: ", inDir$  
# Number of formants to analyse for
number_of_formants = 5  
# Loop through files
for fileNum from 1 to numFiles  	
	# Create sound object of file
	selectObject: wavList  
	wavFile$ = Get string: fileNum
	this_sound = Read from file: inDir$ + "/" + wavFile$
	Play
	sound_name$ = selected$("Sound")
	appendInfoLine: "Current File: ", sound_name$

	# Create pitch object
	pitch = To Pitch... 0.0 75 600 
	pitchValue = Get mean... 0.0 0.0 Hertz
	appendInfoLine: "Pitch (F0): ", 'pitchValue'
	appendInfoLine: ""
	
	# Create spectrogram object
	select 'this_sound'
	spectrogram = To Spectrogram... 0.005 5000.0 0.002 20.0 Gaussian
	
	# Create formant object
	select 'this_sound'
	formant = To Formant (burg)... 0 number_of_formants 5000 0.025 50
	select formant
	
	# Create table of formant data
	table = Down to Table: "no", "yes", 6, "yes", 3, "yes", 3, "yes"
	Append column: "Pitch"
	nRows = Get number of rows

	# Add intensity columns to table
	for f to number_of_formants  
		index = Get column index: "F" + string$(f) + "(Hz)"
		Insert column: index + 1, "I" + string$(f)
	endfor
	# Loop through rows in table
	for row to nRows  
		select table
		time = Object_'table'[row, "time(s)"]
		# Loop through formants, add freq
		for f to number_of_formants  
			frequency = Object_'table'[row, "F" + string$(f) + "(Hz)"]
			# Get intensity from spectrogram
			select spectrogram  
			if frequency != undefined
				intensity = Get power at: time, frequency
			else
				intensity = undefined
			endif
			# Add intensity to table
			select table  
			Set string value: row, "I" + string$(f), fixed$(intensity, 3)
		endfor
		# Get fundamental freq. at each time interval
		select pitch  
		pitchValue = Get value at time: time, "Hertz", "Nearest"
		# Add pitch to table
		select table  
		Set string value: row, "Pitch", fixed$(pitchValue, 3)
	endfor
	# Variables to store no. rows, columns
	select table  
	nRows = Get number of rows
	nColumns = Get number of columns

	# Create table to hold average values
	avgTable = Create Table with column names: "table", 1, "Averages:"
	# Loop through columns	
	for column to nColumns  
		select table
		total_val = 0.0
		val_count = 1
		column_label$ = Get column label... column
		# Loop through rows
		for row to nRows 
			# Get value 
			current_val = Object_'table'[row, column_label$]  
			if current_val != undefined
				# Add up values
				total_val = total_val + current_val  
				val_count = val_count + 1
			endif
		
		endfor
		# Calculate average of values
		average_val = total_val / val_count  
		Set string value: nRows, column_label$, fixed$(average_val, 3)
		appendInfoLine: column_label$, " Average: ", average_val
		# Insert average value to average table		
		select avgTable  
		Append column: column_label$
		Set string value: 1, column_label$, fixed$(average_val, 3)

	endfor
	# Create output file name
	outFile$ = sound_name$ + ".csv" 
	# If user selected "Full" 
	if output_style$ == "Full"  
		# Output = table
		select table  
	# If user selected "Average"	
	else  
		#  Output = average table
		select avgTable  
	endif
	# Write file
	Save as comma-separated file: inDir$ + "/" + outFile$  	
	# Remove created objects
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
exitScript: "Formant extraction complete!"