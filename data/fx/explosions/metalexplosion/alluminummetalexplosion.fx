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
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		
		Bhvr	Behaviors\MetalOffset.bhvr
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
			PhysDebris		1
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		Bhvr	Behaviors\MetalOffset2.bhvr
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
			PhysDebris		1
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		Bhvr	Behaviors\MetalOffset3.bhvr
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
			PhysDebris		1
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		Bhvr	Behaviors\MetalOffset4.bhvr
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
			PhysDebris		1
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Chance	.9

	Event
		Type	start
		At	chest
		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		Bhvr	Behaviors\MetalOffset5.bhvr
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
			PhysDebris		1
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End

Condition
	Time 	0
	Repeat	6

	Event
		Type	start
		At	Chest

		Geom	Metal_Flat04_Alluminum
		Geom	Metal_Curl01_Alluminum
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
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
			PhysDebris		1
			PhysDensity		2
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
#		Lifespan	11
	End

End


############################################################
##Hardware Addition   =more parts come out of boxes=

Condition
	Time 	0
	Repeat	150

	Event
		HardwareOnly

		Type	start 
		At	Chest
		
		Bhvroverride
			
			Startcolor	155 185 225

			PositionOffset		0 0 0
			InitialVelocity		0.0 0.25 0.0		
			InitialVelocityJitter	0.4 0.3 0.4
			pyrRotateJitter		180 180 180
			SpinJitter		.5 1.5 .5
			Drag			0.012
			physics 		1
			physRadius 		.001
			physGravity		.75
			Gravity			.03
			physRestitution 	0.5
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			FadeOutLength		200
			
			PhysDebris		1
			PhysDensity		2

			Scale			.5 .5 .5
		End
		
		Lifespan	300
		LifespanJitter	60
		Geom	Foam_peanut
		#Part	:QuarterHeads.part
	End

End

End			