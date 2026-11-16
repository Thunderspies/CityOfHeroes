#########################################################
## Staff of the M.A.G.I. - Ball Lightning (Initial)
#########################################################

FxInfo

LifeSpan	200

#########################################################

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Origin
		ChildFx	:BallLightning.fx
	End
End

#########################################################

End