#############################################################
## BuildUp_Attack.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Sparkfar
		Type 	local
		At	Chest
		Part	:EnergyCoreElectricity.part
		Part	:EnergyCoreElectricityInvert.part
		Part	:EnergyCoreElectricity_White.part
		Part	:EnergyCoreElectricityInvert_White.part
		Lifespan	30
		LifeSpanJitter	10
	End
End

Condition
	On	Time
	Time	2
	Repeat	2

	Event
		EName	Sparkfar
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
		EName	Sparkfar2
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
		EName	Sparkfar2
		Type 	Start
		At	Chest
		ChildFX	:Child_ElectricSparkSmall.fx
		Lifespan	120
		LifeSpanJitter	80
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End