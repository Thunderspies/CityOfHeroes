#########################################################
##	cot_waterfall_med.fx
##
FxInfo

Input	
	InpName Origin
End

Input  
	InpName	Root
End
Input  
	InpName	Hips
End
###########################################################


#######################################################
##IceChunks
#######################################################

Condition
	Time 30

	Event
		EName	W
		Type	Local
		At	Hips
		bhvr	:scaletest.bhvr
		Geom	IceChunk_07
		
	End

End



End			