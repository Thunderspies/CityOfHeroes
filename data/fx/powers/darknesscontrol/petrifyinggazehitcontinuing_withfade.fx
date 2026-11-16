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
		bhvr	behaviors/soundFade3.bhvr
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
		bhvr	behaviors/Rune_spinFastSmall.bhvr
		part1	:DarkessChillMistz.part
		part2	:DarkessChillMist2z.part
		part3	:DarkessChillTenticles.part
		Sound miasmaburst 70 70 .6
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