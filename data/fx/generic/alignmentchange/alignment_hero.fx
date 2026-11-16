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
		Ename	Lightray
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -2.0 0.0
		End
		Part	:Root_Ray_Tall_Vigilante.part
		Part	:Root_Ray_Vigilante.part
		Lifespan	35
	End
	Event
		Ename	GroundLight
		Type	Local
		At	Root
		Part	:Root_Light_Vigilante.part
		Lifespan	40
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Chest_Rays_Initial_LightBlue.part
		Part	:Chest_Ripples_LightBlue.part
		Lifespan 10
	End


	Event
		Type	PositOnly
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Chest_Rays_LightBlue.part
		Lifespan 115
	End

End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Root
		Sound Alignment_Hero 90 90 .86
	End
	
	Event
		Type	PositOnly
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Rays_Initial_Blue.part
		Part	:Rays_Blue.part
		Part	:Ripples_Blue.part
		Lifespan 10
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Rays_Initial_Blue.part
		Part	:Rays_Blue.part
		Part	:Ripples_Blue.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	30


	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Filter_Subtractive_LightBlue.part
		Part	:Filter_LightBlue.part
		Lifespan 210
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Circle_Blue.part
		Part	:Symbol_Blur_Hero.part
		Lifespan 60
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Clouds_LightBlue.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	Start
		CameraSpace
		Part	:Symbol_Hero.part
		Part	:Symbol_Additive_Hero.part
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
