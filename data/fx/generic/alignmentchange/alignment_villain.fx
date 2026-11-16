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
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Root_Ray_Villain.part
		Part	:Root_Ray_Additive_Villain.part
		Lifespan	35
	End

	Event
		Ename	GroundLight
		Type	Local
		At	Root
		Part	:Root_Light_Villain.part
		Part	:Root_Light_Additive_Villain.part
		Lifespan	40
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Chest_Rays_Initial_Red.part
		Part	:Chest_Ripples_Red.part
		Lifespan 10
	End


	Event
		Type	PositOnly
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Chest_Rays_Red.part
		Lifespan 115
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At 	Root
		Sound Alignment_Villain 90 90 .86
	End
	
	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Symbol_Villain.part
		Lifespan 210
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Filter_Red.part
		Part	:Clouds_Red.part
		Part	:Symbol_Blur_Villain.part
		Part	:Symbol_Blur_Subtractive_Villain.part

		Lifespan 210
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Ripples_Red.part
		Lifespan 8
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Start
		Bhvr	Behaviors\CameraShake02.bhvr
		Lifespan 1
	End
End
############################################################

End
