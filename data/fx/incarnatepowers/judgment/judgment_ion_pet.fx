#############################################################
## CHILD_RIGHT_Judgment_Ion.fx
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
		At	Root
		Sound 	Incarnate_IonJudgementPet_Hit_01 100 100 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Hand_Bolt_Blur_Small.part
		Part	:Beam_Smoke_Small.part
		LookAt	T_Chest
		Pother	T_Chest
	End
End

Condition
	On 	Time
	Time 	2

	Event
		EName	Sparkfar
		Type 	Local
		At	T_Chest
		ChildFX	:Judgment_Ion_Hit_Small.fx
		Lifespan	90

	End
End

#############################################################

End