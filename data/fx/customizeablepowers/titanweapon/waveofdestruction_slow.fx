#############################################################
## WaveOfDestruction_Slow.fx
#############################################################

FxInfo
Lifespan 300
######################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TW_WaveOfDestruction_AttackSlow_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		At T_Chest
		Type Start
		EName ConeTarget
		BhvrOverride
			PositionOffset 0 -1 0
		End
	End

	Event
		At Root
		Type Local
		EName GroundStab
		BhvrOverride
			PositionOffset 1 0.2 4
		End
	End
End

Condition
	On	Time
	Time	72

	Event
		At C_Chest
		Type Local
		Bhvr Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset -1.5 0 -2
			PyrRotate 0 -120 0
		End
		Part :Swoosh_Trail.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	74

	Event
		At 	Chest
		Type 	Start
		EName 	Prime
		LookAt 	ConeTarget
		Magnet 	ConeTarget
		Bhvr 	:Swoosh_Projectile.bhvr
		Part 	:Titan_Swoosh_Horizontal.part
		LifeSpan 3
	End
End

Condition
	On	Time
	Time	75

	Event
		At 	GroundStab
		Type	Local
		Bhvr	:ExpandSwooshRings.bhvr
		Part	:Rock_Chunks.part
		Part	:Dirt_Splatter.part
		Part	:Cracks_Radial.part
		Part	:Cracks_Hole.part
		Part	:WaveOfDestruction_Flash.part
		Part	:WaveOfDestruction_Shockwave.part
		Part 	:Dust_Circle_Puff.part
		Lifespan 3
	End

	Event
		At	T_Root
		Type	Start
		ChildFX	:WaveOfDestruction_Child.fx
	End

	Event
		At	GroundStab
		Type	Start
		BhvrOverride
			PyrRotate 0 180 0
		End
		Part	:Cracks_Spread.part
	End

	Event
		At Root
		Type Local
		EName AirRushAnchor
		BhvrOverride
			PositionOffset 2 0 1
			PyrRotate 0 20 0
		End
		Part	:TitanSwing_Dust_Traveling.part
		Lifespan 3
	End

	Event
		At Root
		Type Local
		EName AirRushAnchor
		BhvrOverride
			PositionOffset 0 0 3
			PyrRotate 0 0 0
		End
		Part	:TitanSwing_Dust_Traveling.part
	Lifespan 3
	End

	Event
		At Root
		Type Local
		EName AirRushAnchor
		BhvrOverride
			PositionOffset -2 0 1
			PyrRotate 0 -20 0
		End
		Part	:TitanSwing_Dust_Traveling.part
	Lifespan 3
	End
End

###########################################
End
