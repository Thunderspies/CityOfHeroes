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
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.85 0.0
			InitialVelocity	0.0 0.07 0.0
			InitialVelocityJitter	0.025 0.02 0.025
			Spin		0 -0.075 0
		End
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-2.0 -.5 0.0
			StartJitter	1.0 0.0 0.0

		End
		Part	:Soul_Trail_Large.part
		Part	:Soul_Trail_Bright.part
		Part	:Soul_Head.part
	#	Part	:Soul_Core.part

		Lifespan 80
	End

End

#############################################################

End