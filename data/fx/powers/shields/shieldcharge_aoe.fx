#############################################################
## ShieldCharge_AOE.fx
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet1
		Type	start
		At	Root
		Geom	SuperStrengthBubble
		bhvr	behaviors\ScaleBubble.bhvr
		Lifespan 37
	End

	Event
		EName 	Mallet2
		Type	start
		At	Root
		Part1	POWERS\SuperStrength\SuperStrengthHitEmber.part
		Part2	POWERS\SuperStrength\SuperStrengthHitStar.part
		Part3	POWERS\SuperStrength\SuperStrengthHitRing.part
		Part4	POWERS\SuperStrength\SuperStrengthHitRing2.part
		Part5	POWERS\SuperStrength\SuperStrengthHitGlows2.part
		Sound PunchHit2 100.0 100.0 .8
		Lifespan 37
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		300
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

########################################################

Condition
	On 	Time
	Time	0

	Event
		Ename	Offset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 2 0
		End
	End

	Event
		EName 	CameraShake
		Type	Start
		At	Offset
		Bhvr	Behaviors\CameraShake01.bhvr
		BhvrOverride
			Shake		2.0	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff	0.25	#how long shake lasts
			ShakeRadius	48	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Offset
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		200
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

End