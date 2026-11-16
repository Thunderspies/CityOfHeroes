#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	2.5 0.0 0.0

		End
		part	:Fireflies.part
		Part	:Fireflies_TightPath.part
		Part	:Fireflies_Trail.part
		PMagnet	Head
	End

End

#############################################################

End