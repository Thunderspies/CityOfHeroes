#############################################################
## Child_Aim.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part1	:ElectricityGlow3.part
		Part2	:ElectricityGlowStar3.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound pp12 50 50 .7
		LifeSpan 70
	End

	Event
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		LifeSpan 70
	End
End

#############################################################

Condition
	On	Time
	Time	2
	Repeat	2

	Event
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSparkSmall.fx
		Lifespan	120
		LifeSpanJitter	80
	End
End

Condition
	On	Time
	Time	2
	Repeat	2

	Event
		HardwareOnly
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSparkSmall.fx
		Lifespan	120
		LifeSpanJitter	80
	End
End

Condition
	On	Time
	Time	2
	Repeat	2

	Event
		HardwareOnly
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSparkSmall.fx
		Lifespan	120
		LifeSpanJitter	80
	End
End

#############################################################

End
