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

###########  Branches  #####################

Condition
	Time 	0
	
	Repeat	2
	RepeatJitter	1

	Event
		Type	start
		At	chest

		Geom	Geo_Debris_branch_01
		Geom	Geo_Debris_branch_02
		Geom	Geo_Debris_twig_01
		Geom	Geo_Debris_twig_02
		
		BhvrOverride
			scale			.7 .7 .7
			SpinJitter		.1 .1 .1
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	1 0.6 1
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

##########  Leaves   ####################

Condition
	Time 	0
	
	Repeat	1
	RepeatJitter	2

	Event
		Type	start
		At	chest

		Geom	Geo_leaf_maple

		
		BhvrOverride
			scale			1.1 1.1 1.1
			SpinJitter		.2 .3 .1
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.9 0.6 0.9
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
			
			StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	Time 	0
	
	Repeat	3
	RepeatJitter	2

	Event
		Type	start
		At	chest

		Geom	Geo_leaf_maple

		
		BhvrOverride
			scale			.9 .9 .9
			SpinJitter		.2 .3 .1
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	1.25 0.75 1.25
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

			
			StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	Time 	0
	
	Repeat	5
	RepeatJitter	2

	Event
		Type	start
		At	chest

		Geom	Geo_leaf_maple

		
		BhvrOverride
			scale			.7 .7 .7
			SpinJitter		.2 .3 .1
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	1.5 1 1.5
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

			
			StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End


End			