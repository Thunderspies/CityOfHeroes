#############################################################
## Mega.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	WepR
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		part3	:CometHandCloudsWhite.part
		Sound Mega6 100.0 100.0	0.8
		PMagnet	WepR
		Lifespan 60
	End
End

#############################################################

Condition
	On	Time
	Time	22

	Event
		EName	core
		Type	Start
		At	WepR
		Bhvr	:BlastScale2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_MegaCore
		Lookat	Target
		Lifespan 38
	End

	Event
		EName	wave
		Type	Start
		At	WepR
		Bhvr	:BlastWave.bhvr
		BhvrOverride
			TintGeom	1

		End
		Geom	Tintable_MegaWave
		Lookat	Target
		Lifespan 28
	End

	Event
		EName	shell
		Type	Start
		At	WepR
		Bhvr	:BlastScale.bhvr
		BhvrOverride
			TintGeom	1

		End
		Geom	Tintable_MegaHull
		Lookat	Target
		Lifespan 26
	End

	Event
		Ename	PushDirection
		Type	Local
		At	core
		Lookat	Target
	End
End

#############################################################

Condition
	On	Time
	Time	33

	Event
		EName	ring
		Type	Start
		At	WepR
		Bhvr	:BlastRingScale.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_MegaRing
		Lookat	Target
		LifeSpan 5
	End

	Event
		EName	Prime
		Type	start
		At	WepR
		Bhvr	Behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Geom	CenterDummy
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	LiftForce
		Type	Local
		At	Prime
		BhvrOverride
			pyrRotate		0 0 30
			PhysForceType		Side
			PhysForceRadius		12
			PhysForcePower		-200
			PhysForcePowerJitter	100
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		15
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End

	Event
		EName	cometParts01
		Type	Local
		At	prime
		altpiv	1
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part4	:CometHead.part
		Part5	:EnergyCoreLocal.part
	End

	Event
		EName	cometParts01a
		Type	Local
		At	prime
		altpiv	1
		Part1	:CometBubblesWhite.part
	End

	Event
		EName	cometPartsLocal
		Type	Local
		At	core
		altpiv	1
		Part5	:EnergyCoreLocal.part
		Part1	:CoreEnergy.part
		Part2	:CoreEnergyBeam.part
		Part3	:CometBubbles2.part
		Part4	:CometBubbles3.part
	End
End

#############################################################

Condition
	On	Time
	Time	40

	Event
		EName	ring2
		Type	start
		At	WepR
		Bhvr	:BlastRingScale.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_MegaRing
		Lookat	Target
		LifeSpan 5
	End

	Event
		EName	Prime1
		Type	start
		At	WepR
		Bhvr	Behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Geom	CenterDummy
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	LiftForce
		Type	Local
		At	Prime1
		BhvrOverride
			pyrRotate		0 0 30
			PhysForceType		Side
			PhysForceRadius		12
			PhysForcePower		-200
			PhysForcePowerJitter	100
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		15
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End

	Event
		EName	cometParts02
		Type	Local
		At	Prime1
		altpiv	1
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part4	:CometHead.part
		Part5	:EnergyCoreLocal.part
	End

	Event
		EName	cometParts02a
		Type	Local
		At	Prime1
		altpiv	1
		Part1	:CometBubblesWhite.part
	End
End

#############################################################

Condition
	On	Time
	Time	55

	Event
		EName	ring3
		Type	start
		At	WepR
		Bhvr	:BlastRingScalex.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_MegaRing
		Lookat	Target
		LifeSpan 5
	End
End

Condition
	On	Time
	Time	60

	Event
		EName	Prime2
		Type	start
		At	WepR
		Bhvr	Behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Geom	CenterDummy
		Magnet	Target
		LookAt	Target
	End

	Event
		Ename	LiftForce
		Type	Local
		At	Prime2
		BhvrOverride
			pyrRotate		0 0 30
			PhysForceType		Side
			PhysForceRadius		12
			PhysForcePower		-200
			PhysForcePowerJitter	100
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		15
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End

	Event
		EName	cometParts03
		Type	Local
		At	Prime2
		altpiv	1
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part4	:CometHead.part
		Part5	:EnergyCoreLocal.part
	End

	Event
		EName	cometParts03a
		Type	Local
		At	Prime2
		altpiv	1
		Part1	:CometBubblesWhite.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

	Event
		EName	Comet01
		Type	Destroy
	End

	Event
		EName	CometParts01
		Type	Destroy
	End

	Event
		EName	CometParts01a
		Type	Destroy
	End
End

Condition
	On	Prime1Hit

	Event
		EName	Prime1
		Type	Destroy
	End

	Event
		EName	Comet02
		Type	Destroy
	End

	Event
		EName	CometParts02
		Type	Destroy
	End

	Event
		EName	CometParts02a
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		EName	Prime2
		Type	Destroy
	End

	Event
		EName	Comet03
		Type	Destroy
	End

	Event
		EName	CometParts03
		Type	Destroy
	End

	Event
		EName	CometParts03a
		Type	Destroy
	End
End

#############################################################

End