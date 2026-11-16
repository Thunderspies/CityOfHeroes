#############################################################
## ElectricShackles_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	HandGlow1
#		Type	Local
#		At	Chest
#		Part	CustomizeablePowers\ElectricControl\Body_Sparks.part
#		Part	CustomizeablePowers\ElectricControl\Hand_Ring_Burst.part
#		Part	CustomizeablePowers\ElectricControl\Hit_Lightning.part
#		Lifespan 30
#		Sound	elecfry 80 70 .8
#	End
#End

Condition

	On 	Cycle
	Time 	30
	#Chance 	0.75

	Event
		Ename	ArmR
		Type	Local
		At	Root
		bhvr	:Slime_Anchor.bhvr
		Lifespan	1
	End

	Event
		Type	Startpositonly
		At	HandR
		Part	:Slime_Chain.part
		#Part	CustomizeablePowers\ElectricControl\Hand_Electricity_Chain_Lingering.part
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
		#Part	CustomizeablePowers\ElectricControl\Arm_Lightning.part
		POther	UArmL
	End

	Event
		Type	local
		At	WepR
		#Part	CustomizeablePowers\ElectricControl\Arm_Lightning.part
		POther	UArmR
	End

#	Event
#		Type	local
#		At	Chest
#		Part	CustomizeablePowers\ElectricControl\Chest_Electricity.part
#		Part	CustomizeablePowers\ElectricControl\Chest_Tendril.part
#	End

	Event
		Type	Local
		At	Root
		#ChildFX	CustomizeablePowers\ElectricControl\Child_Electricity_Fence.fx
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
		bhvr	CustomizeablePowers\ElectricControl\Shackles_Anchor2.bhvr
		Lifespan	1
	End

#	Event
#		Type	positonly
#		At	HandL
#		Part	CustomizeablePowers\ElectricControl\Hand_Electricity_Chain.part
#		Part	CustomizeablePowers\ElectricControl\Hand_Electricity_Chain_Lingering.part
#		Part	CustomizeablePowers\ElectricControl\Hand_Tendril_Shackle.part
#		Part	CustomizeablePowers\ElectricControl\Hand_Electricity_Shackle.part
#		Pother 	ArmL
#		Lifespan	20
#	End
End
#############################################################

End


