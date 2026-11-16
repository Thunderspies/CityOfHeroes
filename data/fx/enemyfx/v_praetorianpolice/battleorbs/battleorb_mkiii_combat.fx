#############################################################
## BattleOrb_Combat.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	11

	Event
		EName	MkIOffsets
		Type	Local
		At	Chest
		BhvrOverride
			Alpha	1
		End
		Geom	FX_BattleOrb_MkI_Offsets
	End

	Event
		EName	MkIIIOffsets
		Type	Local
		At	Chest
		Part	:BattleOrb_Glow.part
		Geom	FX_BattleOrb_MkIII_Offsets
	End

#############################################################

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
		Sound IDF_BattleOrbIII_Idle_Loop 50 50 .35
	End

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	3
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	4
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

#############################################################

	Event
		Type	Local
		At	MkIOffsets
		AltPiv	5
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:Sphere_Light_AlwaysOn_Combat.part
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
	End

	Event
		Type	Local
		At	MkIOffsets
		AltPiv	6
		BhvrOverride
			PositionOffset	-0.12 0.15 0.0
		End
		Part	:Sphere_Light_AlwaysOn_Combat.part
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
	End

	Event
		Type	Local
		At	MkIOffsets
		AltPiv	7
		BhvrOverride
			PositionOffset	0.12 0.15 0.0
		End
		Part	:Sphere_Light_AlwaysOn_Combat.part
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
	End

#############################################################

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	5
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_MkII_Light_AlwaysOn_Combat.part
		Part	:BattleOrb_MkII_Light_Combat.part
		Part	:BattleOrb_MkII_Light_Highlight_Combat.part
	End
End

Condition
	On	Cycle
	Time	15

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	6
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Light_Combat.part
		Part	:BattleOrb_Light_Highlight_Combat.part
		Lifespan 1
	End

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	7
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Light_Combat.part
		Part	:BattleOrb_Light_Highlight_Combat.part
		Lifespan 1
	End

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	8
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Light_Combat.part
		Part	:BattleOrb_Light_Highlight_Combat.part
		Lifespan 1
	End

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	9
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Light_Combat.part
		Part	:BattleOrb_Light_Highlight_Combat.part
		Lifespan 1
	End

	Event
		Type	Local
		At	MkIIIOffsets
		AltPiv	10
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Light_Combat.part
		Part	:BattleOrb_Light_Highlight_Combat.part
		Lifespan 1
	End
End

#############################################################

End