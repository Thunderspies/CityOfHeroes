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
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	:IceChunks.part
		#Part2	:IceChunksLarge.part
		#Part3	:IceScatter.part
		Sound Ice7 100.0 100.0 1.0
		LifeSpan	20
		
	End

	Event	
		Ename   hit
		Type	local
		At	chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	POWERS\ColdControl\IceBoltMist.part
		Part4	:iceboltsnowexp.part
		
		LifeSpan	15
		
	End


End


Condition
	On	Time
	Time	0
	Repeat	3
	RepeatJitter	1
	
	Event	
		Ename   Chunks
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceChunkScatterSmallNonJagged.fx

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
		hardwareOnly
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceCubesScatter.fx

	End

End

End				