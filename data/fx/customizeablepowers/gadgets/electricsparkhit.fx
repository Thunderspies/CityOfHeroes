#########################################################
##	template

FxInfo

LifeSpan	60
#########################################################

Condition
	On Time
	Time 0
	Repeat 5

	Event
		Type 	Start
		At	Origin

		Part	:ElectricSpark.part

		Lifespan	10
		LifespanJitter	2
	End

End

Condition
	On Time
	Time 0
	Repeat 5

	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV/PhysicsEnabled/SparkOut.bhvr

		Part	:ElectricSpark.part
		Part	:ElectricSparkStreak.part

		Lifespan	45
		LifespanJitter	5
	End
End

Condition
	On Time
	Time 0
	Repeat 2

	Event
		HardwareOnly
		Type 	Start
		At	Origin
		Bhvr	V_COV/PhysicsEnabled/SparkExplode.bhvr

		Part	:ElectricSpark.part
		Part	:ElectricSparkStreak.part

		Lifespan	45
		LifespanJitter	5
	End

	Event
		HardwareOnly
		Type 	Start
		At	Origin
		Bhvr	:SparkUp.bhvr

		Part	:ElectricSpark.part
		Part	:ElectricSparkStreak.part

		Lifespan	45
		LifespanJitter	5
	End
End


End

