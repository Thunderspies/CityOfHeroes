#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

#########################################################
##HardwareOnly
########################################################


Condition

	Event
		EName	SmallRocks
		Type	start
		At	root
		
		Bhvr	:RockFadeOut.bhvr

		BhvrOverride
		

			PyrRotateJitter		180 180 180

			PositionOffset		0 70 0
			StartJitter		40 10 40

			InitialVelocity		0.0 -1.3 0.0
			InitialVelocityJitter	0.5 0.3 0.5

			Scale			.5 .5 .5

			Spin			0.0 0.0 0.0
			SpinJitter		.25 .25 .25

			Gravity			0.08 
			Drag			0.01

#			physics 		1
#			physRadius 		1.8
#
#			StartColor		200 150 190
#
#			Gravity			.05
#			physGravity		1.2
#			physRestitution 	0.1
#			physSFriction 	 	0.99
#			physDFriction 	 	0.9
##			PhysDebris		1
		End
		
		Geom	_cvsmhlls_rock_xsml_02
		Geom	_cvsmhlls_rock_xsml_01

#		LifespanJitter	15
		
		CThresh 	.01
	
	End

End


End				