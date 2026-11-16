#############################################################
## CHILD_Electricity_Idle.fx
#############################################################

FxInfo
Lifespan 38

#############################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing1
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Large.part
		part	:Glow_Large.part
		POther  RWing2
		Lifespan 5
	End
End

Condition
	On	Time
	Time	5

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing2
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Large.part
		part	:Glow_Large.part
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
		part	:Electricity_Large.part
		part	:Glow_Large.part
		POther  RWing4
		Lifespan 5
	End
End

Condition
	On	Time
	Time	15

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing4
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Large.part
		part	:Glow_Large.part
		POther  LWing1
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing1
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Large.part
		part	:Glow_Large.part
		POther  LWing2
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	25

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing2
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Large.part
		part	:Glow_Large.part
		POther  LWing3
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	30

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing3
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Large.part
		part	:Glow_Large.part
		POther  LWing4
		Lifespan 	5
		LifespanJitter	3
	End
End

#############################################################

End