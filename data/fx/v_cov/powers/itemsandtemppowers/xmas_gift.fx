#########################################################
##	Fire_Ball
##
FxInfo




######  Gift Box  ###########################################

Condition
	On	Time
	Time	0

	Event
		EName	Gift
		Type	local
		At	Root
		Geom	Holiday_Present_1
		Geom	Holiday_Present_2
		Geom	Holiday_Present_3
		Geom	Holiday_Present_4
		Geom	Holiday_Present_5
		Geom	Holiday_Present_6
	End

End

###### confetti explosion ##########################################

#Condition
#	On	Death
#
#
#	Event 
#		EName 	Sparkz
#		Type 	Local
#		At 	chest
#		ChildFX Explosions\medium\enc_Expolsion_med.fx
#		Lifespan 60
#		Lifespanjitter 	20
#	End
#End


End	


		