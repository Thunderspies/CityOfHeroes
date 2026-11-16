#############################################################
## RockExplosion.Fx
#############################################################

FxInfo
LifeSpan	300

#############################################################




Condition
	Time 	0
	Repeat	4

	Event
		Type	start
		At	chest
		ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan	1130
	End

	Event
		HardwareOnly
		Type	start
		At	chest
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
		At	chest
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04

		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			Drag			0.004
			InitialVelocity		0.5 1.0 0.5
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
		Lifespan	11
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
			Drag			0.004
			InitialVelocity		0.5 1.0 0.5
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
		Lifespan	11
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
			Drag			0.004
			InitialVelocity		0.5 1.0 0.5
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
		Lifespan	11
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
			Drag			0.004
			InitialVelocity		0.5 1.0 0.5
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
		Lifespan	11
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
			Drag			0.004
			InitialVelocity		0.5 1.0 0.5
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