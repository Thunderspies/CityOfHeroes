#############################################################
## N_RuneFire_GRANDCHILD.fx
#############################################################

FxInfo
Lifespan 250
#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Sewer_RuneFire_Loop 100 100 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset  0 -1 0
		End
		Part	:N_RuneFire_Core.part
		Part	:N_RuneFire_Add.part
	#	Lifespan 90
	End

End

#############################################################
End