#############################################################
## TelekineticBlast.fx
#############################################################

FxInfo

LifeSpan 150

#############################################################

Condition
	On 	Time
	Time 	14

	Event
		EName	core
		Type	Start
		At	Chest
		Geom	PsiTele
		Sound telekinetic2 80 80 .8
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		BhvrOverride
			PositionOffset	-1.5 2.0 2.0
			PYRRotate 	120 -15 -180
		End
		part1	CustomizeablePowers\PsychicBlast\PsionicTeleRingsSmall_Cust.part
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Core
		Altpiv	1
		BhvrOverride
			PositionOffset	-1.5 2.0 2.0
			PYRRotate 	120 -15 180
		End
		part2	:PsionicGlow2_Cust.part
		Lifespan 25
	End
End

#############################################################

Condition
	On 	Time
	Time 	15



	Event
		Ename	Prime
		Type	StartPositOnly
		At	Core
		BhvrOverride
			TrackRate	4.0
		End
		Lookat	Target
		Magnet	Target
		LifeSpan 45
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset		0 0 7.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		200
			PhysForcePowerJitter	100
		End
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		Ename 	PushForce
		Type	Destroy
	End
End

#############################################################

End