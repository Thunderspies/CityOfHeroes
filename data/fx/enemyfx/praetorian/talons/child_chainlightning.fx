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
	On	Time
	Time	0

	Event
		Ename	Offset01
		Type	Start
		At	T_Root
		BhvrOverride
			Startjitter	8 0 8

			PositionOffset	0 30 0
		End
	End

	Event
		Ename	Offset02
		Type	Start
		At	T_Root
		BhvrOverride
			Startjitter	6 2 6
			PositionOffset	0 15 0
		End
	End

#############################################################

	Event
		EName	Bolt1
		Type	Local
		At	Offset01
		Part	:ChargedBolt_Flash.part
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Beam_Smoke_Small.part
		LookAt	Offset02
		Pother	Offset02
		Lifespan 5
	End
End


Condition
	On	Time
	Time	1

	Event
		EName	Bolt2
		Type	Local
		At	Offset02
		Part	:ChargedBolt_Flash.part
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Beam_Smoke_Small.part
		LookAt	T_Chest
		Pother	T_Chest
		Lifespan 2
	End
End


Condition
	On	Cycle
	Time	7

	Event
		Ename	CycleOffset01
		Type	Start
		At	T_Root
		BhvrOverride
			Startjitter	8 0 8

			PositionOffset	0 30 0
		End
		Lifespan 5
	End

	Event
		Ename	CycleOffset02
		Type	Start
		At	T_Root
		BhvrOverride
			Startjitter	3 3 3
			PositionOffset	0 15 0
		End
		Lifespan 5
	End

	Event
		EName	Bolt1
		Type	Local
		At	CycleOffset01
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur_Small2.part
		LookAt	CycleOffset02
		Pother	CycleOffset02
		Lifespan 5
	End

	Event
		EName	Bolt2
		Type	Local
		At	CycleOffset02
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur_Small2.part
		LookAt	T_Chest
		Pother	T_Chest
		Lifespan 2
	End
End

#############################################################

End