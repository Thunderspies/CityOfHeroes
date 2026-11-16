#############################################################
## NeuronBomb_Explosion.fx
#############################################################

FxInfo
Lifespan 190

#############################################################



Condition
	On	Time
	Time	0

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Shake          1.5
			ShakeFallOff   .05
			ShakeRadius    80.0
		End
		Lifespan 1
	End
End

#############################################################

End
