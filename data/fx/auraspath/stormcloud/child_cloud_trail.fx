#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 	Time
	Time	0

	Event
		EName	Cloud
		Type	Local
		At	Root
		part	:Clouds_Core.part
		part	:Clouds_Core_Soft.part
		part	:Clouds_Perimeter.part
		part	:Clouds_Perimeter_Highlights.part

	End

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
		part	:Cloud_Core_Trail.part
	End
End

#########################################################

End