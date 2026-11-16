#############################################################
## BattleOrb_Combat.fx
#############################################################

FxInfo
Lifespan	150
#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	11

	Event
		EName	MkIIOffsets
		Type	Local
		At	Chest
		BhvrOverride
			Alpha	1
		End
		Geom	FX_BattleOrb_MkII_Offsets
		Sound Plasma_Spray 200 200 .8
	End

#############################################################

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:Spray_Plasma_Jet.part
		Part	:Spray_Plasma_Body.part
		Part	:Spray_Plasma_Plume.part
		Lifespan 110
	End

	Event
		Type	Local
		At	MkIIOffsets
		AltPiv	6
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
		End
		Part	:Spray_Plasma_Jet.part
		Part	:Spray_Plasma_Body.part
		Part	:Spray_Plasma_Plume.part
		Lifespan 110

	End
End

#############################################################

End