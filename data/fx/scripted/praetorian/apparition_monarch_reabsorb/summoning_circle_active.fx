#############################################################
## SequestrationField_Token3.fx
#############################################################

FxInfo
Flags DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Hookup
		Type	Local
		At	Root
		BhvrOverride
			PYRRotate	0.0 40.0 0.0
		End
	End

	Event
		EName 	Hookup2
		Type	Local
		At	Root
		BhvrOverride
			PYRRotate	0.0 170.0 0.0
		End
	End

	Event
		EName 	Hookup3
		Type	Local
		At	Root
		BhvrOverride
			PYRRotate	0.0 290.0 0.0
		End
	End


	Event
		Type	Local
		At	Root
		BhvrOverride
			Alpha		50
			PositionOffset	0.0 -6.5 0.0
			Scale		0.75 8.0 0.75
			Spin		0.0 0.2 0.0
		End
		Part	:Streaks_Central.part

	End

	Event
		Type	Local
		At	Hookup
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			Alpha		50
			PositionOffset	0.0 -6.5 15.5
			Scale		0.75 8.0 0.75
		End
		Part	:Streaks_Outer.part

	End

	Event
		Type	Local
		At	Hookup2
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			Alpha		50
			PositionOffset	0.0 -6.5 15.5
			Scale		0.75 8.0 0.75
		End
		Part	:Streaks_Outer.part

	End

	Event
		Type	Local
		At	Hookup3
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			Alpha		50
			PositionOffset	0.0 -6.5 15.5
			Scale		0.75 8.0 0.75
		End
		Part	:Streaks_Outer.part

	End
End


#############################################################

End