#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 100

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Cycle
	Time	10
	Chance	.55


	Event
		ENAME	RightHandCycle
		Type	PositOnly
		At	WepR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LarmR
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	5
	Chance	.55


	Event
		ENAME	RightHandCycle
		Type	PositOnly
		At	WepL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LarmL
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	15
	Chance	.55


	Event
		ENAME	UArmLCycle
		Type	PositOnly
		At	UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LarmL
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	10
	Chance	.55


	Event
		ENAME	UArmLCycle
		Type	PositOnly
		At	UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet Chest
		POther  Chest
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	5
	Chance	.55


	Event
		ENAME	UArmLCycle
		Type	PositOnly
		At	UarmR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet Chest
		POther  Chest
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	10
	Chance	.55


	Event
		ENAME	ULegLCycle
		Type	PositOnly
		At	UlegL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part
		Pmagnet Llegl
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End


Condition
	On	Cycle
	Time	15
	Chance	.55


	Event
		ENAME	LeftFootCycle
		Type	PositOnly
		At	FootL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part
		Pmagnet Llegl
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End

Condition
	On	Cycle
	Time	5
	Chance	.55

	Event
		ENAME	ULegRCycle
		Type	PositOnly
		At	UlegR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LlegR
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End


Condition
	On	Cycle
	Time	10
	Chance	.55


	Event
		ENAME	RightFootCycle
		Type	PositOnly
		At	FootR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part
		Pmagnet LlegR
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End

#############################################################

End