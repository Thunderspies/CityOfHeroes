#############################################################
## CHILD_PsychicLash_Windup_Small_Offset.fx
#############################################################

FxInfo
Lifespan 38

#############################################################

Condition
	On	Time
	Time	6

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing2
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		part	:Ring_Windup_Small.part
		POther  RWing3
		Lifespan 5
	End
End

Condition
	On	Time
	Time	9

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing3
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		part	:Ring_Windup_Small.part
		POther  RWing4
		Lifespan 5
	End
End

Condition
	On	Time
	Time	12

	Event
		ENAME	ChestGlow
		Type	Local
		At	RWing4
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		part	:Ring_Windup_Small.part
		POther  LWing1
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
		At	LWing1
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		part	:Ring_Windup_Small.part
		POther  LWing2
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
		At	LWing2
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		part	:Ring_Windup_Small.part
		POther  LWing3
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	21

	Event
		ENAME	ChestGlow
		Type	Local
		At	LWing3
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Electricity_Windup_Small.part
		part	:Glow_Windup_Small.part
		part	:Ring_Windup_Small.part
		POther  LWing4
		Lifespan 	5
		LifespanJitter	3
	End
End

#############################################################

End