#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150

Condition

#	Event
#		Type	start 
#		At	chest
#		
#		childFx	Explosions\StoneStatueExplosions\SmallStatueExplosionLow.Fx
#
#	End

	Event
		Type	start 
		At	chest
		
		childFx	Explosions\StoneStatueExplosions\SmallStatueExplosionMiddle.Fx

	End

End

Condition

	Event
		Type	start 
		At	chest
		
		BhvrOverride	
			PositionOffset	0 8 0
			FadeOutLength		100
		End
		
		Part	:RockSparks.part
		Part	:RockExplosionBig.part
		Part	:RockExplosion01.part
		Part	:RockExplosion02.part
		Part	:RockBits.part
		Part	:RockBits2.part
		Sound graniteexplo4 80 80 .9
	
		LifeSpan	5
	End



End

Condition
	Time 	0
	Chance	.9

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
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
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
			PositionOffset	0 8 0
		End

		ChildFx V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
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
			PositionOffset	0 8 0
		End

		ChildFx V_COV\PhysicsEnabled\RockChunkBrownScatter.fx
		Lifespan	1130
	End

End

Condition
	Time 	0
	Chance	.9

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
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
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
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
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
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
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
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
			Gravity			.048
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
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