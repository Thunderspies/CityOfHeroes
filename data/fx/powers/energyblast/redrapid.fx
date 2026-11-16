#############################################################
## RedRapid.fx
#############################################################

FxInfo

LifeSpan 180

#############################################################

Condition
	On	Time
	Time	0


	Event
		EName	core
		Type	Local
		At	Hips

		Sound PowerBolt11 100.0 100.0 .85
	End

	Event
		Ename	PushDirection
		Type	Posit
		At	core

		Lookat	Target

	End
End


Condition
	On 	Time
	Time 	7

	Event
		EName 	HandGlowRight1
		Type	local
		At	WepR
		Part1	:CometHandBubblesRed.part
		#Part	:CometHandBubblesRed2.part
		Part	:CometHandBubblesWhite2.part
		Part	:CometHandBubbles3red.part
		PMagnet	WepR
		Lifespan 33
	End

	Event
		EName 	HandGlowLeft1
		Type	local
		At	WepL
		Part1	:CometHandBubblesRed.part
		#Part	:CometHandBubblesRed2.part
		Part	:CometHandBubblesWhite2.part
		Part	:CometHandBubbles3red.part
		PMagnet	WepL
		Lifespan 33
	End
End


Condition
	On 	Time
	Time 	17

	Event
		EName 	HandGlowRight2
		Type	local
		At	WepR
		Part1	:HandGlowRed.part
		Part2	:HandGlowEmberRed.part
		PMagnet	WepR
		Lifespan 33
	End

	Event
		EName 	HandGlowLeft2
		Type	local
		At	WepL
		Part1	:HandGlowRed.part
		Part2	:HandGlowEmberRed.part
		PMagnet	WepL
		Lifespan 33
	End
End

###################################################################
##2ndFade
###################################################################
Condition
	On 	Time
	Time 	43
	Event
		EName 	HandGlowRight
		Type	local
		At	WepR
		#Part1	:HandGlowWhite2.part
		#Part2	:HandGlowEmber2.part
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeft
		Type	local
		At	WepL
		#Part1	:HandGlowWhite2.part
		#Part2	:HandGlowEmber2.part
		PMagnet	WepL
	End

End

###################################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	HandGlowRight22
		Type	local
		At	WepR
		Part1	:HandGlowRed2.part
		Part2	:HandGlowEmberRed.part
		PMagnet	WepR
		Lifespan 53
	End

	Event
		EName 	HandGlowLeft22
		Type	local
		At	WepL
		Part1	:HandGlowRed2.part
		Part2	:HandGlowEmberRed.part
		PMagnet	WepL
		Lifespan 53
	End
End

## PROJECTILES #################################################################

Condition
	On	Time
	Time	26

	Event
		EName	Prime
		Type	start
		At	WepR
		Bhvr	behaviors/projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		#Part1	:RapidCometHead.part
		#Part2	:RapidBlastStreak.part
		Part3	:RapidCometBubblesRed.part
		Magnet	Target
		LookAt  Target
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
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		LifeSpan 1
	End

	Event
		EName	cometParts
		Type	Local
		At	Prime
		Part1	:RapidCometBubblesRedRapid.part
		Part2	:RapidCometBubblesBrightRed.part
		Part3	:RapidCometBubblesLocalRed.part
		Part	:RapidCometBubblesRapidBlack.part
		Part5	:RapidEnergyCoreLocalRed.part
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	Prime1
		Type	start
		At	WepL
		Bhvr	behaviors/projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		#Part1	:RapidCometHead.part
		#Part2	:RapidBlastStreak.part
		Part3	:RapidCometBubblesRed.part
		Magnet	Target
		LookAt  Target
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
			PhysForcePower		200
			PhysForcePowerJitter	50
		End
		LifeSpan 1
	End

	Event
		EName	cometParts1
		Type	Local
		At	Prime1
		Part	:RapidCometBubblesRedRapid.part
		Part	:RapidCometBubblesBrightRed.part
		Part	:RapidCometBubblesRapidBlack.part
		Part	:RapidCometBubblesLocalRed.part
		Part	:RapidEnergyCoreLocalRed.part
	End
End

