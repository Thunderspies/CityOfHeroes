#############################################################
## Captured_CarnivalOFLight.fx
#############################################################

FxInfo

lifespan 90

#############################################################

Condition
	On	Time
	Time 	50

	Event
		Type 	Local
		At	Origin
		Sound FW_CapturedCarnivalOfLights_Burst_01 50 50 .35
		Sound FW_CapturedCarnivalOfLights_Burst_02 50 50 .35
		Sound FW_CapturedCarnivalOfLights_Burst_03 50 50 .35
	End
End


Condition
	On	Time
	Time	0

	Event
		Ename	GravitationalForce
		Type	Local
		At	Head
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		20
			PhysForcePower		25
			PhysForcePowerJitter	10
		End
		Lifespan 50
	End
End

#############################################################

Condition
	On 	Time
	Time 	50


	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		35
			PhysForcePower		1500
			PhysForcePowerJitter	200
		End
		Lifespan 1
	End
End




Condition
	On 	Time
	Time 	60

	Event
		EName 	Primex
		Type	Local
		At	hips

		Part3	:Captured_Carnivaloflight_DotsBack.part
		Part3	:Captured_Carnivaloflight_DotsGlow.part
		Lifespan 27
	End


End


#############################################################

End
