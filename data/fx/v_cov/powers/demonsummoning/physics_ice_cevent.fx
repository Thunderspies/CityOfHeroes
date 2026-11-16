#############################################################
## Physics_Sparks.fx
#############################################################

FxInfo
LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	2

	Event
		Type	Start
		At	Origin

		Bhvr	V_COV\PhysicsEnabled\Fire_Demon.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			StartColor		180 230 255
			StartJitter		0 0 0
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.3 0.05 0.3
			SpinJitter		20 30 0
			FadeOutLength		30
		End
		geom	tintable_FX_IceWall_01
		geom	tintable_FX_IceWall_02
		geom	tintable_FX_IceWall_03
		Lifespan	90
		LifespanJitter	30

		CThresh 	.001
		CDestroy 	0
	End
End

Condition
	On	Time
	Time	0
	Event
		Type	Start
		At	Origin

		Part	:Physics_Mist.part
		Lifespan	90
		LifespanJitter	30

		CThresh 	.001
		CDestroy 	0
	End
End

#############################################################

End
