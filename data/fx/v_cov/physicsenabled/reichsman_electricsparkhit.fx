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

		Part	:Reichsman_ElectricSpark.part

		Lifespan	10
		LifespanJitter	2
	End

End

Condition
	On Time
	Time 0
	Repeat 4

	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOut.bhvr

		Part	:Reichsman_ElectricSpark.part
		Part	:Reichsman_ElectricSparkStreak.part

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
		Bhvr	:SparkExplode.bhvr

		Part	:Reichsman_ElectricSpark.part
		Part	:Reichsman_ElectricSparkStreak.part

		Lifespan	45
		LifespanJitter	5
	End

	Event
		HardwareOnly
		Type 	Start
		At	Origin
		Bhvr	:SparkUp.bhvr

		Part	:Reichsman_ElectricSpark.part
		Part	:Reichsman_ElectricSparkStreak.part

		Lifespan	45
		LifespanJitter	5
	End
End


End

