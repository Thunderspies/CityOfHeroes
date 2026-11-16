#########################################################
##	cot_waterfall_med.fx
##
FxInfo

Input  
	InpName	Root
End

###########################################################


Condition
	On	Time
	Time	0
	

	Event
		EName	Pri
		Type	Local
		At	Root
		Geom	IceBoard
		
	End


	Event
		EName	alt
		Type	Local
		At	Pri
		altpiv	1

		part1	:IceBoardChunks.part
		Part2	:IceSlideClouds.part
	End

End

End			