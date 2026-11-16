#########################################################
##	Claws Strike
########################################################

FxInfo

Lifespan 25

#############################################################

Condition
	On 	Time
	Time 	14

	Event
		Type	Local
		At	origin
		Sound Claw2b 100.0 100.0 .7
	End
End

#Condition
#	On 	Time
#	Time 	20
#
#	Event
#		EName	Streak
#		Type	local
#		At	Mystic
#		Geom	swoosh
#		bhvr	behaviors/Claws.bhvr
#	End
#End

#############################################################

End