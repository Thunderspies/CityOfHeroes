#########################################################
##	
##
FxInfo

##########################################################
Condition
	On	Time	
	Time 	40

	Event
		Type 	Local
		At	Hips
		Sound seekerlaunch 80.0 80.0 .5
	End
End

Condition
	On	Time	
	Time 	50

	Event
		Type 	Local
		At	Hips
		Sound seekerdrone2_loop 80.0 80.0 .55
	End
End

Condition
	On	Time	
	Time 	50

	Event
		Type 	Local
		At	Hips
		Sound seekerdrone3_loop 80.0 80.0 .45
	End
End


Condition
		
	Event
		Ename	Body
		Type	local 
		At	Hips
		geom	HunterSeeker01
			
		BHVR	:BodyScale.BHVR
                        BhvrOverride
                                    Behavior
                                    PositionOffset   0 1 0
			End

	End
	
		
	Event
		Ename	lightTop
		Type	local 
		At	Body
		
		BHVR	:BladeScaleSpin.BHVR
		Geom	drone_Blades
				
	End		

End


End