#############################################################
## FrostBreathHit.fx
#############################################################

FxInfo

Lifespan 50

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename   IceShards
		Type	Posit
		At	chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:IceBoltMist.part
		Part	:ChillTouchSnow.part
		Part	:IceChunks.part
		Sound Ice7 100.0 100.0 1.0
		Lifespan 40
	End
End

#############################################################

Condition
	On	Time
	Time	0
	Repeat	5
	RepeatJitter	3

	Event
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	:IceCubesScatter.fx
	End

	Event
		hardwareOnly
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	:IceCubesScatter.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat	2
	RepeatJitter	1

	Event
		HardwareOnly
		Ename   Chunks
		Type	Start
		At	Chest
		ChildFX	:IceChunkScatter.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat	9
	RepeatJitter	3

	Event
		HardwareOnly
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	:IceCubesExplode.fx
	End
End

#############################################################

End