###########################################################

FxInfo
Lifespan 70

###########################################################

Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	Dog_TerrifyingRoar_01 100 100 .8
	End
End


Condition
	On Time
	Time 20

	Event
		At Root
		Type Local
		BhvrOverride
			PositionOffset	0.0 4.0 0.75
		End
		Part	:Howl_Wind_Streak.part
		Part 	:Howl_Wind_Vertical.part
		LifeSpan 65
	End

	Event
		At Root
		Type Local
		BhvrOverride
			PositionOffset	0.0 4.0 0.75
		End
		Part	:Howl_Rings_Vertical.part
		LifeSpan 40
	End
End

Condition
	On	Time
	Time	25

	Event
		At Root
		Type Local
		BhvrOverride
			PositionOffset	0.0 10.0 0.0
		End
		Part	:Howl_Ring.part
		Part 	:Howl_Wind_Shockwave.part
		Part 	:Howl_Wind_Shockwave_Core.part
		LifeSpan 45
	End
End


###########################################################

End