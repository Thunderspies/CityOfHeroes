#########################################################
##	onfire
##
FxInfo  

OnForceRadius 15


Condition
	On 	Force
	ForceThreshold	20
	DoMany	1
	Time	10


	Event
		Type	start
		At	Root

		Geom	Geo_leaf_fir

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.3 .3 .3
			SpinJitter		.2 .3 .1
			PositionOffset		0 10 0
			StartJitter		9 9 9
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	0.2 0.2 0.2
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
			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	120
	DoMany 1
	Time		16

	Repeat	1
	repeatJitter	1
	

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_fir

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.5 .5 .5
			SpinJitter		.2 .3 .1
			PositionOffset		0 10 0
			StartJitter		9 9 9
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	0.4 0.4 0.4
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
			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
	
	Event
		HardwareOnly
		Type	start
		At	Origin

		Geom	Geo_leaf_fir

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.5 .5 .5
			SpinJitter		.2 .3 .1
			PositionOffset		0 10 0
			StartJitter		9 9 9
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	0.4 0.4 0.4
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
			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	On 	Force
	ForceThreshold	150
	DoMany 1
	Time		12

	repeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	Geo_Debris_branch_01
		Geom	Geo_Debris_branch_02
		Geom	Geo_Debris_twig_01
		Geom	Geo_Debris_branch_02
		Geom	Geo_Debris_twig_01
		
		BhvrOverride
			scale			.3 .3 .3
			SpinJitter		.1 .1 .1
			PositionOffset		0 10 0
			StartJitter		9 9 9
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.6 0.3 0.6
			PyrRotateJitter		180 180 180
			Drag			0.005
			physics 		1
			physRadius 		1.8
			physGravity 		1
			Gravity			0.04
			physRestitution 	0.1
			physSFriction 	 	0.95
			physDFriction 	 	0.8
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End

Condition
	On 	Force
	ForceThreshold	250
	DoMany 1
	Time		30
	
	Repeat	4
	repeatJitter	2

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_fir

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.6 .6 .6
			SpinJitter		.2 .3 .1
			PositionOffset		0 10 0
			StartJitter		9 9 9
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	0.6 0.6 0.6
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
			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

	Event
		HardwareOnly
		Type	start
		At	Origin

		Geom	Geo_leaf_fir

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.6 .6 .6
			SpinJitter		.2 .3 .1
			PositionOffset		0 10 0
			StartJitter		9 9 9
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	0.6 0.6 0.6
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
			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End


Condition
	On 	Force
	ForceThreshold	400
	DoMany 1
	
	Time		20
	
	Repeat	1
	repeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	Geo_Debris_branch_01
		Geom	Geo_Debris_branch_02
		Geom	Geo_Debris_twig_01
		Geom	Geo_Debris_twig_02
		
		BhvrOverride
			scale			.6 .6 .6
			SpinJitter		.1 .1 .1
			PositionOffset		0 10 0
			StartJitter		9 9 9
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.6 0.3 0.6
			PyrRotateJitter		180 180 180
			Drag			0.005
			physics 		1
			physRadius 		1.8
			physGravity 		1
			Gravity			0.04
			physRestitution 	0.1
			physSFriction 	 	0.95
			physDFriction 	 	0.8
			FadeOutLength		150

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End


Condition
	On 	Force
	ForceThreshold	500
	DoMany 1
	
	Time		10
	
	Repeat	8
	repeatJitter	4

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_fir

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.7 .7 .7
			SpinJitter		.2 .3 .1
			PositionOffset		0 10 0
			StartJitter		9 9 9
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	1.2 1.2 1.2
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
			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

	Event
		HardwareOnly
		Type	start
		At	Origin

		Geom	Geo_leaf_fir

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.7 .7 .7
			SpinJitter		.2 .3 .1
			PositionOffset		0 10 0
			StartJitter		9 9 9
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	1.2 1.2 1.2
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
			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End

End			