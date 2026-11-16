#########################################################
##	onfire
##
FxInfo  

LifeSpan	1100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	chest
		
		Sound mailboxexplo3 80 80 .9

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	chest
	
		
		Part	:PHS_MetalExplosionBig.part
		
		Part	:PHS_MetalExplosion01.part

		Part	:PHS_MetalExplosion02.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	chest
		
		BhvrOverride
			InitialVelocity		0.0 0.28 0.0		
			InitialVelocityJitter	0.1 0.2 0.1
			Drag			0.006
			physics 		1
			physRadius 		0.12
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			Gravity 		.04
		End

		
		Part	:PHS_MetalBits.part

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	chest
		
		BhvrOverride
		
			InitialVelocity		0.0 0.28 0.0		
			InitialVelocityJitter	0.1 0.2 0.1		
			Drag			0.006
			physics 		1
			physRadius 		0.12
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			Gravity 		.04
		End

		Part	:PHS_MetalBits2.part

	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Bhvr	Behaviors\BlackMetalOffset.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\BlackMetalOffset2.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\BlackMetalOffset3.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\BlackMetalOffset4.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		Bhvr	Behaviors\BlackMetalOffset5.bhvr
		BhvrOverride
			PyrRotateJitter		180 180 180
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End

Condition
	Time 	0
	Repeat	6

	Event
		Type	start
		At	Chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Bhvr	V_COV\PhysicsEnabled\BlackMetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		1110
			PhysDebris		3
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End


End			