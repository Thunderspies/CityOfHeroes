#############################################################
## ElectricityBoltHitContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Incarnate_IonJudgement_Continuing_Loop 100 100 .3
	End
End



Condition
	On	Cycle
	Time	22
	Chance	.75

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  SpadL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 2

	End

End

Condition
	On	Cycle
	Time	20
	Chance	.75


	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks2.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  SpadR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End

End

Condition
	On	Cycle
	Time	34
	Chance	.75


	Event
		ENAME	HeadGlow
		Type	Local
		At	Neck
		Part	:Body_Electricity_Arcs.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		Pmagnet Chest
		POther  Chest
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 2

	End

End

Condition
	On	Cycle
	Time	31
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks2.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End

End

Condition
	On	Cycle
	Time	36
	Chance	.75


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks2.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 2

	End

End

Condition
	On	Cycle
	Time	40
	Chance	.75


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks2.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End

End

Condition
	On	Cycle
	Time	29
	Chance	.75


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks2.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 2

	End
End


Condition
	On	Cycle
	Time	37
	Chance	.75


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks2.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End


Condition
	On	Cycle
	Time	28
	Chance	.75


	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Part	:Body_Electricity_Arcs.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 2
	End
End

Condition
	On	Cycle
	Time	32
	Chance	.75

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks2.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1
	End
End


Condition
	On	Cycle
	Time	30
	Chance	.75

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Part	:Body_Electricity_Arcs.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		PMagnet	LLegR
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 2
	End
End


#############################################################

End