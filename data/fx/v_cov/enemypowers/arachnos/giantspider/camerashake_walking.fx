#########################################################
## Generic Camerashake - Monsterous
#########################################################

FxInfo

## CAMERA SHAKE #######################################################

Condition
	On 	Cycle
	Time 	10
	Chance	0.75

	Event
		EName 	CameraShake
		Type	Start
		At	Chest
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake          0.1	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff   0.0125	#how long shake lasts
			ShakeRadius    512	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
		Lifespan	5
	End
End

#######################################################################

End