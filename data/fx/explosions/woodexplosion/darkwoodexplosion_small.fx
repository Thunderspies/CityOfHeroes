#############################################################
## DarkWoodExplosion_Small.Fx
#############################################################

FxInfo
LifeSpan	1120

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	start
		At	Hips
		Part	:WoodExplosion.part
		Sound wood_explo2 90 90 .9
		LifeSpan	10
	End
End

Condition
	Time 		1
	Repeat		15
	RepeatJitter	5

	Event
		Type	start
		At	chest
		Geom	Hips
		BhvrOverride
			StartJitter		1.75 2.75 1.75
			StartColor		140 140 140
			Scale			.075 .075 .075
			InitialVelocity		.0 0.5 .0
			InitialVelocityJitter	0.4 0.3 0.4
			PyrRotateJitter		180 180 180
			Drag			0.004
			Gravity			0.04
			physics 		1
			physRadius 		.8
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.1 .1 .1
			PhysScale		2.5 2.5 2.5
			PhysDensity		2
			PhysDebris		1
		End
		Lifespan	1120
		LifespanJitter	3
	End
End

Condition
	Time 		2
	Repeat		20
	RepeatJitter	5

	Event
		Type	start
		At	Hips
		Geom	woodchip
		BhvrOverride
			StartJitter		1.75 2.75 1.75
			StartColor		110 110 110
			Scale			.1 .1 .1
			InitialVelocity		.0 .5 .0
			InitialVelocityJitter	0.35 0.25 0.35
			PyrRotateJitter		180 180 180
			Drag			0.004
			Gravity			0.04
			physics 		1
			physRadius 		.8
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.3 .3 .3
			PhysScale		2 2 2
			PhysDensity		2
			PhysDebris		1
		End
		Lifespan	1120
		LifespanJitter	3
	End
End

Condition
	Time 		3
	Repeat		25
	RepeatJitter	5

	Event
		Type	start
		At	Hips
		Geom	woodchip
		BhvrOverride
			StartJitter		1.75 2.75 1.75
			StartColor		70 70 70
			Scale			.15 .15 .15
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.3 0.2 0.3
			PyrRotateJitter		180 180 180
			Drag			0.004
			Gravity			0.04
			physics 		1
			physRadius 		.9
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.3 .3 .3
			PhysScale		1.5 1.5 1.5
			PhysDensity		2
			PhysDebris		1
		End
		Lifespan	1120
		LifespanJitter	3
	End
End






End