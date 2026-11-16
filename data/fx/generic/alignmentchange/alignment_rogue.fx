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
		Part	:Root_Ray_Rogue.part
		Part	:Root_Ray_Additive_Rogue.part
		Lifespan	35
	End

	Event
		Ename	GroundLight
		Type	Local
		At	Root
		Part	:Root_Light_Rogue.part
		Part	:Root_Light_Additive_Rogue.part
		Lifespan	40
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Chest_Rays_Initial_Red.part
		Part	:Chest_Ripples_Orange.part
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
		At		Root
		Sound Alignment_Rogue 90 90 .86
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Symbol_Rogue.part
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
		Part	:Filter_Orange.part
		Part	:Clouds_Orange.part
		Part	:Symbol_Blur_Rogue.part
		Part	:Symbol_Blur_Subtractive_Rogue.part

		Lifespan 210
	End

	Event
		Type	Start
		CameraSpace
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Ripples_Orange.part
		Lifespan 8
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	Start
		Bhvr	Behaviors\CameraShake02.bhvr
		Lifespan 1
	End
End

#############################################################

End
