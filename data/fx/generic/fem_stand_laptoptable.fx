
FxInfo

#########################################################
##################    Laptop Table    ###################
#########################################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	Mystic					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	Mystic_LapTopTable			#call to geometry fx
			BhvrOverride
                                    Behavior
                                    PositionOffset		-.45 .2 .5
			End
	End

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	Mystic					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	Mystic_LapTop				#call to geometry fx
			BhvrOverride
                                    Behavior
                                    PositionOffset		-.45 .2 .4
			End
	End
	

End

End		