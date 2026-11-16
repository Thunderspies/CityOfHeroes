#########################################################
## Talsorian Gauntlet - Slash
########################################################

FxInfo

lifespan	90

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	10

	Event
		Type Local
		At origin
		Sound Eviscerate5 100.0 100.0 .8
	End
End

## SWOOSH FX ######################################################

Condition
	On 	Time
	Time 	41

	Event
		EName	Streak
		Type	start
		At	Mystic
		Geom	swoosh
		bhvr	:Blades_Swoosh.bhvr
	End
End

## CLEAN UP ######################################################

Condition
	On 	Time
	Time 	43

	Event
		EName	streak
		Type	Destroy
	End
End

########################################################

End