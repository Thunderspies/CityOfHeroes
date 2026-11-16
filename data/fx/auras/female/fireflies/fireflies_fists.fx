#############################################################
## Fireflies_Fists.fx
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
			PositionOffset	0.7 0.0 0.0
		End
		part	:Fireflies_Head.part
		Part	:Fireflies_Hand_Trail.part
		PMagnet	WepL
	End

	Event
		Type	PositOnly
		At	WepR
		BhvrOverride
			PositionOffset	0.7 0.0 0.0
		End
		part	:Fireflies_Head.part
		Part	:Fireflies_Hand_Trail.part
		PMagnet	WepR
	End
End

#############################################################

End