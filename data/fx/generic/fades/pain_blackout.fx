#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo

Lifespan 600

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Shake		6.0
			ShakeFalloff	0.5
			ShakeRadius	1024
			Blur		2.5
			BlurFalloff	0.001
			BlurRadius	1024
		End
		Part	:PainFlash.part
		Part	:Pain_SmallFlashes_Dark.part
		Lifespan 92
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Blackout.part
		Part	:Blackout_Edges.part
		Lifespan 92
	End
End

#############################################################

Condition
	On 	Time
	Time 	90

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Freeze_BlackScreen.part
		Lifespan 272
	End
End

#############################################################

Condition
	On 	Time
	Time 	360

	Event
		Type	Start
		CameraSpace
		Part	:FadeFromBlack.part
		Part	:Blackout_FadeInEyelids.part
	End

	Event
		Type	Start
		CameraSpace
		BhvrOverride
			Blur		2.5
			BlurFalloff	0.015
			BlurRadius	1024
		End
		Part	:Blackout_FadeInWhite.part
	End
End

#############################################################

End
