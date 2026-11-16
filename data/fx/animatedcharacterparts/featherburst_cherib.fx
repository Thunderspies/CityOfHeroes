
############################################################################
FxInfo  

Condition
Chance	.5

#	Repeat	1
	repeatJitter	1

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.4 .4 .4
			SpinJitter		.2 .3 .1
			StartJitter		.5 .5 .5
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	.4 .3 .4
			PyrRotateJitter		180 180 180
			Drag			0.1
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
Chance	.5

#	Repeat	1
	repeatJitter	1
	

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.4 .4 .4
			SpinJitter		.2 .3 .1
#			StartJitter		12 20 12
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	.4 .3 .4
			PyrRotateJitter		180 180 180
			Drag			0.1
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
Chance	.5

#	Repeat	1
	repeatJitter	1

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather
		
		BhvrOverride
			scale			.4 .4 .4
			SpinJitter		.1 .1 .1
#			StartJitter		10 15 10
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	.4 .3 .4
			PyrRotateJitter		180 180 180
			Drag			0.1
			physics 		1
			physRadius 		1.8
			physGravity 		1
			Gravity			0.04
			physRestitution 	0.1
			physSFriction 	 	0.95
			physDFriction 	 	0.8
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End

Condition
Chance	.5

#	Repeat	2
	repeatJitter	1

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.4 .4 .4
			SpinJitter		.2 .3 .1
#			StartJitter		12 20 12
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	.4 .3 .4
			PyrRotateJitter		180 180 180
			Drag			0.1
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
Chance	.5
	
#	Repeat	2
	repeatJitter	1

	Event
		HardwareOnly
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.4 .4 .4
			SpinJitter		.2 .3 .1
#			StartJitter		12 20 12
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	.4 .3 .4
			PyrRotateJitter		180 180 180
			Drag			0.1
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End


Condition
Chance	.5
	
#	Repeat	2
	repeatJitter	1

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather
		
		BhvrOverride
			scale			.4 .4 .4
			SpinJitter		.1 .1 .1
#			StartJitter		10 15 10
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	.4 .3 .4
			PyrRotateJitter		180 180 180
			Drag			0.1
			physics 		1
			physRadius 		1.8
			physGravity 		1
			Gravity			0.04
			physRestitution 	0.1
			physSFriction 	 	0.95
			physDFriction 	 	0.8
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End


Condition
Chance	.5
	
#	Repeat	2
	repeatJitter	1

	Event
		Type	start
		At	back

		Geom	LightFeather
		Geom	BentFeather
		Geom	SmallFeather

		
		BhvrOverride
			scale			.4 .4 .4
			SpinJitter		.2 .3 .1
#			StartJitter		12 20 12
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	.4 .3 .4
			PyrRotateJitter		180 180 180
			Drag			0.1
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			
			physScale		2 2 2

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

End			