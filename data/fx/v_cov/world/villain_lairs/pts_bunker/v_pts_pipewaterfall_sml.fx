##########################################################
##	v_pts_pipewaterfall_sml.fx
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part1	V_COV/World/Villain_Lairs/PTS_Bunker/v_pts_PipeWaterOuterMist_sml.part
		Part2	V_COV/World/Villain_Lairs/PTS_Bunker/v_pts_PipeWater_sml.part
		Part3	V_COV/World/Villain_Lairs/PTS_Bunker/v_pts_PipeWater_sml2.part
		Part4	V_COV/World/Villain_Lairs/PTS_Bunker/v_pts_PipeWaterOuterMist2_sml.part
		Sound Waterfall2_loop 80.0 80.0 .6
	End
End


End			