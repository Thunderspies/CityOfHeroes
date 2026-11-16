#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	chest
End

LifeSpan	70
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
		LifeSpan	70
		
	End

	Event	
		Ename   hit
		Type	local
		At	chest
		Bhvr	behaviors/GenericParticleFade.bhvr		
		Part1	:icebolttargetfreeze.part
		Part4	:iceboltsnowexp.part
	
		LifeSpan	45
		
	End


End

Condition
	On	Time
	Time	0
	Repeat	1
	RepeatJitter	1
	
	Event	
		Ename   BigChunks
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceChunkLargeScatter.fx

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