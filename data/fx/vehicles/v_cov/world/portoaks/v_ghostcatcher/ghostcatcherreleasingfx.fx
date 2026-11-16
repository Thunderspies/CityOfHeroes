#########################################################
##	Inferno FX
##
FxInfo

Condition
	On	Time
	Time	5

	Event
		Type	start
		At	Root

		Bhvr	:GhostCatcherScale.bhvr
		Geom	GhostBubble

		LifeSpan	8
	End

End

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root

		Part	:GhostsScatter.part
		Part	:GhostsScatter2.part
		Part	:GhostsScatter3.part
		Part	:GhostMist.part
		Part	:GhostMistScrolling.part
		Part	:GlowScatter.part
		Part	:GlowScatter2.part
		Part	:GhostMistblue2.part
		LifeSpan	8
	End

End



End
