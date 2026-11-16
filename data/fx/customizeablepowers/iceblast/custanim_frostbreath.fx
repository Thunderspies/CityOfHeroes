#############################################################
## FrostBreath.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	8

	Event
		EName 	fist
		Type	Local
		At	head
		Sound coldblast2 80.0 80.0 0.4
	End

	Event
		Type	Local
		At	WepL
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
		LifeSpan 70
	End

	Event
		Type	Local
		At	WepR
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
		LifeSpan 70
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	Spike
		Type	Local
		At	WepR
		bhvr	:scale1a.bhvr
		Geom	LocalPivot01
		Sound Breathattack 80.0 80.0 0.8
		LifeSpan 55
	End

	Event
		Ename	PushForce
		Type	Local
		At	Spike
		BhvrOverride
			PositionOffset		0 0 10
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		15
			PhysForcePower		80
			PhysForcePowerJitter	10
		End
		LifeSpan 30
	End

	Event
		EName 	Prime
		Type	Local
		At	Spike
		Altpiv	1
		BhvrOverride
			PyrRotate	90 0 30
		End
		Part1	:FrostBreathIce.part
		Part2	:FrostBreathSnow.part
		Part3	:FrostBreathMist.part
	End
End

#############################################################

End