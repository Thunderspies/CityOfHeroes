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
		part	:EyeGlow02_left.part
		part	:EyeGlow02.part
		Bhvr	:EyeNodeL.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	Head
		part	:EyeGlow01.part
		part	:EyeGlow02_right.part
		part	:EyeGlow02.part
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