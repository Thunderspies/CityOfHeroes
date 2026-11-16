#############################################################
## BoulderHit.fx
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
		EName 	Mallet
		Type	Local
		At	Chest
		Part	:StalagtiteDustHit.part
		Part	:StalagtiteDustHit2.part
		Sound stonehit2 100 100 0.85
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGold.fx
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
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan 100
	End

	Event
		HardwareOnly
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx
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
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx
		Lifespan 100
	End

	Event
		HardwareOnly
		Type	Local
		At	chest
		ChildFX	V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter 15
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
		ChildFX	V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan 90
		LifespanJitter 15
	End
End

#############################################################

End