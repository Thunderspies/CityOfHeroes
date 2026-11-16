#############################################################
## PetrifyingGaze_Continuing.FX
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

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
		part3	CustomizeablePowers\DarkMiasma\DarkessChillTenticles2.part

		Sound miasmaburst 70 70 .6

	End

	Event
		ENAME   Tendrills
		Type	posit
		At	chest
		Part1	CustomizeablePowers\DarkMiasma\ChillOfNightTendril.part
		part2	CustomizeablePowers\DarkMiasma\PurpleHitMist.part
		part3	CustomizeablePowers\DarkMiasma\DefenceHitMist2.part
		#Sound	soot2_loop 70 60 .25


	End

End


End