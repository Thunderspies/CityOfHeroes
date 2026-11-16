#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Brawling_HeavyBlow_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	1

	Event
		Type	local
		At	Chest
		Part	:Head_Shockwave_Small.part
		Part	:Head_Spikes_Small.part
		Part	:Hit_Smoke_Small.part
		LifeSpan 2
	End
End
#############################################################

End