#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	head
		Sound Minotaur_fetid_howl 140 140 1
	End
End

#############################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Start
		At	Origin
		Bhvr	Behaviors\CameraShake01.bhvr
		BhvrOverride
			Shake		0.125
			ShakeFallOff	0.00375
			ShakeRadius	64
		End
		Lifespan 30
	End
End

Condition
	On 	Time
	Time	30

	Event
		Type	Start
		At	Origin
		Bhvr	Behaviors\CameraShake01.bhvr
		BhvrOverride
			Shake		1.5
			ShakeFallOff	0.0225
			ShakeRadius	128
		End
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MouthOffset
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 -0.15 0.75
		End
	End

	Event
		EName	MouthLookAt
		Type	PositOnly
		At	MouthOffset
		LookAt	T_Chest
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	MouthLookAt
		BhvrOverride
			PyrRotate	-90 0 0
		End
		Part3	:Spittle01.part
		Part3	:Spittle02.part
		Part1	:Gas1.part
		Part1	:Gas2.part
		Lifespan 45
	End
End

## SHOCKWAVE ###########################################################

#Condition
#	On 	Time
#	Time 	35
#
#	Event
#		EName	core
#		Type	Local
#		At	head
#		bhvr	POWERS\SonicControl\Blasts\Offset2.bhvr
#		Geom	LocalPivot01
#		LifeSpan 10
#	End
#
#	Event
#		EName	core3
#		Type	start
#		At	core
#		Altpiv	4
#		part	POWERS\SonicControl\Blasts\MassiveMegaSonicRingCore.part
#		part	POWERS\SonicControl\Blasts\MassiveMegaSonicRing.part
#		LifeSpan 10
#	End
#
#	Event
#		Ename	PushForce
#		Type	Local
#		At	core
#		Altpiv	4
#		BhvrOverride
#			PositionOffset		0 0 16
#			pyrRotate		15 0 0
#			PhysForceType		Forward
#			PhysForceRadius		32
#			PhysForcePower		300
#			PhysForcePowerJitter	50
#		End
#		Lifespan 10
#	End
#End

#############################################################

End