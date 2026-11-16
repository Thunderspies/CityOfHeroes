#############################################################
## Judgment_Ion.fx
#############################################################

FxInfo
LifeSpan	80

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		ChildFX	IncarnatePowers\Incarnate_Gather.fx
		Lifespan	30
	End
End

Condition
	On 	Time
	Time	22
	Event
		Type	Local
		At	Root
		Sound 	Incarnate_IonJudgement_Attack_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	LightningAnchor
		Type	Local
		At	Origin
		Lifespan	30
	End

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Ring.part
		POther	LArmR
		Sound	EC_JoltingChain_Activate_01 100 100 1
	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Ring.part
	End

	Event
		Type	Start
		At	HandL
		Part	:Hand_Glow_Large.part
		Lifespan	4
	End

	Event
		Type	Start
		At	HandR
		Part	:Hand_Glow_Large.part
		Lifespan	4

	End
End

Condition
	On	Time
	Time	23

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_RIGHT_Judgment_Ion.fx
		Lifespan 30
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_LEFT_Judgment_Ion.fx
		Lifespan 30
	End
End

Condition
	On	Cycle
	Time	5
	Random	1
	Chance	.5

	Event
		Type	Local
		At	LightningAnchor
		ChildFX	:CHILD_RIGHT_Judgment_Ion.fx
		Lifespan 30
	End

	Event
		Type	Local
		At	LightningAnchor
		ChildFX	:CHILD_LEFT_Judgment_Ion.fx
		Lifespan 30
	End
End

Condition
	On	Time
	Time	68

	Event
		Type	Local
		At	WepL
		Part	:Hand_Ring_Flash.part
		Part	:Hand_Ring_Flash_Fill.part
		Lifespan 3
	End

	Event
		Type	Local
		At	WepR
		Part	:Hand_Ring_Flash.part
		Part	:Hand_Ring_Flash_Fill.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	75

	Event
		Type	Local
		At	WepL

		Part	:ChargedBolt_Large.part
		Part	:ChargedBolt_Lingering2.part
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur_Small2.part

		LookAt	Target
		Pother	Target

		Lifespan 3
	End

	Event
		Type	Local
		At	Chest

		Part	:ChargedBolt_Large2.part
		Part	:ChargedBolt_Repeated.part
		Part	:ChargedBolt_Flash.part
		Part	:ChargedBolt_Flash_Lingering.part
		Part	:Beam_Smoke.part
		Part	:ChargedBolt_Lingering.part
		LookAt	Target
		Pother	Target
		Lifespan 5
	End
End

#############################################################

End