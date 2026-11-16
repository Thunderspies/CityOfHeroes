#########################################################
##	Claws Strike
########################################################
FxInfo

Input  
	Inpname	Mystic
End

Condition
	On 	Time
	Time 	15

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound Claw2b 100.0 100.0 .6
	End
	
	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		bhvroverride
			PositionOffset 0 .15  .1
		end
	End
	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		bhvroverride
			PositionOffset 0 .3  .2
		end
	End
	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		bhvroverride
			PositionOffset 0 -.15  .1
		end
	End

End


Condition
	On 	Time
	Time 	22

	Event
		EName	All
		Type	Destroy
		
	End
	

End

End
##################################

