#############################################################
## NonCombatFlight_NoFX.fx
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
		Sound flight2_loop 40.0 40.0 .04

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

#############################################################

End