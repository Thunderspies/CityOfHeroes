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
		EName	MkIIOffsets
		Type	Local
		At	Chest
		Part	:BattleOrb_Glow.part
		Geom	FX_BattleOrb_MkII_Offsets
	End

#############################################################

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
		Sound IDF_BattleOrbII_Idle_Loop 50 50 .35
	End

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	3
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	4
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	5
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	6
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
			PositionOffset	0.0 0.05 0.0
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
			PositionOffset	0.0 0.05 0.0
		End
		Part	:Sphere_Light_AlwaysOn_Combat.part
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
	End

#############################################################

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	7
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
		At	MkIIOffsets
		AltPiv	8
		BhvrOverride
			PositionOffset 0.0 0.03 0.0
		End
		Part	:BattleOrb_Light_Combat.part
		Part	:BattleOrb_Light_Highlight_Combat.part
		Lifespan 20
	End
End

#############################################################

End