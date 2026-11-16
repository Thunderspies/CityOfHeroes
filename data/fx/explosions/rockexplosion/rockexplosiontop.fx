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
		At	origin
		
		Bhvroverride
			FadeOutLength		1110
		End

		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
		Sound Graniteexplo 80 80 .9
		LifeSpan	5
	End



End


Condition
	Time 	0
	Repeat	12

	Event
		Type	start
		At	origin
		BhvrOverride
			StartJitter		1 1 1
		End
		ChildFx V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan	1130
	End
	
	Event
		hardwareOnly
		Type	start
		At	origin
		BhvrOverride
			StartJitter		1 1 1
		End
		ChildFx V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan	1130
	End


End

Condition
	Time 	0
	Repeat	4

	Event
		Type	start
		At	origin
		BhvrOverride
			StartJitter		1 1 1
		End
		ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan	1130
	End
	
	Event
		HardwareOnly
		Type	start
		At	origin
		BhvrOverride
			StartJitter		1 1 1
		End
		ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan	1130
	End

End


Condition
	Time 	0
	Repeat	4
	Random	1

	Event
		Type	start
		At	origin
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
		Lifespan	11
	End

	Event
		Type	start
		At	origin
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
		Lifespan	11
	End

	Event
		Type	start
		At	origin
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
		Lifespan	11
	End

	Event
		Type	start
		At	origin
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
		Lifespan	11
	End

	Event
		Type	start
		At	origin
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
		Lifespan	11
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