#########################################################
##	cot_waterfall_med.fx
##
FxInfo

Input  
	InpName	Chest
End



LifeSpan	40

###########################################################


Condition
	On	Time
	Time	0
	

	Event
		EName	hit
		Type	start
		At	Chest
		Part1	:QuillBallsBurst.part
		Part2	:QuillFillBurst.part
		Part3	:QuillFillBurst2.part
		Part4	:QuillBubblesBurst.part
		Part5	:PoisonCore.part
		Sound poisonstaffhit 70 70 .6

	End

End


End			