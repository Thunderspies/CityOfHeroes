#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 300

Condition
	
	Event
		Type	Local 
		At	root
		
		BHVR	Behaviors/EventFireFade.bhvr
		Part	:RoofFire_SmokeDeath.part
		
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
		At	root
		BHVR	Behaviors/EventFireFade.bhvr
		Part	:RoofFire_SmokeDeathWhite.part
		
		Sound torch1_loop 60 60 .6
		
		LifeSpan 300

	End
End

End			