#############################################################
## Flares.fx
#############################################################

FxInfo

LifeSpan 180

## HANDS ###########################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\FieryAssault\FireHandz.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan 30
	End

	Event
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\FieryAssault\FireHandz.part
		Sound Ignite2 100.0 100.0 .8
		LifeSpan 30
	End
End

## PROJECTILE 1 ###########################################################

Condition
	On	Time
	Time	25

	Event
		EName	Prime
		Type	start
		At	WepR
		bhvr	CustomizeablePowers\FieryAssault\Fastprojectile3.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		Geom	CenterDummy
		Magnet	Target
		LookAt  Target
	End

	Event
		EName	PrimeParts
		Type	Local
		At	Prime
		Altpiv	1
		Part1	CustomizeablePowers\FieryAssault\FlareHands2.part
		Part2	CustomizeablePowers\FieryAssault\FlareBoltTail2.part
		Part3	CustomizeablePowers\FieryAssault\FlareBoltTail.part
		Part4	CustomizeablePowers\FieryAssault\FlareSparkTrail.part
		Part5	CustomizeablePowers\FieryAssault\FlareHands2.part
		Sound flare3 60 60 .6
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
		bhvr	CustomizeablePowers\FieryAssault\Fastprojectile3.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		Geom	CenterDummy
		Magnet	Target
		LookAt  Target
	End

	Event
		EName	Prime1Parts
		Type	Local
		At	Prime1
		Altpiv	1
		Part1	CustomizeablePowers\FieryAssault\FlareHands2.part
		Part2	CustomizeablePowers\FieryAssault\FlareBoltTail2.part
		Part3	CustomizeablePowers\FieryAssault\FlareBoltTail.part
		Part4	CustomizeablePowers\FieryAssault\FlareSparkTrail.part
		Part5	CustomizeablePowers\FieryAssault\FlareHands2.part
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
		bhvr	CustomizeablePowers\FieryAssault\Fastprojectile3.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		Geom	CenterDummy
		Magnet	Target
		LookAt  Target
	End

	Event
		EName	Prime2Parts
		Type	Local
		At	Prime2
		Altpiv	1
		Part1	CustomizeablePowers\FieryAssault\FlareHands2.part
		Part2	CustomizeablePowers\FieryAssault\FlareBoltTail2.part
		Part3	CustomizeablePowers\FieryAssault\FlareBoltTail.part
		Part4	CustomizeablePowers\FieryAssault\FlareSparkTrail.part
		Part5	CustomizeablePowers\FieryAssault\FlareHands2.part
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
		bhvr	CustomizeablePowers\FieryAssault\Fastprojectile3.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		Geom	CenterDummy
		Magnet	Target
		LookAt  Target
	End

	Event
		EName	Prime3Parts
		Type	Local
		At	Prime3
		Altpiv	1
		Part1	CustomizeablePowers\FieryAssault\FlareHands2.part
		Part2	CustomizeablePowers\FieryAssault\FlareBoltTail2.part
		Part3	CustomizeablePowers\FieryAssault\FlareBoltTail.part
		Part4	CustomizeablePowers\FieryAssault\FlareSparkTrail.part
		Part5	CustomizeablePowers\FieryAssault\FlareHands2.part
	End

End

##############################################################
#
#Condition
#	On	Time
#	Time	45
#
#	Event
#		EName	Prime4
#		Type	start
#		At	WepL
#		bhvr	behaviors\Fastprojectile3.bhvr
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		8
#			PhysForcePower		50
#			PhysForcePowerJitter	10
#		End
#		Geom	CenterDummy
#		Magnet	Target
#		LookAt  Target
#	End
#
#	Event
#		EName	Prime4Parts
#		Type	Local
#		At	Prime2
#		Altpiv	1
#		Part1	:FlareHands2.part
#		Part2	:FlareBoltTail2.part
#		Part3	:FlareBoltTail.part
#		Part4	:FlareSparkTrail.part
#		Part5	:FlareHands2.part
#	End
#End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	PrimeParts
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
		EName 	Prime1Parts
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
		EName 	Prime2Parts
		Type	Destroy
	End
End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
	End

	Event
		EName 	Prime3Parts
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
#
#	Event
#		EName 	Prime4Parts
#		Type	Destroy
#	End
#End

#############################################################

End