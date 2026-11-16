#############################################################
## Child_Electricity_Fence.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Fence1
		Type	Local
		At	root
		Geom	ElectricityFence
		Bhvr	Behaviors/electricityFenceScale4.bhvr
		BhvrOverride
			PositionOffset	0.0 -2.3 0.0
			Scale		0.1 0.1 0.1
			Spin		0.0 0.0 0.0
		End
	End
End

#######################################################################
##Top
###############################################3

Condition
	On 	cycle
	Time 	4
	Chance	.75

	Event
		Ename	Node1a
		Type	start
		At	Fence1
		Altpiv	12
		Part	:Offset_Electricity_Chain.part
		Part	:Offset_Electricity_Chain2.part
		Part	:Hand_Electricity_Chain_Lingering.part
		POther  Node2a
		LifeSpan	3
	End

	Event
		Ename	Node2a
		Type	start
		At	Fence1
		Altpiv	10
		Part	:Offset_Electricity_Chain.part
		Part	:Offset_Electricity_Chain2.part
		Part	:Hand_Electricity_Chain_Lingering.part
		POther  Node3a
		LifeSpan	3
	End

	Event
		Ename	Node3a
		Type	start
		At	Fence1
		Altpiv	11
		Part	:Offset_Electricity_Chain.part
		Part	:Offset_Electricity_Chain2.part
		Part	:Hand_Electricity_Chain_Lingering.part
		POther  Node4a
		LifeSpan	3
	End

	Event
		Ename	Node4a
		Type	start
		At	Fence1
		Altpiv	7
		Part	:Offset_Electricity_Chain.part
		Part	:Offset_Electricity_Chain2.part
		Part	:Hand_Electricity_Chain_Lingering.part
		POther  Node5a
		LifeSpan	3
	End

	Event
		Ename	Node5a
		Type	start
		At	Fence1
		Altpiv	8
		Part	:Offset_Electricity_Chain.part
		Part	:Offset_Electricity_Chain2.part
		Part	:Hand_Electricity_Chain_Lingering.part
		POther  Node6a
		LifeSpan	3
	End

	Event
		Ename	Node6a
		Type	start
		At	Fence1
		Altpiv	9
		Part	:Offset_Electricity_Chain.part
		Part	:Offset_Electricity_Chain2.part
		Part	:Hand_Electricity_Chain_Lingering.part
		POther  Node1a
		LifeSpan	3
	End
End
