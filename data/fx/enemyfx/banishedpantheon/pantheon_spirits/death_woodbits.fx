#############################################################
## Death_WoodBits.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	start
		At	chest
		Sound telekinetic3 80 80 .9
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	start
		At	chest
		Sound FireExplo 100.0 100.0 .8
		Lifespan 10
	End
End

#############################################################

Condition
	Time 	3
	Repeat	15
	RepeatJitter	5

	Event
		Type	start
		At	chest
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		140 140 140
			Scale			.1 .1 .1
			InitialVelocity		.0 0.2 .0
			InitialVelocityJitter	0.3 0.2 0.3
			PyrRotateJitter		180 180 180
			Drag			0.004
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
		Geom	woodchip
		Lifespan 11
		LifespanJitter	3
	End

	Event
		HardwareOnly
		Type	start
		At	chest
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		140 140 140
			Scale			.1 .1 .1
			InitialVelocity		.0 0.2 .0
			InitialVelocityJitter	0.3 0.2 0.3
			PyrRotateJitter		180 180 180
			Drag			0.004
			physics 		1
			physRadius 		.9
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.3 .3 .3
			PhysScale		1.5 1.5 1.5
			PhysDebris		1
			PhysDensity		2
		End
		Geom	woodchip
		Lifespan 11
	End
End

Condition
	Time 	2
	Repeat	20
	RepeatJitter	5

	Event
		Type	start
		At	chest
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		170 170 170
			Scale			.1 .1 .1
			InitialVelocity		.0 .25 .0
			InitialVelocityJitter	0.35 0.25 0.35
			PyrRotateJitter		180 180 180
			Drag			0.004
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
		Geom	woodchip
		Lifespan 11
		LifespanJitter	3
	End


	Event
		HardwareOnly
		Type	start
		At	chest
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		170 170 170
			Scale			.1 .1 .1
			InitialVelocity		.0 .25 .0
			InitialVelocityJitter	0.35 0.25 0.35
			PyrRotateJitter		180 180 180
			Drag			0.004
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
		Geom	woodchip
		Lifespan 11
		LifespanJitter	3
	End
End

Condition
	Time 	1
	Repeat	15
	RepeatJitter	5

	Event
		Type	start
		At	chest
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		200 200 200
			Scale			.075 .075 .075
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.4 0.3 0.4
			PyrRotateJitter		180 180 180
			Drag			0.004
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
		Geom	woodchip
		Lifespan 11
		LifespanJitter	3
	End

	Event
		HardwareOnly
		Type	start
		At	chest
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		200 200 200
			Scale			.075 .075 .075
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.4 0.3 0.4
			PyrRotateJitter		180 180 180
			Drag			0.004
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
		Geom	woodchip
		Lifespan 11
		LifespanJitter	3
	End
End

Condition
	Time 	3
	Repeat	12
	RepeatJitter	5

	Event
		SoftwareOnly
		Type	start
		At	chest
		BhvrOverride
			PhysScale		1.25 2 1.25
			StartColor		140 140 140
			Scale			.05 .05 .05
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.45 0.45 0.45
			PyrRotateJitter		180 180 180
			Drag			0.004
			physics 		1
			physRadius 		.8
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.1 .1 .1
			PhysDebris		1
			PhysDensity		2.5
		End
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		Lifespan 11
		LifespanJitter	3
	End

	Event
		HardwareOnly
		Type	start
		At	chest
		BhvrOverride
			PhysScale		1.25 2 1.25
			StartColor		140 140 140
			Scale			.05 .05 .05
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.45 0.45 0.45
			PyrRotateJitter		180 180 180
			Drag			0.004
			physics 		1
			physRadius 		.8
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.1 .1 .1

			PhysDebris		1
			PhysDensity		2.5
		End
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		Lifespan 11
	End
End

Condition
	Time 	3
	Repeat	10

	Event
		Type	start
		At	chest
		BhvrOverride
			PhysScale		1.25 2 1.25
			StartColor		140 140 140
			Scale			.05 .05 .05
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.45 0.45 0.45
			PyrRotateJitter		180 180 180
			Drag			0.004
			physics 		1
			physRadius 		.8
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.1 .1 .1
			PhysDebris		1
			PhysDensity		2.5
		End
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		Lifespan 11
	End
End

