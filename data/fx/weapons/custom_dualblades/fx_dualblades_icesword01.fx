#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
        On	Time
        Time	0

#	Event
#		Type	Local
#		At	Origin
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset	0.4 0.0 -0.625
#			PyrRotate	-90 0 -5
#		End
#		Part	:TalsorianBlades_Glow01.part
#	End

#	Event
#		EName	Offset_Start
#		Type	Local
#		At	Offset
#		AltPiv	2
#		Part	Customizeablepowers/icemelee/snowfall.part
#		Part	Customizeablepowers/icemelee/snowfallWhips.part
#		Part	Customizeablepowers/icemelee/SwordSparklies.part
#		POther	Offset_End
#	End

	Event
		EName	Offset_End
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0 0 0
			PyrRotate	90 0 0
		End
		AltPiv	1
		Part	Powers\ColdControl\GreatericeswordFrost.part
		Part1	Powers\ColdControl\Greatericesword2.part
		Part5	Powers\ColdControl\GreaterSwordtrail2.part
		POther	Offset_Start
	End
End

#########################################################

End