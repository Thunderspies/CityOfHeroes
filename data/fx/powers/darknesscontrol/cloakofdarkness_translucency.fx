#############################################################
## CloakOfDarkness_Translucency.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadeout.bhvr
		Flags	AdoptParentEntity
		Lifespan 1
	End
End

Condition
	On	Time
	Time	100

	Event
		Type	Local
		At	Root
		BhvrOverride
			Alpha		1
		End
		Flags	AdoptParentEntity
	End
End

#############################################################

End