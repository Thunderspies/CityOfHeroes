#########################################################
##	chill touch hit
FxInfo

Input
	InpName	chest
End

LifeSpan	100
##################################

Condition
	On	Time
	Time	0


	Event
		Ename   Ice
		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\IcyAssault\IceChunks.part
		Sound Ice7 100.0 100.0 1.0
		LifeSpan	30

	End

	Event
		Ename   hit
		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part4	CustomizeablePowers\IcyAssault\iceboltsnowexp.part

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
		ChildFX	:IceChunkScatter.fx

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
		ChildFX	:IceCubesExplode.fx

	End


	Event
		HardwareOnly
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	:IceCubesExplode.fx

	End


	Event
		HardwareOnly
		Ename   Chunks
		Type	Start
		At	Chest
		ChildFX	:IceChunkScatter.fx

	End

End

End