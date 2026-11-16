#########################################################
##	chill touch hit
FxInfo


Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End


##################################
Condition
	On	Time
	Time	0
	
	Event	
		Ename   targetchill
		Type	Local
		At	FootR
		part1	:FlightCloud.part
		part2	:FlightCloudSteam.part
		part3	:FlightSteam.part
		part4	:FlightCloudSteam2.part
		Part5	:FlightBodyMist.part
		Sound flight2_loop 40.0 40.0 .09

	End

	Event	
		Ename   targetchill
		Type	Local
		At	FootL
		part1	:FlightCloud.part
		part2	:FlightCloudSteam.part
		part3	:FlightSteam.part
		part4	:FlightCloudSteam2.part
		Part5	:FlightBodyMist.part
	
	End
		
	Event	
		Ename   targetchill
		Type	Local
		At	WepL
		part1	:FlightCloud.part
		part2	:FlightCloudSteam.part
		part3	:FlightSteam.part
		part4	:FlightCloudSteam2.part
		Part5	:FlightBodyMist.part
	End

	Event	
		Ename   targetchill
		Type	Local
		At	WepR
		part1	:FlightCloud.part
		part2	:FlightCloudSteam.part
		part3	:FlightSteam.part
		part4	:FlightCloudSteam2.part
		Part5	:FlightBodyMist.part
	End

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

End				