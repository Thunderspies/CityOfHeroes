#############################################################
## PlantControlHit.fx
#############################################################

FxInfo

Lifespan	20

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Mallet
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MartialArtsHit.part
		Part2	Powers/MartialArts/MartialArtsHit1.part
		Part3	Powers/MartialArts/MartialArtsHit2.part
		Part4	Powers/MartialArts/MartialArtsHitRaysSmall.part
		Sound spinyurchin4 60 60 .83
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part


	End

	Event
		EName	Fire5
		Type	Local
		At	chest

		BHVR	Behaviors/GenericParticleFade.bhvr

		Part	:HitDust2.part
		Part	:HitDust.part
		Part	:Hitleaves1.part
		Part	:Hitleaves2.part
		Part	:Hitleaves3.part
		Part	:Hitleaves4.part

		Lifespan	10

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

		Geom	Geo_leaf_maple
		Geom	Geo_leaf_oak
		Geom	Geo_leaf_round
		Geom	Geo_leaf_oval


		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.4 .4 .4
			SpinJitter		.2 .3 .1
			StartJitter		.5 .5 .5
			InitialVelocity		.0 0.2 .0
			InitialVelocityJitter	0.25 0.25 0.25
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		.4
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			#StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End

	Event
		HardwareOnly
		Type	start
		At	Chest

		Geom	Geo_leaf_maple
		Geom	Geo_leaf_oak
		Geom	Geo_leaf_round
		Geom	Geo_leaf_oval


		Bhvr	Explosions\LeafExplosion\GeoLeaf.bhvr
		BhvrOverride
			scale			.4 .4 .4
			SpinJitter		.2 .3 .1
			StartJitter		.5 .5 .5
			InitialVelocity		.0 0.2 .0
			InitialVelocityJitter	0.25 0.25 0.25
			PyrRotateJitter		180 180 180
			Drag			0.025
			physics 		1
			physRadius 		1.8
			physGravity 		.4
			Gravity			0.02
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			FadeOutLength		150

			#StartColor		180 160 140

			PhysDebris		1
			PhysDensity		2
		End

		Lifespan	3
	End
End

End