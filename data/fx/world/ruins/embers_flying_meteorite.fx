#############################################################
## Embers_Ground.fx
#############################################################


FxInfo

LifeSpan 0

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0 2 0
		End
		Part	:Embers_Flying_Meteorite.part
	End


End

#############################################################

End