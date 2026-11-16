#############################################################
## Energy Blast - Explosive Blast
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	13

	Event
		EName	core
		Type	local
		At	BendMystic
		bhvr	behaviors/BlastScale3.bhvr
		bhvrOverride
			Alpha	0
		End
		Sound pbexplosive 100.0 100.0 .75
		geom	MegaCore
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
	Time	25

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
		LifeSpan	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		EName 	HandGlowRightz
		Type	local
		At	WepR
		Part3	:CometHandStarWhite1.part
		Part4	:CometHandCloudsWhite1.part
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeftz
		Type	local
		At	WepL
		Part3	:CometHandStarWhite1.part
		Part4	:CometHandCloudsWhite1.part
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		PMagnet	WepL
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
		PMagnet	WepR
	End

	Event
		EName 	HandGlowFlashL
		Type	local
		At	WepL
		Part1	:HandGlowWhite3.part
		Part2	:HandGlowEmber3.part
		PMagnet	WepL
	End
End

###################################################################

Condition
	On	Time
	Time	23

	Event
		EName	cone
		Type	Local
		At	BendMystic
		bhvr	behaviors/BlastScale3.bhvr
		geom	MegaCore
	End
End

## PROJECTILE #################################################################

Condition
	On	Time
	Time	25

	Event
		EName	Prime
		Type	start
		At	BendMystic
		bhvr	behaviors/projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
		Magnet	Target
		LookAt  Target
		geom	CenterDummy
		Part2	:CometBubblesWhite.part
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
		Part5	:EnergyCoreLocal.part
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
		#Magnet	Target
		#LookAt	Target
	End
End

## CLEAN UP ###########################################################

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
	On 	Time
	Time 	37

	Event
		EName 	HandGlowRight1
		Type	Destroy
	End

	Event
		EName 	HandGlowLeft1
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName 	cometPartsLocal
		Type	Destroy
	End

	Event
		EName 	Core
		Type	Destroy
	End

	Event
		EName 	Cone
		Type	Destroy
	End

	Event
		EName 	HandGlowRightz
		Type	Destroy
	End

	Event
		EName 	HandGlowLeftz
		Type	Destroy
	End
End

#############################################################

End