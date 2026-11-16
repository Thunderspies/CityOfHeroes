#########################################################
##	onfire
##
FxInfo  

LifeSpan	1120

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	start 
		At	chest
		
		Part	:WoodExplosion.part
		Part	:WoodExplosion01.part
		Part	:WoodExplosion02.part

		Sound wood_explo1 90 90 .9
		LifeSpan	10
	End



End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	start 
		At	chest
		Part	:SawDust01.part
		Part	:SawDust02.part
		Part	:SawdustLarge.part
		#Sound	FireExplo 100.0 30.0 .8	

		LifeSpan	10
	End

End


Condition
	Time 	3
	Repeat	30
	RepeatJitter	5

	Event
		Type	start
		At	chest
		Geom	woodchip
		#Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		140 140 140
			Scale			.2 .2 .2
			InitialVelocity		.0 0.2 .0
			InitialVelocityJitter	0.6 0.5 0.6
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
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	11
		LifespanJitter	3
	End

	Event
		HardwareOnly
		Type	start
		At	chest
		Geom	woodchip
		#Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		140 140 140
			Scale			.2 .2 .2
			InitialVelocity		.0 0.2 .0
			InitialVelocityJitter	0.6 0.5 0.6
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
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	11
	End
End


Condition
	Time 	2
	Repeat	25
	RepeatJitter	5

	Event
		Type	start
		At	chest
		Geom	woodchip
		#Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		170 170 170
			Scale			.15 .15 .15
			InitialVelocity		.0 .25 .0
			InitialVelocityJitter	0.5 0.4 0.5
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
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	11
		LifespanJitter	3
	End

	
	Event
		HardwareOnly
		Type	start
		At	chest
		Geom	woodchip
		#Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		170 170 170
			Scale			.15 .15 .15
			InitialVelocity		.0 .25 .0
			InitialVelocityJitter	0.5 0.4 0.5
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
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	Time 	1
	Repeat	20
	RepeatJitter	5

	Event
		Type	start
		At	chest
		Geom	woodchip
		#Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		200 200 200
			Scale			.1 .1 .1
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
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	11
		LifespanJitter	3
	End

	
	Event
		HardwareOnly
		Type	start
		At	chest
		Geom	woodchip
		#Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		200 200 200
			Scale			.1 .1 .1
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
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	11
		LifespanJitter	3
	End

End

Condition
	Time 	3
	Repeat	8

	Event
		Type	start
		At	chest
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		
		Bhvr	Behaviors\PHS_StartOffset5.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			PhysScale		1 1.5 1
			StartColor		140 140 140
			PyrRotateJitter		180 180 180
			Scale			.27 .17 .37
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	.8 1.0 .8
			Drag			0.008
			physics 		1
			physRadius 		.5
			physGravity 		1.2
			physRestitution 	0.5
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.1 .1 .1

			PhysDebris		1
			PhysDensity		1.5
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	11
	End

End

Condition
	Time 	3
	Repeat	12

	Event
		Type	start
		At	chest
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		
		Bhvr	Behaviors\PHS_StartOffset.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			PhysScale		1 1.5 1
			StartColor		140 140 140
			PyrRotateJitter		180 180 180
			Scale			.1 .13 .1
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.45 0.45 0.45
			Drag			0.008
			physics 		1
			physRadius 		.5
			physGravity 		1.2
			physRestitution 	0.5
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110

			SpinJitter		.1 .1 .1

			PhysDebris		1
			
			PhysDensity		2.5
		End
		Lifespan	11
	End

End
###########################################################
##Hardware
###################################

Condition
	Time 	3
	Repeat	8

	Event
		HardwareOnly
		Type	start
		At	chest
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		
		Bhvr	Behaviors\PHS_StartOffset5.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			PhysScale		1 1.5 1
			StartColor		140 140 140
			PyrRotateJitter		180 180 180
			Scale			.20 .13 .20
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	.8 1.0 .8
			Drag			0.008
			physics 		1
			physRadius 		.5
			physGravity 		1.2
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.5
			FadeOutLength		1110
			SpinJitter		.1 .1 .1

			PhysDebris		1
			PhysDensity		1.5
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	11
	End

End

Condition
	Time 	3
	Repeat	12

	Event
		HardwareOnly
		Type	start
		At	chest
		Geom	WoodPlank01
		Geom	WoodPlank02
		Geom	WoodPlank03
		Geom	WoodPlank05
		
		Bhvr	Behaviors\PHS_StartOffset.bhvr
		BhvrOverride
			StartJitter		.75 .75 .75
			PhysScale		1 3 1
			StartColor		140 140 140
			PyrRotateJitter		180 180 180
			Scale			.1 .13 .1
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	.8 1.0 .8
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.45 0.45 0.45
			Drag			0.008
			physics 		1
			physRadius 		.5
			physGravity 		1.2
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.5
			FadeOutLength		1110

			SpinJitter		.1 .1 .1

			PhysDebris		1
			
			PhysDensity		2.5
		End
		Lifespan	11
	End

End

End			