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
		At	T_WepR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet T_LarmR
		POther  T_LarmR
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
		At	T_WepL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet T_LarmL
		POther  T_LarmL
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
		At	T_UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet T_LarmL
		POther  T_LarmL
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
		At	T_UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet T_Chest
		POther  T_Chest
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
		At	T_UarmR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet T_Chest
		POther  T_Chest
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
		At	T_UlegL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part
		Pmagnet T_Llegl
		POther  T_Llegl
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
		At	T_FootL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part
		Pmagnet T_Llegl
		POther  T_Llegl
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
		At	T_UlegR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet T_LlegR
		POther  T_LlegR
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
		At	T_FootR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part
		Pmagnet T_LlegR
		POther  T_LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End

#############################################################

End