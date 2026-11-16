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
	RepeatJitter	3

	Event
		Type	start
		At	chest

		Geom	Geo_leaf_maple

		
		BhvrOverride
			scale			1.2 1.2 1.2
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

			StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	Time 	0
	
	Repeat	4
	RepeatJitter	2

	Event
		Type	start
		At	chest

		Geom	Geo_leaf_maple

		
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

			StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End

Condition
	Time 	0
	
	Repeat	8
	RepeatJitter	1

	Event
		Type	start
		At	chest

		Geom	Geo_leaf_maple

		
		BhvrOverride
			scale			.8 .8 .8
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

			StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

End


End			