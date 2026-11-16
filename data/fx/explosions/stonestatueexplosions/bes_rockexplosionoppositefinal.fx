#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150

#Condition
#
#	Event
#		Type	start 
#		At	origin
#		
##		BhvrOverride	
##			PositionOffset	0 2 0
##
##		End
#		
#		Part	:RockSparks.part
#		Part	:RockExplosionBig.part
#		Part	:RockExplosion01.part
#		Part	:RockExplosion02.part
#		Part	:RockBits.part
#		Part	:RockBits2.part
##		Sound	graniteexplo5 80 30 .9
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
			Scale			.45 .45 .45
			Physdebris	1 #3

			StartColor		100 100 100
			Gravity			.048
#			InitialVelocityJitter	.25 .35 .25
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
#			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
		#Part1	Powers/FireControl/BurningMeteor.part

	End

End

End