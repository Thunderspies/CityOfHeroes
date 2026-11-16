#########################################################
##	Inferno FX
##
FxInfo

Condition
	On	Time
	Time	20

	Event
		Type	start
		At	Root

		Bhvr	:GhostCatcherScale2.bhvr
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

		Part	:ReverseGhostsScatter.part
		Part	:ReverseGhostsScatter2.part
		Part	:ReverseGhostsScatter3.part
		Part	:ReverseGhostMist.part
		Part	:ReverseGhostMistScrolling.part
		Part	:ReverseGlowScatter.part
		Part	:ReverseGlowScatter2.part
		Part	:ReverseGhostMistblue.part
		Part	:ReverseGhostMistblue2.part

		LifeSpan	8
	End

End



End
