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
		EName	Mk1Offsets
		Type	Local
		At	Chest
		Part	:BattleOrb_Glow.part
		Geom	FX_BattleOrb_MkI_Offsets
	End

	Event
		Type	Local
		At	Mk1Offsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	Mk1Offsets
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	Mk1Offsets
		AltPiv	3
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	Mk1Offsets
		AltPiv	4
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:BattleOrb_Turret_Glow.part
		Part	:BattleOrb_Turret_Smoke.part
	End

	Event
		Type	Local
		At	Mk1Offsets
		AltPiv	5
		Part	:Sphere_Light_AlwaysOn_Combat.part
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
	End

	Event
		Type	Local
		At	Mk1Offsets
		AltPiv	6
		Part	:Sphere_Light_AlwaysOn_Combat.part
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
	End

	Event
		Type	Local
		At	Mk1Offsets
		AltPiv	7
		Part	:Sphere_Light_AlwaysOn_Combat.part
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
	End



End

Condition
	On	Cycle
	Time	15

	Event
		Type	Local
		At	Mk1Offsets
		AltPiv	8
		BhvrOverride
			PositionOffset 0.0 0.0 0.0
		End
		Part	:BattleOrb_Light_Combat.part
		Part	:BattleOrb_Light_Highlight_Combat.part
		Lifespan 20
	End
End

#############################################################

End