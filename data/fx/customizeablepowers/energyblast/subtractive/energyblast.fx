#############################################################
## EnergyBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	9

	Event
		Type	local
		At	BendMystic
		Sound PowerBolt4 100.0 100.0 .7
		Lifespan 1
	End
End

Condition
	On	Time
	Time	19

	Event
		EName	core
		Type	local
		At	BendMystic
		Bhvr	:BlastScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Subtractive_MegaCore
		Lifespan 21
	End

	Event
		Ename	PushDirection
		Type	Posit
		At	core
		Lookat	Target
	End
End

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	WepR
		Part3	:CometHandStarWhite1.part
		Part4	:CometHandCloudsWhite1.part
		PMagnet	WepR
		Lifespan 22
	End

	Event
		Type	local
		At	WepL
		Part3	:CometHandStarWhite1.part
		Part4	:CometHandCloudsWhite1.part
		PMagnet	WepL
		Lifespan 22
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	WepR
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		Part2	:CometHandBubblesWhite_Alpha.part

		PMagnet	WepR
		Lifespan 19
	End

	Event
		Type	local
		At	WepL
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		Part2	:CometHandBubblesWhite_Alpha.part

		PMagnet	WepL
		Lifespan 19
	End
End

Condition
	On 	Time
	Time 	37

	Event
		EName 	HandGlowFlashR
		Type	local
		At	WepR
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3.part
		Part2	:HandGlowEmber3_Alpha.part
		PMagnet	WepR
	End

	Event
		EName 	HandGlowFlashL
		Type	local
		At	WepL
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3.part
		Part2	:HandGlowEmber3_Alpha.part
		PMagnet	WepL
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName	Prime
		Type	start
		At	BendMystic
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Geom	CenterDummy
		Part2	:CometBubblesWhite.part
		Magnet	Target
		LookAt  Target
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		altpiv	1
		Part1	:CometBubbles.part
		Part2	:CometBubblesBright.part
		Part3	:CometBubblesLocal.part
		Part4	:CometHead.part
		Part4	:CometHead_Alpha.part

		Part5	:EnergyCoreLocal.part
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

Condition
	On	Time
	Time	20

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
		Lifespan 20
	End
End

Condition
	On	Time
	Time	35

	Event
		EName	Prime1
		Type	start
		At	BendMystic
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Geom	CenterDummy
		Part2	:CometBubblesWhite.part
		Magnet	Target
		LookAt  Target
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
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

Condition
	On	Time
	Time	40

	Event
		EName	Prime2
		Type	start
		At	BendMystic
		Bhvr	behaviors\projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Geom	CenterDummy
		Part2	:CometBubblesWhite.part
		Magnet	Target
		LookAt  Target
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
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		LifeSpan 1
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	cometParts
		Type	Destroy
	End
End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
	End

	Event
		EName 	cometParts02
		Type	Destroy
	End
End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End

	Event
		EName 	cometParts03
		Type	Destroy
	End
End

#############################################################

End