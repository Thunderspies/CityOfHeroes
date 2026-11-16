#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

Input  
	Inpname	HandL
End

Lifespan 200


########################################################

Condition
	On	Time
	Time	48

	Event	
		Type	Local
		At	origin
		Sound powermaul2 90 90 .8
	End

	Event
		EName 	Maul
		Type	local 
		At	WepR
		Geom	GEO_WepR_PowerMaul
		Lifespan 20
	End

	Event
		EName 	MaulTip
		Type	local 
		At	WepR
		Bhvr	:MaulHeadOffset3.bhvr
		Lifespan 20
	End

	Event
		Type	Local
		At	MaulTip
		Part	:MaulAttackGlow.Part
		POther Target	
		Lifespan 13
	End

	Event
		Type	Local
		At	MaulTip
		Part	:MaulStream01.part
		Part	:MaulStream02.part
		Part	:MaulStream03.part
		Part	:MaulStream04.part
		POther Target	
		Lifespan 1
	End	
	
	Event
		Type	Local
		At	Target
		Part	:MaulAttackGlow.Part
		Lifespan 9
	End

	Event
		Type	Local
		At	Target
		Part	:MaulTargetGlow.Part
		Lifespan 11
	End
	
End


End