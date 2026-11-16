#############################################################
## PetrifyingGazeHitContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	chest
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
		part1	:DarkessChillMistz.part
		part2	:DarkessChillMist2z.part
		part3	:DarkessChillTenticles2.part
		Sound miasmaburst 70 70 0.6
	End

	Event
		ENAME   Tendrills
		Type	posit
		At	chest
		Part1	:ChillOfNightTendril.part
		part2	:PurpleHitMist.part
		part3	:DefenceHitMist2.part
	End
End

#############################################################

End