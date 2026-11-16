#########################################################
##	chill touch hit
FxInfo

LifeSpan	45

############################################################




Condition

	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound MakoBellow 70 70 .99
	End

End



Condition

	On	Time
	Time	11
	Event
		Type	Local
		At	Root
		
		Bhvr	POWERS\SonicControl\ScreamPBAOE\CameraShake01.bhvr
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
		Part	POWERS\SonicControl\ScreamPBAOE\CoreRings.part
		Part	POWERS\SonicControl\ScreamPBAOE\RingsOut2.part
		LifeSpan	23

	End

	Event
		EName	RingsDown
		Type	Local
		At	HookUp
		Altpiv	1
		Part	POWERS\SonicControl\ScreamPBAOE\CoreRingsDown.part
		Part	POWERS\SonicControl\ScreamPBAOE\RingsDown.part
		LifeSpan	23

	End

	Event
		EName	RingsOut
		Type	Local
		At	Root
		Part	POWERS\SonicControl\ScreamPBAOE\CoreRingsOut.part
		Part	POWERS\SonicControl\ScreamPBAOE\RingsOut.part
		LifeSpan	32
		
	End
End


Condition
	On	Cycle
	Time	14

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Head
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		60
			PhysForcePower		125
			PhysForcePowerJitter	10
		End

		Lifespan	1

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

End

End			