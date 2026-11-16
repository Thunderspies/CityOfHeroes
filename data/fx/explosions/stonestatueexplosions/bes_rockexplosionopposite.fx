#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150


Condition
	Repeat	13
	Time 	0
	Random	1

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
#		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			Drag			0.004
			
			InitialVelocity		0.15 .5 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			3 3 3
			SpinJitter		1 1 1

			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.15 .15 .15
			Physdebris	1 #3

			StartColor		150 150 150
			Gravity			.048
			StartColor		100 100 100
		End
		#Part1	Powers/FireControl/BurningMeteor.part

	End

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
#		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			Drag			0.004
			
			InitialVelocity		0.15 .5 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			3 3 3
			SpinJitter		1 1 1

			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.35 .35 .35
			Physdebris		1 #3

			StartColor		190 190 190
			Gravity			.048
			StartColor		100 100 100
		End
		#Part1	Powers/FireControl/BurningMeteor.part

	End

End

End