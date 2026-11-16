#############################################################
## FireSwordCircle_Hit.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	hips
		Part1	CustomizeablePowers\FireManipulation\FireWeaponExplosion.part
		Part2	CustomizeablePowers\FireManipulation\FireWeaponHitSpark.part
		Lifespan 15
	End

	Event
		Type	local
		At	hips
		Part1	CustomizeablePowers\FireManipulation\FireBallTargetSteam.part
		Part2	CustomizeablePowers\FireManipulation\FireBallTargetFlame2.part
		Part3	CustomizeablePowers\FireManipulation\FireBallSpark.part
		Sound FireExplo 100.0 100.0 .8
	End

	Event
		Type	Start
		At	hips
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		200
			PhysForcePowerJitter	25
		End
		Lifespan 1
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat  7
	RepeatJitter	3

	Event
		Type 	Local
		At 	Chest
		ChildFX :Child_FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 20
	End

	Event
		HardwareOnly
		Type 	Local
		At 	Chest
		ChildFX :Child_FireBurstUP.fx
		Lifespan 70
		LifespanJitter 20
	End
End

#############################################################

End