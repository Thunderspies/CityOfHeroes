#############################################################
## CHILD_GEO_Leaves.fx
#############################################################

FxInfo

#########################################################
Condition
	On	Time
	Time	0

	Event
		EName	SpinnerOffset
		Type	StartPositOnly
		At	Head
		BhvrOverride
			Spin		0.0 0.05 0.0
			SpinJitter	0.01 0.02 0.01
		End
	End

	Event
		Ename	GEO_Leaves
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			PositionOffset		1.0 1.0 0.0
			StartJitter		0.15 0.8 0.15
			SpinJitter		.1 .1 .1
			StartJitter		0.5 0.5 0.5
			InitialVelocity		0.0 -0.05 0.0
			InitialVelocityJitter	0.05 0.05 0.05
			Drag			0.025
			Gravity			0.0018
		End
		Part	:Particle_Leaves.part
	End
End

#############################################################

End