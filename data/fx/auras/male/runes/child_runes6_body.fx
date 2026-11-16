#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	SpinnerAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.85 0.0
			InitialVelocity	0.0 0.06 0.0
			Spin		0 0.075 0
		End
	End
End

Condition
	On	Time
	Time	0
	Random	1

	Event
		Ename	Spin1
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	1.5 0.0 0.0
			StartJitter	0.5 0.0 0.0
		End
		Part	:Rune6_TightPath.part
	End

	Event
		Ename	Spin2

		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1.5 0.0 0.0
			StartJitter	0.5 0.0 0.0
		End
		Part	:Rune6_TightPath.part
	End
End

#############################################################

End