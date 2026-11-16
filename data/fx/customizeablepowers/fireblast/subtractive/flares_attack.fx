#############################################################
## Flares.fx
#############################################################

FxInfo

LifeSpan 180

### HANDS ###########################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		ChildFX	:FireHands.fx
		LifeSpan 45
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
		bhvr	behaviors\Fastprojectile3.bhvr
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
		Part1	:FlareHands2.part
		Part1	:FlareHands2_Alpha.part
		Part2	:FlareBoltTail2.part
		Part4	:FlareSparkTrail.part
		Sound flare3 60 60 0.6
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
		bhvr	behaviors\Fastprojectile3.bhvr
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
		Part1	:FlareHands2.part
		Part1	:FlareHands2_Alpha.part
		Part2	:FlareBoltTail2.part
		Part4	:FlareSparkTrail.part
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
		bhvr	behaviors\Fastprojectile3.bhvr
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
		Part1	:FlareHands2.part
		Part1	:FlareHands2_Alpha.part
		Part2	:FlareBoltTail2.part
		Part4	:FlareSparkTrail.part
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
		bhvr	behaviors\Fastprojectile3.bhvr
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
		Part1	:FlareHands2.part
		Part1	:FlareHands2_Alpha.part
		Part2	:FlareBoltTail2.part
		Part4	:FlareSparkTrail.part
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

#############################################################

End