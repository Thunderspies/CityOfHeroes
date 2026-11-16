#############################################################
## TimesJuncture_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		Part	:Spiral_Ground_Shrinking_Small.part
		Part	:Spiral_Ground_Shrinking_Additive_Small.part
	End
End

#############################################################

End