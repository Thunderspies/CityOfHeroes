#########################################################
##	chill touch hit
FxInfo

LifeSpan	60

############################################################




Condition

	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound ScreamPBAOE 100 100 1.0
	End

End



Condition

	On	Time
	Time	11
	Event
		Type	Local
		At	Root
		
		Bhvr	:CameraShake01.bhvr
	End

End

Condition

	Event
		EName	HookUp
		Type	Local
		At	Root
		Geom	RootToHeadAdjustment
		#Sound	MaestroScream 80 70 .99
		LifeSpan	32
	End

	Event
		EName	Rings
		Type	Local
		At	Head
		Part	:CoreRings.part
		Part	:RingsOut2.part
		LifeSpan	23

	End

	Event
		EName	RingsDown
		Type	Local
		At	HookUp
		Altpiv	1
		Part	:CoreRingsDown.part
		Part	:RingsDown.part
		LifeSpan	23

	End

	Event
		EName	RingsOut
		Type	Local
		At	Root
		Part	:CoreRingsOut.part
		Part	:RingsOut.part
		LifeSpan	32
		
	End

End

Condition
	On	Cycle
	Time	16

	Event
		Ename	VibrationForce
		Type	Start
		At	Head
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		55
			PhysForcePower		75
			PhysForcePowerJitter	25
		End

		Lifespan	1

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Head
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		60
			PhysForcePower		25
			PhysForcePowerJitter	10
		End

		Lifespan	1

	End
End

End			