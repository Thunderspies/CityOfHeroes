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
		bhvr	CustomizeablePowers\EnergyBlast\BlastScale3.bhvr
		BhvrOverride
			Alpha		1
		End
		Geom	Tintable_MegaCore
		Sound pbexplosive 100.0 100.0 .75
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
		Part3	CustomizeablePowers\EnergyBlast\CometHandStarWhite1.part
		Part4	CustomizeablePowers\EnergyBlast\CometHandCloudsWhite1.part
		Part1	CustomizeablePowers\EnergyBlast\CometHandBubbles.part
		Part2	CustomizeablePowers\EnergyBlast\CometHandBubblesWhite.part
		PMagnet	WepR
		Lifespan 22
	End

	Event
		EName 	HandGlowLeftz
		Type	local
		At	WepL
		Part3	CustomizeablePowers\EnergyBlast\CometHandStarWhite1.part
		Part4	CustomizeablePowers\EnergyBlast\CometHandCloudsWhite1.part
		Part1	CustomizeablePowers\EnergyBlast\CometHandBubbles.part
		Part2	CustomizeablePowers\EnergyBlast\CometHandBubblesWhite.part
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
		Part1	CustomizeablePowers\EnergyBlast\HandGlowWhite3.part
		Part2	CustomizeablePowers\EnergyBlast\HandGlowEmber3.part
		PMagnet	WepR
	End

	Event
		EName 	HandGlowFlashL
		Type	local
		At	WepL
		Part1	CustomizeablePowers\EnergyBlast\HandGlowWhite3.part
		Part2	CustomizeablePowers\EnergyBlast\HandGlowEmber3.part
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
		bhvr	CustomizeablePowers\EnergyBlast\BlastScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_MegaCore
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
		Part5	CustomizeablePowers\EnergyBlast\EnergyCoreLocal.part
		Part1	CustomizeablePowers\EnergyBlast\CoreEnergy.part
		Part2	CustomizeablePowers\EnergyBlast\CoreEnergyBeam.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubbles2.part
		Part4	CustomizeablePowers\EnergyBlast\CometBubbles3.part
		#Magnet	Target
		#LookAt	Target
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
		Part2	CustomizeablePowers\EnergyBlast\CometBubblesWhite.part
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		altpiv	1
		Part1	CustomizeablePowers\EnergyBlast\CometBubbles.part
		Part2	CustomizeablePowers\EnergyBlast\CometBubblesBright.part
		Part3	CustomizeablePowers\EnergyBlast\CometBubblesLocal.part
		Part4	CustomizeablePowers\EnergyBlast\CometHead.part
		Part5	CustomizeablePowers\EnergyBlast\EnergyCoreLocal.part
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