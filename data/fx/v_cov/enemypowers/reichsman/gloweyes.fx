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
		ENAME	EyeL
		Type	Local
		At	Head
		part	:EyeGlow01.part
		part	:EyeGlow02.part
		Bhvr	:EyeNodeL.bhvr
	End

	Event
		ENAME	EyeR
		Type	Local
		At	Head
		part	:EyeGlow01.part
		part	:EyeGlow02.part
		Bhvr	:EyeNodeR.bhvr
	End

End

End