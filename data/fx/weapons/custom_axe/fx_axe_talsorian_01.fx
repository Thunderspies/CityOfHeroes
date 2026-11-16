#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
        On	Time
        Time	0

	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.5 0.0 0.375
			PyrRotate	-90 0 10
		End
		Part	:TalsorianAxe_Glow01.part
	End

	Event
		Type	Local
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 1.625
			PyrRotate	0 -10 90
		End
		Part	:TalsorianAxe_Glow02.part
	End
End

#########################################################

End