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
		bhvr	:BlastScale3.bhvr
		BhvrOverride
			Alpha		1
		End
		Geom	Subtractive_MegaCore
		Sound pbexplosive 100.0 100.0 0.75
		Lifespan 27
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
		LifeSpan 1
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
		Part2	:CometHandBubblesWhite_Alpha.part
		PMagnet	WepR
		Lifespan 22
	End

	Event
		EName 	HandGlowLeftz
		Type	local
		At	WepL
		Part3	:CometHandStarWhite1.part
		Part4	:CometHandCloudsWhite1.part
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		Part2	:CometHandBubblesWhite_Alpha.part
		PMagnet	WepL
		Lifespan 22
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

###################################################################

Condition
	On	Time
	Time	23

	Event
		EName	Cone
		Type	Local
		At	BendMystic
		bhvr	:BlastScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Subtractive_MegaCore
		Lifespan 17
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

## PROJECTILE #################################################################

Condition
	On	Time
	Time	25

	Event
		EName	Prime
		Type	start
		At	BendMystic
		bhvr	behaviors\projectile1.bhvr
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

#############################################################

End