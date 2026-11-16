#############################################################
## SonicPylons_Hit.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	20

	Event
		EName	Anchor1
		Type	Local
		At	Origin
		ChildFX	:SonicPylons_Tips_Damage_Child_Small.fx
		Lifespan 3
	End
End

#############################################################

End