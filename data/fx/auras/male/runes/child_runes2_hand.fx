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
		At	Origin
		BhvrOverride
			PositionOffset	0.0 0.05 0.0
			InitialVelocity	0.0 0.005 0.0
			Spin		0.075 0.01 0
		End
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:Rune2_TightPath_Small.part
	End

End

#############################################################

End