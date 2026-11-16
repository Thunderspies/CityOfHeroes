#############################################################
## PetrifyingGazeHitContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	posit
		At	root
		bhvr	behaviors\soundFade3.bhvr
		Sound soot2_loop 70 70 0.4
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
		bhvr	behaviors\Rune_spinFastSmall.bhvr
		part1	POWERS\DarknessControl\DarkessChillMistz.part
		part2	POWERS\DarknessControl\DarkessChillMist2z.part
		part3	POWERS\DarknessControl\DarkessChillTenticles2.part
		Sound miasmaburst 70 70 0.6
	End

	Event
		ENAME   Tendrills
		Type	posit
		At	chest
		Part1	POWERS\DarknessControl\ChillOfNightTendril.part
		part2	POWERS\DarknessControl\PurpleHitMist.part
		part3	POWERS\DarknessControl\DefenceHitMist2.part
	End
End

#############################################################

End