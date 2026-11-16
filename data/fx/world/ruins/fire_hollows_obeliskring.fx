#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		BHVR	Behaviors/ObliskOffsetPivot.bhvr
		Part	:ObliskRingGlow.part
		Part	:ObliskRing.part
		
		
	End
	
	
End

End			