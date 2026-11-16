#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Target
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	Local 
		At	Chest
		Part1	:SlagDust1.part
		Part2	:SlagDust2.part
		Part	:SlagDustLarge.part
		Part	:SlagChunkHitFlash.part
		Part	:SlagDustRocks2.part
		Part	:SlagDustRocks2.part
		Part3	:SlagRocks.part
		Part4	:SlagChunkBreak.part
		Part3	:SlagRocks.part
		Part4	:SlagChunkBreak.part
		Part	:SlagShrapnelRocks.part
		Bhvr	Behaviors/CameraShake02Subtle.bhvr
		Sound boulderhit 90 90 .99

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1
	RepeatJitter	1

	Event
		EName	debris
		Type	StartPositOnly
		At	Mallet
		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	150
		LifespanJitter	30
				
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	4

	Event
		EName	debris
		Type	StartPositOnly
		At	Mallet
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	150
		LifespanJitter	30
				
	End

	Event
		HardwareOnly
		EName	debris
		Type	StartPositOnly
		At	Mallet
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	150
		LifespanJitter	30
				
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		HardwareOnly
		EName	debris
		Type	StartPositOnly
		At	Mallet
		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	150
		LifespanJitter	30
				
	End

End

#Condition
#	On 	Time
#	Time 	3
#
#	Event
#		EName 	more
#		Type	Local 
#		At	Chest
#		Part	:SlagDustRocks2.part
#		Part3	:SlagRocks.part
#		Part4	:SlagChunkBreak.part
#		Part	:SlagShrapnelRocks.part
#
#	End
#
#End
#
Condition
	On 	Time
	Time 	0
	Repeat	19

	Event
		EName 	Rocky
		Type	Local 
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGrayScatter.fx

	End
	
	Event
		HardwareOnly
		EName 	Rocky
		Type	Local 
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGrayScatter.fx

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	2

	Event
		EName 	Rocky
		Type	Local 
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RockChunkGrayScatter.fx

	End

	Event
		HardwareOnly
		EName 	Rocky
		Type	Local 
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RockChunkGrayScatter.fx

	End
End

End
