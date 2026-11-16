#############################################################
## Red_LightningBolt.fx
#############################################################

FxInfo

Lifespan 200

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
			Alpha			1
		End
		Geom	LightningBoltRigging
		Magnet	Target
		LookAt	Target
	End

	Event
		Type	local
		At	Target
		BhvrOverride
			FadeOutLength		220
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt03.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt04.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Node08
	End

	Event
		Ename	Node08
		Type	local
		At	Prime
		Altpiv	9
		BhvrOverride
			StartJitter		.25 .25 .25
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt04.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Node07
	End

	Event
		Ename	Node07
		Type	local
		At	Prime
		Altpiv	8
		BhvrOverride
			StartJitter		.25 .25 .25
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Node06
	End

	Event
		Ename	Node06
		Type	local
		At	Prime
		Altpiv	7
		BhvrOverride
			StartJitter		.25 .25 .25
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt04.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Node05
	End

	Event
		Ename	Node05
		Type	local
		At	Prime
		Altpiv	6
		BhvrOverride
			StartJitter		.25 .25 .25
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt03.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Node04
	End

	Event
		Ename	Node04
		Type	local
		At	Prime
		Altpiv	5
		BhvrOverride
			StartJitter		.25 .25 .25
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Node03
	End

	Event
		Ename	Node03
		Type	local
		At	Prime
		Altpiv	4
		BhvrOverride
			StartJitter		.25 .25 .25
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Node02
	End

	Event
		Ename	Node02
		Type	local
		At	Prime
		Altpiv	3
		BhvrOverride
			StartJitter		.25 .25 .25
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Node01
	End

	Event
		Ename	Node01
		Type	local
		At	Prime
		Altpiv	2
		BhvrOverride
			StartJitter		.25 .25 .25
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt03.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Home
	End

	Event
		Ename	Home
		Type	local
		At	Prime
		altpiv	1
		BhvrOverride
			StartJitter		.25 .25 .25
		End
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite1.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltWhite2.part
		Part	V_COV\Powers\ElectricalMelee\Red_ElectricityBlastHaze.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt01.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBolt02.part
		Part	V_COV\Powers\ElectricalMelee\Red_ChargedBoltFade03.part
		POther  Prime
	End
End

#############################################################

End