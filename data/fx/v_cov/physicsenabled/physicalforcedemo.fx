##############################
###  PHYS-FORCE Template   ###
##############################

FxInfo

LifeSpan	300

########  Debris   #################

Condition
	On	Time
	Time	0
	
	Repeat	5
	RepeatJitter	2

	Event
		Type	start
		At	origin

		Geom	bottle1
		Geom	bottle2
		
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.4 0
			InitialVelocityJitter	0.5 0.35 0.5
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.015

			physics 		1
			physRadius 		0.8
			physGravity 		.75
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.3
			FadeOutLength		510
			PhysDebris		1
		End

		Lifespan	10
		LifespanJitter	5
	End

End

Condition
	On 	Time
	Time 	0
	
	Repeat	3
	RepeatJitter	1

	Event
		Type	start
		At	origin

		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 .5 0
			StartJitter		.25 .25 .25		
			scale			1 1 1

			InitialVelocity		0 0.3 0
			InitialVelocityJitter	0.35 0.25 0.35
			StartColor		200 190 180

			PyrRotateJitter		180 180 180
			spinJitter		.2 .3 .2

			Drag			0.015

			physics 		1
			physRadius 		0.8
			physGravity 		1
			physRestitution 	0.3
			physSFriction 	 	0.4
			physDFriction 	 	0.4
			FadeOutLength		510

			PhysDebris		1
		End

		Lifespan	3000
		LifespanJitter	90
	End

End

#############  COllect   #############

Condition
	On	Time
	Time	30

	Event
		Ename	GravitationalForce
		Type	Local
		At	Head
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		50
			PhysForcePower		200
			PhysForcePowerJitter	25
		End

		Lifespan	60

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		25
			PhysForcePower		25
			PhysForcePowerJitter	5
		End

		Lifespan	30

	End
End

#########  Levitate   ########################

Condition
	On	Time
	Time	60

	Event
		Ename	LevitationForce2
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		75
			PhysForcePowerJitter	10
		End

		Lifespan	600

	End
End

#########  Scatter   ########################

Condition
	On	Time
	Time	150

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Head
		
		BhvrOverride
			PositionOffset		0 7 0

			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		2000
			PhysForcePowerJitter	200
		End

		Lifespan	1

	End

End


End