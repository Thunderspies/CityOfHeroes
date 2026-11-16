#########################################################
##	
##

FxInfo


##################################
Condition	

	Event	
	
		Type	Start
		At	Root
		
		ChildFx	Powers/ColdControl/DeepFreeze.fx
		#Geom	Hybernate

#		Part1	POWERS\ColdControl\IceRootRing.part
#		Part2	POWERS\ColdControl\IceRootRing2.part
#		Part	POWERS\ColdControl\IceRootChunks.part
#		part	POWERS\ColdControl\coldMistRoot.part
#		part	POWERS\ColdControl\coldMistRootFlat.part
#		Part	POWERS\ColdControl\Sparklies.part
#		Part	POWERS\ColdControl\IceRootChunksSmall.part
#		part	POWERS\ColdControl\coldMistRootFlat.part

	End

	Event
		Ename	BlockofIce1
		Type	local 
		At	chest
		Part5	POWERS\ColdControl\HybernateSparklies.part
		
	End

	Event
		Ename	BlockofIce2
		Type	local 
		At	LlegL
		Part5	POWERS\ColdControl\HybernateSparklies2.part
		
	End

	Event
		Ename	BlockofIce3
		Type	local 
		At	LlegR
		Part5	POWERS\ColdControl\HybernateSparklies2.part
		
	End

	Event	
	
		Type	Start
		At	Chest
		Part1	POWERS\ColdControl\IceBoltMist.part
		Part1	POWERS\ColdControl\IceBoltMist.part

		LifeSpan	20

	End

	Event	
	
		Type	Start
		At	Chest
		Part1	Powers/ColdControl/IceChunks.part

	End
End

#Condition
#	On	Time
#	Time	0
#	Repeat	15
#	RepeatJitter	2
#	
#	Event	
#		Ename   Chunks
#		Type	Start
#		At	Chest
#		ChildFX	V_COV\PhysicsEnabled\IceChunkScatterNonJagged.fx
#	End
#End

#Condition
#	On 	Death
#	Repeat	4
#	RepeatJitter	3
#	
#	Event	
#		Ename   Chunks
#		Type	Start
#		At	Chest
#		ChildFX	V_COV\PhysicsEnabled\IceChunkScatterNonJaggedMedium.fx
#	End
#End


#Condition
#	On	Time
#	Time	0
#	Repeat	4
#	RepeatJitter	3
#	
#	Event	
#		Ename   Cubes
#		Type	Start
#		At	Chest
#		ChildFX	V_COV\PhysicsEnabled\IceCubesScatter.fx	
#
#	End
#
#End
#
Condition
	On 	Death
	Repeat	4
	RepeatJitter	3
	
	Event	
		Ename   Cubes
		Type	Start
		At	Root
		ChildFX	V_COV\PhysicsEnabled\IceCubesScatter.fx	

	End
End	

Condition
	On 	Death
	Repeat	10
	RepeatJitter	3
	
	Event	
		Ename   Cubes
		Type	Start
		At	Root
		ChildFX	V_COV\PhysicsEnabled\IceChunkScatterNonJagged.fx

	End

End

Condition
	On 	Death
	Repeat	6
	RepeatJitter	3
		
	Event	
		Ename   Cubes
		Type	Start
		At	Root
		ChildFX	V_COV\PhysicsEnabled\IceChunkScatterNonJagged.fx

	End

End	

#Condition
#	On 	Death
#	Repeat	4
#	RepeatJitter	3
#	
#	Event	
#		Ename   Cubes
#		Type	Start
#		At	Chest
#		ChildFX	V_COV\PhysicsEnabled\IceCubesScatter.fx	
#
#	End
#End	

#Condition
#	On 	Death
#	Repeat	6
#	RepeatJitter	3
#	
#	Event	
#		Ename   Cubes
#		Type	Start
#		At	Chest
#		ChildFX	V_COV\PhysicsEnabled\IceChunkScatterNonJagged.fx
#
#	End
#
#End

Condition
	On 	Death
	Repeat	6
	RepeatJitter	3
		
	Event	
		Ename   Cubes
		Type	Start
		At	Root
		ChildFX	V_COV\PhysicsEnabled\IceChunkScatterNonJagged.fx

	End

End	

End