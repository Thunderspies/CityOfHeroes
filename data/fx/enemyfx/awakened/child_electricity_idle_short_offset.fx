#############################################################
## CHILD_Electricity_Idle_Short_Offset.fx
#############################################################

FxInfo
Lifespan 38

#############################################################


Condition
	On	Time
	Time	7

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing2
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Small.part
		part	:Glow_Small.part
		POther  RWing3
		Lifespan 5
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing3
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Small.part
		part	:Glow_Small.part
		POther  RWing4
		Lifespan 5
	End
End

Condition
	On	Time
	Time	13

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing4
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Small.part
		part	:Glow_Small.part
		POther  LWing1
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	16

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing1
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Small.part
		part	:Glow_Small.part
		POther  LWing2
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	19

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing2
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Small.part
		part	:Glow_Small.part
		POther  LWing3
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	22

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing3
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Small.part
		part	:Glow_Small.part
		POther  LWing4
		Lifespan 	5
		LifespanJitter	3
	End
End

#############################################################

End