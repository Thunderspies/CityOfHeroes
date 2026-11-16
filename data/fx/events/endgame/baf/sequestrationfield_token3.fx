#############################################################
## SequestrationField_Token3.fx
#############################################################

FxInfo
Flags DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SeqFieldAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.25 2.5 -0.25
			Scale		5.4 1.5 5.4
			Spin		0.0 0.02 0.0
			Alpha		1
		End
		Geom	GEO_SeqField_Bar

	End

	Event
		Type	StartPositOnly
		At	SeqFieldAnchor
		AltPiv	3
		BhvrOverride
			PositionOffset	0.0 1.75 0.0
		End
		Part	:Shockwave.part
		Part	:Flash.part
		Part	:Flash_Star.part

		Lifespan	5
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	SeqFieldAnchor
		Geom	GEO_SeqField_Bar
	End
End

Condition
	On	Time
	Time	7

	Event
		EName 	Hookup
		Type	Local
		At	root
		bhvr	behaviors\Invulnerability_Spin.bhvr
		BhvrOverride
			PositionOffset	0.25 -0.25 -0.25
			Scale		1.5 0.25 1.5
		End
		geom	FX_RuneStreak
		Sound Sequestration_Field_03_Loop 50 50 .25
	End

	Event
		Type	Local
		At	Hookup
		altpiv  1
		Part	:ForceCage_VerticalStreaks01.part
	End

	Event
		Type	Local
		At	Hookup
		altpiv  2
		Part	:ForceCage_VerticalStreaks01.part
	End


	Event
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part	:Ground_Fill_Bright.part

	End
End


#############################################################

End