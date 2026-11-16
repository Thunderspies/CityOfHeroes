#############################################################
## Child_Lightning_Arc_Smoke.fx
#############################################################

FxInfo
Lifespan 9

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Prime
		Type	start
		At	Chest
		Bhvr	V_COV\Powers\ElectricalMelee\LightningBoltBeam.bhvr
		BhvrOverride
			PyrRotateJitter	180 0 0
			Alpha		1
		End
		Geom	LightningBoltRigging
		Magnet	Target
		LookAt	Target
	#	Lifespan 10
	End

	Event
		Type	local
		At	Target
		BhvrOverride
			FadeOutLength		220
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node08
		Lifespan 2
	End

	Event
		Ename	Node08
		Type	local
		At	Prime
		Altpiv	9
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node07
		Lifespan 2
	End

	Event
		Ename	Node07
		Type	local
		At	Prime
		Altpiv	8
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node06
		Lifespan 2
	End

	Event
		Ename	Node06
		Type	local
		At	Prime
		Altpiv	7
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node05
		Lifespan 2
	End

	Event
		Ename	Node05
		Type	local
		At	Prime
		Altpiv	6
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node04
		Lifespan 2
	End

	Event
		Ename	Node04
		Type	local
		At	Prime
		Altpiv	5
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node03
		Lifespan 2
	End

	Event
		Ename	Node03
		Type	local
		At	Prime
		Altpiv	4
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node02
		Lifespan 2
	End

	Event
		Ename	Node02
		Type	local
		At	Prime
		Altpiv	3
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node01
		Lifespan 2
	End

	Event
		Ename	Node01
		Type	local
		At	Prime
		Altpiv	2
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part

		POther  Home
		Lifespan 2
	End

	Event
		Ename	Home
		Type	local
		At	Prime
		altpiv	1
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Smoke.part
		Part	:Offset_Smoke_Glow.part
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Prime
		Lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.8

	Event
		Ename	Prime
		Type	start
		At	Chest
		Bhvr	V_COV\Powers\ElectricalMelee\LightningBoltBeam.bhvr
		BhvrOverride
			PyrRotateJitter	180 0 0
			Alpha		1
		End
		Geom	LightningBoltRigging
		Magnet	Target
		LookAt	Target
		Lifespan 2
	End

	Event
		Type	local
		At	Target
		BhvrOverride
			FadeOutLength		220
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node08
		Lifespan 2
	End

	Event
		Ename	Node08
		Type	local
		At	Prime
		Altpiv	9
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node07
		Lifespan 2
	End

	Event
		Ename	Node07
		Type	local
		At	Prime
		Altpiv	8
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node06
		Lifespan 2
	End

	Event
		Ename	Node06
		Type	local
		At	Prime
		Altpiv	7
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node05
		Lifespan 2
	End

	Event
		Ename	Node05
		Type	local
		At	Prime
		Altpiv	6
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node04
		Lifespan 2
	End

	Event
		Ename	Node04
		Type	local
		At	Prime
		Altpiv	5
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node03
		Lifespan 2
	End

	Event
		Ename	Node03
		Type	local
		At	Prime
		Altpiv	4
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node02
		Lifespan 2
	End

	Event
		Ename	Node02
		Type	local
		At	Prime
		Altpiv	3
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Node01
		Lifespan 2
	End

	Event
		Ename	Node01
		Type	local
		At	Prime
		Altpiv	2
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Home
		Lifespan 2
	End

	Event
		Ename	Home
		Type	local
		At	Prime
		altpiv	1
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Fade.part
		Part	:Offset_Electricity_Chain_Big.part
		Part	:Offset_Electricity_Chain_Big_Fade.part
		Part	:Offset_Electricity_Chain_Thin.part
		POther  Prime
		Lifespan 2
	End
End

#############################################################

End