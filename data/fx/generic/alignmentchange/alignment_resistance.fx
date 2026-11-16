#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo
Lifespan 210

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At Root
		Sound Alignment_Resistance 90 90 .86
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Symbol_Blur_Resistance.part
		Part	:Rays_Teal.part
		Part	:Ripples_Teal.part
		Part	:Glow_WhiteNoise_Teal.part
		Lifespan 15
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Circle_Teal.part
		Lifespan 200
	End
End

Condition
	On	Time
	Time	5


	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Filter_Teal.part
		Lifespan 210
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 60
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Clouds_Teal.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Start
		CameraSpace
		Part	:Symbol_Resistance.part
		Part	:Symbol_Additive_Resistance.part
		Lifespan 220
	End

	Event
		Type	Start
		Bhvr	Behaviors\CameraShake02.bhvr
		Lifespan 1
	End
End
#############################################################

End
