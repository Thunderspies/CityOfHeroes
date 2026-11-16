
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
                                    PositionOffset		.37 0 0
			End
	End

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	Mystic					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	Mystic_LapTop				#call to geometry fx
			BhvrOverride
                                    Behavior
                                    PositionOffset		.4 0 -.3
			End
	End
	

End

End		