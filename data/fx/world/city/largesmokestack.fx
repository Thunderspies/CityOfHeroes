#########################################################
##	BuildingRuinSmolder
##
FxInfo
	
#LifeSpan 30000

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
		Part1	LargeSmolder.part 
		Part2	LargeSmolder2.part
		Sound smoke2_loop 300 300 .85
		#bhvr	Behaviors/genericParticleFade.bhvr
	End
End

End			