#########################################################
##	tech_directional_bubbles
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
		Part1	world/Lairs/tech/tech_bubbles.part
		##Part2	world/City/DirectionalSteamSM2.part
	End
End


End			