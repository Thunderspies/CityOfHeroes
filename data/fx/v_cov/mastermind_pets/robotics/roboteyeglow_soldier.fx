#########################################################
##	
##
FxInfo
	

Flags    InheritAlpha
###########################################################


Condition
	On 	Time
	Time 	0
	
	Event
		EName 	EyeGlow
		Type	local
		At	Head
		Part	:EyeGlow02.part
		Bhvr	:Soldier_EyeGlowOffset_01.bhvr
		Sound robobeep_loop 12 12 .22

	End

	Event
		EName 	EyeGlow
		Type	local
		At	Head
		Part	:EyeGlow01.part
		Bhvr	:Soldier_EyeGlowOffset_02.bhvr

	End

	Event
		EName 	EyeGlow
		Type	local
		At	Head
		Part	:EyeGlow01.part
		Bhvr	:Soldier_EyeGlowOffset_03.bhvr

	End


	Event
		EName 	EyeGlow
		Type	local
		At	Head
		Part	:EyeGlow01.part
		Bhvr	:Soldier_EyeGlowOffset_04.bhvr

	End

		
End


End			