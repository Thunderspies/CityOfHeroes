#########################################################
## Talsorian Gauntlet - Strike (Initial delayed)
########################################################

FxInfo

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	34

	Event
		EName	Streak
		Type	local
		At	Mystic
		Sound Claw2b 100.0 100.0 .6
	End
End

## SWOOSH ######################################################

Condition
	On 	Time
	Time 	34

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	:Blades_Swoosh.bhvr
	End
End

## CLEAN UP ######################################################

Condition
	On 	Time
	Time 	36

	Event
		EName	All
		Type	Destroy
		
	End
End

########################################################

End