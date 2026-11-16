#########################################################
##	onfire
##
FxInfo  

OnForceRadius 3


Condition
	On 	Force
	ForceThreshold	25
	DoMany 1
	Time		9
	
	Repeat	1
	RepeatJitter	2

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_oval

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.5 .5 .5
			SpinJitter		.2 .3 .1
			StartJitter		1.5 1 1.5
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
			StartColor		120 160 120

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
	
	Repeat	2
	RepeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_oval

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.6 .6 .6
			SpinJitter		.2 .3 .1
			StartJitter		1.5 1 1.5
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.6 0.4 0.6
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
			StartColor		120 160 120

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

	Event
		HardwareOnly
		Type	start
		At	Origin

		Geom	Geo_leaf_oval

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.6 .6 .6
			SpinJitter		.2 .3 .1
			StartJitter		1.5 1 1.5
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.6 0.4 0.6
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
			StartColor		120 160 120

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
	
	Repeat	3
	RepeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	Geo_leaf_oval

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.7 .7 .7
			SpinJitter		.2 .3 .1
			StartJitter		1.5 1 1.5
			InitialVelocity		.0 0.6 .0
			InitialVelocityJitter	.7 .5 .7
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
			StartColor		120 160 120

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
	
	Event
		HardwareOnly
		Type	start
		At	Origin

		Geom	Geo_leaf_oval

		
		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.7 .7 .7
			SpinJitter		.2 .3 .1
			StartJitter		1.5 1 1.5
			InitialVelocity		.0 0.6 .0
			InitialVelocityJitter	.7 .5 .7
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
			StartColor		120 160 120

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
	Time		20

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
			scale			.2 .2 .2
			SpinJitter		.1 .1 .1
			StartJitter		1.5 1 1.5
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.5 0.3 0.5
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
	
	Time		60
	
	repeatJitter	1

	Event
		Type	start
		At	Origin

		Geom	Geo_Debris_branch_02
		Geom	Geo_Debris_twig_01
		
		BhvrOverride
			scale			.3 .3 .3
			SpinJitter		.1 .1 .1
			StartJitter		1.5 1 1.5
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.5 0.3 0.5
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