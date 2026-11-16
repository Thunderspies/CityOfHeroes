#########################################################
#	onfire
#
FxInfo  

LifeSpan	1150

#Condition
#
#	Event
#		Type	start 
#		At	origin
#		
#		BhvrOverride	
#			PositionOffset	0 2 0
#
#		End
#		
#		Part	:RockSparks.part
#		Part	:RockExplosionBig.part
#		Part	:RockExplosion01.part
#		Part	:RockExplosion02.part
#		Part	:RockBits.part
#		Part	:RockBits2.part
#		Sound	graniteexplo5 80 30 .9
#	
#		LifeSpan	5
#	End
#
#
#
#End

Condition
	Repeat	6
	Time 	0
	Random	1

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			Drag			0.004
			
			InitialVelocity		0.15 .5 -0.15
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			3 3 3
			SpinJitter		1 1 1

			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.35 .35 .35
			Physdebris	1 #3

			StartColor		100 100 100
			Gravity			.048
			InitialVelocityJitter	.25 .35 .25
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
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			Drag			0.004
			
			InitialVelocity		0.15 .5 -0.15
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			3 3 3
			SpinJitter		1 1 1

			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.65 .65 .65
			Physdebris		1 #3

			StartColor		190 190 190
			Gravity			.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
		#Part1	Powers/FireControl/BurningMeteor.part

	End

End

Condition
	Time 	0
	Repeat	6

		
	Event
		Type	start
		At	origin

		BhvrOverride
			FadeOutLength		1110	
#			PositionOffset		0 2 0
			Gravity			.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			Gravity			.048

		End

		Part1	V_COV\PhysicsEnabled\RockDebrisGold.part
		

	End

End

Condition
	Time 	0
	Repeat	6
	Random	1

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.35 .35 .35
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.7 .7 .7
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End

End

Condition
	Time 	0
	Repeat	6
	Random	1

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.65 .65 .65
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.45 .45 .45
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End
End

Condition
	Time 	0
	Repeat	6
	Random	1

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.65 .65 .65
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.35 .35 .35
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End
End

Condition
	Time 	0
	Repeat	6
	Random	1

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.85 .85 .85
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End

	Event
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.75 .75 .75
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End
End


#####################################################
##Hardware

Condition
	Repeat	6
	Random	1

	Event
		HardwareOnly
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.05 .05 .05
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End

	Event
		HardwareOnly
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.45 .45 .45
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End
End

Condition
	Repeat	6
	Random	1

	Event
		HardwareOnly
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.3 .3 .3
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End

	Event
		HardwareOnly
		Type	start
		At	origin
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
#			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.52 .52 .52
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
	End
End

End