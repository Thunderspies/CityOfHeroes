#########################################################
##	onfire
##
FxInfo  

OnForceRadius 10


Condition
	On 	Force
	ForceThreshold	25
	DoMany 1
	Time		6

	Random	1
	
	Repeat	1
	RepeatJitter	2

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.5 0.3 0.5
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

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.5 0.3 0.5
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

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.5 0.3 0.5
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

Condition
	On 	Force
	ForceThreshold	120
	DoMany 1
	Time		12

	Random	1
	
	Repeat	3
	RepeatJitter	2

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.1 1.1 1.1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.55 .0
			InitialVelocityJitter	0.65 0.4 0.65
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

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.1 1.1 1.1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.55 .0
			InitialVelocityJitter	0.65 0.4 0.65
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

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.1 1.1 1.1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.55 .0
			InitialVelocityJitter	0.65 0.4 0.65
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

Condition
	On 	Force
	ForceThreshold	250
	DoMany 1
	Time		23

	Random	1
	
	Repeat	5
	RepeatJitter	2

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.2 1.2 1.2
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.65 .0
			InitialVelocityJitter	.8 .5 .8
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

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.2 1.2 1.2
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.65 .0
			InitialVelocityJitter	.8 .5 .8
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

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.2 1.2 1.2
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.65 .0
			InitialVelocityJitter	.8 .5 .8
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


Condition
	On 	Force
	ForceThreshold	450
	DoMany 1
	Time		31

	Random	1
	
	Repeat	7
	RepeatJitter	2

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.3 1.3 1.3
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.75 .0
			InitialVelocityJitter	1 .75 1
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

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.3 1.3 1.3
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.75 .0
			InitialVelocityJitter	1 .75 1
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

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.3 1.3 1.3
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.75 .0
			InitialVelocityJitter	1 .75 1
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

##### Hardware Proliferation   ############3


Condition
	On 	Force
	ForceThreshold	25
	DoMany 1
	Time		6

	#Random	1
	
	Repeat	0
	RepeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.5 0.3 0.5
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
		HardwareOnly
	End

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.5 0.3 0.5
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
		HardwareOnly
	End

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.5 0.3 0.5
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
		HardwareOnly
	End

End

Condition
	On 	Force
	ForceThreshold	120
	DoMany 1
	Time		12

	#Random	1
	
	Repeat	2
	RepeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.1 1.1 1.1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.55 .0
			InitialVelocityJitter	0.65 0.4 0.65
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
		HardwareOnly
	End

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.1 1.1 1.1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.55 .0
			InitialVelocityJitter	0.65 0.4 0.65
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
		HardwareOnly
	End

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.1 1.1 1.1
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.55 .0
			InitialVelocityJitter	0.65 0.4 0.65
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
		HardwareOnly
	End

End

Condition
	On 	Force
	ForceThreshold	250
	DoMany 1
	Time		23

	#Random	1
	
	Repeat	3
	RepeatJitter	2

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.2 1.2 1.2
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.65 .0
			InitialVelocityJitter	.8 .5 .8
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
		HardwareOnly
	End

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.2 1.2 1.2
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.65 .0
			InitialVelocityJitter	.8 .5 .8
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
		HardwareOnly
	End

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.2 1.2 1.2
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.65 .0
			InitialVelocityJitter	.8 .5 .8
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
		HardwareOnly
	End

End


Condition
	On 	Force
	ForceThreshold	450
	DoMany 1
	Time		31

	#Random	1
	
	Repeat	4
	RepeatJitter	2

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.3 1.3 1.3
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.75 .0
			InitialVelocityJitter	1 .75 1
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
		HardwareOnly
	End

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.3 1.3 1.3
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.75 .0
			InitialVelocityJitter	1 .75 1
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
		HardwareOnly
	End

	Event
		Type	start
		At	Origin

		Geom	Geo_straw_hay01
		Geom	Geo_straw_hay02
		Geom	Geo_straw_hay03
		Geom	Geo_straw_hay04

		
		BhvrOverride
			scale			1.3 1.3 1.3
			SpinJitter		.2 .1 .3
			StartJitter		2 4 2
			InitialVelocity		.0 0.75 .0
			InitialVelocityJitter	1 .75 1
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
		HardwareOnly
	End

End



End			