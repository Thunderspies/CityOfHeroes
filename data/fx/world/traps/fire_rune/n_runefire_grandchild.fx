#############################################################
## N_RuneFire_GRANDCHILD.fx
#############################################################

FxInfo
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset  0 -1 0
		End
		Part	:N_RuneFire_Core.part
		Part	:N_RuneFire_Add.part
		Lifespan 90
	End

End

#############################################################
End