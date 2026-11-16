#########################################################
##	PTS Bunker Splash
##
FxInfo
	

Input  
	InpName	Origin
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	bubbles
		Type	Local 
		At	Origin
		part1	V_COV/world/Villain_Lairs/PTS_Bunker/v_pts_PipeSplash_Sml.part
		part2	V_COV/world/Villain_Lairs/PTS_Bunker/v_pts_PipeSplash2_Sml.part
		part3	V_COV/world/Villain_Lairs/PTS_Bunker/v_pts_PipeSplashMist_Sml.part
		part4	V_COV/world/Villain_Lairs/PTS_Bunker/v_pts_PipeSplashFlat_Sml.part
		part5	V_COV/world/Villain_Lairs/PTS_Bunker/v_pts_PipeSplashMist2_Sml.part
	End
	

End

End			