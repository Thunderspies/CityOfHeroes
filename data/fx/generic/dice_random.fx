#############################################################
## Dice_Random.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Dice1
		Type	Start
		At	Mystic
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Geom	dice1
		Geom	dice2
		Geom	dice3
		Geom	dice4
		Geom	dice5
		Geom	dice6
	End
End

#############################################################

End