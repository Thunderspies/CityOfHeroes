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
		ENAME	EyeNodeLOffset
		Type	Local
		At	Head
		#part	:EyeArcs01.part
		Bhvr	:EyeNodeOffsetL.bhvr
		POther	EyeNodeL
	End

	Event	
		ENAME	EyeNodeROffset
		Type	Local
		At	Head
		#part	:EyeArcs02.part
		Bhvr	:EyeNodeOffsetR.bhvr
		POther	EyeNodeR
	End
	
	Event	
		ENAME	EyeNodeL
		Type	Local
		At	Head
		#part	:EyeGlow01.part
		#part	:EyeGlow02.part
		Bhvr	:EyeNodeOffsetL.bhvr
	End

	Event	
		ENAME	EyeNodeR
		Type	Local
		At	Head
		#part	:EyeGlow01.part
		#part	:EyeGlow02.part
		Bhvr	:EyeNodeOffsetR.bhvr
	End

End

End		