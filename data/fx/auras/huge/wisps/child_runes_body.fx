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

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.0 0.0 0.0

		End
	End

End

#############################################################

End