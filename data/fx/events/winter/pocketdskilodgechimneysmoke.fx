#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	Root

		Part1	:SkiLodge_ChimneySmoke1.part
		Sound smoke2_loop 200 200 .33
	End
End

Condition
	On 	Time
	Time 	160

	Event
		EName 	Prime
		Type	Start 
		At	Root
		BhvrOverride
			PositionOffset		0 60 0
		End

		Part2	:SkiLodge_ChimneySmoke2.part
	End
End

End			