#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 60

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On	Cycle
	Time	12

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
		ChildFX	:Lieutenant_DispersedImpression_Flash_Merged.fx
		Lifespan 5
	End
End

#############################################################

End