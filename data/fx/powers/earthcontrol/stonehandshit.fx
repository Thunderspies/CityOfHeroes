#############################################################
## StoneHandsHit.fx
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
		Bhvr	Behaviors\CameraShake01Subtle.bhvr
		Part	:StoneHandsDustHit.part
		Part	:StoneHandsDustHit2.part
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
		ChildFX	V_COV\PhysicsEnabled\RockDebrisDarkScatter.fx
		Lifespan 100
	End

	Event
		HARDWAREONLY
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\RockDebrisDarkScatter.fx
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
		ChildFX	V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter	15
	End

	Event
		HARDWAREONLY
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter	15
	End
End

#############################################################

End