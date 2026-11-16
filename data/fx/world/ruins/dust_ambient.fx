#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 5.0 0.0
		End
		Part	:Fog_Coastal_Strip.part
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
		Part	:Fog_Coastal_Strip_Lingering.part
	End
End

#############################################################

End