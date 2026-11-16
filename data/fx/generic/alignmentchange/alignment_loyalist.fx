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
		Sound Alignment_Loyalist 90 90 .86
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Symbol_Loyalist.part
		Part	:Circle_Gold.part
		Lifespan 210
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Rays_Initial_Gold.part
		Part	:Rays_Blue.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	5


	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Filter_Gold.part
		Part	:Clouds_Gold.part
		Part	:Symbol_Glow_Loyalist.part
		Lifespan 210
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Start
		Bhvr	Behaviors\CameraShake02.bhvr
		Lifespan 1
	End
End
#############################################################

End
