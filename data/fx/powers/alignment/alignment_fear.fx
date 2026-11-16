#############################################################
## LeadershipTactics.fx
#############################################################

FxInfo
Lifespan 60

##################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		BhvrOverride
			PyrRotate	0 90 90
			PyrRotateJitter	10 10 10
		End
		Part	:Rings_Traveling.part
		Part	:Rings_Traveling_Highlight.part
		Part	:Smoke_Traveling.part
		Part	:Smoke_Traveling_Additive.part
		Part	:Smoke_Traveling_Highlight.part
		Lifespan 57
	End

End