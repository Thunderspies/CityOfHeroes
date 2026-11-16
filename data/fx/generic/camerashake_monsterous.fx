#########################################################
## Generic Camerashake - Monsterous
#########################################################

FxInfo

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		256
			PhysForcePower		500
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End
End

## CAMERA SHAKE #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	CameraShake
		Type	Start
		At	Chest
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake          0.75	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.025	#how long shake lasts
			ShakeRadius    768	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

#######################################################################

End