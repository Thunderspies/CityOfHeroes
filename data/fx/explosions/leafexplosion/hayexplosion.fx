#########################################################
##	onfire
##
FxInfo  

LifeSpan	600

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	chest
		
		#Sound	CashExplo 80 30 .9

	End

End


Condition
	Time 	0
	
	Repeat	2
	RepeatJitter	2

	Event
		Type	start
		At	chest

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04
	

		
		BhvrOverride
			scale			1.25 1.25 1.25
			SpinJitter		.2 .3 .1
			InitialVelocity		.0 0.2 .0
			InitialVelocityJitter	0.6 0.3 0.6
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			StartColor		200 175 120

			PhysDebris		1
			PhysDensity		5
			PhysScale		3 3 1
		End

		Lifespan	3
	End

End

Condition
	Time 	0
	
	Repeat	3
	RepeatJitter	3

	Event
		Type	start
		At	chest

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .3 .1
			InitialVelocity		.0 0.25 .0
			InitialVelocityJitter	0.75 0.4 0.75
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			StartColor		200 175 120

			PhysDebris		1
			PhysDensity		5
			PhysScale		3 3 1
		End

		Lifespan	3
	End

End

Condition
	Time 	0
	
	Repeat	3
	RepeatJitter	3

	Event
		Type	start
		At	chest

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .3 .1
			InitialVelocity		.0 0.25 .0
			InitialVelocityJitter	0.75 0.4 0.75
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			StartColor		250 225 200

			PhysDebris		1
			PhysDensity		5
			PhysScale		3 3 1
		End

		Lifespan	3
	End

End

Condition
	Time 	0
	
	Repeat	3
	RepeatJitter	3

	Event
		Type	start
		At	chest

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .3 .1
			InitialVelocity		.0 0.25 .0
			InitialVelocityJitter	0.75 0.4 0.75
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		.5
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			StartColor		140 100 80

			PhysDebris		1
			PhysDensity		5
			PhysScale		3 3 1
		End

		Lifespan	3
	End

End

End			