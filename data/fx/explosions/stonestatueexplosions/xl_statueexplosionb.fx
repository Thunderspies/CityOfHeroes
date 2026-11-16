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
#####################################
##Left arm
#######################

Condition
	Time 	0
	Repeat	20
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

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 .5
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		0 0 0

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx


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

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 .5
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		0 0 0

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx


		Lifespan	1120
		LifespanJitter	130
	End
End

Condition
	Time 	0
	Repeat	20
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

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 .5
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		0 -4 0

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx


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

			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 .5
			InitialVelocityJitter	0.3 0.3 0.3

			PositionOffset		0 -4 0

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx


		Lifespan	1120
		LifespanJitter	130
	End
End

#############################################################

Condition
	Time 	0
	Repeat	20
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

			Scale			.23 .23 .23
			Gravity			.048
		
		End
		
		ChildFx		:DustTrail.fx


		Lifespan	1120
		LifespanJitter	130
	End
End

#############################################################

Condition

	Event
		Type	start
		At	Hookup
		Altpiv	12

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
	Repeat	20
	random	1

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

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx


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
			PositionOffset		0 3 0
			Drag			0.004
			physics 		1
			physRadius 		1.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.1 .1 .1
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx


		Lifespan	1120
		LifespanJitter	130
	End

End
###################################################
##Left Leg facing back of model
####################################

Condition
	Time 	0
	Repeat	20
	random	1

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

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx


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
			PositionOffset		0 3 0
			Drag			0.004
			physics 		1
			physRadius 		1.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.1 .1 .1
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx


		Lifespan	1120
		LifespanJitter	130
	End

End

#################################################################

Condition
	Time 	0
	Repeat	20
	random	1

	Event
		Type	start
		At	Hookup
		Altpiv	1
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

			Scale			.13 .13 .13
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx


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
			StartJitter		1 1 1
			PositionOffset		0 3 0
			Drag			0.004
			physics 		1
			physRadius 		1.3
			physGravity		1.2
			physRestitution 	1.5
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			FadeOutLength		1110
			InitialVelocity		0 .2 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0.1 0.1 0.1
			SpinJitter		0.3 0.3 0.3

			Scale			.1 .1 .1
			Gravity			.048
		
		End
		
		ChildFx		:DustTrailLight.fx


		Lifespan	1120
		LifespanJitter	130
	End

End

End			