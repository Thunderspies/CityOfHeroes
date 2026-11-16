#############################################################
## BeastlyRage_BodyAndHead.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Origin
		AltPiv	1
		BhvrOverride
			PositionOffset		-0.1 0.0 0.2
			PyrRotate		0 60 -90
		End
		ChildFX	:Child_BodyAndChest.fx
		Lifespan 45
	End
End

#############################################################

End