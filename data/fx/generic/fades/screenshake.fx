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
	#	Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Shake          2.0
			ShakeFallOff   .1
			ShakeRadius    80.0
		End
		Lifespan 30
	End
End

#############################################################

End
