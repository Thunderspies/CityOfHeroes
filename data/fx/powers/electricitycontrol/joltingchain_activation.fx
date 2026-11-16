#############################################################
## JoltingChain_Activation.fx
#############################################################

FxInfo
LifeSpan	50

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part3	:Hand_Ring.part
		Part	:Hand_Sparks.part
		POther	LArmR
		Sound	cagebolt2 80 30 .8
	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Body_Electricity_Arcs.part

		Part	:Hand_Glow.part
		Part3	:Hand_Ring.part
		Part	:Hand_Sparks.part
	End
End

Condition
	On	Time
	Time	28


	Event
		EName	WepAnchor
		Type	Local
		At	WepL
		Part	:Hand_Ring_Burst.part
		Part	:WitchSparks.part
		Lifespan	10
	End

	Event
		Type	Local
		At	WepL
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Beam_Glow.part
		Part	:Beam_Bolt_Small.part
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur.part
		Part	:Beam_Bolt.part
		Part	:Beam_Smoke.part
		Part	:Beam_Smoke_LongLifespan.part
		LookAt	Target
		Pother	Target
		Lifespan 1
	End
End


Condition
	On	Time
	Time	33

	Event
		Type	Local
		At	WepL
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Beam_Bolt_Small.part
		LookAt	Target
		Pother	Target
		Lifespan 1
	End
End

Condition
	On	Time
	Time	38

	Event
		Type	Local
		At	WepL
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Beam_Bolt_Blur_Small.part

		LookAt	Target
		Pother	Target
		Lifespan 1
	End
End

Condition
	On	Time
	Time	39

	Event
		Type	Local
		At	WepL
		Part	:Beam_Bolt_Blur.part
		Part	:Beam_Bolt.part
		LookAt	Target
		Pother	Target
		Lifespan 1
	End
End

#############################################################

Condition
	On	Time
	Time	34

	Event
		EName	TargetAnchor
		Type	Local
		At	Target
	End

	Event
		Ename	Prime
		Type	start
		At	WepL
		Bhvr	V_COV\Powers\ElectricalMelee\LightningBoltBeam.bhvr
		Geom	LightningBoltRigging
		Magnet	Target
		LookAt	Target
	End

	Event
		Type	local
		At	TargetAnchor
		BhvrOverride
			FadeOutLength		220
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node07
		Lifespan 5
	End


	Event
		Ename	Node07
		Type	local
		At	Prime
		Altpiv	8
		BhvrOverride
			PositionOffset	0 .5 0
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node06
		Lifespan 5
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
		Lifespan 5
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
		Lifespan 5
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
		Lifespan 5
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
		Lifespan 5
	End
End

Condition
	On	Time
	Time	40

	Event
		EName	TargetAnchor2
		Type	Local
		At	Target
	End

	Event
		Ename	Prime2
		Type	start
		At	WepL
		Bhvr	V_COV\Powers\ElectricalMelee\LightningBoltBeam.bhvr
		Geom	LightningBoltRigging
		Magnet	Target
		LookAt	Target
	End

	Event
		Type	local
		At	TargetAnchor2
		BhvrOverride
			FadeOutLength		220
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node07a
		Lifespan 5
	End


	Event
		Ename	Node07a
		Type	local
		At	Prime2
		Altpiv	8
		BhvrOverride
			PositionOffset	0 .5 0
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node06a
		Lifespan 5
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
		Lifespan 5
	End

	Event
		Ename	Node04a
		Type	local
		At	Prime2
		Altpiv	4
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2_Inverted.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Offset_Smoke.part
		POther  Node02a
		Lifespan 5
	End

	Event
		Ename	Node02a
		Type	local
		At	Prime2
		Altpiv	2
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2_Inverted.part
		Part	:OFfset_Smoke.part
		POther  Homea
		Lifespan 5
	End

	Event
		Ename	Homea
		Type	local
		At	Prime2
		altpiv	1
		BhvrOverride
			StartJitter		0.35 0.35 0.35
		End
		Part	:Offset_Electricity_Chain_Glow.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:OFfset_Smoke.part
		POther  Prime2
		Lifespan 5
	End
End
#############################################################

End