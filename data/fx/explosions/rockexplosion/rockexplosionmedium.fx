#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150

Condition
	On 	Time
	Time 	0

	Event
		Type	start 
		At	chest

		
		BhvrOverride
			StartJitter		0.5 0.5 0.5
			PositionOffset		0 2.5 0
		End

		childFx	Explosions\RockExplosion\RockExplosionTop.Fx
		
	End



End

Condition
	On 	Time
	Time 	0

	Event
		Type	start 
		At	chest
		
		Bhvroverride
			FadeOutLength		1110
		End

		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
		Sound Graniteexplo2 80 80 .9
		Sound Graniteexplo3 80 80 .9
		Sound Graniteexplo2 80 80 .9
		LifeSpan	5
	End

End


Condition
	Time 	0
	Repeat	8

	Event
		Type	start
		At	chest
		BhvrOverride
			StartJitter		1 1 1
		End
		ChildFx V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx
		Lifespan	1130
		LifespanJitter	130
	End
	
	Event
		Type	start
		At	chest
		BhvrOverride
			StartJitter		1 1 1
		End
		ChildFx V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan	1130
		LifespanJitter	130
	End

	
	Event
		HardwareOnly
		Type	start
		At	chest
		BhvrOverride
			StartJitter		1 1 1
		End
		ChildFx V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx
		Lifespan	1130
		LifespanJitter	130
	End
	
#	Event
#		HardwareOnly
#		Type	start
#		At	chest
#		BhvrOverride
#			StartJitter		1 1 1
#		End
#		ChildFx V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
#		Lifespan	1130
#		LifespanJitter	130
#	End

End

Condition
	Time 	0
	Repeat	6

	Event
		Type	start
		At	chest
		BhvrOverride
			StartJitter		1.5 1 1.5
		End
		ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan	1130
		LifespanJitter	130
	End
	
	Event
		hardwareOnly
		Type	start
		At	chest
		BhvrOverride
			StartJitter		1.5 1 1.5
		End
		ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan	1130
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	7
	Random	1

	Event
		Type	start
		At	chest
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			StartJitter		0.5 0.5 0.5
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	chest
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			StartJitter		0.5 0.5 0.5
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	chest
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			StartJitter		0.5 0.5 0.5
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	chest
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			StartJitter		0.5 0.5 0.5
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	chest
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			StartJitter		0.5 0.5 0.5
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		110
			PhysDebris		1
			PhysDensity		2
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

End


End			