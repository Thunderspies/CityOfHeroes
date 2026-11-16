#########################################################
##	onfire
##
FxInfo  

LifeSpan	1100

Condition
	On 	Time
	Time 	0

	Event
		Type	start 
		At	chest
		
		Sound ParkingMeterExplo2 80 80 .9
		Sound ParkingMeterExplo 80 80 .9
		Sound ParkingMeterExplo3 80 80 .9
		Sound ParkingMeterExplo2 80 80 .9
		LifeSpan	5
	End

	Event
		Type	start
		At	Root
		Geom	Broken_Fx_Payphone
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			StartJitter		0 0 0		
			scale			1 1 1

			InitialVelocity		0 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25

			StartColor		140 140 140
			PyrRotateJitter		0 0 0
			SpinJitter		.2 .1 .2

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		510

			PhysDebris		3
		End
#		CEvent 	Explosions\ChangeExplosion\ChangeExplosionLite.Fx	
#		CThresh 0.0001

		Lifespan	10
	End

	Event
		Type	start
		At	Root
		Geom	Broken_Fx_phonebook
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			PositionOffset		0 2 0

			StartJitter		0 0 0		
			scale			1.2 1.2 1.2

			InitialVelocity		0 0.4 0
			InitialVelocityJitter	0.25 0.25 0.25

			StartColor		140 140 140
#			PyrRotateJitter		180 180 180

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		510

			PhysDebris		1
			PhysDensity		2
		End

		CThresh 0.0001

		Lifespan	20
		LifespanJitter	13
	End

	Event
		Type	start
		At	Root
		BhvrOverride
			PositionOffset		0 3 0
		End

		ChildFx	Explosions\ChangeExplosion\ChangeExplosion.Fx

		LifeSpan	600

	End


	Event
		Type	start
		At	Root
		BhvrOverride
			PositionOffset		0 3 0
		End

		ChildFx	Explosions\ChangeExplosion\ChangeExplosion.Fx

		LifeSpan	600

	End


End			