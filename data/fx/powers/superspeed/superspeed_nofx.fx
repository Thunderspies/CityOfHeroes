#############################################################
## SuperSpeed_NoFX.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound ForceField5_loop 40.0 40.0 .6
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 1
	End

	Event
		EName 	Prime
		Type	Local
		At	Root
		Sound Run5 40.0 40.0 .7
	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		8
			PhysForcePower		200
			PhysForcePowerJitter	15
			PhysForceSpeedScaleMax	60
		End
	End

	Event
		Ename	WakeForce
		Type	Local
		At	Hips

		BhvrOverride
			PhysForceType		In
			PhysForceRadius		10
			PhysForcePower		30
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	120
		End
	End


	Event
		Ename	WakeForce2
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		In
			PhysForceRadius		5
			PhysForcePower		50
			PhysForcePowerJitter	10
			PhysForceSpeedScaleMax	80
		End
	End
End

#############################################################

End