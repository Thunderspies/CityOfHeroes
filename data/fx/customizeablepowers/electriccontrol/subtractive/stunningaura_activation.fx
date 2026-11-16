#############################################################
## StunningField_Pet.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName 	Thingy
		Type	local
		At	WepR
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part3	:Hand_Ring.part
		Part	:Hand_Sparks.part
		Lifespan 60
		Sound	EC_StunningAura_Activate_01 100 100 .7
	End

	Event
		EName 	Thingy1
		Type	local
		At	WepL
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Sparks.part
		Lifespan 60

	End
End

Condition
	On Time
	Time 40
	Event
		Type Local
		At Root
		Sound 	EC_StunningAura_Continuing_Loop 50 50 .35
		bhvr	behaviors\sound\SoundOut5.bhvr
		Lifespan 300
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 2 0
		End
		Part	:Root_Electricity_Ring_Cascading.part
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 .25 0
		End
		Part	:Root_Electricity_Ring_Inner.part
		Part	:Root_Electricity_Ring_Inner_Highlight.part
		Part	:Root_Electricity_Ring_Outer.part
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 .25 0
		End
		Part	:Root_Electricity_Ring_Expanding_Highlight.part
		Part	:Root_Electricity_Ring_Expanding_Highlight2.part
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	120

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 .25 0
		End
		Part	:Root_Electricity_Ring_Expanding_Highlight.part
		Part	:Root_Electricity_Ring_Expanding_Highlight2.part
		Lifespan 5
		Lifespan 2
	End
End
#############################################################

End
