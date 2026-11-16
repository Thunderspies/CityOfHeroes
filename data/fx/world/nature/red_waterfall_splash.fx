#########################################################
##	cot_waterfall.fx
##
FxInfo

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Sound venicewaterfall_loop 290 290 .3
	End
End
	
Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part	:Red_waterfall_med_1.part
		Part	:Red_waterfall_med_Flat.part
		Part	:Red_waterfall_med_2.part
		Part	:Red_waterfall_med_2.part
		Part	:Red_waterfall_med_3.part
		Part	:Red_waterfall_med_4.part
		Part	:Red_waterfall_Base.part
		Part	:Red_waterfall_Ripples_Flat.part	
	End
End


End			