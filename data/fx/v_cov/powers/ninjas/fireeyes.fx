#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################




#########################################################
#################### HEAD GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	EyeNodeL
		Type	Local
		At	Head
		Part1	:OniEyes_Green.part
		Part2	:OniEyes_Yellow.part
		part	:FireEyesLeft01.part
		part	:FireEyesLeft01.part
		Bhvr	:EyeNodeL.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	Head
		
		Part1	:OniEyes_Green.part
		Part2	:OniEyes_Yellow.part
		part	:FireEyesRight01.part
		part	:FireEyesRight01.part
		Bhvr	:EyeNodeR.bhvr
	End

End

End		