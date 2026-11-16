#############################################################
## RefractionShield_Continuing.FX
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound leaderglow3 80.0 80.0 .55
	End
End

#############################################################

Condition
	On	Time
	Time 	8

	Event
		Type 	Local
		At	Origin
		Sound glow2b_loop 80.0 80.0 .38
	End
End

Condition
	On	Time
	Time	0

	Event
		EName 	Prime2
		Type	Posit
		At	HIPS
		Bhvr	CustomizeablePowers\ForceField\DetentionField.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
	End

	Event
		Type	local
		At	Root
		Bhvr	CustomizeablePowers\ForceField\PivotOffsetChest.bhvr
		part	CustomizeablePowers\ForceField\DetentionShieldRings.part
		part	CustomizeablePowers\ForceField\DetentionShieldRingsUp.part
		part	CustomizeablePowers\ForceField\DetentionShieldRingsDown.part
	End
End

#############################################################

End