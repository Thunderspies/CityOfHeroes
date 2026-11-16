#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150

Condition

	Event
		Ename	Hookup
		Type	local 
		At	origin		
		Geom	StatueHookup01
		Sound graniteexplo4 80 80 .9

	End

End

#############HEAD and torso
##############################

Condition
	Time 	0

	Event
		Type	start
		At	Hookup
		Altpiv	12

		Geom	Fem_Torso

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			PositionOffset		0 -16 0
			StartJitter		1 1 1

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 -.5
			InitialVelocityJitter	0.3 0.3 0.3

#			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

#			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx


		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	12

		Geom	Fem_Head

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		1 1 1

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 -.5
			InitialVelocityJitter	0.3 0.3 0.3

#			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

#			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
##Leg
#####################

	Event
		Type	start
		At	Hookup
		Altpiv	2

		Geom	StatuePiece_01

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			PositionOffset		0 12 0
			StartJitter		1 1 1

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 -.5
			InitialVelocityJitter	0.3 0.3 0.3

#			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

#			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End

###############################################################

Condition
	Time 	0
	Repeat	10
	Random	1

	Event
		Type	start
		At	Hookup
		Altpiv	1
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		1 1 1

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 -.5
			InitialVelocityJitter	0.3 0.3 0.3

#			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	1
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			PositionOffset		0 2 0
			StartJitter		1 1 1

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 -.5
			InitialVelocityJitter	0.3 0.3 0.3

#			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End

#############################################################

Condition

	Event
		Type	start
		At	Hookup
		Altpiv	1

		BhvrOverride
			PositionOffset		0 1 0
		End

		Part	:RockSparks.part
		Part	:RockExplosionBigA.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
		LifeSpan	15
	End

End

Condition
	Time 	0
	Random	1
	Repeat	10

	Event
		Type	start
		At	Hookup
		Altpiv	2
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
#			PositionOffset		0 3 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	2
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
#			PositionOffset		0 3 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End
#######################################################################
##Left Leg from front view
############################3

Condition
	Time 	0
	Repeat	10
	Random	1


	Event
		Type	start
		At	Hookup
		Altpiv	3
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
#			PositionOffset		0 3 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	3
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
#			PositionOffset		0 3 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	10
	Random	1

	Event
		Type	start
		At	Hookup
		Altpiv	4
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		0 5 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx


		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9
		
		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	4
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		0 5 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	10
	Random	1

	Event
		Type	start
		At	Hookup
		Altpiv	5
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		0 3 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	5
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		0 3 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

End

#######################################################################
##Left arm from front view
############################3

Condition
	Time 	0
	Repeat	10
	Random	1

	Event
		Type	start
		At	Hookup
		Altpiv	7
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		-4 -2 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	7
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		-4 -2 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	10
	Random	1

	Event
		Type	start
		At	Hookup
		Altpiv	8
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		0 -2 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	8
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		0 -2 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

End

#######################################################################
##Right arm from front view
############################3


Condition
	Time 	0
	Repeat	3
	random	1

	Event
		Type	start
		At	Hookup
		Altpiv	10
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		-3 2 3
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	10
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		-3 2 3
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
#########################################

End

Condition
	Time 	0
	Repeat	3
	random	1
	
	Event
		Type	start
		At	Hookup
		Altpiv	10
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		-3 8 3
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
	
	Event
		Type	start
		At	Hookup
		Altpiv	10
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		-3 8 3
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
####################################################
End

Condition
	Time 	0
	Repeat	3
	random	1

	Event
		Type	start
		At	Hookup
		Altpiv	10
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		-3 8 3
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	13
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		1 1 1
			PositionOffset		0 10 5
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	10
	Random	1

	Event
		Type	start
		At	Hookup
		Altpiv	12
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		3 3 3 
			PositionOffset		0 -3 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	12
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			StartJitter		3 3 3 
			PositionOffset		0 -3 0
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.5 .5 .5
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

End

End			