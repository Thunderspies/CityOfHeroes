#############################################################
## Mine.fx
#############################################################

FxInfo
LifeSpan	470

#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	minepivot
		Type	start
		At	root
		Sound mine2_loop 13 13 .12
		Geom	gadgetHookup
	End

	Event
		EName	mine
		Type	start
		At	minepivot
		altpiv	1
		Geom	mine01
		Bhvr	behaviors/mineFade.bhvr
	End

	Event
		EName	antenna
		Type	start
		At	minepivot
		altpiv	1
		Sound minebeep_loop 13 13 .12
		geom	antennafx

	End

	Event
		EName	light
		Type	start
		At	antenna
		Sound mineset 18 18 .12
		altpiv	1
		Part1	:RedLightPulsingSmall.part
		Part2	:RedLightPulsingSmall.part
		Part	:RedLightPulsingSmall.part
		LifeSpan		250
	End

	Event
		EName	light
		Type	start
		At	mine

		Part1	:BlueLightPulsingFlat.part
		Part2	:BlueLightPulsingFlat.part

		LifeSpan	250
	End

End

Condition
	On 	Time
	Time 	250

	Event
		EName	light2
		Type	start
		At	antenna

		altpiv	1
		Part1	:RedLightPulsingSmallFast.part
		Part2	:RedLightPulsingSmallFast.part
		Part	:RedLightPulsingSmallFast.part

		LifeSpan	100
	End

	Event
		EName	light3
		Type	start
		At	mine


		Part	:BlueLightPulsingFlatFast1.part
		Part	:BlueLightPulsingFlatFast1.part
		LifeSpan	100
	End

End

Condition
	On 	Time
	Time 	350

	Event
		EName	light3
		Type	start
		At	antenna

		altpiv	1
		Part1	:RedLightPulsingSmallFast2.part
		Part2	:RedLightPulsingSmallFast2.part
		Part	:RedLightPulsingSmallFast2.part

		#LifeSpan	270
	End

	Event
		EName	light3
		Type	start
		At	mine


		Part	:BlueLightPulsingFlatFast.part
		Part	:BlueLightPulsingFlatFast.part

	End
End


End