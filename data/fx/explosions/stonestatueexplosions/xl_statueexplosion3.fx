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

#############################################
##Leg
###########################

Condition

	Event
		Type	start
		At	Hookup
		Altpiv	3
		
		Geom	StatuePiece_01

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 -5 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.5 .2 0 
			InitialVelocityJitter	0.3 0.3 0.3


			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.8 .8 .8
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
		Geom	StatuePiece_01
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 -8 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.5 .2 0 
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
			
			PositionOffset		0 -8 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.5 .2 0 
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
		Altpiv	3
		Geom	FX_Stone_01
		Geom	StatuePiece_01
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 -12 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.5 .2 0 
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
			
			PositionOffset		0 -12 0

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		.5 .2 0 
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
End

#############################################################
##Head
############################

Condition
	Time 	0
	Repeat	10
	Random	1

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
			
			PositionOffset		0 0 -1

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		-0.2 .3 0  
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
		Altpiv	10
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 0 -1

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		-0.2 .3 0  
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

#############################################################
##Shoulder
############################

Condition
	Time 	0
	Repeat	10
	Random	1

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
			
			PositionOffset		11 -3 11

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		-0.2 .4 0  
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
			
			PositionOffset		11 -3 11

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		-0.2 .4 0  
			InitialVelocityJitter	0.3 0.3 0.3


			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.43 .43 .43
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

		CEvent 	V_COV\PhysicsEnabled\RockDebrisSmallCotPortal.fx
		CThresh .9

		Lifespan	1120
		LifespanJitter	130
	End
End

#############################################################

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
			
			PositionOffset		4 0 18
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
			
			PositionOffset		4 0 18

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

			Scale			.53 .53 .53
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx

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
		Altpiv	8

		Geom	Statue_GloveHand

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 0 0
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

			Scale			.7 .7 .7
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
		Altpiv	8
		Geom	FX_Stone_01
		Geom	StatuePiece_02
		Geom	StatuePiece_03

#		Bhvr	Behaviors\RockOffset.bhvr

		BhvrOverride
			
			StartJitter		1 1 1
			
			PositionOffset		0 0 0
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
			
			PositionOffset		0 0 0

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

			Scale			.13 .13 .13
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