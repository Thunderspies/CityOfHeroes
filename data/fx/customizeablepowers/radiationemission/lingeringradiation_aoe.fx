#########################################################
## Radiation - Lingering Radiaiton
#########################################################

FxInfo

LifeSpan 78

ClampMaxScale 1 1 1

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	HitRing
		Type	start
		At	Origin
		Part4	:LingeringRingsFlat.part
		Sound airwhoosh 66 66 .8
		LifeSpan 120
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Orb
		Type	start
		At	Origin
		part1	:LingeringMist.part
		part2	:LingeringMist2.part
		Sound glow3_loop 66 66 .35
	End

	Event
		EName 	Orb
		Type	start
		At	Origin
		part1	:LingeringRings.part
		part2	:LingeringGlows.part
		Sound enervating2_loop 66 66 .55
	End
End

#############################################################

End