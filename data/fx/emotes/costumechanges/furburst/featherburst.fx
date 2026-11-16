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
		Sound Emote_Confetti 200 200 1
	End

	Event
		EName	ChestAnchor
		Type	Start
		At	Chest
	#	Part	:DustDevil_Dust_Core.part
	#	Part	:DustDevil_Dust_Expanding.part
	#	Part	:DustDevil_Dust_Ground.part
		LifeSpan 	5

	End


	Event
		EName	ChestAnchor
		Type	Start
		At	Chest
	#	Part	:Smoke_Additive_Lingering.part
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
		#Geom	SmallFeather


		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			Alpha			255
			StartColor		200 200 235
			Scale			.5 .5 .5
			SpinJitter		.2 .3 .1
			StartJitter		.5 .5 .5
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	.5 .4 .5
			PyrRotateJitter		180 180 180
			Drag			0.05
			physics 		1
			physRadius 		1.8
			physGravity 		.15
			Gravity			0.005

			#physScale		2 2 2

			PhysDebris		2
			#PhysDensity		2
		End
		Lifespan	80
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