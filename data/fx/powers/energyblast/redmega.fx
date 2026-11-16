#############################################################
## RedMega.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	HandGlowRight1
		Type	local
		At	WepR
		Part1	:CometHandBubblesRed2.part
		Part2	:CometHandBubblesWhite2.part
		part3	:CometHandBubbles3red.part
		PMagnet	WepR
		Sound Mega6 100.0 100.0 .8
		LifeSpan 40
	End

	Event
		EName 	HandGlowLeft1
		Type	local
		At	WepL
		Part1	:CometHandBubblesRed2.part
		Part2	:CometHandBubblesWhite2.part
		part3	:CometHandBubbles3red.part
		PMagnet	WepL
		LifeSpan 40
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName 	HandGlowRightz
		Type	local
		At	WepR
		Part3	:CometHandStarRed1.part
		Part4	:CometHandCloudsRed1.part
		PMagnet	WepR
		LifeSpan 30
	End

	Event
		EName 	HandGlowLeftz
		Type	local
		At	WepL
		Part3	:CometHandStarRed1.part
		Part4	:CometHandCloudsRed1.part
		PMagnet	WepL
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	22


	Event
		EName	core
		Type	local
		At	BendMystic
		bhvr	behaviors\BlastScale2.bhvr
		geom	CoreWolfPack
		LifeSpan 38
	End

	Event
		EName	wave
		Type	local
		At	BendMystic
		bhvr	behaviors\BlastWave.bhvr
		geom	RedMegaWave
		Lifespan 28
	End

	Event
		EName	shell
		Type	local
		At	BendMystic
		geom	RedMegaHull
		bhvr	behaviors\RedBlastScale.bhvr
		Lifespan 32
	End

	Event
		Ename	PushDirection
		Type	Posit
		At	core
		Lookat	Target
	End
End

Condition
	On	Time
	Time	33

	Event
		EName	ring
		Type	start
		At	BendMystic
		geom	RedMegaRing
		bhvr	behaviors\RedBlastRingScale.bhvr
		LifeSpan 5
	End

	Event
		EName	Prime
		Type	start
		At	BendMystic
		bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		geom	CenterDummy
		Magnet	Target
		LookAt  Target
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
		Part1	:CometBubblesRed.part
		Part2	:CometBubblesBrightRed.part
		Part3	:CometBubblesLocalRed.part
		Part4	:CometHeadRed.part
		Part5	:EnergyCoreLocalRed.part
	End

	Event
		EName	cometParts01a
		Type	Local
		At	prime
		altpiv	1
		Part1	:CometBubblesRed.part
	End

	Event
		EName	cometPartsLocal
		Type	Local
		At	core
		altpiv  1
		Part5	:EnergyCoreLocalRed.part
		Part1	:CoreEnergyRed.part
		Part2	:CoreEnergyBeamRed.part
		Part3	:CometBubbles2Red.part
		Part4	:CometBubbles3Red.part
		LifeSpan	15
	End
End

Condition
	On	Time
	Time	40

	Event
		EName	ring2
		Type	start
		At	BendMystic
		geom	RedMegaRing
		bhvr	behaviors\RedBlastRingScale.bhvr
		LifeSpan 5
	End

	Event
		EName	Prime1
		Type	start
		At	BendMystic
		bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		geom	CenterDummy
		Magnet	Target
		LookAt  Target
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
		LifeSpan	1
	End

	Event
		EName	cometParts02
		Type	Local
		At	Prime1
		altpiv	1
		Part1	:CometBubblesRed.part
		Part2	:CometBubblesBrightRed.part
		Part3	:CometBubblesLocalRed.part
		Part4	:CometHeadRed.part
		Part5	:EnergyCoreLocalRed.part
	End

	Event
		EName	cometParts02a
		Type	Local
		At	Prime1
		altpiv	1
		Part1	:CometBubblesRed.part
	End
End

Condition
	On	Time
	Time	55

	Event
		EName	ring3
		Type	start
		At	BendMystic
		geom	RedMegaRing
		bhvr	behaviors\BlastRingScalex.bhvr
		LifeSpan 5
	End
End

Condition
	On	Time
	Time	50

	Event
		EName	Prime2
		Type	start
		At	BendMystic
		geom	CenterDummy
		bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Magnet	Target
		LookAt  Target
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
		LifeSpan	1
	End

	Event
		EName	cometParts03
		Type	Local
		At	Prime2
		altpiv	1
		Part1	:CometBubblesRed.part
		Part2	:CometBubblesBrightRed.part
		Part3	:CometBubblesLocalRed.part
		Part4	:CometHeadRed.part
		Part5	:EnergyCoreLocalRed.part
	End

	Event
		EName	cometParts03a
		Type	Local
		At	Prime2
		altpiv	1
		Part1	:CometBubblesRed.part
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