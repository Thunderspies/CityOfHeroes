#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	150

########  Freeze   ##########

Condition
	On 	Time
	Time 	5

	Event
		Ename	BlockofIce
		Type	local 
		At	Root
		Bhvr 	behaviors/BlockOfIce.bhvr
		geom	Hybernate
		Sound Ice1_loop 80 80 .75
		
	End

	Event
		Ename	Icicle1
		Type	local 
		At	Root
		Bhvr 	behaviors/Stalagtite1.bhvr
		geom	Icicle01
		geom	Icicle02
		geom	Icicle03
		
		lifespan	65
		
	End
	Event
		Ename	Icicle2
		Type	local 
		At	Root
		Bhvr 	behaviors/Stalagtite1.bhvr
		geom	Icicle01
		geom	Icicle02
		geom	Icicle05
		geom	Icicle06
		
		lifespan	65
		
	End

	Event
		Ename	Icicle3
		Type	local 
		At	Root
		Bhvr 	behaviors/Stalagtite1.bhvr
		geom	Icicle04
		geom	Icicle05
		geom	Icicle06
		
		lifespan	65
		
	End

End

Condition
	On 	Time
	Time 	25

	Event
		Ename	BlockofIce
		Type	local 
		At	neck
		Part1	POWERS/ColdControl/IceBoltMistA.part
		#Sound	IceBuild2 80 30 .96
		
	End

End

Condition
	On 	Time
	Time 	25

	Event
		Ename	BlockofIce1
		Type	local 
		At	chest
		Part5	POWERS/ColdControl/HybernateSparklies.part
		
	End

	Event
		Ename	BlockofIce2
		Type	local 
		At	LlegL
		Part5	POWERS/ColdControl/HybernateSparklies2.part
		
	End

	Event
		Ename	BlockofIce3
		Type	local 
		At	LlegR
		Part5	POWERS/ColdControl/HybernateSparklies2.part
		
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	Particles
		Type	local
		At	root
		
		Part1	POWERS/ColdControl/IceRootRing.part
		Part2	POWERS/ColdControl/IceRootRing2.part
		Part	POWERS/ColdControl/IceRootChunks.part
		part	POWERS/ColdControl/coldMistRoot.part
		part	POWERS/ColdControl/coldMistRootFlat.part
		#Part	POWERS/ColdControl/Sparklies.part
		Part	POWERS/ColdControl/IceRootChunksSmall.part
		part	POWERS/ColdControl/coldMistRootFlat.part
		#Sound	IceEmerge2 80 30 .77
	End


End

###########  Explosion   ################3

Condition
	On	Time
	Time	88

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		
		BhvrOverride
			PositionOffset		0 3 0

			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		750
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End

End

Condition
	On	Time
	Time	90
	Repeat	12
	RepeatJitter	4
	
	Event	
		Ename   Chunks
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceChunkExplode.fx

	End

End


Condition
	On	Time
	Time	90
	Repeat	8
	RepeatJitter	2
	
	Event	
		Ename   Cubes
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceCubesExplode.fx

	End

End


Condition
	On	Time
	Time	90
	Repeat	10
	RepeatJitter	3
	
	Event	
		Ename   BOOM
		Type	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\IceChunkLargeExplosion.fx

	End

End


Condition
	On 	Time
	Time 	90

	Event
		Ename	BlockofIceExplosion
		Type	local 
		At	chest
		Part1	:IceNovaShards01.part
		Part2	:IceNovaShards02.part		
		lifespan	40
	End
	

	Event
		Ename	BlockofIceExplosion
		Type	local 
		At	chest
		Part1	POWERS/ColdControl/HybernateBreakoutCrystal01.part
		Part2	POWERS/ColdControl/HybernateBreakoutCrystal02.part
		Sound IceBreak1 80 80 .82
		lifespan	40
	End
	
	Event
		Ename	BlockofIceExplosion
		Type	local 
		At	neck
		Part3	POWERS/ColdControl/SnowBreakout.part
				
		lifespan	40
	End

	Event
		Ename	BlockofIceExplosion
		Type	local 
		At	chest
		
		Part1	POWERS/ColdControl/IceChunks.part
		Part2	POWERS/ColdControl/IceChunksLarge.part
		lifespan	21
	End

	Event
		Ename	BlockofIceExplosion
		Type	local 
		At	chest
		
		Part3	POWERS/ColdControl/ChillTouchSnowA.part
		Part4	POWERS/ColdControl/ChillingTouchFlash.part
		lifespan	27
	End

	Event
		Ename	BlockofIce
		Type	Destroy
		
	End

	Event
		Ename	BlockofIce1
		Type	Destroy
		
	End
	
	Event
		Ename	BlockofIce2
		Type	Destroy
		
	End

	Event
		Ename	BlockofIce3
		Type	Destroy
		
	End

	Event
		Ename	Icicle1
		Type	Destroy
		
	End
	
	Event
		Ename	Icicle2
		Type	Destroy
		
	End

	Event
		Ename	Icicle3
		Type	Destroy
		
	End


	Event
		Ename	Particles
		Type	Destroy
		
	End

	Event
		EName 	Prime
		Type	Local 
		At	Root
		
		Bhvr	Behaviors/CameraShake01.bhvr

	End

	Event
		EName   RadiusFxScale
		Type	local
		At	hips
		Bhvr	powers/firecontrol/Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	POWERS/ColdControl/ColdMistAura.part
		
	End
	
	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble2.bhvr
		Part1	POWERS/ColdControl/IceAuraSnow.part	
		
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble3.bhvr
			
		Part2	POWERS/ColdControl/IceAuraBits.part
	End

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	behaviors/IceAuraRing.bhvr
		LifeSpan	10

	End

End

Condition
	On 	Time
	Time	157

	Event
		EName 	Prime
		Type	Destroy
	End

	
End
	

End
