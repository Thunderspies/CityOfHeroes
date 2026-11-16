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
		part	:StormClouds_Core.part
		part	:StormClouds_Core_Soft.part
		part	:StormClouds_Perimeter.part
		part	:StormClouds_Perimeter_Highlights.part

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
		part	:StormCloud_Core_Trail.part
	End
End

Condition
	On	Cycle
	Time	12
	Chance	0.45

	Event
		EName	Flashes
		Type	Local
		At	CloudAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Flash_Highlight.part
		part	:Flash_Main.part

		Lifespan	10
		LifespanJitter	3
	End
End

#########################################################

End