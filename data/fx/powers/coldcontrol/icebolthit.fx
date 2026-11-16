#############################################################
## IceBoltHit.fx
#############################################################

FxInfo
LifeSpan	100

#############################################################

Input
	InpName	Hips
End

##################################

Condition
	On	Time
	Time	0


	Event
		Ename   Ice
		Type	local
		At	chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	:IceChunks.part
		#Part2	:IceChunksLarge.part
		#Part3	:IceScatter.part
		Sound Ice7 100.0 100.0 1.0
		LifeSpan	30

	End

	Event
		Ename   hit
		Type	local
		At	chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		#Part1	:icebolttargetfreeze.part
		Part4	:iceboltsnowexp.part

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