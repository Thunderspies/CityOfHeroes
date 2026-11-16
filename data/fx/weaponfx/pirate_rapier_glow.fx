#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	WepR
End

Lifespan 250


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		BHVR	:WepROffsetUp.bhvr
		Part	:RapierGlow01.part
				
	End
End	


End
