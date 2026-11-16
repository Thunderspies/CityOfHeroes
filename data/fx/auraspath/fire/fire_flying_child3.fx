#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE

	Event
		Type	Local
		At	Root
		WHILE	MALE
		Bhvr	:FireShrink.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
			PyrRotate 	0 0.5 0
		End
		Part	:FireTrail_Alpha2.part
		Part	:FireTrail_CoreAdd2.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	#DoMany		1
	Event
		Type	Local
		At	Root
		WHILE	FEMALE
		Bhvr	:FireShrink.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
			PyrRotate 	0 0.5 0
		End
		Part	:FireTrail_Alpha2.part
		Part	:FireTrail_CoreAdd2.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	#DoMany		1
	Event
		Type	Local
		At	Root
		WHILE	HUGE
		Bhvr	:FireShrink.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
			PyrRotate 	0 0.5 0
		End
		Part	:FireTrail_Alpha2.part
		Part	:FireTrail_CoreAdd2.part
	End
End



#########################################################

End