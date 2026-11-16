#############################################################
## Thrown Confetti FX
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type Local
		At Root
		Sound AnimalPack_FeatherBurst 100 100 1
	End

	Event
		EName	ChestAnchor
		Type	Start
		At	Chest
		Part	:Blast.part
		Part	:Smoke_Light.part
		Part	:Smoke_Light_Lingering.part
		Part	:Smoke_Burst.part
		Part	:Feathers_Burst.part
		LifeSpan 	5

	End


	Event
		EName	ChestAnchor
		Type	Start
		At	Chest
		Part	:Smoke_Additive_Lingering.part
		ChildFX	:BodyGlow.fx
		LifeSpan 	20
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	20


	Event
		Type	Start
		At	Hips

		Geom	LightFeatherADD
		Geom	BentFeatherADD

		Lifespan	200
		LifespanJitter	50

		Bhvr	:FeatherBurst.bhvr
		BhvrOverride
			Alpha			255
			StartColor		200 200 235
			Scale			.5 .5 .5
			SpinJitter		.2 .3 .1
			StartJitter		1.0 1.2 1.0
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	.5 .4 .5
			PyrRotateJitter		180 180 180
			Drag			0.05
		End
	End
End

Condition
	On	Time
	Time	0
	Repeat	40


	Event
		Ename	GEO_Leaves
		Type	Start
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			StartJitter		0.15 0.15 0.15
			SpinJitter		.075 .075 .075
			StartJitter		0.25 0.25 0.25
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	.5 .4 .5
			Drag			0.05
			Gravity			0.005
		End
		Part	:Feathers.part
		Lifespan 110
	End
End

#############################################################

End