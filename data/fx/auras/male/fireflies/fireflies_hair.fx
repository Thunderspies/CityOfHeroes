#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Head
		BhvrOverride
			PositionOffset	1.5 0.0 0.0
		End
		part	:Fireflies_Head.part
		Part	:Fireflies_Head_Trail.part
		PMagnet	Head
	End

End

#############################################################

End