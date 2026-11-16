#############################################################
## Sprint.fx
#############################################################

FxInfo

#############################################################

Condition

	On 		Cycle
	Time		3
	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Bhvr	:CracksTrail_Camerashake.bhvr
		Lifespan 6
	End
End




#
#Condition
#	On	TriggerBits
#	TriggerBits	CONCRETE
#	DoMany		1
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	Root
#		While	CONCRETE
#		Flags	OneAtaTime
#		Part	:Crackstrail_DustCloud.part
#		Part	:Crackstrail_DustCloud2.part
#		part	:Crackstrail_Rocks.part
#	End
#End
#
#Condition
#	On	TriggerBits
#	TriggerBits	GRASS
#	DoMany		1
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	Root
#		While	GRASS
#		Flags	OneAtaTime
#		Part	:Crackstrail_DustCloud.part
#		Part	:Crackstrail_DustCloud2.part
#		part	:Crackstrail_Rocks.part
#	End
#End
#
#Condition
#	On	TriggerBits
#	TriggerBits	GRAVEL
#	DoMany		1
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	Root
#		While	Air
#		Flags	OneAtaTime
#		Part	:Crackstrail_DustCloud.part
#		Part	:Crackstrail_DustCloud2.part
#		part	:Crackstrail_Rocks.part
#	End
#End
#
#Condition
#	On	TriggerBits
#	TriggerBits	DIRT
#	DoMany		1
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	Root
#		While	DIRT
#		Flags	OneAtaTime
#		Part	:Crackstrail_DustCloud.part
#		Part	:Crackstrail_DustCloud2.part
#		part	:Crackstrail_Rocks.part
#	End
#End
#
#Condition
#	On	TriggerBits
#	TriggerBits	WOOD
#	DoMany		1
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	Root
#		While	WOOD
#		Flags	OneAtaTime
#		Part	:Crackstrail_DustCloud.part
#		Part	:Crackstrail_DustCloud2.part
#		part	:Crackstrail_Rocks.part
#	End
#End
#
#Condition
#	On	TriggerBits
#	TriggerBits	CARPET
#	DoMany		1
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	Root
#		While	CARPET
#		Flags	OneAtaTime
#		Part	:Crackstrail_DustCloud.part
#		Part	:Crackstrail_DustCloud2.part
#		part	:Crackstrail_Rocks.part
#	End
#End
#
#Condition
#	On	TriggerBits
#	TriggerBits	METAL
#	DoMany		1
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	Root
#		While	METAL
#		Flags	OneAtaTime
#		Part	:Crackstrail_DustCloud.part
#		Part	:Crackstrail_DustCloud2.part
#		part	:Crackstrail_Rocks.part
#	End
#End
#
####################################	Forces	  ########################
#
#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	LevitationForce
#		Type	Local
#		At	Root
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
#		BhvrOverride
#			PhysForceType		In
#			PhysForceRadius		4
#			PhysForcePower		30
#			PhysForcePowerJitter	5
#			PhysForceSpeedScaleMax	75
#		End
#	End
#End

#############################################################

End