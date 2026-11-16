#########################################################
##	onfire
##
FxInfo  

OnForceRadius 10


Condition
	On 	Force
	ForceThreshold	20
	DoMany 1
	Time		9
	
	Repeat	2
	RepeatJitter	3

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_oak

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			1 1 1
			SpinJitter		.2 .3 .1
			StartJitter		10 10 10
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
	
	Repeat	4
	RepeatJitter	2

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_oak

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.8 .8 .8
			SpinJitter		.2 .3 .1
			StartJitter		10 10 10
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
	
	Repeat	8
	RepeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_oak

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			1.2 1.2 1.2
			SpinJitter		.2 .3 .1
			StartJitter		10 10 10
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	.9 .5 .9
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

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

	Event
		HardwareOnly
		Type	start
		At	Origin

		Geom	Geo_leaf_oak

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			1.2 1.2 1.2
			SpinJitter		.2 .3 .1
			StartJitter		10 10 10
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	.9 .5 .9
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

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End

############  Branches   ##################3

Condition
	On 	Force
	ForceThreshold	200
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
			scale			.4 .4 .4
			SpinJitter		.1 .1 .1
			StartJitter		7 7 7
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
			StartJitter		7 7 7
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


End			