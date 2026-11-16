#############################################################
## TeslaCage_Continuing.FX
#############################################################

FxInfo

########################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part	:Body_Sparks.part
		Part	:Hand_Ring_Burst.part
		Part	:Hit_Lightning.part
		Lifespan 30
		Sound	EC_TeslaCage_Hit_01 100 100 .8
	End
	Event
		Type Local
		At Chest
		Sound EC_TeslaCage_Continuing_Loop 50 50 .35
	End
End

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
		POther  LLegL
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
		POther  LLegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 1

	End
End

Condition
	On 	Time
	Time 	15

	Event

		Type	PositOnly
		At	Hips
		bhvr	CustomizeablePowers\ElectricalBlast\cagespin2.bhvr
		BhvrOverride
			Spin			0.1 .002 0.1
			SpinJitter		0.001 0.0 0.001
			Alpha		150
			TintGeom	1
		End
		Geom	Tintable_Dark_Cagearchs2
	End



End

Condition
	On 	Time
	Time 	16
	Event

		Type	PositOnly
		At	Hips
		bhvr	CustomizeablePowers\ElectricalBlast\cagespin.bhvr
		BhvrOverride
			Spin		0.08 -.002 -0.08
			Alpha		150
			TintGeom	1
		End
		Geom	Tintable_Dark_Cagearchs
	End


End

########################################################

End
