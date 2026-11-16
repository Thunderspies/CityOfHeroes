#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	32
	
	Event	
		ENAME	Spinnode
		Type	Posit
		At	Head
		Bhvr	:SpinNode01.bhvr
	End

	Event	
		Type	Local
		At	Spinnode
		part	:Triangle01.part
		part	:Triangle02.part
		Bhvr	:Node01.bhvr
	End

	Event	
		Type	Local
		At	Spinnode
		part	:Triangle01.part
		part	:Triangle02.part
		Bhvr	:Node02.bhvr
	End

	Event	
		Type	Local
		At	Spinnode
		part	:Triangle01.part
		part	:Triangle02.part
		Bhvr	:Node03.bhvr
	End

	Event	
		Type	Local
		At	Spinnode
		part	:Triangle01.part
		part	:Triangle02.part
		Bhvr	:Node04.bhvr
	End

End

Condition
	On	Death
	
	Event	
		ENAME	Spinnode2
		Type	Posit
		At	Head
		Bhvr	:SpinNode02.bhvr
		Lifespan 32
	End

	Event	
		Type	Local
		At	Spinnode2
		part	:Triangle01D.part
		part	:Triangle02D.part
		Bhvr	:Node01.bhvr
		Lifespan 32
	End


End

End		