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
	
	Repeat	1
	RepeatJitter	2

	Event
		Type	start
		At	chest

		Geom	Geo_leaf_fir

		
		BhvrOverride
			scale			.5 .5 .5
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

			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		3
		End

		Lifespan	3
	End

End

Condition
	Time 	0
	
	Repeat	4
	RepeatJitter	3

	Event
		Type	start
		At	chest

		Geom	Geo_leaf_fir

		
		BhvrOverride
			scale			.3 .3 .3
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

			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		3
		End

		Lifespan	3
	End

End

Condition
	Time 	0
	
	Repeat	7
	RepeatJitter	1

	Event
		Type	start
		At	chest

		Geom	Geo_leaf_fir

		
		BhvrOverride
			scale			.15 .2 .3
			SpinJitter		.2 .3 .1
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

			StartColor		140 140 120

			PhysDebris		1
			PhysDensity		5
		End

		Lifespan	3
	End

End


End			