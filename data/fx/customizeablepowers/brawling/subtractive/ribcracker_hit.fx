#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	3
	Event
		Type	Local
		At	Root
		Sound 	Brawling_RibCracker_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 -0.5 1.0
		End
		Part	:Head_Shockwave_Small.part
		Part	:Head_Spikes.part
		Part	:Hit_Streaks_Small.part
		LifeSpan 2
	End
End

#############################################################

End