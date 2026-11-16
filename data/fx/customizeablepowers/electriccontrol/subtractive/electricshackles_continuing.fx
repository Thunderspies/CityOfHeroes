#############################################################
## ElectricShackles_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

########################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part	:Body_Sparks.part
		Part	:Hand_Ring_Burst.part
		Part	:Hit_Lightning.part
		Lifespan 30
		Sound	elecfry 80 70 .8
	End
End

Condition

	On 	Cycle
	Time 	4
	Chance 	0.75

	Event
		Ename	ArmR
		Type	Local
		At	Root
		bhvr	:Shackles_Anchor.bhvr
		Lifespan	1
	End

	Event
		Type	positonly
		At	HandR
		Part	:Hand_Electricity_Chain.part
		Part	:Hand_Electricity_Chain_Lingering.part
		Part	:Hand_Tendril_Shackle.part
		Part	:Hand_Electricity_Shackle.part
		Pother 	ArmR
		Lifespan	40
	End

End

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	WepL
		Part	:Arm_Lightning.part
		POther	UArmL
	End

	Event
		Type	local
		At	WepR
		Part	:Arm_Lightning.part
		POther	UArmR
	End

	Event
		Type	local
		At	Chest
		Part	:Chest_Electricity.part
		Part	:Chest_Tendril.part
	End

	Event
		Type	Local
		At	Root
		ChildFX	:Child_Electricity_Fence.fx
	End
End

Condition

	On 	Cycle
	Time 	4
	Chance 	0.75

	Event
		Ename	ArmL
		Type	local
		At	Root
		bhvr	:Shackles_Anchor2.bhvr
		Lifespan	1
	End

	Event
		Type	positonly
		At	HandL
		Part	:Hand_Electricity_Chain.part
		Part	:Hand_Electricity_Chain_Lingering.part
		Part	:Hand_Tendril_Shackle.part
		Part	:Hand_Electricity_Shackle.part
		Pother 	ArmL
		Lifespan	20
	End
End
#############################################################

End


