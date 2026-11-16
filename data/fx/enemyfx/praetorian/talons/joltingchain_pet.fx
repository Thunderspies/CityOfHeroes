#############################################################
## JoltingChain_Pet.fx
#############################################################

FxInfo
LifeSpan	40

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	# Chance .25
	Event
		Type Local
		At Root
		SoundNoRepeat 1
		Sound	ToV_ChainLightning_Pet_01 100 100 .8
		Sound	ToV_ChainLightning_Pet_02 100 100 .8
		Sound	ToV_ChainLightning_Pet_03 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt.part
		Part	:Beam_Bolt_Huge_Inverted.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Beam_Bolt_Blur_Small2_Inverted.part
		LookAt	Target
		Pother	Target
		Lifespan 1
	End
End

Condition
	On	Time
	Time	2

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
	End

	Event
		Type	local
		At	Target
		BhvrOverride
			FadeOutLength		220
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node07
		Lifespan 4
	End


	Event
		Ename	Node07
		Type	local
		At	Prime
		Altpiv	8
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node06
		Lifespan 4
	End



	Event
		Ename	Node06
		Type	local
		At	Prime
		Altpiv	6
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2_Inverted.part
		Part	:Offset_Smoke.part
		POther  Node04
		Lifespan 4
	End

	Event
		Ename	Node04
		Type	local
		At	Prime
		Altpiv	4
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2_Inverted.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node02
		Lifespan 4
	End

	Event
		Ename	Node02
		Type	local
		At	Prime
		Altpiv	2
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2_Inverted.part
		Part	:OFfset_Smoke.part
		POther  Home
		Lifespan 4
	End

	Event
		Ename	Home
		Type	local
		At	Prime
		altpiv	1
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:OFfset_Smoke.part
		POther  Prime
		Lifespan 4
	End
End

Condition
	On	Time
	Time	4

	Event
		Ename	Prime2
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
	End

	Event
		Type	local
		At	Target
		BhvrOverride
			FadeOutLength		220
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node07a
		Lifespan 4
	End


	Event
		Ename	Node07a
		Type	local
		At	Prime2
		Altpiv	8
		BhvrOverride
			StartJitter		0.0 0.0 0.0
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node06a
		Lifespan 4
	End



	Event
		Ename	Node06a
		Type	local
		At	Prime2
		Altpiv	6
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2_Inverted.part
		Part	:Offset_Smoke.part
		POther  Node04a
		Lifespan 4
	End

	Event
		Ename	Node04a
		Type	local
		At	Prime
		Altpiv	4
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2_Inverted.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node02a
		Lifespan 4
	End

	Event
		Ename	Node02a
		Type	local
		At	Prime
		Altpiv	2
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2_Inverted.part
		Part	:OFfset_Smoke.part
		POther  Homea
		Lifespan 4
	End

	Event
		Ename	Homea
		Type	local
		At	Prime
		altpiv	1
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:OFfset_Smoke.part
		POther  Prime2
		Lifespan 4
	End
End

Condition
	On	Time
	Time	5

	Event
		EName	HandGlow1
		Type	Local
		At	T_Chest
		Part	:ElectricityGlow.part
		Part	:Hit_Lightning_Streak.part
		Part	:Hit_Lightning.part
		Part	:ElectricityArch4a.part
		Part	:ElectricityArchSmalla.part
		Part	:Hand_Ring_Burst.part
		Part	:ElectricityArchContinuing.part
		Part	:ElectricityArchContinuing2.part
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	2
	Repeat	3

	Event
		EName	Sparkfar
		Type 	Local
		At	T_Chest
		ChildFX	:Child_LightningHit.fx

		Lifespan	90

	End
End

Condition
	On	Time
	Time	2
	Repeat	5

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Local
		At	T_Chest
		ChildFX	:Child_ElectricSparkHit.fx

		Lifespan	90

	End
End

#############################################################

End