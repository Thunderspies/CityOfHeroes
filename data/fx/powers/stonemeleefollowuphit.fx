#############################################################
## StoneMeleeFollowUpHit.fx
#############################################################

FxInfo

Lifespan 75

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	root
		ChildFx	Powers\StoneMeleeExplosion_Child.Fx
		Lifespan 80
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	root
		ChildFx	Powers\Child_BuildUp.fx
		Lifespan 80
	End
End

#############################################################

End