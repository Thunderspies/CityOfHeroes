#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 300



Condition
	
	Event
		Type	Local 
		At	Root
		
		BHVR	Behaviors/EventFireFade.bhvr
		Part	:Opposite90Window_SmokeLarge.part
		
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
		At	Root
		#BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:Opposite90Window_SmokeLargeWhite.part
		
		LifeSpan 300
	End
End

End			