#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150


Condition

	Event
		Type	start 
		At	origin
		
#		BhvrOverride	
#			PositionOffset	0 12 0
#
#		End
		
		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
		Sound graniteexplo5 80 80 .9
	
		LifeSpan	5
	End



End

Condition
	Time 	0

	Event
		Ename	Hookup
		Type	Local
		At	root

		Geom	obelisk_Hookup

	End

	Event
		Ename	Hookup2
		Type	Local
		At	root

		Geom	obelisk_Hookup01

	End
	
	Event
		Type	start
		At	Hookup2
		Altpiv	4
		Geom	_V_Dest_Bes_Obelisk_Piece04
		
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris	1 #2
			Gravity			.048
			InitialVelocity		0 .05 .25
			InitialVelocityJitter	.15 .05 .05
			
			StartColor		95 95 20
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

	End

	Event
		Type	start
		At	Hookup2
		Altpiv	3
		Geom	_V_Dest_Bes_Obelisk_Piece03
		
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris	1 #2
			Gravity			.048
			InitialVelocity		0 .05 .25
			InitialVelocityJitter	.15 .05 .05
			
			StartColor		95 95 20
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

	End


	Event
		Type	start
		At	Hookup2
		Altpiv	2
		Geom	_V_Dest_Bes_Obelisk_Piece02
		
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris	1 #2
			Gravity			.048
			InitialVelocity		0 .05 .25
			InitialVelocityJitter	.15 .05 .05
			
			StartColor		95 95 20
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

	End


	Event
		Type	start
		At	Hookup2
		Altpiv	1
		Geom	_V_Dest_Bes_Obelisk_Piece01
		
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			Physdebris	1 #2
			Gravity			.048
			InitialVelocity		0 .05 .25
			InitialVelocityJitter	.15 .05 .05
			
			StartColor		95 95 20
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

	End

End

####################################################################################

Condition
	Repeat	3
	Time 	0
#	Chance	.9

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
#		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Scale			.35 .35 .35
			Physdebris	1 #3
			Gravity			.048
			InitialVelocityJitter	.25 .15 .25
			
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

	End

End

Condition
	Time 	0
	Repeat	3

		
	Event
		Type	start
		At	Hookup
		Altpiv	15

		BhvrOverride
			FadeOutLength		1110	
			PositionOffset	0 12 0
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
	Repeat	3

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Scale			.85 .85 .85
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .15 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Time 	0
	Repeat	3

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Scale			.75 .75 .75
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .15 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Time 	0
	Repeat	3

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Scale			.65 .65 .65
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .15 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Time 	0
	Repeat	3

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Scale			.55 .55 .55
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .15 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

#####################################################################################3
##Higer Fill in rocks
##################################

Condition
	Repeat	3
	Time 	0
#	Chance	.9

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
#		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 32 0
			FadeOutLength		1110
			Scale			.35 .35 .35
			Physdebris	1 #3
			Gravity			.048
			InitialVelocityJitter	.25 .15 .25
			
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

	End

End

Condition
	Time 	0
	Repeat	3

		
	Event
		Type	start
		At	Hookup
		Altpiv	15

		BhvrOverride
			FadeOutLength		1110	
			PositionOffset	0 32 0
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
	Repeat	3

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 32 0
			FadeOutLength		1110
			Scale			.45 .45 .45
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .15 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Time 	0
	Repeat	3

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 32 0
			FadeOutLength		1110
			Scale			.25 .25 .25
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .15 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Time 	0
	Repeat	3

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 32 0
			FadeOutLength		1110
			Scale			.35 .35 .35
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .15 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

Condition
	Time 	0
	Repeat	3

	Event
		Type	start
		At	Hookup
		Altpiv	15
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			StartJitter		4 4 4
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 32 0
			FadeOutLength		1110
			Scale			.15 .15 .15
			Physdebris	1 #3
			Gravity		.048
			InitialVelocityJitter	.25 .15 .25
			StartColor		100 100 100
		End
		
		ChildFx		:DustTrailLightest.Fx

		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
	End

End

#####################################################################################3

Condition

	Event
		Type	start 
		At	Hookup
		Altpiv	13

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionFinal.Fx

	End

	Event
		Type	start 
		At	Hookup
		Altpiv	13
				
		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
#		Sound	graniteexplo5 80 30 .9
	
		LifeSpan	5
	End

####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	2

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionFinal.Fx


	End

	Event
		Type	start 
		At	Hookup
		Altpiv	2
				
		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
#		Sound	graniteexplo5 80 30 .9
	
		LifeSpan	5
	End

############################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	7

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionFinal.Fx


	End

	Event
		Type	start 
		At	Hookup
		Altpiv	7
				
		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
#		Sound	graniteexplo5 80 30 .9
	
		LifeSpan	5
	End

End


#############################################################################

#####################################################################################3

Condition

	Event
		Type	start 
		At	Hookup
		Altpiv	9

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionOppositeFinal.Fx

	End

	Event
		Type	start 
		At	Hookup
		Altpiv	9
				
		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
#		Sound	graniteexplo5 80 30 .9
	
		LifeSpan	5
	End

####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	10

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionOppositeFinal.Fx

	End

	Event
		Type	start 
		At	Hookup
		Altpiv	10
				
		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
#		Sound	graniteexplo5 80 30 .9
	
		LifeSpan	5
	End

############################################################
##TopRocks
####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	3

		ChildFx	Explosions\StonestatueExplosions\Top_Bes_RockExplosionFinal.Fx

	End

	Event
		Type	start 
		At	Hookup
		Altpiv	3
				
		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
#		Sound	graniteexplo5 80 30 .9
	
		LifeSpan	5
	End


####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	8

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosion2OppositeFinal.Fx

	End

	Event
		Type	start 
		At	Hookup
		Altpiv	8
				
		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
#		Sound	graniteexplo5 80 30 .9
	
		LifeSpan	5
	End
End