#Condition
#	On	Time
#	Time	30
#
#	Event
#		EName	Prime2
#		Type	start
#		At	WepR
#		bhvr	behaviors/projectile1.bhvr
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		10
#			PhysForcePower		100
#			PhysForcePowerJitter	50
#		End
#		#Part1	:RapidCometHead.part
#		#Part2	:RapidBlastStreak.part
#		Part3	:RapidCometBubblesRed.part
#		Magnet	Target
#		LookAt  Target
#	End
#
#	Event
#		Ename	PushForce
#		Type	Local
#		At	PushDirection
#		BhvrOverride
#			PositionOffset		0 0 5
#			pyrRotate		15 0 0
#			PhysForceType		Forward
#			PhysForceRadius		10
#			PhysForcePower		200
#			PhysForcePowerJitter	50
#		End
#		LifeSpan 1
#	End
#
#	Event
#		EName	cometParts2
#		Type	Local
#		At	Prime2
#		Part1	:RapidCometBubblesRedRapid.part
#		Part2	:RapidCometBubblesBrightRed.part
#		Part3	:RapidCometBubblesLocalRed.part
#		Part	:RapidCometBubblesRapidBlack.part
#		Part5	:RapidEnergyCoreLocalRed.part
#	End
#End
#
#Condition
#	On	Time
#	Time	32
#
#	Event
#		EName	Prime3
#		Type	start
#		At	WepL
#		bhvr	behaviors/projectile1.bhvr
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		10
#			PhysForcePower		100
#			PhysForcePowerJitter	50
#		End
#		#Part1	:RapidCometHead.part
#		#Part2	:RapidBlastStreak.part
#		Part3	:RapidCometBubblesRed.part
#		Magnet	Target
#		LookAt  Target
#	End
#
#	Event
#		Ename	PushForce
#		Type	Local
#		At	PushDirection
#		BhvrOverride
#			PositionOffset		0 0 5
#			pyrRotate		15 0 0
#			PhysForceType		Forward
#			PhysForceRadius		10
#			PhysForcePower		200
#			PhysForcePowerJitter	50
#		End
#		LifeSpan 1
#	End
#
#	Event
#		EName	cometParts3
#		Type	Local
#		At	Prime3
#		Part1	:RapidCometBubblesRedRapid.part
#		Part2	:RapidCometBubblesBrightRed.part
#		Part3	:RapidCometBubblesLocalRed.part
#		Part	:RapidCometBubblesRapidBlack.part
#		Part5	:RapidEnergyCoreLocalRed.part
#	End
#End
#
#Condition
#	On	Time
#	Time	34
#
#	Event
#		EName	Prime4
#		Type	start
#		At	WepR
#		bhvr	behaviors/projectile1.bhvr
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		10
#			PhysForcePower		100
#			PhysForcePowerJitter	50
#		End
#		#Part1	:RapidCometHead.part
#		#Part2	:RapidBlastStreak.part
#		Part3	:RapidCometBubblesRed.part
#		Magnet	Target
#		LookAt  Target
#	End
#
#	Event
#		Ename	PushForce
#		Type	Local
#		At	PushDirection
#		BhvrOverride
#			PositionOffset		0 0 5
#			pyrRotate		15 0 0
#			PhysForceType		Forward
#			PhysForceRadius		10
#			PhysForcePower		200
#			PhysForcePowerJitter	50
#		End
#		LifeSpan 1
#	End
#
#	Event
#		EName	cometParts4
#		Type	Local
#		At	Prime4
#		Part1	:RapidCometBubblesRedRapid.part
#		Part2	:RapidCometBubblesBrightRed.part
#		Part3	:RapidCometBubblesLocalRed.part
#		Part	:RapidCometBubblesRapidBlack.part
#		Part5	:RapidEnergyCoreLocalRed.part
#	End
#End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
	End
End

#Condition
#	On 	Prime2Hit
#
#	Event
#		EName 	Prime2
#		Type	Destroy
#	End
#End
#
#Condition
#	On 	Prime3Hit
#
#	Event
#		EName 	Prime3
#		Type	Destroy
#	End
#End
#
#Condition
#	On 	Prime4Hit
#
#	Event
#		EName 	Prime4
#		Type	Destroy
#	End
#End

#############################################################

End