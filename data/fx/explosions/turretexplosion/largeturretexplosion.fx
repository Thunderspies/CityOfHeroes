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
		BHVR	:LargeTurretOffset.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	TurretOffset
		Part	:TurretSparks.part
		Part	:TurretExplosion01.part
		Part	:TurretExplosion02.part
		Part	:TurretBits.part
		Part	:TurretBits2.part
		Sound metal_explo2 80 80 .88
		Lifespan 9
	End

	Event
		EName 	FireBurst
		Type	Local
		At	TurretOffset
		Part	:TurretSparks.part
		Part	:LargeTurretFireBig.part
		Part	:LargeTurretExplosionFire01.part
		Part	:LargeTurretExplosionFire02.part
		LifeSpan 10
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	TurretOffset
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		600
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

Condition
	On 	time
	Time 	3
	Repeat 	4

	Event
		EName 	Sparkz
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 60
	End

	Event
		EName 	Sparkz2
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkUP.fx
		Lifespan 60
	End


	Event
		EName 	Sparkz3
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	3
	Repeat 	4  #11

	Event
		EName 	Sparkz
		HardwareOnly
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 60
	End

	Event
		EName 	Sparkz2
		HardwareOnly
		Type 	Start
		At 	TurretOffset
		ChildFX V_COV\PhysicsEnabled\HotSparkUP.fx
		Lifespan 60
	End


	Event
		EName 	Sparkz3
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
	Repeat	5

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.75 0.2 0.75

			PhysDebris		1
			PhysDensity		2
			StartColor		250 220 200
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End


	Event
		HardwareOnly
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.75 0.2 0.75

			PhysDebris		1
			PhysDensity		2
			StartColor		250 220 200
		End
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

End

Condition
	Time 	1
	Repeat	8
	Random	1

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum

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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
	End

	Event
		Type	start
		At	TurretOffset
		Geom	Metal_Curl01_Alluminum
		Geom	Metal_Flat04_Alluminum
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
		#CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	12
		LifespanJitter	3
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
		At	TurretOffset

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

			StartColor		250 220 200

			PyrRotateJitter		180 180 180

			Scale			.4 .4 .4

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
		LifespanJitter	5
	End

	Event
		Ename	parts2
		HardwareOnly
		Type	Start
		At	TurretOffset

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

			StartColor		250 220 200

			PyrRotateJitter		180 180 180

			Scale			.4 .4 .4

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
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
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

			Scale			.4 .4 .4

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


		#Geom	Metal_Flat05_Alluminum
		#Geom	Metal_Flat06_Alluminum
		#Geom	Metal_Flat07_Alluminum
		#Geom	Widget_15_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_17_Alluminum
		#Geom	Widget_18_Alluminum
		#Geom	Widget_19_Alluminum
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

			StartColor		250 220 200

			PyrRotateJitter		180 180 180

			Scale			.25 .25 .25

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
		At	TurretOffset

		#Geom	Metal_Flat05_Alluminum
		#Geom	Metal_Flat06_Alluminum
		#Geom	Metal_Flat07_Alluminum
		#Geom	Widget_15_Alluminum
		Geom	Widget_16_Alluminum
		#Geom	Widget_17_Alluminum
		#Geom	Widget_18_Alluminum
		#Geom	Widget_19_Alluminum
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

			StartColor		250 220 200

			PyrRotateJitter		180 180 180

			Scale			.25 .25 .25

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
End

#############################################################

End