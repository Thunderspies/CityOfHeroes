#########################################################
##	Tech Curtain
########################################################
FxInfo

#### needsound ####

##########################  Shield #############################
Condition
	On 	Time
	Time 	0

	Event
		EName	Rig
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0 0
		End
						
	End

	Event
		EName	Shield
		Type	Local
		At	Rig
		BhvrOverride
			PositionOffset	0 0 0
			Alpha		255
			StartColor	255 255 255
		End
		Geom	_basetek_aux_projectorsld_02
					
	End

End
###############################################################


End