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
			StartJitter	1.0 0.0 0.0
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
		EName	Spin1
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	1.5 0.0 0.0

		End
		Part	:Runes_Continuous.part
		Part	:Smoke.part
		Part	:Smoke2.part

		Lifespan	40
	End

	Event
		EName	Spin2

		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1.5 0.0 0.0

		End
		Part	:Runes_Continuous.part
		Part	:Smoke.part
		Part	:Smoke2.part

		Lifespan	40
	End
End

#############################################################

End