#############################################################
## StoneFist_Hit.fx
#############################################################

FxInfo

LifeSpan 100

## FALLBACK NODES ###########################################################

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
		bhvr	Behaviors\CameraShake01Subtle.bhvr
		Part	CustomizeablePowers\StoneMelee\StoneHandsDustHit.part
		Part	CustomizeablePowers\StoneMelee\StoneHandsDustHit2.part
		Sound stonehit 80 80 0.9
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
		ChildFX	CustomizeablePowers\EarthControl\Lava\Child_RockDebrisDarkScatter.fx
		Lifespan 100
	End

	Event
		hardwareOnly
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Lava\Child_RockDebrisDarkScatter.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	1
	RepeatJitter	1

	Event
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Lava\Child_RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter 15
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	1
	RepeatJitter	1

	Event
		HardwareOnly
		Type	Local
		At	chest
		ChildFX	CustomizeablePowers\EarthControl\Lava\Child_RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter 15
	End
End

#############################################################

End