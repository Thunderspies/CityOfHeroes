#############################################################
## HurlBoulder_Hit.fx
#############################################################

FxInfo

Lifespan 100

## FALLBACK NODES ###########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part	CustomizeablePowers\StoneMelee\StalagtiteDustHit.part
		Part	CustomizeablePowers\StoneMelee\StalagtiteDustHit2.part
		Sound stonehit2 100 100 0.85
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGold.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	19

	Event
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldScatter.fx
		Lifespan 100
	End

	Event
		HardwareOnly
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldExplode.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldExplode.fx
		Lifespan 100
	End

	Event
		HardwareOnly
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Child_RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter	15
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	3
	RepeatJitter	1

	Event
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Child_RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter	15
	End
End

#############################################################

End