#############################################################
## DetentionField.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound leaderglow3 80.0 80.0 .55
	End
End

Condition
	On	Time
	Time 	8

	Event
		Type 	Local
		At	Origin
		Sound glow2b_loop 80.0 80.0 .38
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prime2
		Type	Posit
		At	HIPS
		Bhvr 	behaviors\powers\forcefield\DetentionField.bhvr
		Geom	FX_ForceBubble
	End

	Event
		Type	local
		At	Root
		Bhvr 	behaviors\PivotOffsetChest.bhvr
		part	Powers\ForceField\DetentionShieldRings.part
		part	Powers\ForceField\DetentionShieldRingsUp.part
		part	Powers\ForceField\DetentionShieldRingsDown.part
	End
End

#############################################################

End