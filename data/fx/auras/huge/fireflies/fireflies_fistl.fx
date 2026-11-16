#############################################################
## Fireflies_FistL.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	WepL
		BhvrOverride
			PositionOffset	1.0 0.0 0.0
		End
		part	:Fireflies_Head.part
		Part	:Fireflies_Hand_Trail.part
		PMagnet	WepL
	End
End

#############################################################

End