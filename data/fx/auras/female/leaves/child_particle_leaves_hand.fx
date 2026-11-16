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
		At	Origin
		BhvrOverride
			Spin		0.0 0.02 0.0
			SpinJitter	0.005 0.01 0.005
		End
	End

	Event
		Ename	GEO_Leaves
		Type	Local
		At	SpinnerOffset
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			StartJitter		0.15 0.15 0.15
			SpinJitter		.075 .075 .075
			StartJitter		0.25 0.25 0.25
			InitialVelocity		0.0 -0.025 0.0
			InitialVelocityJitter	0.025 0.025 0.025
			Drag			0.025
			Gravity			0.0012
		End
		Part	:Particle_Leaves_Hand.part
		Lifespan 80
	End
End

#############################################################

End