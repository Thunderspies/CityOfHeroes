#############################################################
## ChainLightning_Attack.fx
#############################################################

FxInfo
LifeSpan	60

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
		Sound	ToV_ChainLightning_Attack_01 100 100 .6
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
End

Condition
	On	Time
	Time	35

	Event
		EName	BoltAnchor
		Type	Local
		At	T_Root
		Lifespan 50
	End


	Event
		Ename	Offset01
		Type	Start
		At	BoltAnchor
		BhvrOverride
			Startjitter	8 0 8

			PositionOffset	0 40 0
		End
	End

	Event
		Ename	Offset02
		Type	Start
		At	BoltAnchor
		BhvrOverride
			Startjitter	6 2 6
			PositionOffset	0 20 0
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
	On 	Time
	Time	36
	Event
		Type	Local
		At	CycleOffset01
		Sound 	ToV_ChainLightning_Pet_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	46
	Event
		Type	Local
		At	CycleOffset01
		Sound 	ToV_ChainLightning_Pet_02 100 100 .8
	End
End

Condition
	On 	Time
	Time	56
	Event
		Type	Local
		At	CycleOffset01
		Sound 	ToV_ChainLightning_Pet_03 100 100 .8
	End
End


Condition
	On	Time
	Time	36

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
		At	BoltAnchor
		BhvrOverride
			Startjitter	8 0 8

			PositionOffset	0 40 0
		End
		Lifespan 5
	End

	Event
		Ename	CycleOffset02
		Type	Start
		At	BoltAnchor
		BhvrOverride
			Startjitter	3 3 3
			PositionOffset	0 20 0
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