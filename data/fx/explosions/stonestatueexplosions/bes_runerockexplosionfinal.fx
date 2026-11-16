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
#	Chance	.9

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
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.35 .35 .35
			Physdebris	1 #3
			Gravity			.048
			InitialVelocityJitter	.25 .35 .25
			
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

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
			PositionOffset		0 2 0
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
	Chance	.9

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
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.85 .85 .85
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Time 	0
	Chance	.9

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
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.75 .75 .75
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Time 	0
	Chance	.9

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
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.65 .65 .65
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Time 	0
	Chance	.9

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
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.55 .55 .55
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End


#####################################################
##Hardware

Condition
	Repeat	11
	Time 	0
#	Chance	.9

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
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.75 .75 .75
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Repeat	11
	Time 	0
#	Chance	.9

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
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 2 0
			FadeOutLength		1110
			Scale			.5 .5 .5
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .35 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

End