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
		part	:Inner_EyeGlow.part
		Bhvr	:EyeNodeL.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	Head
		part	:EyeGlow01.part
		part	:EyeGlow02.part
		part	:Inner_EyeGlow.part
		Bhvr	:EyeNodeR.bhvr
	End

	Event	
		ENAME	EyeNodeL
		Type	Local
		At	Eyes
		#part	:EyeGlow02.part
	End

End

End		