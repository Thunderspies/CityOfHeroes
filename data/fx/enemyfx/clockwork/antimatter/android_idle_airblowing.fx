#############################################################
## Android_Idle_AirBlowing.fx
#############################################################



FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	C_Neck
		Part	:Idle_AirBlowing_Core.part
		Part	:Idle_AirBlowing_Lingering.part
	End

	Event
		Type	Local
		At	C_Hair
		Part	:Idle_AirBlowing_Core.part
		Part	:Idle_AirBlowing_Lingering.part

	End

	Event
		Type	Local
		At	C_Hair
		Part	:Idle_AirBlowing_Leaf1.part
		Part	:Idle_AirBlowing_Leaf2.part
	End

	Event
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		120
			PhysForcePowerJitter	10
		End
	End
	
	Event
		Type	Local
		At	Origin
		Sound Clockwork_CleanGround_Loop 75 75 .35
		bhvr	behaviors/sound/SoundIn2Out2.bhvr
	End
End


#############################################################


#############################################################

End