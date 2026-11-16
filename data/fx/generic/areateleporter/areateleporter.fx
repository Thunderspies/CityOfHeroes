#############################################################
## SequestrationField_Token3.fx
#############################################################

FxInfo
Flags DontSuppress

#############################################################

Condition
	On	Time
	Time	7

	Event
		EName 	Hookup
		Type	Local
		At	Root
		bhvr	behaviors\Invulnerability_Spin.bhvr
		BhvrOverride
			PositionOffset	0.25 -0.25 -0.25
			Scale		2.15 0.25 2.15
			Spin		0.0 0.05 0.0
		End
		geom	FX_RuneStreak
	End

	Event
		EName 	Hookup2
		Type	Local
		At	Root
		bhvr	behaviors\Invulnerability_Spin.bhvr
		BhvrOverride
			PositionOffset	0.25 -0.25 -0.25
			Scale		2.35 0.25 2.35
			Spin		0.0 -0.05 0.0

		End
		geom	FX_RuneStreak
	End

	Event
		EName 	Hookup3
		Type	Local
		At	Root
		bhvr	behaviors\Invulnerability_Spin.bhvr
		BhvrOverride
			PositionOffset	0.25 -0.25 -0.25
			Scale		0.5 0.25 0.5
			Spin		0.0 -0.15 0.0

		End
		geom	FX_RuneStreak
	End

	Event
		Type	Local
		At	Hookup
		altpiv  1
		Part	:ForceCage_VerticalStreaks01.part
	End

	Event
		Type	Local
		At	Hookup2
		altpiv  2
		Part	:ForceCage_VerticalStreaks01.part
	End

	Event
		Type	Local
		At	Hookup3
		altpiv  2
		Part	:ForceCage_VerticalStreaks_Center.part
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part	:Ground_Fill_Bright.part
		Part	:Ground_Rings.part
		Part	:Ground_Rings_Small.part

	End
End


#############################################################

End