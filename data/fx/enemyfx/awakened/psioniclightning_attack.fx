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
	Time	30

	Event
		EName	LightningAnchor
		Type	Local
		At	T_Root
		BhvrOverride
			PositionOffset	0.0 40.0 0.0
		End
		Part	:ChargedBolt_Large.part
		Part	:ChargedBolt_Lingering2.part
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur_Small2.part

		LookAt	Target
		Pother	Target
		Sound 	Incarnate_IonJudgementPet_Hit_01 100 100 .8

		Lifespan 3
	End

	Event
		Type	Local
		At	LightningAnchor
		Part	:ChargedBolt_Large2.part
		Part	:ChargedBolt_Repeated.part
		Part	:ChargedBolt_Flash.part
		Part	:ChargedBolt_Flash_Lingering.part
		Part	:Beam_Smoke.part
		Part	:ChargedBolt_Lingering.part
		LookAt	T_Root
		Pother	T_Root
		Lifespan 5
	End
End

#############################################################

End