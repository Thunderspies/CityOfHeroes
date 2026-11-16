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
		At	Head
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			InitialVelocity	0.0 0.005 0.0
			Spin		0 0.025 0
		End
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 0.0

		End
		Part	:Sparkles_Small.part
		Part	:Smoke_Small.part
		Part	:Smoke2_Small.part
		Lifespan	40
	End

End

#############################################################

End