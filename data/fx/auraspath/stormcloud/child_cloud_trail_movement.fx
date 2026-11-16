#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 	Time
	Time	0

	Event
		EName	CloudAnchor
		Type	Local
		AT	Root
		BhvrOverride
			PositionOffset	0.0 2.0 0.0
		End
	End

	Event
		Type	PositOnly
		At	CloudAnchor
		part	:Cloud_Core_Trail_Movement.part
	End
End

#########################################################

End