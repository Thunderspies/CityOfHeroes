#########################################################
##	template

FxInfo

#########################################################


Condition
	On Time 
	Time 0

	Event
		Ename	Base
		Type	Local
		At	Origin
		
#		BhvrOverride
#			PositionOffset		0 0 0
#			Scale			1 1 1
#			
#			pyrRotate		0 0 0
#		End

	End

	Event
		Ename	Door1
		Type	Start
		At	Base

		Geom	_cap_pdr_03
		
		BhvrOverride
			InitialVelocity		0.0 0.1 0.0
			InitialVelocityJitter	0.1 0.1 0.2
			SpinJitter		0.1 0.15 0.05

			Physics			1
			PhysGravity		1
			PhysRestitution		.1
			PhysSFriction		.5
			PhysDFriction		.3
			PhysScale		1 1 2

			Drag			0.015

			PhysDebris		3
						
			PhysDensity		2
		End

	End

End

Condition
	On Time 
	Time 0

	Repeat	10
	
	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\SparkUP.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			StartJitter	0 3 0
		End

		CThresh 	0.000001

		Part	V_COV\PhysicsEnabled\MetalSpark.part
		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
	
	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\SparkOUT.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			StartJitter	0 3 0
		End

		CThresh 	0.000001

		Part	V_COV\PhysicsEnabled\MetalSpark.part
		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End

	Event
		HardwareOnly
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\SparkOUT.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			StartJitter	0 3 0
		End

		CThresh 	0.000001

		Part	V_COV\PhysicsEnabled\MetalSpark.part
		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
	
	Event
		HardwareOnly
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\SparkOUTbig.bhvr
		BhvrOverride
			PositionOffset	0 4 0
			StartJitter	0 3 0
		End

		CThresh 	0.000001

		Part	V_COV\PhysicsEnabled\MetalSpark.part
		Part	V_COV\PhysicsEnabled\MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
End	

Condition
	Time 	0
	Repeat	5

	Event
		Type	start
		At	Origin
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_12
		Geom	Widget_13
		
		Bhvr	V_COV\PhysicsEnabled\BlackMetalChunkScatter.bhvr
		BhvrOverride
		
			PositionOffset	0 4 0
			StartJitter	0 3 0

			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5

			Scale			.1 .1 .1

			StartColor		120 130 140

			PhysDebris		1
			PhysDensity		2.5
			PhysScale		1.5 1.5 1.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End

	
	Event
		HardwareOnly
		Type	start
		At	Origin
		Geom	Widget_02
		Geom	Widget_03
		Geom	Widget_05
		Geom	Widget_06
		Geom	Widget_08
		Geom	Widget_09
		Geom	Widget_12
		Geom	Widget_13
		
		Bhvr	V_COV\PhysicsEnabled\BlackMetalChunkScatter.bhvr
		BhvrOverride
		
			PositionOffset	0 4 0
			StartJitter	0 3 0

			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5

			Scale			.2 .2 .2

			StartColor		120 130 140
			
			PhysDebris		3
			PhysDensity		2
			PhysScale		1.5 1.5 1.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End
End	

	