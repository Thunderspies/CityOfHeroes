#########################################################
##	This is the FX for the Horse (smoke & thrusters)
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Back
End

Input  
	Inpname	F1_L
End

Input  
	Inpname	F1_R
End

Input  
	Inpname	F2_L
End

Input  
	Inpname	F2_R
End

Input  
	Inpname	T1_L
End

Input  
	Inpname	T1_R
End

Input  
	Inpname	Target
End

Flags    InheritAlpha
########################################################

Condition
	On 	Time
	Time 	0

	
	
	Event
		EName 	NemesisSmoke1
		Type	local 
		At	Back
		Part1	:HorseTailGlow.part
		Part1	:HorseTailSmoke_01.part
				
	End
	
	
End

Condition
	On 	Time
	Time 	0

	
	
	Event
		EName 	Thruster_1
		Type	local 
		At	F1_L
		Part1	:HorseFlameGlow_01.part
		Part3	:HorseFlame_03.part
		Part4	:HorseFlame_04.part
		Part5	:HorseFlame_05.part
		Part1	:HorseSmoke_01.part		
	End

	Event
		EName 	Thruster_2
		Type	local 
		At	F1_R
		Part1	:HorseFlameGlow_01.part
		Part3	:HorseFlame_03.part
		Part4	:HorseFlame_04.part
		Part5	:HorseFlame_05.part
		Part1	:HorseSmoke_01.part		
	End
	
	Event
		EName 	Thruster_3
		Type	local 
		At	F2_L
		Part1	:HorseFlameGlow_01.part
		Part3	:HorseFlame_03.part
		Part4	:HorseFlame_04.part
		Part5	:HorseFlame_05.part
		Part1	:HorseSmoke_01.part	
	End

	Event
		EName 	Thruster_4
		Type	local 
		At	F2_R
		Part1	:HorseFlameGlow_01.part
		Part3	:HorseFlame_03.part
		Part4	:HorseFlame_04.part
		Part5	:HorseFlame_05.part
		Part1	:HorseSmoke_01.part		
	End
	


End

########################################################
#Snort Holes
########################################################

Condition
	On 	cycle
	Time	100
	Chance	1

	Event
		EName 	SnortR
		Type	Local 
		At	T1_R
		part1	:HorseSnortGlow.part
		part1	:HorseSnortSmokeRight_01.part
		part1	:HorseSnortSmokeRight_02.part
		
		lifespan	5
	End

	Event
		EName 	SnortL
		Type	Local
		At	T1_L
		part1	:HorseSnortGlow.part
		part1	:HorseSnortSmokeLeft_01.part
		part1	:HorseSnortSmokeLeft_02.part		
		lifespan	5
	End

End


End
################################
