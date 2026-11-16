#############################################################
## Header
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
		Type	local
		At	WepR
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		PMagnet	WepR
		Lifespan 30
	End

	Event
		Type	local
		At	WepR
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepR
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	local
		At	WepL
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		PMagnet	WepL
		Lifespan 30
	End

	Event
		Type	local
		At	WepL
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepL
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	HandGlowRight2
		Type	local
		At	WepR
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepR
		Lifespan 30
	End

	Event
		EName 	HandGlowLeft2
		Type	local
		At	WepL
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepL
		Lifespan 30
	End
End

#############################################################

Condition
	On	Time
	Time	25

	Event
		EName	Prime
		Type	start
		At	WepR
		bhvr	behaviors/projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Part3	:RapidCometBubblesWhite.part
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
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		Part5	:RapidEnergyCoreLocal.part
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName	Prime1
		Type	start
		At	WepL
		bhvr	behaviors/projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Part3	:RapidCometBubblesWhite.part
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
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		Part5	:RapidEnergyCoreLocal.part
	End
End

#############################################################

Condition
	On	Time
	Time	35

	Event
		EName	Prime2
		Type	start
		At	WepR
		bhvr	behaviors/projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Part3	:RapidCometBubblesWhite.part
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
		EName	cometParts4
		Type	Local
		At	Prime2
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		Part5	:RapidEnergyCoreLocal.part
	End
End

#############################################################

Condition
	On	Time
	Time	40

	Event
		EName	Prime3
		Type	start
		At	WepL
		bhvr	behaviors/projectile1.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Part3	:RapidCometBubblesWhite.part
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
		EName	cometParts3
		Type	Local
		At	Prime3
		Part1	:RapidCometBubbles.part
		Part2	:RapidCometBubblesBright.part
		Part3	:RapidCometBubblesLocal.part
		Part5	:RapidEnergyCoreLocal.part
	End
End

#############################################################

#Condition
#	On	Time
#	Time	45
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
#		Part3	:RapidCometBubblesWhite.part
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
#		At	Prime4
#		Part1	:RapidCometBubbles.part
#		Part2	:RapidCometBubblesBright.part
#		Part3	:RapidCometBubblesLocal.part
#		Part5	:RapidEnergyCoreLocal.part
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

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
	End
End

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