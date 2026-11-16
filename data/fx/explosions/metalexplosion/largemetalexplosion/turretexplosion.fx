#########################################################
##	onfire
##
FxInfo  

LifeSpan	160

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	chest
		
		Part	:TurretSparks.part
		Part	:TurretExplosionBig.part
		Part	:TurretExplosion01.part
		Part	:TurretExplosion02.part
		Part	:TurretBits.part
		Part	:TurretBits2.part
		Sound turret_explo1 80 80 .86

	End



End

Condition
	Time 	3
	Repeat	11

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	3
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Bhvr	Behaviors\MetalOffset.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	3
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset2.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	3
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset3.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	3
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset4.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	3
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

########################################################################

Condition
	Time 	3
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset6.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	3
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset7.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	3
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset8.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	Time 	3
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\MetalOffset9.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

End

###########################################################################

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