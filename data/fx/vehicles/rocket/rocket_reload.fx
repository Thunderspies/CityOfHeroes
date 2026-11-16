#########################################################
##	Warburg Rocket - Launch
##

FxInfo
	
lifespan	197

Condition

	On Time 
	Time 0
	
	Event
		Ename	Start
		Type	Start #PositOnly
		At	Origin
		Sound rumble_loop 1200.0 1200.0 .6
	End

End

Condition

	On Time 
	Time 10
		
	Event
		EName 	ReloadSteam
		Type	Local
		At	Start
		BhvrOverride
			PositionOffset	0 -5 0
		End
		Part1	:Reload_Steam.part
		Lifespan	100
	End
	
End


Condition

	On Time 
	Time 50
		
	Event
		EName 	ReloadSteam2
		Type	Local
		At	Start
		BhvrOverride
			PositionOffset	0 -5 0
		End
		Part1	:Reload_Steam2.part
		Lifespan	150
	End
	
End

End