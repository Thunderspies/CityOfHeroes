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
		Geom	StatueHookup
		Sound graniteexplo4 80 80 .9

	End

End

#########################################
##hand
#############################3

Condition

	Event
		Type	start
		At	Hookup
		Altpiv	9
		Geom	Statue_Hand

		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 0 -8

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0.0 .1 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

#			Scale			.53 .53 .53
			Gravity			.048

		End
		
		ChildFx		:DustTrail.fx

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
			
			StartJitter		1 1 1
			
			PositionOffset		0 0 -2

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 .0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

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
			
			StartJitter		1 1 1
			
			PositionOffset		0 0 -2

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

			Scale			.33 .33 .33
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End

#######################################################################

Condition
	Time 	0
	Repeat	10
	Random	1

	Event
		Type	start
		At	Hookup
		Altpiv	9
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 0 -2

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 .0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	9
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 0 -2

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

			Scale			.33 .33 .33
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End

#######################################################################

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
			
			PositionOffset		0 4 0

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

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

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
			
			PositionOffset		0 4 0

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

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.33 .33 .33
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

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
		Altpiv	3
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 4 0

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

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

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
			
			PositionOffset		0 4 0

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

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.33 .33 .33
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End

###########################################################################
##Thigh
################################

Condition
	Time 	0
	Repeat	10
	Random	1

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
			
			PositionOffset		0 4 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.3 .2 .15
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

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
			
			PositionOffset		0 4 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.3 .2 .15
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.33 .33 .33
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

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
		Altpiv	14
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 4 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.3 .2 .15
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	14
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 4 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.3 .2 .15
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.33 .33 .33
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End
#########################################
##Opposite Leg right from back view
#############################3

Condition
	Time 	0
	Repeat	10
	Random	1

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
			
			PositionOffset		0 4 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.2 .1 .15
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

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
			
			PositionOffset		0 4 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.2 .1 .15
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		3 0 3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End

#########################################
##Opposite arm right from back view
#############################3

Condition
	Time 	0
	Repeat	10
	Random	1

	Event
		Type	start
		At	Hookup
		Altpiv	9
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		4 0 11

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0.2 .1 0
			InitialVelocityJitter	0.3 0.3 0.3


			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	Hookup
		Altpiv	9
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		4 0 11

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.2 .1 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End

End		
