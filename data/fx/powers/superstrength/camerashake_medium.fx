#########################################################
## Camera Shake - Minor Hit
########################################################

FxInfo

Lifespan	30

########################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Start
		At	Origin
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake		0.2	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff	0.0075	#how long shake lasts
			ShakeRadius	14	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End

	Event
		Type	Start
		At	Origin
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		12
			PhysForcePower		200
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End

########################################################

End