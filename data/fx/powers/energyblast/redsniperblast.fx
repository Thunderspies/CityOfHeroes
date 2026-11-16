#############################################################
## RedSniperBlast.fx
#############################################################

FxInfo

LifeSpan 180

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	HandGlowRight1
		Type	local
		At	WepR
		Part1	:HandGlowRed.part
		Part2	:HandGlowEmberRed.part
		Lifespan 50
	End

	Event
		EName 	HandGlowRight3
		Type	local
		At	WepR
		Part1	:RedSniperBubbles.part
		Part2	:RedSniperBubblesWhite.part
		Lifespan 37
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	HandGlowRight2
		Type	local
		At	WepR
		Part3	:RedSniperCloudsWhite1.part
		Sound sniper3 80 80 .8
		Lifespan 60
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		EName	Prime
		Type	start
		At	WepR
		bhvr	behaviors/projectile3.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Part1	:RapidCometBubblesRed.part
		Part2	:RapidCometBubblesBrightRed.part
		Part3	:RapidCometBubblesLocalRed.part
		Part	:RapidCometBubblesRapidBlack.part
		Part5	:RapidEnergyCoreLocalRed.part
		Part5	:RapidCometBubblesWhite.part
		Geom	RedSniperBolt
		Magnet	Target
		LookAt  Target
	End

	Event
		Ename	LiftForce
		Type	Local
		At	Prime
		BhvrOverride
			pyrRotate		0 0 15
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime

		BhvrOverride
			PositionOffset		0 0 3
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		12
			PhysForcePower		400
			PhysForcePowerJitter	50
		End
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End