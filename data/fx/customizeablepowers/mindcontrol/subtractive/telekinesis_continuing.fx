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

Input  
	InpName	root
End

Input  
	InpName T_root
End

Input  
	InpName Head
End
##################################

Condition
	On	Time
	Time	0

	Event	
		ENAME	poo
		Type	start
		At	root
		part1	CustomizeablePowers\MindControl\LevitateMistBright2.part
		#part2	Powers\AirControl\LevitateMistBright.part
		part3	CustomizeablePowers\MindControl\EnsnareRing1.part
		geom	AirFunnel03
		bhvr	CustomizeablePowers\MindControl\LevitateScale.bhvr
		PMAGNET	Head
		
		
		LifeSpan	30

	End
	
	Event
		Ename	LevitationForce2
		Type	Local
		At	Hips
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		40
			PhysForcePowerJitter	10
		End

	End
	
End


Condition
	On	Time
	Time	15
	
	Event	
		ENAME	poo2
		Type	local
		At	poo
		altpiv	1
		part1	CustomizeablePowers\MindControl\LevitateSplash.part
		part2	CustomizeablePowers\MindControl\LevitateSteam.part
		part3	CustomizeablePowers\MindControl\LevitateRocks.part
		

		LifeSpan	30
		
	End

	Event	
		Ename   targetchill
		Type	Local
		At	FootR
		part1	CustomizeablePowers\MindControl\FlightCloud.part
		part2	CustomizeablePowers\MindControl\FlightCloudSteam.part
		part3	CustomizeablePowers\MindControl\FlightSteam.part
		part4	CustomizeablePowers\MindControl\FlightCloudSteam2.part
		Part5	CustomizeablePowers\MindControl\FlightBodyMist.part
		

	End

	Event	
		Ename   targetchill
		Type	Local
		At	FootL
		part1	CustomizeablePowers\MindControl\FlightCloud.part
		part2	CustomizeablePowers\MindControl\FlightCloudSteam.part
		part3	CustomizeablePowers\MindControl\FlightSteam.part
		part4	CustomizeablePowers\MindControl\FlightCloudSteam2.part
		Part5	CustomizeablePowers\MindControl\FlightBodyMist.part
		
	End
		
	Event	
		Ename   targetchill
		Type	Local
		At	WepL
		part1	CustomizeablePowers\MindControl\FlightCloud.part
		part2	CustomizeablePowers\MindControl\FlightCloudSteam.part
		part3	CustomizeablePowers\MindControl\FlightSteam.part
		part4	CustomizeablePowers\MindControl\FlightCloudSteam2.part
		Part5	CustomizeablePowers\MindControl\FlightBodyMist.part
	End

	Event	
		Ename   targetchill
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MindControl\FlightCloud.part
		part2	CustomizeablePowers\MindControl\FlightCloudSteam.part
		part3	CustomizeablePowers\MindControl\FlightSteam.part
		part4	CustomizeablePowers\MindControl\FlightCloudSteam2.part
		Part5	CustomizeablePowers\MindControl\FlightBodyMist.part
	End

End

End				