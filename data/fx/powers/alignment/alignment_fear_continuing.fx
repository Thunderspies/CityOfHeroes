#############################################################
## LeadershipTactics.fx
#############################################################

FxInfo

##################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Part	:Smoke.part
		Part	:Glow_Small_Blue.part
		Part	:Smoke_Highlight.part
		Part	:Rings_Continuing.part
		Lifespan 300
	End

End