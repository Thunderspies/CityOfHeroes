#############################################################
## DistortionField_Continuing.fx
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
		At	Head
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		Part	:Swirl_Slow_Core_Additive_Head.part
		Part	:Swirl_Slow_Core_Head.part
		Part	:Swirl_Slow_Highlights_Head.part
	End
End


#############################################################

End