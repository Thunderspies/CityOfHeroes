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
		part	:Cloud_Core_Trail_MOVEMENT.part
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