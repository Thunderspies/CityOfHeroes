#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 300



Condition
	
	Event
		Type	Local 
		At	chest
		
		BHVR	Behaviors/EventFireFade.bhvr
		Part	:Window_SmokeLarge.part
		
		Sound torch1_loop 60 60 .6
		
		LifeSpan 100
		
	End
End

Condition
	On 	TriggerBits
	TriggerBits	Death

	Event
		EName 	Prime
		Type	Local 
		At	chest
		BHVR	:OffsetDown.bhvr
		Part	:Window_SmokeLargeWhite.part
		
		LifeSpan 300
	End
End

End			