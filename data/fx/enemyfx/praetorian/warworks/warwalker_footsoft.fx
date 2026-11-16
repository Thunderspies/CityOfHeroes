#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 2

#############################################################
Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FF_01 200 200 .35
		Sound WW_FF_02 200 200 .35
		Sound WW_FF_03 200 200 .35
		Sound WW_FF_04 200 200 .35
		Sound WW_FF_05 200 200 .35
		Sound WW_FF_06 200 200 .35
		Sound WW_FF_07 200 200 .35
	End

End

# Condition
	# On 	Time
	# Time 	0

	# Event
		# Type	Local
		# At	Root
		# SoundNoRepeat 3
		# Sound WW_FFClose_01 75 75 .9
		# Sound WW_FFClose_02 75 75 .9
		# Sound WW_FFClose_03 75 75 .9
		# Sound WW_FFClose_04 75 75 .9
		# Sound WW_FFClose_05 75 75 .9
		# Sound WW_FFClose_06 75 75 .9
		# Sound WW_FFClose_07 75 75 .9
	# End

# End
