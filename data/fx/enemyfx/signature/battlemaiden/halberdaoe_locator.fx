#############################################################
## HalberdAOE.fx
#############################################################

FxInfo
Lifespan	500

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Hookup
		Type	Start
		At	Root
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:Locator_Shockwave.part

	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	:Locator_Shockwave_Small.part
	End
End

Condition
	On	Cycle
	Time	5


	Event
		Type	Local
		At	Hookup
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 1.5 0
			Scale			0.4 0.4 0.4
			ScaleRate		1.05 1.05 1.05
 			EndScale		1.5 1.5 1.5
		End
		Part	:Locator_DustRing.part
		Part	:Locator_DustRing_Highlight.part
		Lifespan 15
	End
End


#############################################################


End