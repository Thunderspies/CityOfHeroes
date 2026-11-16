#########################################################
##	template

FxInfo

Lifespan 32
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
		ENAME	Spinnode
		Type	Posit
		At	Head
		Bhvr	:SpinNode02.bhvr
	End

	Event	
		Type	Local
		At	Spinnode
		part	:Star01A.part
		part	:StarGlow01A.part
		part	:Triangle01A.part
		Bhvr	:Node01.bhvr
	End

#	Event	
#		Type	Local
#		At	Spinnode
#		part	:Star01A.part
#		part	:Triangle01A.part
#		Bhvr	:Node02.bhvr
#	End
#
#	Event	
#		Type	Local
#		At	Spinnode
#		part	:Star01A.part
#		part	:Triangle01A.part
#		Bhvr	:Node03.bhvr
#	End
#
#	Event	
#		Type	Local
#		At	Spinnode
#		part	:Star01A.part
#		part	:Triangle01A.part
#		Bhvr	:Node04.bhvr
#	End
#
End



End		