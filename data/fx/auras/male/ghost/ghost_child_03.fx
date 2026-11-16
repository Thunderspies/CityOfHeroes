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
			Spin		0.0 -0.08 0.0
			SpinJitter	0.0 0.02 0.0
		End
	End



	Event
		EName	PRIME
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr

		BhvrOverride
			PositionOffset	0.0 -1.5 0.0
			StartJitter	3.0 0.0 3.0
			InitialVelocity	0.0 0.10 0.0

		End



		Lifespan 60
	End

	Event
		EName	WispAnchor
		Type	Local
		At	PRIME

		BhvrOverride
			PositionOffset	1.0 -0.5 0.0
			StartJitter	0.5 1.0 0.0
		End

		Part	:Ghost_Trail_Large.part
		Lifespan 80
	End

	Event
		Type	Local
		At	WispAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr

		Part	:Ghost_HalfBody_01.part

		Lifespan 65
	End
End


#############################################################

End