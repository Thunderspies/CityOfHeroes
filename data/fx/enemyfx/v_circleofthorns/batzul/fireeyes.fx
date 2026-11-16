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
		part	:EyeGlow01.part
		part	:EyeGlow02.part
		part	:FireEyesLeft01.part
		part	:FireEyesLeft01.part
		Bhvr	:EyeNodeL.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	Head
		part	:EyeGlow01.part
		part	:EyeGlow02.part
		part	:FireEyesRight01.part
		part	:FireEyesRight01.part
		Bhvr	:EyeNodeR.bhvr
	End

End

End		