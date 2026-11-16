#############################################################
## HeavyMallet_Hit.fx
#############################################################

FxInfo

Lifespan 140

## FALLBACK INPUTS ###########################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	chest
		Part	CustomizeablePowers\StoneMelee\StoneHandsDustHit.part
		Part	CustomizeablePowers\StoneMelee\StoneHandsDustHit2.part
		bhvr	Behaviors\CameraShake02Subtle.bhvr
		Sound stonehit2 100 100 0.9
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	15

	Event
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Lava\Child_RockDebrisDarkScatter.fx
		Lifespan 100
	End


	Event
		HardwareOnly
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Lava\Child_RockDebrisDarkScatter.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	2
	RepeatJitter	1

	Event
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Lava\Child_RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter	15
	End

	Event
		HardwareOnly
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Lava\Child_RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter	15
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	5

	Event
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\StoneMelee\Lava\Child_RockDebrisGoldExplode.fx
		Lifespan 100
	End


	Event
		HardwareOnly
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\StoneMelee\Lava\Child_RockDebrisGoldExplode.fx
		Lifespan 100
	End
End

#############################################################

End