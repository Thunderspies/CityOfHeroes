#############################################################
## Physics_Sparks.fx
#############################################################

FxInfo
LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	9

	Event
		Type	Start
		At	Chest

		Bhvr	V_COV\PhysicsEnabled\Fire_Demon.bhvr
		BhvrOverride
		Scale			.2 .2 .2
		StartJitter		0 0 0
		InitialVelocity		0.0 0.5 0.0
		InitialVelocityJitter	0.7 0.5 0.7
		Gravity			0.004

		StartColor		180 230 255
		SpinJitter		20 30 0
		End
		geom	tintable_FX_IceWall_01
		geom	tintable_FX_IceWall_02
		geom	tintable_FX_IceWall_03
		Lifespan	90

		CEvent 	:Physics_Ice_Death_CEvent.fx
		CThresh 	.001
		CDestroy 	1
	End

End

#############################################################

End
