#############################################################
## CHILD_PsionicShockwave_Cooldown_Small.fx
#############################################################

FxInfo
Lifespan 110

#############################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing1
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		POther  RWing2
		Lifespan 5
	End
End

Condition
	On	Time
	Time	3

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing2
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		POther  RWing3
		Lifespan 5
	End
End

Condition
	On	Time
	Time	6

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing3
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		POther  RWing4
		Lifespan 5
	End
End

Condition
	On	Time
	Time	9

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing4
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		POther  LWing1
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	12

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing1
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		POther  LWing2
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	15

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing2
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		POther  LWing3
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	18

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing3
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		POther  LWing4
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	18

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing3
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:PsionicShockwave_Smoke_Small.part
		POther  LWing4
		Lifespan 	80
		LifespanJitter	10
	End
End

#############################################################

End