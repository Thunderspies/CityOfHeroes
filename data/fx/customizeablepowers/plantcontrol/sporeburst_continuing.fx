#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	Head
End

########################################################

#LifeSpan 30

Condition
	On	Time
	Time	0

	Event
		
		Type	Posit
		At	Head
		
		part2	CustomizeablePowers\PlantControl\ChockingSporesHitDots.part
		
		part4	CustomizeablePowers\PlantControl\ChockingSporesHit.part
		part5	CustomizeablePowers\PlantControl\ChockingSporesHit2.part
		PMagnet	Head
		Sound mushroomhit3_loop 40 40 .3
		
	End


End

End				