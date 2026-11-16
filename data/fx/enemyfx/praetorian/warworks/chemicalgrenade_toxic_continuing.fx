#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound Plasma_Welding_Ray_Hit_Loop 100 100 .15
		bhvr	behaviors/Sound/SoundIn1.bhvr
	End

	Event
		Type	PositOnly
		At	LArmL
		Part	:Continuing_Toxic_Core.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	LArmR
		Part	:Continuing_Toxic_Core.part
		Part	:Continuing_Toxic_Detached.part
		POther	UArmR
	End

	Event
		Type	PositOnly
		At	UArmL
		Part	:Continuing_Toxic_Core.part
		Part	:Continuing_Toxic_Long.part

		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Continuing_Toxic_Core.part
		Part	:Continuing_Toxic_Long.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	Neck
	#	Part	:Continuing_Plasma_Fire_Core_Detached.part
		Part	:Continuing_Toxic_Core.part
	#	Part	:Continuing_Trail_Plasma.part
		Part	:Continuing_Toxic_Long.part
	#	Part	:Continuing_Plasma_Fire.part

		POther	Hair
	End

	Event
		Type	PositOnly
		At	Chest
	#	Part	:Continuing_Plasma_Fire_Core_Chest.part
		Part	:Continuing_Trail_Toxic.part

	End
End

#############################################################

End