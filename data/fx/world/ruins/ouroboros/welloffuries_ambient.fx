#############################################################
## WellOfFuries_Ambient.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Time
	Time	0

	Event
		EName	AshAnchor
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 5.0 0.0
		End
		Part	:Sparkles.part
		Part	:Dust.part
		Part	:Dust_Rainbow.part
	End

	Event
		Type	StartPositOnly
		At	Root
		Part	:Sparkles_Ground.part
		Part	:Shadow_Ground.part

	End
End

#############################################################

End


