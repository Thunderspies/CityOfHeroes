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
#			PositionOffset	0 2 0
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

#######################################################################################3
#### Geom 
###################################

	Event
		Type	start
		At	Hookup
		Altpiv	12
		Geom	Obelisk_CarvedHead

		BhvrOverride
#			PositionOffset		-.1 0 0
			
			Startcolor		35 35 35

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 .2
			InitialVelocityJitter	0.3 0.3 0.1

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.8 .8 .8
			Gravity			.048
			Physdebris		1 #3
		End
		
		ChildFx		:DustTrailLight.fx


		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
	
	Event
		Type	start
		At	Hookup
		Altpiv	11
		Geom	Obelisk_CarvedHead

		BhvrOverride
#			PositionOffset		-.1 0 0
			
			Startcolor		35 35 35

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		-.2 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.2
			SpinJitter		0.3 0.3 0.1

			Scale			.8 .8 .8
			Gravity			.048
			Physdebris		1 #3
		
		End
		
		ChildFx		:DustTrailLight.fx


		#CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End

#####################################################################################3

Condition

	Event
		Type	start 
		At	Hookup
		Altpiv	13

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosion.Fx

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

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosion.Fx

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

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosion.Fx

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

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionOpposite.Fx

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

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionOpposite.Fx

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

		ChildFx	Explosions\StonestatueExplosions\Top_Bes_RockExplosion.Fx

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

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosion2Opposite.Fx

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

