
	
##########################################         RobotSpawn Drop         ###########################################


##################################################################################################################

FxInfo





#########################################################
####################  S o u n d s  ######################
#########################################################






##########################################################
#######################  J e t s  ########################
##########################################################

Condition	
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName	JetRight
		Type	Start
		At	Hips
		While	FLY
		Flags	OneAtATime
		ChildFX	:RocketBoots_Fly_JetR.fx
	End
End


##### Other Leg  ####

Condition	
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName	JetLeft
		Type	Start
		At	Hips
		While	FLY
		Flags	OneAtATime
		ChildFX	:RocketBoots_Fly_JetL.fx
	End
End




End
















		