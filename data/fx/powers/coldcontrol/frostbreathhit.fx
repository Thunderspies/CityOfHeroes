#############################################################
## FrostBreathHit.fx
#############################################################

FxInfo

Input
	InpName	Hips
End

LifeSpan	50

##################################

Condition
	On	Time
	Time	0

	Event
		Ename   IceShards
		Type	Posit
		At	chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	:IceBoltMist.part
		Part2	:ChillTouchSnow.part
		Part3   :ChillingTouchFlash.part
		Part1	:IceChunks.part

		Sound Ice7 100.0 100.0 1.0

		lifespan	40
	End


End

Condition
	On	Time
	Time	0
	Repeat	5
	RepeatJitter	3

	Event
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceCubesScatter.fx

	End

	Event
		hardwareOnly
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceCubesScatter.fx

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
		ChildFX	V_COV\PhysicsEnabled\IceChunkScatter.fx

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
		ChildFX	V_COV\PhysicsEnabled\IceCubesExplode.fx

	End

End

End