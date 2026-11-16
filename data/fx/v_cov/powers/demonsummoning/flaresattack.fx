#############################################################
## FlaresAttack.fx
#############################################################

FxInfo
LifeSpan 180

## HANDS ###########################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	PositOnly
		At	WepR
		Part	:Demonling_Hands_Fire.part
		Part	:Demonling_Hands_Fire_Core.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan 30
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Demonling_Hands_Fire.part
		Part	:Demonling_Hands_Fire_Core.part
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
		Type	StartPositOnly
		At	WepR
		bhvr	behaviors\Fastprojectile3.bhvr
		BhvrOverride
			InitialVelocity		0 0.2 0
			InitialVelocityJitter	0 0.4 0
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		50
			PhysForcePowerJitter	10
		End

		Part1	:Demonling_Fire_Core.part
		Part2	:Demonling_Fire_Tail2.part
		Part3	:Demonling_Fire_Tail.part
		Part4	:Demonling_Fire_SparkTrail.part
		Sound flare3 60 60 .6
		Geom	CenterDummy
		Magnet	Target
		LookAt  Target
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName	Prime1
		Type	StartPositOnly
		At	WepL
		bhvr	behaviors\Fastprojectile3.bhvr
		BhvrOverride
			InitialVelocity		0 0.2 0
			InitialVelocityJitter	0 0.4 0
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		Part1	:Demonling_Fire_Core.part
		Part2	:Demonling_Fire_Tail2.part
		Part3	:Demonling_Fire_Tail.part
		Part4	:Demonling_Fire_SparkTrail.part
		Geom	CenterDummy
		Magnet	Target
		LookAt  Target
	End
End

#############################################################

Condition
	On	Time
	Time	35


	Event
		EName	Prime2
		Type	StartPositOnly
		At	WepR
		bhvr	behaviors\Fastprojectile3.bhvr
		BhvrOverride
			InitialVelocity		0 0.2 0
			InitialVelocityJitter	0 0.4 0
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		Part1	:Demonling_Fire_Core.part
		Part2	:Demonling_Fire_Tail2.part
		Part3	:Demonling_Fire_Tail.part
		Part4	:Demonling_Fire_SparkTrail.part
		Geom	CenterDummy
		Magnet	Target
		LookAt  Target
	End
End

#############################################################

Condition
	On	Time
	Time	40

	Event
		EName	Prime3
		Type	StartPositOnly
		At	WepL
		bhvr	behaviors\Fastprojectile3.bhvr
		BhvrOverride
			InitialVelocity		0 0.2 0
			InitialVelocityJitter	0 0.4 0
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		Part2	:Demonling_Fire_Tail2.part
		Part3	:Demonling_Fire_Tail.part
		Part4	:Demonling_Fire_SparkTrail.part
		Geom	CenterDummy
		Magnet	Target
		LookAt  Target
	End
End

##############################################################

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

#############################################################

End