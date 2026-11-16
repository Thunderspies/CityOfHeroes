#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0
Flags DontSuppress

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
			PositionOffset	0.0 2.0 -8.0
			PYRRotate	0 270 0
		End
		Part	:Fog_Directional_Square.part
	End
End

#############################################################

End