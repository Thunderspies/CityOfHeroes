#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150

Condition

	Event
		Type	start 
		At	chest
		
		BhvrOverride	
			FadeOutLength		1100
		End

		Part	Explosions\StoneStatueExplosionsRockSparks.part
		Part	Explosions\StoneStatueExplosionsRockExplosionBig.part
		Part	Explosions\StoneStatueExplosionsRockExplosion01.part
		Part	Explosions\StoneStatueExplosionsRockExplosion02.part
		
		LifeSpan	5
	End



End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	11

	Event
		Type	start
		At	chest
		
		BhvrOverride	
			FadeOutLength		1110
		End

		ChildFx V_COV\PhysicsEnabled\RockDebrisDarkScatter.fx
		Lifespan	1130
	End

End

Condition
	Time 	0
	Repeat	3

	Event
		Type	start
		At	chest
		
		BhvrOverride	
			FadeOutLength		1110
		End

		ChildFx V_COV\PhysicsEnabled\RockChunkGrayScatter.fx
		Lifespan	1130
	End

	Event
		HardwareOnly
		Type	start
		At	chest
		
		BhvrOverride	
			FadeOutLength		1110
		End

		ChildFx V_COV\PhysicsEnabled\RockChunkGrayScatter.fx
		Lifespan	1130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

#########################################################################
##Lowest


Condition

	Event
		Type	start 
		At	chest
		
		BhvrOverride	
			FadeOutLength		1100
		End

		Part	Explosions\StoneStatueExplosionsRockSparks.part
		Part	Explosions\StoneStatueExplosionsRockExplosionBig.part
		Part	Explosions\StoneStatueExplosionsRockExplosion01.part
		Part	Explosions\StoneStatueExplosionsRockExplosion02.part
		
		LifeSpan	5
	End



End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	11

	Event
		Type	start
		At	chest
		
		BhvrOverride	
			FadeOutLength		1110
		End

		ChildFx V_COV\PhysicsEnabled\RockDebrisDarkScatter.fx
		Lifespan	1130
	End

End

Condition
	Time 	0
	Repeat	3

	Event
		Type	start
		At	chest
		
		BhvrOverride	
			FadeOutLength		1110
		End

		ChildFx V_COV\PhysicsEnabled\RockChunkGrayScatter.fx
		Lifespan	1130
	End

	
	Event
		HardwareOnly
		Type	start
		At	chest
		
		BhvrOverride	
			FadeOutLength		1110
		End

		ChildFx V_COV\PhysicsEnabled\RockChunkGrayScatter.fx
		Lifespan	1130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			Scale			.1 .1 .1
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			FadeOutLength		1110
			StartColor		200 210 255
			PhysDebris		3
			PhysDensity		2
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End

End			