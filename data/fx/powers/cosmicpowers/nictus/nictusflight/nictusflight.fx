#########################################################
##	chill touch hit
FxInfo

##################################
Condition
	On	Time
	Time	0
	
	Event	
		Ename   targetchill
		Type	Local
		At	FootR
		part	:NictusFlightCloud.part
		part	:NictusFlightCloudSteam.part
		part	:NictusFlightSteam.part
		part	:NictusFlightCloudSteam2.part
		Part	:NictusFlightBodyMist.part
		Part	:BlueTendrils.part
		Part	:PurpleTendrils.part
		Sound flight2_loop 40.0 40.0 .09

	End

	Event	
		Ename   targetchill
		Type	Local
		At	FootL
		part	:NictusFlightCloud.part
		part	:NictusFlightCloudSteam.part
		part	:NictusFlightSteam.part
		part	:NictusFlightCloudSteam2.part
		Part	:NictusFlightBodyMist.part
		Part	:BlueTendrils.part
		Part	:PurpleTendrils.part
		
	End
		
	Event	
		Ename   targetchill
		Type	Local
		At	WepL
		part	:NictusFlightCloud.part
		part	:NictusFlightCloudSteam.part
		part	:NictusFlightSteam.part
		part	:NictusFlightCloudSteam2.part
		Part	:NictusFlightBodyMist.part
		Part	:BlueTendrils.part
		Part	:PurpleTendrils.part
	End

	Event	
		Ename   targetchill
		Type	Local
		At	WepR
		part	:NictusFlightCloud.part
		part	:NictusFlightCloudSteam.part
		part	:NictusFlightSteam.part
		part	:NictusFlightCloudSteam2.part
		Part	:NictusFlightBodyMist.part
		Part	:BlueTendrils.part
		Part	:PurpleTendrils.part
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