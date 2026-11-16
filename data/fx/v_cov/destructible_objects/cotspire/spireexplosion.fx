#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150

##########################################################################
###################################################################

Condition

	Event
		Type	start 
		At	Root
		
		BhvrOverride	
			Startjitter	2 3 2
			PositionOffset	0 15 0
			FadeOutLength		100
		End
		
		Part	Explosions\StoneStatueExplosions\RockSparks.part
		Part	Explosions\StoneStatueExplosions\RockExplosionBig.part
		Part	Explosions\StoneStatueExplosions\RockExplosion01.part
		Part	Explosions\StoneStatueExplosions\RockExplosion02.part
		Part	Explosions\StoneStatueExplosions\RockBits.part
		Part	Explosions\StoneStatueExplosions\RockBits2.part
		Sound graniteexplo4 80 80 .9
	
		LifeSpan	5
	End


	
	Event
		Type	start 
		At	Root
		
		BhvrOverride
			PositionOffset	0 10 0
		End

		Part	:Earthdust.part
		Part	:Earthdust2.part
		LifeSpan	5
		
	End

	Event
		Type	start 
		At	Root
		
#		BhvrOverride
#			PositionOffset	0 10 0
#		End

		Part	:Earthdust.part
		Part	:Earthdust2.part
		LifeSpan	5
		
	End

End

Condition
	Time 	0
	Repeat	11

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			Scale			.35 .35 .35
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 15 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 15 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	11

	Event
		Type	start
		At	Root
		
		BhvrOverride	
			Startjitter	2 3 2
			FadeOutLength		1110
			PositionOffset	0 15 0
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
		At	Root
		
		BhvrOverride
			Startjitter	2 3 2
			FadeOutLength		1110	
			PositionOffset	0 15 0
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
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 15 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 15 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 15 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 15 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

########################################################


##########################################################################
###################################################################

Condition

	Event
		Type	start 
		At	Root
		
		BhvrOverride
			Startjitter	2 3 2	
			PositionOffset	0 12 0
			FadeOutLength		100
		End
		
		Part	Explosions\StoneStatueExplosions\RockSparks.part
		Part	Explosions\StoneStatueExplosions\RockExplosionBig.part
		Part	Explosions\StoneStatueExplosions\RockExplosion01.part
		Part	Explosions\StoneStatueExplosions\RockExplosion02.part
		Part	Explosions\StoneStatueExplosions\RockBits.part
		Part	Explosions\StoneStatueExplosions\RockBits2.part
	
		LifeSpan	5
	End



End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	11

	Event
		Type	start
		At	Root
		
		BhvrOverride
			Startjitter	2 3 2	
			FadeOutLength		1110
			PositionOffset	0 12 0
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
		At	Root
		
		BhvrOverride
			Startjitter	2 3 2
			FadeOutLength		1110	
			PositionOffset	0 12 0
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
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 12 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

########################################################################

Condition

	Event
		Type	start 
		At	Root
		
		BhvrOverride	
			Startjitter	2 3 2
			PositionOffset	0 8 0
			FadeOutLength		100
		End
		
		Part	Explosions\StoneStatueExplosions\RockSparks.part
		Part	Explosions\StoneStatueExplosions\RockExplosionBig.part
		Part	Explosions\StoneStatueExplosions\RockExplosion01.part
		Part	Explosions\StoneStatueExplosions\RockExplosion02.part
		Part	Explosions\StoneStatueExplosions\RockBits.part
		Part	Explosions\StoneStatueExplosions\RockBits2.part
	
		LifeSpan	5
	End



End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	11

	Event
		Type	start
		At	Root
		
		BhvrOverride	
			Startjitter	2 3 2
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
		At	Root
		
		BhvrOverride
			Startjitter	2 3 2
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
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 8 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

########################################################################

Condition

	Event
		Type	start 
		At	Root
		
		BhvrOverride	
			Startjitter	2 3 2
			PositionOffset	0 10 0
			FadeOutLength		100
		End
		
		Part	Explosions\StoneStatueExplosions\RockSparks.part
		Part	Explosions\StoneStatueExplosions\RockExplosionBig.part
		Part	Explosions\StoneStatueExplosions\RockExplosion01.part
		Part	Explosions\StoneStatueExplosions\RockExplosion02.part
		Part	Explosions\StoneStatueExplosions\RockBits.part
		Part	Explosions\StoneStatueExplosions\RockBits2.part

	
		LifeSpan	5
	End



End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		
		Bhvr	Behaviors\RockOffset.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 10 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Repeat	11

	Event
		Type	start
		At	Root
		
		BhvrOverride	
			Startjitter	2 3 2
			FadeOutLength		1110
			PositionOffset	0 10 0
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
		At	Root
		
		BhvrOverride
			Startjitter	2 3 2
			FadeOutLength		1110
			PositionOffset	0 10 0
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
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset2.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 10 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset3.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 10 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset4.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 10 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	Root
		Geom	FX_Stone_01
		Geom	FX_Stone_02
		Geom	FX_Stone_03
		Geom	FX_Stone_04
		Bhvr	Behaviors\RockOffset5.bhvr
		BhvrOverride
			InitialVelocityJitter	0.25 .9 0.25
			Startjitter	2 3 2
			Drag			0.004
			physics 		1
			physRadius 		.3
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.9
			PositionOffset	0 10 0
			FadeOutLength		1110
			Physdebris		3
			PhysDensity		2
		End
		Lifespan	1120
		LifespanJitter	130
	End

End



End			