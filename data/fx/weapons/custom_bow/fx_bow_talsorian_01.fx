#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	GlowOffset_Top1
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	-0.25 0.0 0.55
		End
	End

	Event
		EName	GlowOffset_Top2
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.175 0.0 1.625
		End
		Part	:TalsorianBow_Glow01.part
		pOther	GlowOffset_Top1
	End

	Event
		EName	GlowOffset_Top3
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.6875 0.0 2.875
		End
		Part	:TalsorianBow_Glow02.part
		pOther	GlowOffset_Top2
	End
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	GlowOffset_Bottom1
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	-0.25 0.0 -0.55
		End
	End

	Event
		EName	GlowOffset_Bottom2
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-0.175 0.0 -1.625
		End
		Part	:TalsorianBow_Glow01.part
		pOther	GlowOffset_Bottom1
	End

	Event
		EName	GlowOffset_Bottom3
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.6875 0.0 -2.875
		End
		Part	:TalsorianBow_Glow01.part
		pOther	GlowOffset_Bottom2
	End
End

#########################################################

End