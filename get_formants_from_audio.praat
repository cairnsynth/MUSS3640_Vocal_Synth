
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


clearinfo  
# Clear praat output window

inDir$ = chooseDirectory$: "Select folder of .wav files"
inDirWav$ = inDir$ + "/*.wav" 
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

appendInfoLine: "Working directory: ", inDir$  
# Print directory

number_of_formants = 5  
# Number of formants to analyse for

for fileNum from 1 to numFiles  
# Loop through files

	
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

	for f to number_of_formants  
	# Add intensity columns to table
		index = Get column index: "F" + string$(f) + "(Hz)"
		Insert column: index + 1, "I" + string$(f)
	endfor

	for row to nRows  
	# Loop through rows in table
		select table
		time = Object_'table'[row, "time(s)"]

		for f to number_of_formants  
		# Loop through formants, add freq
			frequency = Object_'table'[row, "F" + string$(f) + "(Hz)"]

			select spectrogram  
			# Get intensity from spectrogram
			if frequency != undefined
				intensity = Get power at: time, frequency
			else
				intensity = undefined
			endif
			select table  
			# Add intensity to table
			Set string value: row, "I" + string$(f), fixed$(intensity, 3)
		endfor

		select pitch  
		# Get fundamental freq. at each time interval
		pitchValue = Get value at time: time, "Hertz", "Nearest"
		select table  
		# Add pitch to table
		Set string value: row, "Pitch", fixed$(pitchValue, 3)
	endfor

	select table  
	# Variables to store no. rows, columns
	nRows = Get number of rows
	nColumns = Get number of columns

	
	# Create table to hold average values
	avgTable = Create Table with column names: "table", 1, "Averages:"
	
	for column to nColumns  
	# Loop through columns
		select table
		total_val = 0.0
		val_count = 1
		column_label$ = Get column label... column
		for row to nRows  
		# Loop through rows
			current_val = Object_'table'[row, column_label$]  
			# Get value
			if current_val != undefined
				total_val = total_val + current_val  
				# Add up values
				val_count = val_count + 1
			endif
		
		endfor

		average_val = total_val / val_count  
		# Calculate average of values
		Set string value: nRows, column_label$, fixed$(average_val, 3)
		appendInfoLine: column_label$, " Average: ", average_val
		
		select avgTable  
		# Insert average value to average table
		Append column: column_label$
		Set string value: 1, column_label$, fixed$(average_val, 3)

	endfor

	outFile$ = sound_name$ + ".csv"  
	# Create output file name
	if output_style$ == "Full"  
	# If user selected "Full"
		select table  
		# Output = table
		
	else  
	# If user selected "Average"
		select avgTable  
		#  Output = average table
	endif

	Save as comma-separated file: inDir$ + "/" + outFile$  
	# Write file

	
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