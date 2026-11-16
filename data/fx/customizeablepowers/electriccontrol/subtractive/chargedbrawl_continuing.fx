#############################################################
## JoltingChain_Hit.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Chest
End

#############################################################

Condition
	On	Cycle
	Time	20
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End

End

Condition
	On	Cycle
	Time	20
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End

End

Condition
	On	Cycle
	Time	10
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End

End

Condition
	On	Cycle
	Time	12
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End

End

Condition
	On	Cycle
	Time	10
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  SpadL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End

End

Condition
	On	Cycle
	Time	12
	Chance	.75


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Sparks.part
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part
		POther  SpadR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End

End
#############################################################

End
