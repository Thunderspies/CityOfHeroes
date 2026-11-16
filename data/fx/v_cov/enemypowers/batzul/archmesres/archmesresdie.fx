#########################################################
##	template

FxInfo

Lifespan 34
#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	Spinnode2
		Type	Posit
		At	Root
		Bhvr	:SpinNode03.bhvr
		Lifespan 32
	End

	Event	
		Type	Local
		At	Spinnode2
		part	:Star01D.part
		part	:StarGlow01A.part
		part	:Triangle01D.part
		part	:Triangle02D.part
		Bhvr	:Node01.bhvr
		Lifespan 32
	End

End



End		