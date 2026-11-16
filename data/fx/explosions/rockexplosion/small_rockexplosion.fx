#########################################################
##	onfire
##
FxInfo  

LifeSpan	150

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
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
		Sound smallrockexplo2 80 80 .9

	End



End

Condition
	Time 	0
	Repeat	10

	Event
		Type	start
		At	chest
		ChildFx V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan	130
	End

End

Condition
	Time 	0
	Repeat	2

	Event
		Type	start
		At	chest
		ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan	130
	End

	
	Event
		HardwareOnly
		Type	start
		At	chest
		ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan	130
	End

End


Condition
	Time 	0
	Repeat	6
	Random	1

	Event
		Type	start
		At	chest
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\Small_RockOffset.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.2
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
		Bhvr	Behaviors\Small_RockOffset2.bhvr
		BhvrOverride
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
		Bhvr	Behaviors\Small_RockOffset3.bhvr
		BhvrOverride
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
		Bhvr	Behaviors\Small_RockOffset4.bhvr
		BhvrOverride
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
		Bhvr	Behaviors\Small_RockOffset5.bhvr
		BhvrOverride
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

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End	

Condition
	On 	Time
	Time 	120

	Event
		EName 	All
		Type	Destroy 
	End


End


End			