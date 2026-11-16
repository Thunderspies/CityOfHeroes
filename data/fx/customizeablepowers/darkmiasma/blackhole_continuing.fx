#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Alpha
		Type	Local
		At	Origin
		BhvrOverride
			FadeInLength		100
			FadeOutLength		100
			Alpha			180
		End
		Flags	AdoptParentEntity
	End

	Event
		Type	posit
		At	root
		Sound soot2_loop 70 70 .4
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	OrbMist
		Type	posit
		At	root
		bhvr	CustomizeablePowers\DarkMiasma\Rune_spinFastSmall.bhvr
		part1	CustomizeablePowers\DarkMiasma\DarkessChillMistz.part
		part2	CustomizeablePowers\DarkMiasma\DarkessChillMist2z.part
		part3	CustomizeablePowers\DarkMiasma\DarkessChillTenticles.part
		Sound miasmaburst 70 70 .6
	End

	Event
		ENAME   Tendrills
		Type	posit
		At	chest
		Part1	CustomizeablePowers\DarkMiasma\ChillOfNightTendril.part
		part2	CustomizeablePowers\DarkMiasma\PurpleHitMist.part
		part3	CustomizeablePowers\DarkMiasma\DefenceHitMist2.part
	End
End

#############################################################

End