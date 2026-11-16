#############################################################
## quick_entityFadeOut.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors\quick_fadeout.bhvr
		Flags	AdoptParentEntity
	End
End

#############################################################

End