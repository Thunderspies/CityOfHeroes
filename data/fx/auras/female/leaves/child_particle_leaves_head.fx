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
			Spin		0.0 0.015 0.0
			SpinJitter	0.005 0.005 0.005
		End
	End

	Event
		Ename	GEO_Leaves
		Type	Local
		At	SpinnerOffset

		BhvrOverride
			PositionOffset		0.0 0.2 0.0
			StartJitter		0.1 0.1 0.1
			PYRRotateJitter		180 180 180
			SpinJitter		.05 .05 .05
			InitialVelocityJitter	0.01 0.0 0.01
			Gravity			0.00035
		End
		Part	:Particle_Leaves_Head.part
		Lifespan 60
	End
End

#############################################################

End