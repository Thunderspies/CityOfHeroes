#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	50

###########################################################

Condition
	On	Time
	Time	17
	
	Event
		
		Type	local
		At	wepR
	
		Bhvr	Behaviors/Offset90.bhvr
		Part	:FetteringNimbusRings.part
		Sound fetteringnimbus 70 70 .8
		LifeSpan		12
		
	End
	
End

End			