#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo

Lifespan 450

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		CameraSpace
		BhvrOverride
			Shake		4.0
			ShakeFalloff	0.5
			ShakeRadius	1024
			Blur		2.5
			BlurFalloff	0.001
			BlurRadius	1024
		End
		Part	:PainFlash.part
		Lifespan 90
	End

	Event
		Type	Start
		CameraSpace
		Part	:Redout.part
		Part	:Redout_Edges.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	89

	Event
		Type	Start
		CameraSpace
		Part	:Freeze_BlackScreen.part
		Lifespan 91
	End
End

#############################################################

Condition
	On 	Time
	Time 	180

	Event
		Type	Start
		CameraSpace
		BhvrOverride
			Blur		2.5
			BlurFalloff	0.01
			BlurRadius	1024
		End
		Part	:FadeFromBlack.part
		Lifespan 90
	End
End

#############################################################

End