#############################################################
## SequestrationField_Token1.fx
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
			PositionOffset	0.25 -2.5 -0.25
			Scale		5.4 1.5 5.4
			Spin		0.0 0.02 0.0
			Alpha		1
		End
		Geom	GEO_SeqField_Bar

	End

	Event
		Type	StartPositOnly
		At	SeqFieldAnchor
		AltPiv	1
		BhvrOverride
			PositionOffset	0.0 0.75 0.0
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
		Part	:Ground_Swirls.part
		Sound Sequestration_Field_01_Loop 50 50 .25
	End


	Event
		Type	Local
		At	SeqFieldAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 1.75 0.0
		End
		Part	:Ground_Fill.part
	End
End

#############################################################

End