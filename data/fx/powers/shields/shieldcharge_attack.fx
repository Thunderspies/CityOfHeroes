#############################################################
## ShieldCharge_Attack.fx
#############################################################

FxInfo

Lifespan 60

Flags InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	StartOffset
		Type	start
		At	Root
		Sound ThornRumble 100.0 100.0 0.8
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	StartOffset
		Type	start
		At	Root
		Sound Zombie_Emerge_01 100.0 100.0 0.85
	End
End

#############################################################

Condition
	On	TriggerBits
	TriggerBits	CONCRETE

	Event
		EName	GroundFX
		Type	start
		At	Root
		ChildFX	:Child_ShieldCharge_GroundFX.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	GRASS

	Event
		EName	GroundFX
		Type	start
		At	Root
		ChildFX	:Child_ShieldCharge_GroundFX.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	GRAVEL

	Event
		EName	GroundFX
		Type	start
		At	Root
		ChildFX	:Child_ShieldCharge_GroundFX.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	DIRT

	Event
		EName	GroundFX
		Type	start
		At	Root
		ChildFX	:Child_ShieldCharge_GroundFX.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	WOOD

	Event
		EName	GroundFX
		Type	start
		At	Root
		ChildFX	:Child_ShieldCharge_GroundFX.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	CARPET

	Event
		EName	GroundFX
		Type	start
		At	Root
		ChildFX	:Child_ShieldCharge_GroundFX.fx
	End
End

Condition
	On	TriggerBits
	TriggerBits	METAL

	Event
		EName	GroundFX
		Type	start
		At	Root
		ChildFX	:Child_ShieldCharge_GroundFX.fx
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	StartOffset_Head
		Type	start
		At	Head
	End

	Event
		EName	StartOffset_Chest
		Type	start
		At	Chest
	End

	Event
		EName	StartOffset_UArmL
		Type	start
		At	LArmL
	End

	Event
		EName	StartOffset_UArmR
		Type	start
		At	LArmR
	End

	Event
		EName	StartOffset_Hips
		Type	start
		At	Hips
	End

	Event
		EName	StartOffset_LLegL
		Type	start
		At	LLegL
	End

	Event
		EName	StartOffset_LLegR
		Type	start
		At	LLegR
	End
End

Condition
	On	Time
	Time	21

	Event
		Type	Local
		At	Head
		part3	:ShieldCharge_BodyStreaks1.part
		POther	StartOffset_Head
		Lifespan 1
	End

	Event
		Type	Local
		At	Chest
		part3	:ShieldCharge_BodyStreaks1.part
		POther	StartOffset_Chest
		Lifespan 1
	End

	Event
		Type	Local
		At	UArmL
		part3	:ShieldCharge_BodyStreaks1.part
		POther	StartOffset_UArmL
		Lifespan 1
	End

	Event
		Type	Local
		At	UArmR
		part3	:ShieldCharge_BodyStreaks1.part
		POther	StartOffset_UArmR
		Lifespan 1
	End

	Event
		Type	Local
		At	Hips
		part3	:ShieldCharge_BodyStreaks1.part
		POther	StartOffset_Hips
		Lifespan 1
	End

	Event
		Type	Local
		At	LLegL
		part3	:ShieldCharge_BodyStreaks1.part
		POther	StartOffset_LLegL
		Lifespan 1
	End

	Event
		Type	Local
		At	LLegR
		part3	:ShieldCharge_BodyStreaks1.part
		POther	StartOffset_LLegR
		Lifespan 1
	End
End


##################################################################
## Physics FX
##################################################################

#Condition
#	Event
#		Ename	LevitationForce
#		Type	Local
#		At	Root
#
#		BhvrOverride
#			PhysForceType		Up
#			PhysForceRadius		5
#			PhysForcePower		60
#			PhysForcePowerJitter	10
#			PhysForceSpeedScaleMax	75
#		End
#	End
#
#	Event
#		Ename	WakeForce
#		Type	Local
#		At	Root
#
#		BhvrOverride
#			PhysForceType		In
#			PhysForceRadius		4
#			PhysForcePower		30
#			PhysForcePowerJitter	5
#			PhysForceSpeedScaleMax	75
#		End
#	End
#End

#########################################################

End