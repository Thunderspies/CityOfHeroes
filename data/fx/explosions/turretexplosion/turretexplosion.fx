#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	TurretOffset
		Type	Local
		At	Chest
		BHVR	:TurretOffset.bhvr
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	TurretOffset
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		300
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End

	Event
		EName 	FireBurst
		Type	Local
		At	TurretOffset
		Part	:TurretSparks.part
		Part	:TurretFireBig.part
		Part	:TurretExplosionFire01.part
		Part	:TurretExplosionFire02.part
		LifeSpan 10
	End
End

## PHYSICS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local
		At	TurretOffset
		Part	:TurretSparks.part
		Part	:TurretExplosion01.part
		Part	:TurretExplosion02.part
		Part	:TurretBits.part
		Part	:TurretBits2.part
		Sound metal_explo2 80 80 .7
		Lifespan 9
	End
End

#############################################################

Condition
	On 	time
	Time 	3
	Repeat 	4

	Event
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 60
	End

	Event
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkUP.fx
		Lifespan 60
	End


	Event
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	3
	Repeat 	4

	Event
		HardwareOnly
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 60
	End

	Event
		HardwareOnly
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkUP.fx
		Lifespan 60
	End


	Event
		HardwareOnly
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End

############  Metal Shell  ####################

Condition
	Time 	2
	Repeat	3

	Event
		Type	start
		At	TurretOffset
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5

			PhysDebris		1
			PhysDensity		2
			StartColor		250 220 200
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End

	Event
		HardwareOnly
		Type	start
		At	TurretOffset
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5

			PhysDebris		1
			PhysDensity		2
			StartColor		250 220 200
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End
End

Condition
	Time 	1
	Repeat	6
	Random	1

	Event
		Type	start
		At	TurretOffset
		Bhvr	Behaviors\MetalOffset.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			StartColor		250 220 200
			PhysDebris		1
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Bhvr	Behaviors\MetalOffset2.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			StartColor		250 220 200
			PhysDebris		1
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Bhvr	Behaviors\MetalOffset3.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			StartColor		250 220 200
			PhysDebris		1
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Bhvr	Behaviors\MetalOffset4.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			StartColor		250 220 200
			PhysDebris		1
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Bhvr	Behaviors\MetalOffset5.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			StartColor		250 220 200
			PhysDebris		1
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Bhvr	Behaviors\MetalOffset6.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			StartColor		250 220 200
			PhysDebris		1
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Bhvr	Behaviors\MetalOffset7.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			StartColor		250 220 200
			PhysDebris		1
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Bhvr	Behaviors\MetalOffset8.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			StartColor		250 220 200
			PhysDebris		1
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Bhvr	Behaviors\MetalOffset9.bhvr
		BhvrOverride
			Drag			0.003
			physics 		1
			physRadius 		0.8
			physGravity 		.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			StartColor		250 220 200
			PhysDebris		1
		End
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
		CThresh 0.0001
		Lifespan	12
		LifespanJitter	3
	End
End

#############################################################

Condition
	On	Time
	Time 	3
	Repeat	5

	Event
		SoftwareOnly
		Ename	parts1
		Type	Start
		At	TurretOffset
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		Geom	Widget_15_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_18_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_26_Alluminum
		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
			StartColor		250 220 200
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
		CThresh 0.0001
		Lifespan	20
		LifespanJitter	5
	End

	Event
		Ename	parts2
		HardwareOnly
		Type	Start
		At	TurretOffset
		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
			StartColor		250 220 200
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
		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		Geom	Metal_Flat05_Alluminum
		Geom	Metal_Flat06_Alluminum
		Geom	Metal_Flat07_Alluminum
		Geom	Widget_15_Alluminum
		Geom	Widget_17_Alluminum
		Geom	Widget_18_Alluminum
		Geom	Widget_22_Alluminum
		Geom	Widget_26_Alluminum
		CThresh 0.0001
		Lifespan	20
		LifespanJitter	15
	End

	Event
		Ename	parts3
		HardwareOnly
		Type	Start
		At	TurretOffset

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

			StartColor		250 220 200

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
		At	TurretOffset

		Part3	Explosions\VehicleExplosion\FireBallSpark.part
		Geom	Widget_16_Alluminum
		Geom	Widget_19_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_21_Alluminum
		Geom	Widget_23_Alluminum
		Geom	Widget_24_Alluminum
		Geom	Widget_25_Alluminum
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
			StartColor		250 220 200
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
		CThresh 0.0001
		Lifespan	20
		LifespanJitter	15
	End

	Event
		Ename	smallparts2
		Type	Start
		At	TurretOffset
		Geom	Widget_16_Alluminum
		Geom	Widget_19_Alluminum
		Geom	Widget_20_Alluminum
		Geom	Widget_21_Alluminum
		Geom	Widget_23_Alluminum
		Geom	Widget_24_Alluminum
		Geom	Widget_25_Alluminum
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
			StartColor		250 220 200
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
		CThresh 0.0001
		Lifespan	20
		LifespanJitter	15
	End
End

#############################################################

End