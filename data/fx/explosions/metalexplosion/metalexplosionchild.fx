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
		Type	Local 
		At	Origin
		
		Bhvr	Behaviors\MetalOffsetFire.bhvr
		Part	:MetalSparks.part
		Part	:MetalExplosionBig.part
		Part	:MetalExplosion01.part
		Part	:MetalExplosion02.part
		#Part	:MetalBits.part
		#Part	:MetalBits2.part
		Sound telekinetic3 80 80 .9

		Lifespan	9

	End



End

#########  Metal Shell   ###############

Condition
	Time 	0
	Repeat	3
	Random	1

	Event
		Type	start
		At	Origin
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

	Event
		Type	start
		At	Origin
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

	Event
		Type	start
		At	Origin
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

	Event
		Type	start
		At	Origin
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

	Event
		Type	start
		At	Origin
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
	Repeat	3

	Event
		Type	start
		At	Origin
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

End

#####   Metal Parts    ##############

Condition
	On	Time
	Time 	3
	Repeat	5

	Event
		SoftwareOnly
		Ename	parts1
		Type	Start
		At	Origin
		
		#Part3	Explosions\VehicleExplosion\FireBallSpark.part
		#Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		
		#Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		Geom	Widget_15_Alluminum
		#Geom	Widget_16_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_18_Alluminum
		#Geom	Widget_19_Alluminum
		#Geom	Widget_20_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_22_Alluminum
		#Geom	Widget_23_Alluminum
		#Geom	Widget_24_Alluminum
		#Geom	Widget_25_Alluminum
		Geom	Widget_26_Alluminum

		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			#PositionOffset		0 3 0
			StartJitter		1 1 1
			
			StartColor		150 120 100

			PyrRotateJitter		180 180 180

			Scale			.25 .25 .25
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	.6 0.4 .6

			Drag			0.01

			physSFriction 	 	0.6
			PhysDensity		2
			PhysScale		1.25 1.25 1.25
			
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	320
		LifespanJitter	150
	End

	Event
		Ename	parts2
		HardwareOnly
		Type	Start
		At	Origin
		
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		Geom	Widget_15_Alluminum
		#Geom	Widget_16_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_18_Alluminum
		#Geom	Widget_19_Alluminum
		#Geom	Widget_20_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_22_Alluminum
		#Geom	Widget_23_Alluminum
		#Geom	Widget_24_Alluminum
		#Geom	Widget_25_Alluminum
		Geom	Widget_26_Alluminum

		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			#PositionOffset		0 3 0
			StartJitter		1 1 1
			
			StartColor		150 120 100

			PyrRotateJitter		180 180 180

			Scale			.25 .25 .25
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	.6 0.4 .6

			Drag			0.01

			physSFriction 	 	0.6

			PhysDebris		1
			PhysDensity		2
			PhysScale		1.25 1.25 1.25
			
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	20
		LifespanJitter	15
	End


	Event
		Ename	parts3
		HardwareOnly
		Type	Start
		At	Origin
		
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		Geom	Widget_15_Alluminum
		#Geom	Widget_16_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_18_Alluminum
		#Geom	Widget_19_Alluminum
		#Geom	Widget_20_Alluminum
		#Geom	Widget_21_Alluminum
		Geom	Widget_22_Alluminum
		#Geom	Widget_23_Alluminum
		#Geom	Widget_24_Alluminum
		#Geom	Widget_25_Alluminum
		Geom	Widget_26_Alluminum

		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			#PositionOffset		0 3 0
			StartJitter		1 1 1
			
			StartColor		150 120 100

			PyrRotateJitter		180 180 180

			Scale			.25 .25 .25
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.2 0.0
			InitialVelocityJitter	.4 0.3 .4

			Drag			0.01

			physSFriction 	 	0.6

			PhysDebris		1
			PhysDensity		2
			PhysScale		1.25 1.25 1.25
			
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	40
		LifespanJitter	15
	End
	
	Event
		Ename	smallparts1
		HardwareOnly
		Type	Start
		At	Origin
		
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		
		
		#Geom	Metal_Flat05_Alluminum
		#Geom	Metal_Flat06_Alluminum
		#Geom	Metal_Flat07_Alluminum
		#Geom	Widget_15_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_17_Alluminum
		#Geom	Widget_18_Alluminum
		Geom	Widget_19_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_21_Alluminum
		#Geom	Widget_22_Alluminum
		Geom	Widget_23_Alluminum
		Geom	Widget_24_Alluminum
		Geom	Widget_25_Alluminum
		#Geom	Widget_26_Alluminum

		Geom	carpart_sprocket
		Geom	carpart_pipe

		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			#PositionOffset		0 3 0
			StartJitter		1 1 1
			
			StartColor		150 120 100

			PyrRotateJitter		180 180 180

			Scale			.15 .15 .15
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.4 0.0
			InitialVelocityJitter	.4 0.4 .4

			Drag			0.01

			physSFriction 	 	0.6

			PhysDebris		1
			PhysDensity		2
			PhysScale		1.5 1.5 1.5
			
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	20
		LifespanJitter	15
	End

		
	Event
		Ename	smallparts2
		Type	Start
		At	Origin
		
		#Geom	Metal_Flat05_Alluminum
		#Geom	Metal_Flat06_Alluminum
		#Geom	Metal_Flat07_Alluminum
		#Geom	Widget_15_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_17_Alluminum
		#Geom	Widget_18_Alluminum
		Geom	Widget_19_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_21_Alluminum
		#Geom	Widget_22_Alluminum
		Geom	Widget_23_Alluminum
		Geom	Widget_24_Alluminum
		Geom	Widget_25_Alluminum
		#Geom	Widget_26_Alluminum

		Geom	carpart_sprocket
		Geom	carpart_pipe

		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			#PositionOffset		0 3 0
			StartJitter		1 1 1
			
			StartColor		150 120 100

			PyrRotateJitter		180 180 180

			Scale			.15 .15 .15
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.4 0.0
			InitialVelocityJitter	.4 0.4 .4

			Drag			0.01

			physSFriction 	 	0.6

			PhysDebris		1
			PhysDensity		2
			PhysScale		1.5 1.5 1.5
			
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	20
		LifespanJitter	15
	End


End

End			