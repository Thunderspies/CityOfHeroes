#########################################################
##	cot_waterfall_med.fx
##
FxInfo

	LifeSpan	100

###########################################################

Condition
#	On	cycle	#Time
#	Time	1


	Event
		EName	Prime
		Type	start
		At	origin
		Geom	CenterDummy
		
		BhvrOverride
		
			InitialVelocity		0.0 -2.0 -0.01
			InitialVelocityJitter	0.0 0.5 0.005
			PositionOffset		20 50 20
			StartJitter		60.0 10.0 60.0
			FadeOutLength		280
		End

		bhvr	behaviors/Fastprojectile3.bhvr

		LifeSpan	100
	
	End

	Event
		EName	cometParts
		Type	posit
		At	Prime
		Altpiv	1

		Part1	:RainDropBright.part
		
		BhvrOverride

			physics 		1
			physRadius 		2.8
			physGravity 		.5
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	:RainDropSplashBright.fx
		CThresh	.001
		LifeSpan	70
	End

	Event
		EName	Prime1
		Type	start
		At	origin
		Geom	CenterDummy
		
		BhvrOverride
		
			InitialVelocity		0.0 -2.0 -0.01
			InitialVelocityJitter	0.0 0.5 0.005
			PositionOffset		20 50 20
			StartJitter		60.0 10.0 60.0
			FadeOutLength		280
		End

		bhvr	behaviors/Fastprojectile3.bhvr

		LifeSpan	100
	
	End

	Event
		EName	cometParts
		Type	posit
		At	Prime1
		Altpiv	1

		Part1	:RainDrop2Bright.part
		
		BhvrOverride

			physics 		1
			physRadius 		2.8
			physGravity 		.5
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	:RainDropSplashBright.fx
		CThresh	.001
		LifeSpan	70
	End

End


End			