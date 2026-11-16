#############################################################
## ElectricGremlin.fx
#############################################################

FxInfo
Lifespan 30

#########################################################
Condition
	Event
		Type Local
		At Hips
		Sound EC_Gremlins_Death_01 .8
	End
End
Condition
	On	Cycle
	Time	5
	Chance	.5

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Hand_Electricity_Chain.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Electricity_Arcs2.part
		Pmagnet SpadL
		POther  SpadL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5
	End

End

Condition
	On	Cycle
	Time	7
	Chance	.5


	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet SpadR
		POther  SpadR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5
	End

End

Condition
	On	Cycle
	Time	6
	Chance	.55


	Event
		ENAME	HeadGlow
		Type	Local
		At	Neck
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
	Time	8
	Chance	.55


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
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
		ENAME	RightLarmGlow
		Type	Local
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
	Time	7
	Chance	.55


	Event
		ENAME	LeftUarmGlow
		Type	Local
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
	Time	6
	Chance	.55


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LarmLNode2
		POther  LarmLNode2
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End


Condition
	On	Cycle
	Time	8
	Chance	.55


	Event
		ENAME	LeftULegGlow
		Type	Local
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
	Time	5
	Chance	.55


	Event
		ENAME	LeftLLegGlow
		Type	Local
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
	Time	7
	Chance	.55

	Event
		ENAME	RightULegGlow
		Type	Local
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
	Time	6
	Chance	.55


	Event
		ENAME	RightLLegGlow
		Type	Local
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


End
