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
			PositionOffset	0.4 0.0 -0.625
			PyrRotate	-90 0 -5
		End
		Part	:TalsorianBlades_Glow01.part
	End
End

#########################################################

End