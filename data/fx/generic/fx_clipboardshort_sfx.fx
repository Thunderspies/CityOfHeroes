#########################################################
##	
########################################################
FxInfo
Lifespan 1

Input  
	Inpname	WepR
End



##################################################################

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	WepR 
		SoundNoRepeat 1
		Sound WritingShort_01 30 30 .1
		Sound WritingShort_02 30 30 .1
		Sound WritingShort_03 30 30 .1
		Sound WritingShort_04 30 30 .1

	End
End


