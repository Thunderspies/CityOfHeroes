#########################################################
##	template

FxInfo

#########################################################

Condition
	On Time 
	Time 0

	Repeat	5
	
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkUP.bhvr
		BhvrOverride
			PositionOffset	0 3 0
			StartJitter	0 4 0
		End

		CThresh 	0.000001

		Part	:MetalSpark.part
		Part	:MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
	
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOUT.bhvr
		BhvrOverride
			PositionOffset	0 3 0
			StartJitter	0 4 0
		End

		CThresh 	0.000001

		Part	:MetalSpark.part
		Part	:MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End

	
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkUP.bhvr
		BhvrOverride
			PositionOffset	0 9 0
			StartJitter	0 4 0
		End

		CThresh 	0.000001

		Part	:MetalSpark.part
		Part	:MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
		
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOUT.bhvr
		BhvrOverride
			PositionOffset	0 9 0
			StartJitter	0 4 0
		End

		CThresh 	0.000001

		Part	:MetalSpark.part
		Part	:MetalSparkStreak.part

		Lifespan	50
		LifespanJitter	10
	End
End	

Condition
	Time 	0
	Repeat	4

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
		
			PositionOffset	0 6 0
			StartJitter	0 8 0

			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5

			Scale			.15 .15 .15

			StartColor		100 100 100
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	120
		LifespanJitter	30
	End
End	

	