#############################################################
## IceSword_Hit.fx
#############################################################

FxInfo

LifeSpan 100

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0


	Event
		Ename   Ice
		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\IceManipulation\IceChunks.part
		#Part2	:IceChunksLarge.part
		#Part3	:IceScatter.part
		Sound Ice7 100.0 100.0 1.0
		LifeSpan	30

	End

	Event
		Ename   hit
		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Part1	:icebolttargetfreeze.part
		Part4	CustomizeablePowers\IceManipulation\iceboltsnowexp.part

		LifeSpan	15

	End


End


Condition
	On	Time
	Time	0
	Repeat	2
	RepeatJitter	1

	Event
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
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceCubesExplode.fx

	End


	Event
		HardwareOnly
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceCubesExplode.fx

	End


	Event
		HardwareOnly
		Ename   Chunks
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceChunkScatter.fx

	End

End

End