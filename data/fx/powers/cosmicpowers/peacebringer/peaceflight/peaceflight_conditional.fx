#############################################################
## Peacebringer - Energy Flight
#############################################################

FxInfo


#############################################################

Condition
	On	Time
	Time	0
	
	Event	
		Ename   targetchill
		Type	Local
		At	FootR
		part	:PeaceFlightCloud.part
		part	:PeaceFlightCloudSteam.part
		part	:PeaceFlightSteam.part
		part	:PeaceFlightCloudSteam2.part
		Part	:PeaceFlightBodyMist.part
		part	:PeaceCoreEnergy2.part
		Sound flight2_loop 40.0 40.0 .09
	End

	Event	
		Ename   targetchill
		Type	Local
		At	FootL
		part1	:PeaceFlightCloud.part
		part2	:PeaceFlightCloudSteam.part
		part3	:PeaceFlightSteam.part
		part4	:PeaceFlightCloudSteam2.part
		Part5	:PeaceFlightBodyMist.part
		part	:PeaceCoreEnergy2.part
	End
		
	Event	
		Ename   targetchill
		Type	Local
		At	WepL
		part1	:PeaceFlightCloud.part
		part2	:PeaceFlightCloudSteam.part
		part3	:PeaceFlightSteam.part
		part4	:PeaceFlightCloudSteam2.part
		Part5	:PeaceFlightBodyMist.part
		part	:PeaceCoreEnergy2.part
	End

	Event	
		Ename   targetchill
		Type	Local
		At	WepR
		part1	:PeaceFlightCloud.part
		part2	:PeaceFlightCloudSteam.part
		part3	:PeaceFlightSteam.part
		part4	:PeaceFlightCloudSteam2.part
		Part5	:PeaceFlightBodyMist.part
		part	:PeaceCoreEnergy2.part
	End

## PHYSICS FORCES ###########################################################

	Event
		Ename	ConstantLevitationForce
		Type	Posit
		At	Hips
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		7
			PhysForcePower		25
			PhysForcePowerJitter	10
		End
	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Hips
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		12
			PhysForcePower		75
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	60
		End
	End
	
	Event
		Ename	WakeForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		10
			PhysForcePower		40
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	75
		End
	End
End

#############################################################

End				