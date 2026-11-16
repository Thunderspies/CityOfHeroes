#############################################################
## Child_ProjectileCards.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On	Cycle
	Time	1

	Event
		Type	Local
		At	Origin
		ChildFX	:Child_SingleCardProjectile.fx
		Lifespan 300
	End
End

#############################################################

End