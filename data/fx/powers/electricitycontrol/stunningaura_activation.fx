#############################################################
## StunningField_Pet.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Thingy
		Type	local
		At	WepR
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part3	:Hand_Ring.part
		Part	:Hand_Sparks.part
	#	Sound	cagebolt2 80 30 .8
	End

	Event
		EName 	Thingy1
		Type	local
		At	WepL
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Body_Electricity_Arcs.part
		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Sparks.part
	End

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
			PositionOffset	0 .1 0
		End

		Part	:Root_Electricity_Ring_Inner.part
		Part	:Root_Electricity_Ring_Inner_Highlight.part
		Part	:Root_Electricity_Ring_Outer.part
	End

	Event
		Type	Start
		At	Root
		Part	:Root_Electricity_Ring_Expanding_Highlight.part
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	120

	Event
		Type	Start
		At	Root
		Part	:Root_Electricity_Ring_Expanding_Highlight.part


		Lifespan 5
	End
End
#############################################################

End
