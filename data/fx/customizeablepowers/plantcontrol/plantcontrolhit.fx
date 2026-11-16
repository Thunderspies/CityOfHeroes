#############################################################
## PlantControlHit.fx
#############################################################

FxInfo

Lifespan 20

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Chest
		Part1	Powers\MartialArts\MartialArtsHit.part
		Part2	Powers\MartialArts\MartialArtsHit1.part
		Part3	Powers\MartialArts\MartialArtsHit2.part
		Part4	Powers\MartialArts\MartialArtsHitRaysSmall.part
	End

	Event
		Type	local
		At	Chest
		Part1	Powers\MartialArts\MAHitStar01Sml.part
		Part2	Powers\MartialArts\MAHitStar02Sml.part
		Part3	Powers\MartialArts\MAHitStar03Sml.part
		Part4	Powers\MartialArts\MAHitGlow01Sml.part
		Part5	Powers\MartialArts\MAHitRays01Sml.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Fire5
		Type	Local
		At	chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\PlantControl\HitDust.part
		Part	CustomizeablePowers\PlantControl\HitDust2.part
		Part	CustomizeablePowers\PlantControl\Hitleaves1.part
		Part	CustomizeablePowers\PlantControl\Hitleaves2.part
		Part	CustomizeablePowers\PlantControl\Hitleaves3.part
		Part	CustomizeablePowers\PlantControl\Hitleaves4.part
		Sound spinyurchin4 60 60 0.83
		Lifespan 10
	End
End

Condition
	On 	Time
	Time	5
	Repeat		2
	RepeatJitter	1

	Event
		Type	start
		At	Chest
		Bhvr	CustomizeablePowers\PlantControl\GeoLeaf.bhvr
		BhvrOverride
			scale			0.4 0.4 0.4
			SpinJitter		0.2 0.3 0.1
			StartJitter		0.5 0.5 0.5
			InitialVelocity		0.0 0.2 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		0.4
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysDebris		1
			PhysDensity		2
			TintGeom		1
		End
		Geom	Tintable_Geo_leaf_maple
		Geom	Tintable_Geo_leaf_oak
		Geom	Tintable_Geo_leaf_round
		Geom	Tintable_Geo_leaf_oval
		Lifespan 3
	End

	Event
		HardwareOnly
		Type	start
		At	Chest
		Bhvr	CustomizeablePowers\PlantControl\GeoLeaf.bhvr
		BhvrOverride
			scale			0.4 0.4 0.4
			SpinJitter		0.2 0.3 0.1
			StartJitter		0.5 0.5 0.5
			InitialVelocity		0.0 0.2 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		0.4
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150
			PhysDebris		1
			PhysDensity		2
			TintGeom		1
		End
		Geom	Tintable_Geo_leaf_maple
		Geom	Tintable_Geo_leaf_oak
		Geom	Tintable_Geo_leaf_round
		Geom	Tintable_Geo_leaf_oval
		Lifespan 3
	End
End

#############################################################

End