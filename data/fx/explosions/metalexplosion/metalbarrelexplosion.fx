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
		
		Sound telekinetic3 80 80 .9

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
		Lifespan	11
	End
	
	Event
		HardwareOnly
		Type	start
		At	Chest
		Geom	Metal_Curl
		Geom	Metal_Flat
		
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	11
	End

End



Condition
	On	Time
	Time 	1
	Repeat	4

	Event
		Ename	parts3
		HardwareOnly
		Type	Start
		At	Chest
		
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat02
		Geom	Widget_17_Alluminum
		Geom	Widget_18_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_26_Alluminum				
		Geom	Metal_Flat01
		Geom	Metal_Flat02
		Geom	Metal_Flat02

		Bhvr	:MetalPanelLightFade.bhvr
		BhvrOverride
			#PositionOffset		0 2 0
			StartJitter		.5 1.5 .5
			
			StartColor		120 120 130

			PyrRotateJitter		180 180 180

			Scale			.25 .25 .25
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.5 0.0
			InitialVelocityJitter	.75 0.75 .75

			Drag			0.01

			physSFriction 	 	0.6

			PhysDebris		1
			PhysDensity		2
			
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		#CThresh 0.0001
		
		Lifespan	20
		LifespanJitter	15
	End
End

End			