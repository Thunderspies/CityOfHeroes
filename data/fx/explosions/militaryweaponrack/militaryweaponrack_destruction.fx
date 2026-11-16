#############################################################
## Archaic Weapon Rack - Destruction FX
#############################################################

FxInfo

LifeSpan 900

## OFFSETS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	OffsetAxe
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 3.75 0.75
			PyrRotate	-90 0 0
		End
		Lifespan 15
	End

	Event
		EName 	OffsetMace
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	2.0 3.75 0.75
			PyrRotate	90 0 0
		End
		Lifespan 15
	End

	Event
		EName 	OffsetBroadsword
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	1.0 3.75 0.75
			PyrRotate	-90 0 0
		End
		Lifespan 15
	End


	Event
		EName 	OffsetRod
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-1.0 3.75 0.75
			PyrRotate	90 0 0
		End
		Lifespan 15
	End


	Event
		EName 	OffsetSledgehammer
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-2.0 3.75 0.75
			PyrRotate	90 0 0
		End
		Lifespan 15
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	start
		At	OffsetAxe
		Part	Explosions\WoodExplosion\WoodExplosion.part
		Part	Explosions\WoodExplosion\WoodExplosion01.part
		Part	Explosions\WoodExplosion\WoodExplosion02.part
		LifeSpan 2
	End
End

Condition
	On 	Time
	Time 	1

	Event
		EName 	Prime
		Type	start
		At	OffsetAxe
		Part	Explosions\WoodExplosion\SawDust01.part
		Part	Explosions\WoodExplosion\SawDust02.part
		Part	Explosions\WoodExplosion\SawdustLarge.part
		Sound WeaponRack1 100.0 100.0 .8
		LifeSpan 2
	End
End

#############################################################

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
		Lifespan	2
		LifespanJitter	3
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
		Lifespan	2
		LifespanJitter	3
	End
End

Condition
	Time 	3
	Repeat	25
	RepeatJitter	5

	Event
		Type	start
		At	chest
		BhvrOverride
			StartJitter		.75 .75 .75
			StartColor		140 140 140
			Scale			.15 .15 .15
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
		Lifespan	2
		LifespanJitter	3
	End
End

## WEAPONS ###########################################################

Condition
	Time 	1

	Event
		EName	Mace
		Type	start
		At	OffsetMace
		BhvrOverride
			Scale			1.0 1.0 1.0
			InitialVelocity		0.0 0.1 0.0
			InitialVelocityJitter	0.1 0.1 0.1
			SpinJitter		0.05 0.05 0.05
			physics 		1
			physRadius 		1
			physGravity 		1
			Drag			0.004
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			PhysScale		1.0 1.0 1.0
			PhysDensity		15
			PhysDebris		1
		End
		Geom	GEO_WepR_Mace_1
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	2
		LifespanJitter	3
	End

	Event
		EName	Broadsword
		Type	start
		At	OffsetBroadsword
		BhvrOverride
			Scale			1.0 1.0 1.0
			InitialVelocity		0.0 0.1 0.0
			InitialVelocityJitter	0.1 0.1 0.1
			SpinJitter		0.05 0.05 0.05
			physics 		1
			physRadius 		1
			physGravity 		1
			Drag			0.004
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			PhysScale		1.0 1.0 1.0
			PhysDensity		15
			PhysDebris		1
		End
		Geom	GEO_WepR_Sword_1
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	2
		LifespanJitter	3
	End

	Event
		EName	Axe
		Type	start
		At	OffsetAxe
		BhvrOverride
			Scale			1.0 1.0 1.0
			InitialVelocity		0.0 0.1 0.0
			InitialVelocityJitter	0.1 0.1 0.1
			SpinJitter		0.05 0.05 0.05
			physics 		1
			physRadius 		1
			physGravity 		1
			Drag			0.004
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			PhysScale		1.0 1.0 1.0
			PhysDensity		15
			PhysDebris		11
		End
		Geom	GEO_WepR_Axe_1
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	2
		LifespanJitter	3
	End

	Event
		EName	Rod
		Type	start
		At	OffsetRod
		BhvrOverride
			Scale			1.0 1.0 1.0
			InitialVelocity		0.0 0.1 0.0
			InitialVelocityJitter	0.1 0.1 0.1
			SpinJitter		0.05 0.05 0.05
			physics 		1
			physRadius 		1
			physGravity 		1
			Drag			0.004
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			PhysScale		1.0 1.0 1.0
			PhysDensity		15
			PhysDebris		1
		End
		Geom	GEO_WepR_Staff_1
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	2
		LifespanJitter	3
	End

	Event
		EName	Sledgehammer
		Type	start
		At	OffsetSledgehammer
		BhvrOverride
			Scale			1.0 1.0 1.0
			InitialVelocity		0.0 0.1 0.0
			InitialVelocityJitter	0.1 0.1 0.1
			SpinJitter		0.05 0.05 0.05
			physics 		1
			physRadius 		1
			physGravity 		1
			Drag			0.004
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			PhysScale		1.0 1.0 1.0
			PhysDensity		15
			PhysDebris		1
		End
		Geom	GEO_WepR_Sledgehammer_1
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	2
		LifespanJitter	3
	End
End

#############################################################

End