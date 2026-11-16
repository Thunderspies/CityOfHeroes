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
		Sound 	Brawling_InitialStrike_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	local
		At	Chest
		Part	:Head_Shockwave_Tiny.part
		Part	:Head_Spikes_Small.part
		LifeSpan 2
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	local
		At	Chest
		Part	:Head_Shockwave_Tiny.part
		Part	:Head_Spikes_Small.part
		LifeSpan 2
	End
End

#############################################################

End