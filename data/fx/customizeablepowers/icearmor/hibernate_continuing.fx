#############################################################
## Ice Armor - Hibernate
#############################################################

FxInfo

Lifespan 5400

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Particles
		Type	local
		At	root
		Sound IceEmerge2 80 80 0.77
		Part1	:Hibernate_Ring1.part
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	Particles
		Type	local
		At	root
		Part	:IceRootChunks.part
		part	:Hibernate_RingMist1.part
		part	:Hibernate_RingMist2.part
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Ename	BlockofIce
		Type	local
		At	root
		Bhvr 	behaviors\BlockOfIce.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_Hybernate
		Sound Ice1_loop 80 80 0.75
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Ename	BlockofIce1
		Type	local
		At	Root
		Sound IceBuild2 80 80 0.96
		Part5	:Hibernate_Farkles1.part
	End
End

#############################################################

Condition
#	On 	Death
	Repeat	2

	Event
		Ename	BlockofIceExplosionNew1
		Type	local
		At	chest
		BhvrOverride
			StartJitter	1 1 1
		End
		ChildFX	CustomizeablePowers\ContinuingConditionalFX\IceChunkLargeScatter.fx
		lifespan 100
	End

	Event
		Ename	BlockofIceExplosionNew1
		Type	local
		At	Hips
		BhvrOverride
			StartJitter	1 1 1
		End
		ChildFX	CustomizeablePowers\ContinuingConditionalFX\IceChunkLargeScatter.fx
		lifespan 100
	End

	Event
		Ename	BlockofIceExplosionNew1
		Type	local
		At	Root
		BhvrOverride
			PositionOffset	0 2 0
			StartJitter	1 1 1
		End
		ChildFX	CustomizeablePowers\ContinuingConditionalFX\IceChunkLargeScatter.fx
		lifespan 100
	End

	Event
		HardwareOnly
		Ename	BlockofIceExplosionNew1
		Type	local
		At	chest
		BhvrOverride
			StartJitter	1 1 1
		End
		ChildFX	CustomizeablePowers\ContinuingConditionalFX\IceChunkLargeScatter.fx
		lifespan 100
	End

	Event
		HardwareOnly
		Ename	BlockofIceExplosionNew1
		Type	local
		At	Root
		BhvrOverride
			PositionOffset	0 2 0
			StartJitter	1 1 1
		End
		ChildFX	CustomizeablePowers\ContinuingConditionalFX\IceChunkLargeScatter.fx
		lifespan 100
	End
End

#############################################################

Condition
	On 	Death

	Event
		Ename	BlockofIceExplosionOld
		Type	local
		At	chest
		Part1	:HybernateBreakoutCrystal01.part
		Part2	:HybernateBreakoutCrystal02.part
		Sound IceBreak1 80 80 0.82
		lifespan 40
	End

	Event
		Ename	BlockofIceExplosion
		Type	local
		At	neck
		Part3	:SnowBreakout.part
		lifespan 40
	End

	Event
		Ename	BlockofIceExplosion
		Type	local
		At	chest
		Part1	:IceChunks.part
		Part2	:IceChunksLarge.part
		lifespan 21
	End

	Event
		Ename	BlockofIceExplosion
		Type	local
		At	chest
		Part3	:ChillTouchSnowA.part
		Part4	:ChillingTouchFlash.part
		lifespan 27
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
		Ename	Particles
		Type	Destroy
	End
End

#############################################################

End