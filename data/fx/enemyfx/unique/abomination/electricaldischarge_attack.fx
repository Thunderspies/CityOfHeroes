#############################################################
## ElectricalDischarge_Attack.fx
#############################################################

FxInfo
LifeSpan	100

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Abomination_ElecDischarge_Attack_01 100 100 .9
	End
End


Condition
	On	Time
	Time	10

	Event
		EName	ChestAnchor
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0.5 1.0
		End
		Part	:Electricity_Hands.part
		Part	:Electricity_Hands_Bright.part
		Part	:Electricity_Burst.part
		Lifespan 50
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.75

	Event
		Type	Local
		At	ChestAnchor
		Part	:Sparks_Burst_Light.part
		Part	:Flash_Burst.part
		Lifespan 50
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Chest
		ChildFX	:CHILD_ElectricalDischarge_Attack.fx
		Lifespan 60
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	Chest
		Part	:Electricity_Burst_Large.part
		Part	:Electricity_Burst_Large_Broken.part
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0.5 1.0
		End
		Part	:Sparks_Continuing.part
		Lifespan 100
	End

	Event
		Type	Local
		At	Back
		Part	:Flash_Continuing.part
		Lifespan 100
	End

	Event
		Type	Local
		At	Root
		Part	:Electricity_Burst_Large_Broken_Flat.part
	End
End



#############################################################

End